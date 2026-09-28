#include "exerciseprogresschartwidget.h"

#include <QPainter>
#include <QPainterPath>
#include <algorithm>
#include <cmath>

ExerciseProgressChartWidget::ExerciseProgressChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(140);
}

void ExerciseProgressChartWidget::setLogs(
    const QList<ExerciseLog> &logs,
    double startValue,
    double goalValue
    )
{
    this->startValue = startValue;
    this->goalValue = goalValue;

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

// Wählt einen "schönen" Abstand zwischen den Werten der Y-Achse.
// Dadurch entstehen z. B. 10er-, 20er- oder 50er-Schritte
// statt krummer Werte wie 14.6 oder 29.2.
static double niceTickStep(double range)
{
    if (range <= 0.0)
        return 1.0;

    // Wir möchten ungefähr 5 Abstände auf der Achse haben.
    const double roughStep = range / 5.0;

    // Größenordnung des ungefähren Abstands bestimmen.
    const double magnitude =
        std::pow(
            10.0,
            std::floor(
                std::log10(roughStep)
                )
            );

    // Auf 1, 2 oder 5 innerhalb dieser Größenordnung runden.
    const double normalized =
        roughStep / magnitude;

    double niceNormalized;

    if (normalized <= 1.0)
        niceNormalized = 1.0;
    else if (normalized <= 2.0)
        niceNormalized = 2.0;
    else if (normalized <= 5.0)
        niceNormalized = 5.0;
    else
        niceNormalized = 10.0;

    return niceNormalized * magnitude;
}

// Bestimmt einen sinnvollen Abstand zwischen den Datumsmarkierungen.
// Der Abstand wird anhand des gesamten Zeitraums gewählt.
static qint64 niceDateTickDays(qint64 totalDays)
{
    if (totalDays <= 7)
        return 1;

    if (totalDays <= 14)
        return 2;

    if (totalDays <= 31)
        return 5;

    if (totalDays <= 60)
        return 10;

    if (totalDays <= 120)
        return 14;

    if (totalDays <= 240)
        return 30;

    if (totalDays <= 480)
        return 60;

    return 90;
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

    // Links wird Platz für die Werte der Y-Achse benötigt.
    // Unten wird Platz für die Datumsbeschriftung benötigt.
    const qreal leftPadding   = 42.0;
    const qreal rightPadding  = 6.0;
    const qreal topPadding    = 10.0;
    const qreal bottomPadding = 30.0;

    const QRectF plotRect(
        leftPadding,
        topPadding,
        width() - leftPadding - rightPadding,
        height() - topPadding - bottomPadding
        );

    if (plotRect.width() <= 0 || plotRect.height() <= 0)
        return;

    // ── Wertebereich der Y-Achse ─────────────────────────────────────────

    // Start und Ziel bilden zunächst die Grundlage für die Y-Achse.
    double minValue =
        std::min(
            startValue,
            goalValue
            );

    double maxValue =
        std::max(
            startValue,
            goalValue
            );

    // Auch alle tatsächlich gemessenen Werte müssen berücksichtigt werden.
    //
    // Beispiel:
    // Start = 50
    // Ziel  = 100
    // Wert  = 120
    //
    // Dann muss die Y-Achse mindestens bis 120 reichen.
    for (const DataPoint &point : points) {

        minValue =
            std::min(
                minValue,
                point.value
                );

        maxValue =
            std::max(
                maxValue,
                point.value
                );
    }

    // Falls alle Werte identisch sind, brauchen wir trotzdem
    // einen gültigen Wertebereich für die Darstellung.
    if (qFuzzyCompare(minValue + 1.0, maxValue + 1.0)) {

        minValue -= 1.0;
        maxValue += 1.0;
    }

    // ── Zeitbereich der X-Achse ──────────────────────────────────────────

    const QDateTime firstDateTime =
        points.first().performedAt;

    const QDateTime lastDateTime =
        points.last().performedAt;

    const qint64 startMs =
        firstDateTime.toMSecsSinceEpoch();

    const qint64 endMs =
        lastDateTime.toMSecsSinceEpoch();

    // Wenn nur ein Messwert existiert, wird trotzdem ein gültiger
    // Zeitbereich für das Diagramm benötigt.
    const qint64 spanMs =
        std::max<qint64>(
            endMs - startMs,
            1
            );

    const QDate firstDate =
        firstDateTime.date();

    const QDate lastDate =
        lastDateTime.date();

    // QDate::daysTo() liefert in Qt 6 einen qint64.
    // Deshalb verwenden wir auch hier qint64 für den Vergleich.
    const qint64 totalDays =
        std::max<qint64>(
            firstDate.daysTo(lastDate),
            1
            );

    // ── Hilfsfunktionen für die Positionen ──────────────────────────────

    // Wandelt einen Messpunkt in eine X-Position um.
    auto xForIndex = [&](int index) -> qreal {

        if (points.size() == 1)
            return plotRect.center().x();

        const qint64 ms =
            points[index].performedAt.toMSecsSinceEpoch();

        const qreal ratio =
            qreal(ms - startMs) /
            qreal(spanMs);

        return plotRect.left() +
               ratio * plotRect.width();
    };

    // Wandelt einen Messwert in eine Y-Position um.
    auto yForValue = [&](double value) -> qreal {

        const qreal ratio =
            (value - minValue) /
            (maxValue - minValue);

        return plotRect.bottom() -
               ratio * plotRect.height();
    };

    // Wandelt ein Datum in eine X-Position um.
    auto xForDate = [&](const QDate &date) -> qreal {

        if (firstDate == lastDate)
            return plotRect.center().x();

        const qint64 daysFromStart =
            firstDate.daysTo(date);

        const qint64 axisDays =
            std::max<qint64>(
                firstDate.daysTo(lastDate),
                1
                );

        const qreal ratio =
            qreal(daysFromStart) /
            qreal(axisDays);

        return plotRect.left() +
               ratio * plotRect.width();
    };

    // ── Schrift für Achsen ──────────────────────────────────────────────

    painter.setPen(mutedColor);

    QFont axisFont =
        painter.font();

    axisFont.setPointSizeF(
        std::max(
            7.0,
            axisFont.pointSizeF() - 2.0
            )
        );

    painter.setFont(axisFont);

    // ── Y-Achse ──────────────────────────────────────────────────────────

    painter.setPen(
        QPen(
            mutedColor,
            1
            )
        );

    painter.drawLine(
        QPointF(
            plotRect.left(),
            plotRect.top()
            ),
        QPointF(
            plotRect.left(),
            plotRect.bottom()
            )
        );

    // ── Y-Achsen-Einteilung ─────────────────────────────────────────────

    const double tickStep =
        niceTickStep(
            maxValue - minValue
            );

    // Zeichnet einen einzelnen Tick inklusive Beschriftung.
    auto drawYTick = [&](double value) {

        const qreal y =
            yForValue(value);

        // Kleiner Strich an der Y-Achse.
        painter.setPen(
            QPen(
                mutedColor,
                1
                )
            );

        painter.drawLine(
            QPointF(
                plotRect.left() - 5,
                y
                ),
            QPointF(
                plotRect.left(),
                y
                )
            );

        // Wert links neben dem Strich.
        painter.drawText(
            QRectF(
                0,
                y - 8,
                plotRect.left() - 8,
                16
                ),
            Qt::AlignRight | Qt::AlignVCenter,
            QString::number(
                value,
                'g',
                6
                )
            );
    };

    // Der kleinste relevante Wert wird unten angezeigt.
    drawYTick(minValue);

    // Dazwischen werden automatisch sinnvolle Werte angezeigt.
    for (double value = minValue + tickStep;
         value < maxValue;
         value += tickStep) {

        drawYTick(value);
    }

    // Der größte relevante Wert wird oben angezeigt.
    drawYTick(maxValue);

    // ── Zielwert-Markierung ─────────────────────────────────────────────

    // Das Ziel bleibt unabhängig vom aktuellen Messwert
    // ein fester Referenzwert.
    //
    // Dadurch bleibt z. B. bei:
    // Start = 50
    // Ziel  = 100
    // Wert  = 120
    //
    // weiterhin erkennbar, wo das eigentliche Ziel liegt.
    const qreal goalY =
        yForValue(goalValue);

    if (goalY >= plotRect.top() &&
        goalY <= plotRect.bottom()) {

        painter.setPen(
            QPen(
                mutedColor,
                1,
                Qt::DashLine
                )
            );

        painter.drawLine(
            QPointF(
                plotRect.left(),
                goalY
                ),
            QPointF(
                plotRect.right(),
                goalY
                )
            );
    }

    // ── X-Achse ──────────────────────────────────────────────────────────

    painter.setPen(
        QPen(
            mutedColor,
            1
            )
        );

    painter.drawLine(
        QPointF(
            plotRect.left(),
            plotRect.bottom()
            ),
        QPointF(
            plotRect.right(),
            plotRect.bottom()
            )
        );

    // ── X-Achsen-Einteilung ─────────────────────────────────────────────

    const qint64 tickDays =
        niceDateTickDays(
            totalDays
            );

    // Zeichnet einen einzelnen Datumstick.
    auto drawXTick = [&](const QDate &date) {

        const qreal x =
            xForDate(date);

        painter.setPen(
            QPen(
                mutedColor,
                1
                )
            );

        // Vertikaler Strich an der X-Achse.
        painter.drawLine(
            QPointF(
                x,
                plotRect.bottom()
                ),
            QPointF(
                x,
                plotRect.bottom() + 5
                )
            );

        QString dateText;

        // Bei längeren Zeiträumen zeigen wir zusätzlich das Jahr.
        if (totalDays > 240) {

            dateText =
                date.toString(
                    "dd.MM.yy"
                    );

        } else {

            dateText =
                date.toString(
                    "dd.MM."
                    );
        }

        const QRectF dateLabelRect(
            x - 30,
            plotRect.bottom() + 6,
            60,
            bottomPadding - 6
            );

        painter.drawText(
            dateLabelRect,
            Qt::AlignHCenter | Qt::AlignTop,
            dateText
            );
    };

    // Der erste Tag wird immer angezeigt.
    drawXTick(firstDate);

    // Weitere sinnvolle Datumsmarkierungen.
    QDate currentTickDate =
        firstDate.addDays(
            tickDays
            );

    while (currentTickDate < lastDate) {

        drawXTick(
            currentTickDate
            );

        currentTickDate =
            currentTickDate.addDays(
                tickDays
                );
    }

    // Der letzte Tag wird immer angezeigt.
    if (lastDate != firstDate)
        drawXTick(lastDate);

    // ── Verbindungslinie ─────────────────────────────────────────────────

    QPolygonF polyline;

    for (int i = 0; i < points.size(); ++i) {

        polyline << QPointF(
            xForIndex(i),
            yForValue(
                points[i].value
                )
            );
    }

    if (polyline.size() > 1) {

        painter.setPen(
            QPen(
                accentColor,
                2
                )
            );

        painter.drawPolyline(
            polyline
            );
    }

    // ── Messpunkte ───────────────────────────────────────────────────────

    painter.setPen(Qt::NoPen);
    painter.setBrush(accentColor);

    for (const QPointF &point : polyline) {

        painter.drawEllipse(
            point,
            3.5,
            3.5
            );
    }
}