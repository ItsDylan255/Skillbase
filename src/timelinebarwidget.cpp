#include "timelinebarwidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include <QLocale>
#include <QMouseEvent>
#include <QScrollArea>
#include <QShowEvent>
#include <QEvent>
#include <algorithm>
#include <QScrollBar>

namespace {

constexpr int AxisHeight = 32;
constexpr int BarTopMargin = 8;
constexpr int BarHeight = 44;
constexpr int SegmentGap = 2;
constexpr int SegmentRadius = 4;

// Farbe der Heute-Markierung.
// Sie ist bewusst neutral gehalten, damit die Hobbyfarbe
// weiterhin für die Phasen reserviert bleibt.
const QColor TodayMarkerColor("#eaeaec");

// Wählt Text in Schwarz oder Weiß, je nachdem,
// was auf der jeweiligen Hobbyfarbe besser lesbar ist.
QColor contrastingTextColor(const QColor &background)
{
    const qreal luminance =
        (0.299 * background.red()
         + 0.587 * background.green()
         + 0.114 * background.blue()) / 255.0;

    return luminance > 0.6
               ? QColor("#171012")
               : QColor("#eaeaec");
}

}

TimelineBarWidget::TimelineBarWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(
        AxisHeight
        + BarTopMargin
        + BarHeight
        );

    // Für den Hover-Cursor über den einzelnen Phasen-Segmenten
    // muss mouseMoveEvent auch ohne gedrückte Maustaste ausgelöst werden.
    setMouseTracking(true);
}

void TimelineBarWidget::setPhases(
    const QList<TimelinePhase> &newPhases
    )
{
    phases = newPhases;

    // Die Anzahl der Monate kann sich durch neue oder gelöschte Phasen
    // ändern. Deshalb wird die Content-Breite anschließend neu berechnet.
    updateTimelineWidth();

    update();
}

void TimelineBarWidget::setHobbyColor(
    const QColor &color
    )
{
    hobbyColor = color;

    update();
}

void TimelineBarWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // Erst wenn das Widget sichtbar ist, befindet es sich zuverlässig
    // innerhalb der QScrollArea.
    QScrollArea *scrollArea =
        qobject_cast<QScrollArea *>(parentWidget());

    if (!scrollArea && parentWidget()) {
        scrollArea =
            qobject_cast<QScrollArea *>(
                parentWidget()->parentWidget()
                );
    }

    if (scrollArea) {

        // Der Viewport meldet uns künftig, wenn sich seine Größe ändert.
        scrollArea->viewport()->installEventFilter(this);

        viewportWidth =
            scrollArea->viewport()->width();

        updateTimelineWidth();

        // Nach dem Berechnen der Content-Breite wird die Timeline
        // automatisch auf den heutigen Zeitraum positioniert.
        scrollToToday();
    }
}

bool TimelineBarWidget::eventFilter(
    QObject *watched,
    QEvent *event
    )
{
    // Nur Größenänderungen des ScrollArea-Viewports interessieren uns.
    if (event->type() == QEvent::Resize) {

        if (auto *viewport =
            qobject_cast<QWidget *>(watched)) {

            viewportWidth =
                viewport->width();

            // Wenn sich die sichtbare Breite ändert,
            // muss sich auch die Monatsbreite ändern.
            updateTimelineWidth();
        }
    }

    return QWidget::eventFilter(
        watched,
        event
        );
}

void TimelineBarWidget::updateTimelineWidth()
{
    const int width =
        viewportWidth > 0
            ? viewportWidth
            : VisibleMonthCount
                  * DefaultMonthPixelWidth;

    // Die Timeline enthält mindestens 12 Monate.
    //
    // Wenn beispielsweise 24 Monate vorhanden sind,
    // benötigt der gesamte Content doppelt so viel Breite
    // wie der sichtbare 12-Monats-Bereich.
    const int monthCount =
        axisMonthCount();

    const int contentWidth =
        std::max(
            width,
            static_cast<int>(
                monthCount * monthPixelWidth()
                )
            );

    resize(
        contentWidth,
        AxisHeight
            + BarTopMargin
            + BarHeight
        );

    update();
}

void TimelineBarWidget::scrollToToday()
{
    QScrollArea *scrollArea =
        qobject_cast<QScrollArea *>(parentWidget());

    if (!scrollArea && parentWidget()) {
        scrollArea =
            qobject_cast<QScrollArea *>(
                parentWidget()->parentWidget()
                );
    }

    if (!scrollArea)
        return;

    const QDate today =
        QDate::currentDate();

    // Berechnet die horizontale Position des heutigen Tages
    // innerhalb des gesamten Timeline-Contents.
    const int todayX =
        qRound(xForDate(today));

    // Die Hälfte des sichtbaren Bereichs wird abgezogen,
    // damit "Heute" ungefähr mittig angezeigt wird.
    const int targetScrollPosition =
        todayX
        - scrollArea->viewport()->width() / 2;

    // Verhindert, dass die Scrollposition außerhalb
    // des tatsächlich möglichen Bereichs liegt.
    const int clampedPosition =
        std::clamp(
            targetScrollPosition,
            0,
            scrollArea->horizontalScrollBar()->maximum()
            );

    scrollArea->horizontalScrollBar()->setValue(
        clampedPosition
        );
}

qreal TimelineBarWidget::monthPixelWidth() const
{
    if (viewportWidth <= 0)
        return DefaultMonthPixelWidth;

    // Der sichtbare Bereich wird immer exakt
    // in 12 gleich große Monate aufgeteilt.
    return viewportWidth /
           static_cast<qreal>(
               VisibleMonthCount
               );
}

QDate TimelineBarWidget::axisStartMonth() const
{
    const QDate today =
        QDate::currentDate();

    QDate startMonth(
        today.year(),
        today.month(),
        1
        );

    for (const TimelinePhase &phase : phases) {

        const QDate start =
            QDate::fromString(
                phase.startDate,
                "yyyy-MM-dd"
                );

        if (start.isValid()) {

            const QDate phaseMonth(
                start.year(),
                start.month(),
                1
                );

            if (phaseMonth < startMonth)
                startMonth = phaseMonth;
        }
    }

    return startMonth;
}

int TimelineBarWidget::axisMonthCount() const
{
    const QDate startMonth =
        axisStartMonth();

    QDate latestMonth =
        startMonth;

    for (const TimelinePhase &phase : phases) {

        const QDate end =
            QDate::fromString(
                phase.endDate,
                "yyyy-MM-dd"
                );

        if (!end.isValid())
            continue;

        const QDate phaseMonth(
            end.year(),
            end.month(),
            1
            );

        if (phaseMonth > latestMonth)
            latestMonth = phaseMonth;
    }

    const int months =
        (latestMonth.year() - startMonth.year()) * 12
        + (latestMonth.month() - startMonth.month())
        + 1;

    return std::max(
        months,
        VisibleMonthCount
        );
}

qreal TimelineBarWidget::xForDate(
    const QDate &date
    ) const
{
    const QDate startMonth =
        axisStartMonth();

    const int wholeMonths =
        (date.year() - startMonth.year()) * 12
        + (date.month() - startMonth.month());

    const int daysInMonth =
        std::max(
            date.daysInMonth(),
            1
            );

    const qreal monthFraction =
        (date.day() - 1)
        / qreal(daysInMonth);

    return (
               wholeMonths
               + monthFraction
               ) * monthPixelWidth();
}

QSize TimelineBarWidget::sizeHint() const
{
    const int width =
        viewportWidth > 0
            ? std::max(
                  viewportWidth,
                  static_cast<int>(
                      axisMonthCount()
                      * monthPixelWidth()
                      )
                  )
            : VisibleMonthCount
                  * DefaultMonthPixelWidth;

    return QSize(
        width,
        AxisHeight
            + BarTopMargin
            + BarHeight
        );
}

QSize TimelineBarWidget::minimumSizeHint() const
{
    return QSize(
        VisibleMonthCount
            * DefaultMonthPixelWidth,
        AxisHeight
            + BarTopMargin
            + BarHeight
        );
}


bool TimelineBarWidget::isCurrentPhase(
    const TimelinePhase &phase
    ) const
{
    const QDate today =
        QDate::currentDate();

    const QDate start =
        QDate::fromString(
            phase.startDate,
            "yyyy-MM-dd"
            );

    const QDate end =
        QDate::fromString(
            phase.endDate,
            "yyyy-MM-dd"
            );

    if (!start.isValid() || !end.isValid())
        return false;

    return today >= start && today <= end;
}

QList<QPair<TimelinePhase, QRectF>>




TimelineBarWidget::segmentRects() const
{
    QList<QPair<TimelinePhase, QRectF>> result;

    const int barTop =
        AxisHeight + BarTopMargin;

    for (const TimelinePhase &phase : phases) {

        const QDate start =
            QDate::fromString(
                phase.startDate,
                "yyyy-MM-dd"
                );

        const QDate end =
            QDate::fromString(
                phase.endDate,
                "yyyy-MM-dd"
                );

        if (!start.isValid() || !end.isValid())
            continue;

        const qreal x1 =
            xForDate(start);

        // Das Ende ist inklusive.
        // Der Tag danach markiert deshalb das rechte Ende.
        const qreal x2 =
            xForDate(
                end.addDays(1)
                );

        QRectF segmentRect(
            x1,
            barTop,
            std::max(
                x2 - x1,
                qreal(4)
                ),
            BarHeight
            );

        segmentRect.adjust(
            SegmentGap / 2.0,
            0,
            -SegmentGap / 2.0,
            0
            );

        result.append({
            phase,
            segmentRect
        });
    }

    return result;
}

void TimelineBarWidget::drawTodayMarker(
    QPainter &painter
    ) const
{
    const QDate today =
        QDate::currentDate();

    const QDate startMonth =
        axisStartMonth();

    // Liegt heute vor dem dargestellten Zeitraum,
    // gibt es innerhalb der Timeline keinen Marker.
    if (today < startMonth)
        return;

    const QDate lastMonth =
        startMonth.addMonths(
            axisMonthCount() - 1
            );

    const QDate endOfTimeline =
        lastMonth.addMonths(1).addDays(-1);

    // Liegt heute hinter dem dargestellten Zeitraum,
    // gibt es ebenfalls keinen Marker.
    if (today > endOfTimeline)
        return;

    const qreal x =
        xForDate(today);

    // Der Marker beginnt direkt oberhalb der Phasen
    // und reicht bis zum unteren Rand der Timeline.
    const qreal markerTop =
        AxisHeight - 4;

    const qreal markerBottom =
        AxisHeight
        + BarTopMargin
        + BarHeight;

    painter.setPen(
        QPen(
            TodayMarkerColor,
            2
            )
        );

    painter.drawLine(
        QPointF(x, markerTop),
        QPointF(x, markerBottom)
        );

    // Kleines Dreieck an der Achse.
    // Dadurch ist die aktuelle Position auch bei mehreren
    // übereinanderliegenden Phasen schnell erkennbar.
    QPainterPath triangle;

    triangle.moveTo(
        x - 5,
        AxisHeight - 5
        );

    triangle.lineTo(
        x + 5,
        AxisHeight - 5
        );

    triangle.lineTo(
        x,
        AxisHeight + 2
        );

    triangle.closeSubpath();

    painter.fillPath(
        triangle,
        TodayMarkerColor
        );

    // Beschriftung direkt oberhalb der Markierung.
    QFont todayFont =
        painter.font();

    todayFont.setPointSize(8);
    todayFont.setBold(true);

    painter.setFont(
        todayFont
        );

    painter.setPen(
        TodayMarkerColor
        );

    const QRectF labelRect(
        x - 35,
        0,
        70,
        16
        );

    painter.drawText(
        labelRect,
        Qt::AlignCenter,
        "Heute"
        );
}

void TimelineBarWidget::paintEvent(
    QPaintEvent *event
    )
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHint(
        QPainter::Antialiasing
        );

    const QDate startMonth =
        axisStartMonth();

    const int monthCount =
        axisMonthCount();

    const qreal monthWidth =
        monthPixelWidth();

    const QLocale germanLocale(
        QLocale::German
        );

    // ── Achse ──────────────────────────────────────────────────────────────

    QFont monthFont =
        painter.font();

    monthFont.setPointSize(9);

    QFont yearFont =
        monthFont;

    yearFont.setBold(true);

    for (int i = 0; i < monthCount; ++i) {

        const QDate month =
            startMonth.addMonths(i);

        const int x0 =
            qRound(
                i * monthWidth
                );

        const int cellWidth =
            qRound(monthWidth);

        const QRect monthCell(
            x0,
            AxisHeight - 16,
            cellWidth,
            16
            );

        painter.setFont(
            monthFont
            );

        painter.setPen(
            QColor("#9a9aa0")
            );

        painter.drawText(
            monthCell,
            Qt::AlignCenter,
            germanLocale.monthName(
                month.month(),
                QLocale::ShortFormat
                )
            );

        // Jahreszahl am Anfang und beim Jahreswechsel.
        if (i == 0 || month.month() == 1) {

            const QRect yearCell(
                x0,
                0,
                cellWidth * 3,
                16
                );

            painter.setFont(
                yearFont
                );

            painter.setPen(
                QColor("#eaeaec")
                );

            painter.drawText(
                yearCell,
                Qt::AlignLeft
                    | Qt::AlignVCenter,
                QString::number(
                    month.year()
                    )
                );
        }

        painter.setPen(
            QColor("#29292d")
            );

        painter.drawLine(
            x0,
            AxisHeight - 6,
            x0,
            AxisHeight
            );
    }

    // Der Heute-Marker wird unabhängig davon gezeichnet,
    // ob bereits Lernphasen vorhanden sind.
    drawTodayMarker(
        painter
        );

    if (phases.isEmpty())
        return;

    // ── Phasen-Segmente ────────────────────────────────────────────────────

    const QColor fillColor =
        hobbyColor.isValid()
            ? hobbyColor
            : QColor("#ff6f61");

    QFont nameFont =
        painter.font();

    nameFont.setPointSize(9);
    nameFont.setWeight(
        QFont::DemiBold
        );

    painter.setFont(
        nameFont
        );

    const QFontMetrics fontMetrics(
        nameFont
        );

    for (const auto &entry :
         segmentRects()) {

        const TimelinePhase &phase =
            entry.first;

        const QRectF &segmentRect =
            entry.second;

        QPainterPath path;

        path.addRoundedRect(
            segmentRect,
            SegmentRadius,
            SegmentRadius
            );

        // Die aktuelle Phase bekommt eine zusätzliche Hervorhebung,
        // damit sie sich klar von vergangenen und zukünftigen Phasen unterscheidet.
        if (isCurrentPhase(phase)) {

            // Grundfarbe bleibt die Hobbyfarbe.
            painter.fillPath(
                path,
                fillColor
                );

            // Heller Rand macht die aktuelle Phase sichtbar,
            // ohne eine komplett neue Farbe einzuführen.
            painter.setPen(
                QPen(
                    QColor("#eaeaec"),
                    2
                    )
                );

            painter.drawPath(path);

        } else {

            // Vergangene und zukünftige Phasen bleiben unverändert.
            painter.fillPath(
                path,
                fillColor
                );
        }

        // Namen nur anzeigen, wenn der Balken genug Platz bietet.
        if (segmentRect.width() >= 32) {

            painter.setPen(
                contrastingTextColor(
                    fillColor
                    )
                );

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

void TimelineBarWidget::mousePressEvent(
    QMouseEvent *event
    )
{
    if (event->button() ==
        Qt::LeftButton) {

        for (const auto &entry :
             segmentRects()) {

            if (entry.second.contains(
                    event->pos())) {

                emit phaseClicked(
                    entry.first
                    );

                return;
            }
        }
    }

    QWidget::mousePressEvent(event);
}

void TimelineBarWidget::mouseMoveEvent(
    QMouseEvent *event
    )
{
    bool overSegment = false;

    for (const auto &entry :
         segmentRects()) {

        if (entry.second.contains(
                event->pos())) {

            overSegment = true;
            break;
        }
    }

    setCursor(
        overSegment
            ? Qt::PointingHandCursor
            : Qt::ArrowCursor
        );

    QWidget::mouseMoveEvent(event);
}