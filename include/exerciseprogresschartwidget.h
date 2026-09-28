#ifndef EXERCISEPROGRESSCHARTWIDGET_H
#define EXERCISEPROGRESSCHARTWIDGET_H

#include <QWidget>
#include <QList>
#include <QDateTime>

#include "exerciselog.h"

// Zeigt die zeitliche Entwicklung der geloggten Werte einer Übung als
// kompaktes Punkt-/Liniendiagramm (Bereich "Entwicklung" der
// Übungs-Detail-/Fortschrittsansicht).
//
// Der Wertebereich (Y-Achse) passt sich automatisch an die vorhandenen
// Messwerte an, die Zeitachse (X-Achse) an den Zeitraum zwischen dem
// ersten und letzten Log-Eintrag.
class ExerciseProgressChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExerciseProgressChartWidget(QWidget *parent = nullptr);

    // Übergibt die Log-Einträge, aus denen der Verlauf gezeichnet wird.
    // Die Einträge müssen nicht vorsortiert sein.
    void setLogs(const QList<ExerciseLog> &logs);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct DataPoint
    {
        QDateTime performedAt;
        double value = 0.0;
    };

    QList<DataPoint> points;
};

#endif // EXERCISEPROGRESSCHARTWIDGET_H
