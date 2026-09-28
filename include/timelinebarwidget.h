#ifndef TIMELINEBARWIDGET_H
#define TIMELINEBARWIDGET_H

#include <QWidget>
#include <QColor>
#include <QList>
#include <QDate>
#include <QPair>

#include "timelinephase.h"

class QMouseEvent;
class QEvent;
class QShowEvent;

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

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    // Wird ausgelöst, wenn der Benutzer auf das Segment einer Phase klickt.
    void phaseClicked(const TimelinePhase &phase);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    // Beobachtet die Größe des sichtbaren Bereichs der QScrollArea.
    bool eventFilter(
        QObject *watched,
        QEvent *event
        ) override;

    // Wird aufgerufen, sobald das Widget sichtbar wird.
    void showEvent(QShowEvent *event) override;

private:
    QDate axisStartMonth() const;
    int axisMonthCount() const;

    // Berechnet die Position eines Datums innerhalb der Timeline.
    qreal xForDate(const QDate &date) const;

    QList<QPair<TimelinePhase, QRectF>> segmentRects() const;

    // Prüft, ob die Phase den heutigen Tag enthält.
    bool isCurrentPhase(const TimelinePhase &phase) const;

    // Aktualisiert die Breite der Timeline anhand des sichtbaren Bereichs.
    void updateTimelineWidth();

    // Scrollt die übergeordnete QScrollArea so,
    // dass der heutige Monat sichtbar ist.
    void scrollToToday();

    // Berechnet die Breite eines Monats.
    qreal monthPixelWidth() const;

    // Zeichnet die Markierung für den heutigen Tag.
    void drawTodayMarker(
        QPainter &painter
        ) const;

    // Die Anzahl der Monate, die gleichzeitig sichtbar sein sollen.
    static constexpr int VisibleMonthCount = 12;

    // Fallback, falls die ScrollArea beim ersten Berechnen
    // noch keine gültige Breite besitzt.
    static constexpr int DefaultMonthPixelWidth = 64;

    QList<TimelinePhase> phases;
    QColor hobbyColor;

    // Tatsächliche Breite des sichtbaren Bereichs der QScrollArea.
    int viewportWidth = 0;
};

#endif