#ifndef TIMELINEBARWIDGET_H
#define TIMELINEBARWIDGET_H

#include <QWidget>
#include <QColor>
#include <QList>

#include "timelinephase.h"

class TimelineBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimelineBarWidget(QWidget *parent = nullptr);

    // Übergibt die Phasen, die im Balken dargestellt werden sollen.
    void setPhases(
        const QList<TimelinePhase> &phases
        );

    // Setzt die Grundfarbe des Hobbys.
    void setHobbyColor(
        const QColor &color
        );

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<TimelinePhase> phases;
    QColor hobbyColor;
};

#endif