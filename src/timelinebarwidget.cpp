#include "timelinebarwidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include <QLocale>
#include <QMouseEvent>
#include <algorithm>

namespace {

// Feste Breite pro Monat (statt Stauchung auf die verfügbare Breite).
// Bei vielen Monaten wächst die benötigte Breite entsprechend,
// die umgebende QScrollArea zeigt dann einen horizontalen Scrollbalken.
constexpr int MonthPixelWidth = 64;

constexpr int AxisHeight = 32;      // Jahres- + Monatszeile der Achse
constexpr int BarTopMargin = 8;     // Abstand zwischen Achse und Balken
constexpr int BarHeight = 44;       // Höhe der eigentlichen Phasen-Segmente
constexpr int SegmentGap = 2;       // Sichtbarer Abstand zwischen Phasen
constexpr int SegmentRadius = 4;    // Kleiner, konsistenter Radius (Regel #7)

constexpr int MinAxisMonthCount = 3;

// Wählt Text in Schwarz oder Weiß, je nachdem, was auf der
// jeweiligen Hobbyfarbe besser lesbar ist.
QColor contrastingTextColor(const QColor &background)
{
    const qreal luminance =
        (0.299 * background.red()
         + 0.587 * background.green()
         + 0.114 * background.blue()) / 255.0;

    return luminance > 0.6 ? QColor("#171012") : QColor("#eaeaec");
}

}

TimelineBarWidget::TimelineBarWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(AxisHeight + BarTopMargin + BarHeight);

    // Für den Hover-Cursor über den einzelnen Phasen-Segmenten
    // (siehe mouseMoveEvent) muss mouseMoveEvent auch ohne
    // gedrückte Maustaste ausgelöst werden.
    setMouseTracking(true);
}

void TimelineBarWidget::setPhases(
    const QList<TimelinePhase> &newPhases
    )
{
    phases = newPhases;

    // Die benötigte Breite hängt von den neuen Phasen ab (Anzahl Monate).
    // updateGeometry() allein reicht hier NICHT aus: Das Widget hängt
    // direkt in einer QScrollArea mit widgetResizable=false und nicht
    // unter einem QLayout, das auf eine LayoutRequest reagieren würde.
    // Deshalb muss die Breite aktiv neu gesetzt werden - sonst bleibt
    // das Widget auf seiner ursprünglichen (zu schmalen) Größe stehen
    // und spätere Phasen werden abgeschnitten bzw. überlappen sich.
    resize(sizeHint());

    update();
}

void TimelineBarWidget::setHobbyColor(
    const QColor &color
    )
{
    hobbyColor = color;

    update();
}

QDate TimelineBarWidget::axisStartMonth() const
{
    if (phases.isEmpty())
        return QDate::currentDate().addDays(1 - QDate::currentDate().day());

    QDate earliest =
        QDate::fromString(phases.first().startDate, "yyyy-MM-dd");

    for (const TimelinePhase &phase : phases) {
        const QDate start =
            QDate::fromString(phase.startDate, "yyyy-MM-dd");

        if (start.isValid() && start < earliest)
            earliest = start;
    }

    return QDate(earliest.year(), earliest.month(), 1);
}

int TimelineBarWidget::axisMonthCount() const
{
    if (phases.isEmpty())
        return MinAxisMonthCount;

    const QDate startMonth = axisStartMonth();

    QDate latest =
        QDate::fromString(phases.first().endDate, "yyyy-MM-dd");

    for (const TimelinePhase &phase : phases) {
        const QDate end =
            QDate::fromString(phase.endDate, "yyyy-MM-dd");

        if (end.isValid() && end > latest)
            latest = end;
    }

    const QDate endMonth(latest.year(), latest.month(), 1);

    const int months =
        (endMonth.year() - startMonth.year()) * 12
        + (endMonth.month() - startMonth.month())
        + 1;

    return std::max(months, MinAxisMonthCount);
}

qreal TimelineBarWidget::xForDate(const QDate &date) const
{
    const QDate startMonth = axisStartMonth();

    const int wholeMonths =
        (date.year() - startMonth.year()) * 12
        + (date.month() - startMonth.month());

    const int daysInMonth = std::max(date.daysInMonth(), 1);
    const qreal monthFraction = (date.day() - 1) / qreal(daysInMonth);

    return (wholeMonths + monthFraction) * MonthPixelWidth;
}

QSize TimelineBarWidget::sizeHint() const
{
    const int width = axisMonthCount() * MonthPixelWidth;
    return QSize(width, AxisHeight + BarTopMargin + BarHeight);
}

QSize TimelineBarWidget::minimumSizeHint() const
{
    return sizeHint();
}

QList<QPair<TimelinePhase, QRectF>> TimelineBarWidget::segmentRects() const
{
    QList<QPair<TimelinePhase, QRectF>> result;

    const int barTop = AxisHeight + BarTopMargin;

    for (const TimelinePhase &phase : phases) {

        const QDate start =
            QDate::fromString(phase.startDate, "yyyy-MM-dd");

        const QDate end =
            QDate::fromString(phase.endDate, "yyyy-MM-dd");

        if (!start.isValid() || !end.isValid())
            continue;

        const qreal x1 = xForDate(start);
        // Das Ende ist inklusive - der Tag danach markiert das rechte Ende.
        const qreal x2 = xForDate(end.addDays(1));

        QRectF segmentRect(x1, barTop, std::max(x2 - x1, qreal(4)), BarHeight);
        segmentRect.adjust(SegmentGap / 2.0, 0, -SegmentGap / 2.0, 0);

        result.append({phase, segmentRect});
    }

    return result;
}

void TimelineBarWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QDate startMonth = axisStartMonth();
    const int monthCount = axisMonthCount();

    const QLocale germanLocale(QLocale::German);

    // ── Achse: Jahr oben, Monat darunter, dünne Tick-Linien ────────────────

    QFont monthFont = painter.font();
    monthFont.setPointSize(9);

    QFont yearFont = monthFont;
    yearFont.setBold(true);

    for (int i = 0; i < monthCount; ++i) {

        const QDate month = startMonth.addMonths(i);
        const int x0 = i * MonthPixelWidth;

        const QRect monthCell(x0, AxisHeight - 16, MonthPixelWidth, 16);

        painter.setFont(monthFont);
        painter.setPen(QColor("#9a9aa0"));
        painter.drawText(
            monthCell,
            Qt::AlignCenter,
            germanLocale.monthName(month.month(), QLocale::ShortFormat)
            );

        // Jahreszahl nur beim Jahreswechsel (bzw. ganz am Anfang) einblenden.
        if (i == 0 || month.month() == 1) {
            const QRect yearCell(x0, 0, MonthPixelWidth * 3, 16);

            painter.setFont(yearFont);
            painter.setPen(QColor("#eaeaec"));
            painter.drawText(
                yearCell,
                Qt::AlignLeft | Qt::AlignVCenter,
                QString::number(month.year())
                );
        }

        painter.setPen(QColor("#29292d"));
        painter.drawLine(x0, AxisHeight - 6, x0, AxisHeight);
    }

    if (phases.isEmpty())
        return;

    // ── Phasen-Segmente ─────────────────────────────────────────────────────

    const QColor fillColor =
        hobbyColor.isValid() ? hobbyColor : QColor("#ff6f61");

    QFont nameFont = painter.font();
    nameFont.setPointSize(9);
    nameFont.setWeight(QFont::DemiBold);
    painter.setFont(nameFont);
    const QFontMetrics fontMetrics(nameFont);

    for (const auto &entry : segmentRects()) {

        const TimelinePhase &phase = entry.first;
        const QRectF &segmentRect = entry.second;

        QPainterPath path;
        path.addRoundedRect(segmentRect, SegmentRadius, SegmentRadius);

        painter.fillPath(path, fillColor);

        // Namen nur einblenden, wenn genug Platz für lesbaren Text ist.
        // Der Balken selbst ändert dabei nie seine Größe - es wird
        // ausschließlich der Text gekürzt.
        if (segmentRect.width() >= 32) {

            painter.setPen(contrastingTextColor(fillColor));

            const QString elidedName =
                fontMetrics.elidedText(
                    phase.name,
                    Qt::ElideRight,
                    int(segmentRect.width()) - 10
                    );

            painter.drawText(
                segmentRect,
                Qt::AlignCenter,
                elidedName
                );
        }
    }
}

void TimelineBarWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        for (const auto &entry : segmentRects()) {

            if (entry.second.contains(event->pos())) {
                emit phaseClicked(entry.first);
                return;
            }
        }
    }

    QWidget::mousePressEvent(event);
}

void TimelineBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    bool overSegment = false;

    for (const auto &entry : segmentRects()) {

        if (entry.second.contains(event->pos())) {
            overSegment = true;
            break;
        }
    }

    setCursor(overSegment ? Qt::PointingHandCursor : Qt::ArrowCursor);

    QWidget::mouseMoveEvent(event);
}
