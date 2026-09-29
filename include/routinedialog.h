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
    explicit RoutineDialog(
        int hobbyId,
        QWidget *parent = nullptr
        );

    ~RoutineDialog();

private:
    // Lädt alle nicht archivierten Übungen des aktuellen Hobbys
    // in die Liste zur Auswahl.
    void loadAvailableExercises();

    // Aktualisiert die Liste der verfügbaren Übungen anhand
    // des aktuellen Suchtexts und der ausgewählten Kategorie.
    void updateAvailableExercisesList();

    // Lädt die Kategorien des aktuellen Hobbys in den Filter.
    void loadCategories();

    // Fügt die ausgewählte Übung zur Routine hinzu.
    void addSelectedExercise(int exerciseId);

    // Erstellt eine sichtbare Zeile für eine ausgewählte Übung.
    void createSelectedExerciseRow(int exerciseId);

    // Entfernt eine ausgewählte Übung aus der Liste.
    void removeSelectedExerciseRow(QWidget *row);

    // Aktualisiert die Verknüpfung zwischen Listeneinträgen und Übungen,
    // nachdem die Reihenfolge per Drag & Drop geändert wurde.
    void updateSelectedExerciseIds();

    // Prüft die Eingaben und speichert die komplette Routine
    // inklusive Übungen, Reihenfolge und Dauer.
    void saveRoutine();

    Ui::routineDialog *ui;

    // Das Hobby, zu dem die neue Routine gehört.
    int hobbyId = 0;

    void moveSelectedExerciseRow(QWidget *row, int delta);

    bool eventFilter(QObject *watched, QEvent *event) override;

    QList<Exercise> availableExercises;

    // Verknüpft jede sichtbare Zeile mit ihrer Übungs-ID.
    QMap<QWidget*, int> selectedExerciseIds;

    QCompleter *exerciseCompleter = nullptr;
    QStringListModel *exerciseNamesModel = nullptr;
};

#endif