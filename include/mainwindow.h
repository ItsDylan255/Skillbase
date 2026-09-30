#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QElapsedTimer>
#include <QHash>
#include <QList>
#include <QMainWindow>
#include <QMargins>
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
class QTimer;
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

    // Lädt die Übungen der aktuell ausgeführten Routine
    // und erstellt daraus die auswählbare Liste.
    void loadRoutineExecution();

    void loadDashboardRoutineCards();

    // ── Routine-Ausführung ───────────────────────────────────────────────
    // Öffnet die Ausführungsansicht für eine Routine.
    void showRoutineExecution(int routineId);

    // Verlässt die Ausführungsansicht OHNE zu speichern.
    void leaveRoutineExecution();

    // Speichert die Eingaben und verlässt die Ausführungsansicht.
    void finishRoutineExecution();

    // Schreibt Übungs-Logs und Routine-Log in die Datenbank.
    // Liefert true, wenn mindestens etwas gespeichert wurde.
    bool saveRoutineExecutionResults();

    void selectRoutineExecutionExercise(int index);
    void updateRoutineExecutionTimerDisplay();
    void updateRoutineExecutionNavigation();
    void setRoutineExecutionRunning(bool running);

    // ── Zustand ──────────────────────────────────────────────────────────
    Ui::MainWindow *ui;
    QString currentHobby;
    int currentHobbyId = 0;

    // Übung, deren Detail-/Fortschrittsansicht gerade angezeigt wird.
    // 0, solange die normale Übungsübersicht sichtbar ist.
    int currentDetailExerciseId = 0;

    // Routine, die gerade ausgeführt wird.
    // 0 bedeutet, dass momentan keine Routine ausgeführt wird.
    int currentExecutionRoutineId = 0;

    // Daten einer Übung innerhalb der laufenden Routine.
    struct RoutineExecutionItem
    {
        int exerciseId = 0;
        QString name;
        QString description;
        QString unit;
        double currentValue = 0.0;
        double goal = 0.0;
        int durationSeconds = 0;

        // Millisekunden, damit der Fortschrittsbalken flüssig läuft.
        int remainingMs = 0;

        // Tatsächlich gelaufene Zeit. Stopp setzt nur den Countdown
        // zurück, die bereits gelaufene Zeit bleibt erhalten.
        int elapsedMs = 0;

        // Bereits eingetippter neuer Wert (bleibt beim Wechseln erhalten).
        QString enteredText;
    };

    QList<RoutineExecutionItem> routineExecutionItems;
    int routineExecutionIndex = -1;
    QTimer *routineExecutionTimer = nullptr;

    // Misst die echte Zeit zwischen zwei Timer-Ticks (kein Driften).
    QElapsedTimer routineExecutionClock;

    // Die Hobby-Seiten haben seitlich Rand. Für die Ausführung wird er auf 0
    // gesetzt, damit die Trennlinie über die ganze Breite geht.
    QMargins routineExecutionSavedMargins;
    bool routineExecutionMarginsOverridden = false;

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