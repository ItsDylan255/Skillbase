#ifndef HISTORYENTRYWIDGET_H
#define HISTORYENTRYWIDGET_H

#include <QWidget>
#include <QString>
#include <QMouseEvent>

namespace Ui {
class HistoryEntryWidget;
}

class HistoryEntryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryEntryWidget(QWidget *parent = nullptr);
    ~HistoryEntryWidget() override;

    // Füllt den Eintrag mit den Daten einer ausgeführten Aktivität.
    // Routine und Übung werden dabei unterschiedlich dargestellt.
    void setData(
        bool isRoutine,
        const QString &title,
        const QString &valueText,
        const QString &durationText,
        const QString &timeText
        );

signals:
    // Das Signal wird ausgelöst, wenn der Nutzer auf den gesamten Eintrag klickt.
    // Die eigentliche Reaktion darauf wird später außerhalb des Widgets festgelegt.
    void clicked();

protected:
    // Wir fangen den Mausklick auf dem gesamten Widget ab,
    // damit nicht jedes einzelne Label separat klickbar gemacht werden muss.
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::HistoryEntryWidget *ui;
};

#endif