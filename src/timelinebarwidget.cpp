#include "timelinebarwidget.h"

#include <QPainter>
#include <QDate>
#include <algorithm>

TimelineBarWidget::TimelineBarWidget(QWidget *parent)
    : QWidget(parent)
{
    // Der Balken soll seinen Inhalt selbst zeichnen.
    setMinimumHeight(60);
}

void TimelineBarWidget::setPhases(
    const QList<TimelinePhase> &newPhases
    )
{
    phases = newPhases;

    // Nach einer Änderung muss Qt den Balken neu zeichnen.
    update();
}

void TimelineBarWidget::setHobbyColor(
    const QColor &color
    )
{
    hobbyColor = color;

    update();
}

void TimelineBarWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (phases.isEmpty())
        return;

    // Wir bestimmen den gesamten Zeitraum der Timeline.
    QDate timelineStart =
        QDate::fromString(
            phases.first().startDate,
            "yyyy-MM-dd"
            );

    QDate timelineEnd =
        QDate::fromString(
            phases.first().endDate,
            "yyyy-MM-dd"
            );

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

        if (start < timelineStart)
            timelineStart = start;

        if (end > timelineEnd)
            timelineEnd = end;
    }

    const int totalDays =
        timelineStart.daysTo(timelineEnd) + 1;

    if (totalDays <= 0)
        return;

    const QRect barRect =
        rect().adjusted(0, 0, -1, -1);

    // Jede Phase bekommt einen Anteil der Gesamtbreite,
    // der ihrer tatsächlichen Dauer entspricht.
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

        const int daysBefore =
            timelineStart.daysTo(start);

        const int duration =
            start.daysTo(end) + 1;

        const int x =
            barRect.left()
            + (daysBefore * barRect.width()) / totalDays;

        const int width =
            (duration * barRect.width()) / totalDays;

        QRect phaseRect(
            x,
            barRect.top(),
            width,
            barRect.height()
            );

        painter.fillRect(
            phaseRect,
            hobbyColor
            );
    }
}