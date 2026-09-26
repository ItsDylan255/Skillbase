#ifndef TIMELINEBARWIDGET_H
#define TIMELINEBARWIDGET_H

#include <QWidget>
#include <QColor>
#include <QList>
#include <QDate>
#include <QPair>

#include "timelinephase.h"

class QMouseEvent;

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

    // Feste Breite pro Monat statt Stauchung auf die verfügbare Breite -
    // der Balken meldet seine tatsächlich benötigte Breite nach oben,
    // damit die umgebende QScrollArea bei Bedarf horizontal scrollt.
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    // Wird ausgelöst, wenn der Benutzer auf das Segment einer Phase klickt.
    // Der komplette Balken ist klickbar, nicht nur der Textbereich.
    void phaseClicked(const TimelinePhase &phase);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    // Erster Tag des Monats der frühesten Phase.
    QDate axisStartMonth() const;

    // Anzahl der Monate, die die Achse abdecken muss (mindestens 1).
    int axisMonthCount() const;

    // Position eines Datums in Pixeln relativ zum Achsenstart,
    // auf Basis der festen Monatsbreite (siehe monthPixelWidth).
    qreal xForDate(const QDate &date) const;

    // Berechnet für jede Phase das gezeichnete Segment-Rechteck.
    // Wird sowohl beim Zeichnen als auch beim Hit-Test für Klicks
    // verwendet, damit beides garantiert deckungsgleich bleibt.
    QList<QPair<TimelinePhase, QRectF>> segmentRects() const;

    QList<TimelinePhase> phases;
    QColor hobbyColor;
};

#endif
