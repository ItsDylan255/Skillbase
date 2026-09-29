#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
#include <QMainWindow>
#include <QPoint>
#include <QSoundEffect>
#include <QString>

// Vorwärtsdeklarationen: Dieser Header verwendet Qt-Widgets und Modelltypen
// nur per Zeiger oder Referenz. Die vollständigen Header werden erst in
// mainwindow.cpp eingebunden, damit Änderungen daran nicht jede Datei
// neu kompilieren lassen, die mainwindow.h einbindet.
QT_BEGIN_NAMESPACE
class QCheckBox;
class QDialog;
class QFrame;
class QListWidgetItem;
class QMouseEvent;
class QToolButton;
class QVBoxLayout;

namespace Ui {
class MainWindow;
class ExerciseDialog;
}
QT_END_NAMESPACE

struct Exercise;
struct Goal;
struct Routine;
struct TimelinePhase;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    // ── Übungen ──────────────────────────────────────────────────────────
    void loadExerciseCards();
    void editExercise(int exerciseId);

    // Übungen: Wechsel zwischen Kartenübersicht und Detail-/
    // Fortschrittsansicht innerhalb derselben Seite (kein neues Fenster).
    void showExerciseOverview();
    void showExerciseDetail(int exerciseId);
    void loadExerciseDetail(int exerciseId);

    // ── Verlauf ──────────────────────────────────────────────────────────
    void loadHistory();

    // ── Timeline ─────────────────────────────────────────────────────────
    void loadTimeline();

    // ── Ziele ────────────────────────────────────────────────────────────
    void loadGoalCards();

    // goalId == 0 öffnet den Dialog im Erstellen-Modus (kein Status-/
    // Löschen-Button); ein bestehendes Ziel öffnet ihn im Bearbeiten-Modus.
    void openGoalDialog(int goalId);

    // ── Routinen ─────────────────────────────────────────────────────────
    // Lädt die Routinen des aktuellen Hobbys und verteilt
    // aktive und archivierte Routinen auf die jeweiligen Ansichten.
    void loadRoutineCards();

    // ── Zustand ──────────────────────────────────────────────────────────
    Ui::MainWindow *ui;
    QString currentHobby;
    int currentHobbyId = 0;

    // Übung, deren Detail-/Fortschrittsansicht gerade angezeigt wird.
    // 0, solange die normale Übungsübersicht sichtbar ist.
    int currentDetailExerciseId = 0;

    enum class HistoryFilter
    {
        All,
        Exercises,
        Routines
    };

    HistoryFilter historyFilter =
        HistoryFilter::All;

    // Zustand für das Ziehen einer Ziel-Card per Drag & Drop (nur offene
    // Ziele lassen sich neu sortieren). 0, solange nichts gezogen wird.
    int goalDragCandidateId = 0;
    QPoint goalDragStartPos;
    QSoundEffect *goalCompletedSound = nullptr;
};
#endif // MAINWINDOW_H