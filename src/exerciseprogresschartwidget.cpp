#include "exerciseprogresschartwidget.h"

#include <QPainter>
#include <QPainterPath>
#include <algorithm>

ExerciseProgressChartWidget::ExerciseProgressChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(140);
}

void ExerciseProgressChartWidget::setLogs(const QList<ExerciseLog> &logs)
{
    points.clear();
    points.reserve(logs.size());

    for (const ExerciseLog &log : logs) {

        const QDateTime performedAt =
            QDateTime::fromString(
                log.performedAt,
                "yyyy-MM-dd HH:mm:ss"
                ).toLocalTime();

        points.append({performedAt, log.value});
    }

    // Für das Diagramm muss die zeitliche Reihenfolge stimmen,
    // unabhängig davon, wie die Log-Einträge übergeben wurden.
    std::sort(
        points.begin(),
        points.end(),
        [](const DataPoint &a, const DataPoint &b) {
            return a.performedAt < b.performedAt;
        }
        );

    update();
}

QSize ExerciseProgressChartWidget::minimumSizeHint() const
{
    return QSize(220, 140);
}

QSize ExerciseProgressChartWidget::sizeHint() const
{
    return QSize(360, 180);
}

void ExerciseProgressChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QColor mutedColor("#6d6d73");
    const QColor accentColor("#ff6f61");

    // ── Leerer Zustand ───────────────────────────────────────────────────
    if (points.isEmpty()) {

        painter.setPen(mutedColor);
        painter.drawText(
            rect(),
            Qt::AlignCenter,
            "Noch keine Messwerte vorhanden."
            );
        return;
    }

    // ── Zeichenbereich ───────────────────────────────────────────────────
    // Etwas Innenabstand, damit Punkte am Rand und die
    // Datumsbeschriftung unten nicht abgeschnitten werden.
    const qreal leftPadding   = 6.0;
    const qreal rightPadding  = 6.0;
    const qreal topPadding    = 10.0;
    const qreal bottomPadding = 18.0;

    const QRectF plotRect(
        leftPadding,
        topPadding,
        width() - leftPadding - rightPadding,
        height() - topPadding - bottomPadding
        );

    if (plotRect.width() <= 0 || plotRect.height() <= 0)
        return;

    // ── Wertebereich automatisch an die Daten anpassen ──────────────────
    double minValue = points.first().value;
    double maxValue = points.first().value;

    for (const DataPoint &point : points) {
        minValue = std::min(minValue, point.value);
        maxValue = std::max(maxValue, point.value);
    }

    if (qFuzzyCompare(minValue + 1.0, maxValue + 1.0)) {
        // Ein einzelner Messwert bzw. lauter identische Werte:
        // etwas künstlichen Puffer einführen, damit die Linie
        // nicht auf der Kante des Diagramms verläuft.
        minValue -= 1.0;
        maxValue += 1.0;
    } else {
        const double valuePadding = (maxValue - minValue) * 0.15;
        minValue -= valuePadding;
        maxValue += valuePadding;
    }

    const qint64 startMs = points.first().performedAt.toMSecsSinceEpoch();
    const qint64 endMs   = points.last().performedAt.toMSecsSinceEpoch();
    const qint64 spanMs  = std::max<qint64>(endMs - startMs, 1);

    auto xForIndex = [&](int index) -> qreal {

        if (points.size() == 1)
            return plotRect.center().x();

        const qint64 ms = points[index].performedAt.toMSecsSinceEpoch();
        const qreal ratio = qreal(ms - startMs) / qreal(spanMs);
        return plotRect.left() + ratio * plotRect.width();
    };

    auto yForValue = [&](double value) -> qreal {
        const qreal ratio = (value - minValue) / (maxValue - minValue);
        return plotRect.bottom() - ratio * plotRect.height();
    };

    // ── Verbindungslinie ─────────────────────────────────────────────────
    QPolygonF polyline;

    for (int i = 0; i < points.size(); ++i)
        polyline << QPointF(xForIndex(i), yForValue(points[i].value));

    if (polyline.size() > 1) {
        painter.setPen(QPen(accentColor, 2));
        painter.drawPolyline(polyline);
    }

    // ── Messpunkte ───────────────────────────────────────────────────────
    painter.setPen(Qt::NoPen);
    painter.setBrush(accentColor);

    for (const QPointF &point : polyline)
        painter.drawEllipse(point, 3.5, 3.5);

    // ── X-Achsen-Beschriftung ────────────────────────────────────────────
    // Nur erster, mittlerer und letzter Zeitpunkt, damit das Diagramm
    // kompakt bleibt und nicht überladen wirkt.
    painter.setPen(mutedColor);

    QFont axisFont = painter.font();
    axisFont.setPointSizeF(std::max(7.0, axisFont.pointSizeF() - 2.0));
    painter.setFont(axisFont);

    const QRectF labelRect(
        plotRect.left(),
        plotRect.bottom() + 2,
        plotRect.width(),
        bottomPadding - 2
        );

    const QString firstLabel =
        points.first().performedAt.date().toString("dd.MM.");
    const QString lastLabel =
        points.last().performedAt.date().toString("dd.MM.");

    painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, firstLabel);
    painter.drawText(labelRect, Qt::AlignRight | Qt::AlignVCenter, lastLabel);

    if (points.size() > 2) {

        const QString middleLabel =
            points[points.size() / 2].performedAt.date().toString("dd.MM.");

        painter.drawText(
            labelRect,
            Qt::AlignHCenter | Qt::AlignVCenter,
            middleLabel
            );
    }
}
