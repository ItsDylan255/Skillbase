#ifndef ROUTINECARDWIDGET_H
#define ROUTINECARDWIDGET_H

#include <QFrame>

#include "routine.h"

class QLabel;
class QPushButton;

class RoutineCardWidget : public QFrame
{
    Q_OBJECT

public:
    explicit RoutineCardWidget(
        const Routine &routine,
        QWidget *parent = nullptr
        );

signals:
    // Wird ausgelöst, wenn die Routine bearbeitet werden soll.
    void editRequested(int routineId);

    // Wird ausgelöst, wenn die Routine ausgeführt werden soll.
    void executeRequested(int routineId);

    // Wird ausgelöst, wenn der Archivstatus geändert werden soll.
    void archiveRequested(int routineId, bool archived);

private:
    // Baut den sichtbaren Inhalt der Karte auf.
    void setupUi();

    // Aktualisiert den Text und Zustand des Archiv-Buttons.
    void updateArchiveButton();

    Routine routine;

    QLabel *nameLabel = nullptr;
    QLabel *descriptionLabel = nullptr;
    QLabel *lastExecutionLabel = nullptr;

    QPushButton *favoriteButton = nullptr;
    QPushButton *archiveButton = nullptr;
    QPushButton *editButton = nullptr;
    QPushButton *executeButton = nullptr;
};

#endif