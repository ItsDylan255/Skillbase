#ifndef ROUTINEDIALOG_H
#define ROUTINEDIALOG_H

#include <QDialog>
#include <QMap>
#include <QList>

#include "exercise.h"

class QCompleter;
class QStringListModel;

namespace Ui {
class routineDialog;
}

class RoutineDialog : public QDialog
{
    Q_OBJECT

public:
    // Erstellt eine neue Routine.
    explicit RoutineDialog(
        int hobbyId,
        QWidget *parent = nullptr
        );

    // Öffnet eine bestehende Routine zum Bearbeiten.
    explicit RoutineDialog(
        int hobbyId,
        int routineId,
        QWidget *parent = nullptr
        );


    ~RoutineDialog();

private:
    // Lädt alle nicht archivierten Übungen des aktuellen Hobbys.
    void loadAvailableExercises();

    // Aktualisiert die Übungsauswahl anhand von Suchtext und Kategorie.
    void updateAvailableExercisesList();

    // Lädt die Kategorien des aktuellen Hobbys in den Filter.
    void loadCategories();

    // Fügt die ausgewählte Übung zur Routine hinzu.
    void addSelectedExercise(int exerciseId);

    // Erstellt die sichtbare Zeile einer ausgewählten Übung.
    void createSelectedExerciseRow(
        int exerciseId,
        int durationMinutes = 5
        );

    // Entfernt eine ausgewählte Übung aus der Routine.
    void removeSelectedExerciseRow(QWidget *row);

    // Aktualisiert die Zuordnung zwischen sichtbaren Zeilen und Übungs-IDs.
    void updateSelectedExerciseIds();

    // Verschiebt eine ausgewählte Übung innerhalb der Liste.
    void moveSelectedExerciseRow(QWidget *row, int delta);

    // Prüft die Eingaben und speichert die komplette Routine.
    void saveRoutine();

    // Verarbeitet spezielle Maus-/Tastaturereignisse des Dialogs.
    bool eventFilter(QObject *watched, QEvent *event) override;

    // Lädt eine bestehende Routine inklusive ihrer Übungen
    // und deren gespeicherten Reihenfolge und Dauer.
    void loadRoutineForEditing();

    void showSelectedExerciseChip(const QString &exerciseName);
    void resetExerciseInput();

    Ui::routineDialog *ui;

    // Das Hobby, zu dem die neue Routine gehört.
    int hobbyId = 0;

    // 0 bedeutet: neue Routine erstellen.
    // Eine andere ID bedeutet: bestehende Routine bearbeiten.
    int routineId = 0;

    // Blendet die Pfeil-Buttons der ersten/letzten Zeile aus,
    // damit oben/unten nicht ins Leere zeigt.
    void updateMoveButtonVisibility();

    // Alle nicht archivierten Übungen des aktuellen Hobbys.
    QList<Exercise> availableExercises;

    // Verknüpft jede sichtbare Übungszeile mit ihrer Übungs-ID.
    QMap<QWidget*, int> selectedExerciseIds;

    QCompleter *exerciseCompleter = nullptr;
    QStringListModel *exerciseNamesModel = nullptr;
};

#endif