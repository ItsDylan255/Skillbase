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

#include "exercise.h"

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
class QLineEdit;
class QTreeWidgetItem;

namespace Ui {
class MainWindow;
class ExerciseDialog;
}
QT_END_NAMESPACE

struct Exercise;
struct Goal;
struct Routine;
struct TimelinePhase;
struct RoadmapStep;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    // Lädt ein Hobby anhand seiner ID: Sidebar-Auswahl markieren,
    // currentHobbyId setzen, Notizen laden, Dashboard aktualisieren,
    // Cards aller Tabs laden.
    void selectHobbyById(int hobbyId);

    // Wird beim App-Start aufgerufen. Lädt das zuletzt geöffnete
    // Hobby aus QSettings; falls das nicht existiert, das erste
    // Hobby; falls gar keins existiert, wird der Willkommens-
    // Bildschirm angezeigt.
    void restoreLastOpenedHobby();
    // ── Übungen ──────────────────────────────────────────────────────────
    void loadExerciseCards();
    void editExercise(int exerciseId);
    // Öffnet den Dialog zum Anlegen eines neuen Hobbys und fügt es
    // der Sidebar-Liste hinzu. Wird sowohl vom "+ hinzufügen"-Eintrag
    // in der hobbyList als auch vom Willkommens-Button aufgerufen.
    void onAddHobby();
    // Übungen: Wechsel zwischen Kartenübersicht und Detail-/
    // Fortschrittsansicht innerhalb derselben Seite (kein neues Fenster).
    // Übergang vom Willkommens-Screen zum ersten Hobby.
    // Blendet den Startscreen aus, wechselt dann auf die Hobby-
    // Ansicht und blendet sie ein. Bei "Animationen reduzieren"
    // passiert der Wechsel sofort.
    void animateToHobby(int hobbyId);
    void showExerciseOverview();
    void showExerciseDetail(int exerciseId);
    void loadExerciseDetail(int exerciseId);
    // Hobby-Einstellungen-Dialog öffnen (Zahnrad unten rechts).
    void openHobbySettingsDialog();

    // Aktualisiert die Roadmap-Sektion im Hobby-Dashboard.
    void refreshHobbyRoadmapSection(int hobbyId);

    // Aktualisiert die Stats-Zeile im Hobby-Dashboard.
    void refreshHobbyStatsSection(int hobbyId);

    // ── Übungs-Ausführung ────────────────────────────────────────────────
    // Öffnet die Ausführungsansicht für eine einzelne Übung.
    void showExerciseExecution(int exerciseId);

    // Verlässt die Ausführungsansicht OHNE zu speichern.
    void leaveExerciseExecution();

    // Speichert die Eingaben und verlässt die Ausführungsansicht.
    void finishExerciseExecution();

    // Schreibt den Übungs-Log in die Datenbank.
    // Liefert true, wenn etwas gespeichert wurde.
    bool saveExerciseExecutionResults();

    // Aktualisiert Timer-Label und Fortschrittsbalken.
    void updateExerciseExecutionTimerDisplay();

    // Startet/stoppt den Timer und passt die Button-Beschriftung an.
    void setExerciseExecutionRunning(bool running);
    // ── Übungsname in der Ausführung klickbar machen ─────────────────────
    // Öffnet die Fortschrittsseite der aktuellen Übung.
    void openCurrentExerciseProgress();
    // ── Timer inline bearbeiten (Routine + Übung) ────────────────────────
    // Klick auf das Timer-Label öffnet ein Edit-Feld an gleicher Stelle.
    void startRoutineExecutionTimerEdit();
    void commitRoutineExecutionTimerEdit();
    void cancelRoutineExecutionTimerEdit();

    void startExerciseExecutionTimerEdit();
    void commitExerciseExecutionTimerEdit();
    void cancelExerciseExecutionTimerEdit();

    // ── Roadmap ──────────────────────────────────────────────────────────

    // Hauptübersicht: Cards für alle Root-Steps.
    void loadRoadmapCards();

    // Detail-Ansicht: Tree eines Root-Steps.
    void loadRoadmapDetail(int rootId);

    // Wechsel zwischen Übersicht und Detail.
    void showRoadmapOverview();
    void showRoadmapDetail(int rootId);

    // Aktiviert/deaktiviert die Aktionsbuttons je nach Auswahl im Tree.
    void updateRoadmapActionButtons();

    // Aktions-Handler (Detail-Ansicht).
    void onRoadmapAddRootStep();          // "+ Neuer Step" in der Übersicht
    void onRoadmapAddChildStep();
    void onRoadmapRenameSelectedStep();
    void onRoadmapToggleSelectedStepDone();
    void onRoadmapDeleteSelectedStep();
    // Neuer Step direkt unter dem Root der aktuell geöffneten Roadmap.
    // Wird vom "+ Step"-Button in der Detail-Ansicht ausgelöst.
    void onRoadmapAddChildOfRootStep();

    // Aktualisiert die Sichtbarkeit des Empty-State-Labels in der
    // Detail-Ansicht, abhängig davon, ob der Suchfilter Treffer hat.
    void updateRoadmapDetailEmptyState();

    // Baut rekursiv einen Tree-Zweig in der Detail-Ansicht auf.
    void buildRoadmapTreeItem(
        QTreeWidgetItem *parentItem,
        int parentId,
        const QList<struct RoadmapStep> &steps
        );

    // Stern-Handling: markiert einen Root-Step als Haupt-Roadmap.
    void onRoadmapToggleStar(int rootStepId);

    // Positioniert die Overlay-Buttons rechts in der aktuellen Zeile.
    void updateRoadmapRowButtons();

    // Aktiviert/deaktiviert die Overlay-Buttons je nach Auswahl.
    void updateRoadmapRowButtonsState();

    // Prüft rekursiv, ob `step` ein Nachkomme von `rootId` ist.
    bool isDescendantOf(
        const struct RoadmapStep &step,
        int rootId,
        const QList<struct RoadmapStep> &allSteps
        );

    // ── Verlauf ──────────────────────────────────────────────────────────
    void loadHistory();

    // ── Übungs-Detailansicht: Startwert inline bearbeiten ───────────────
    // Macht aus dem Startwert-Label ein editierbares Feld, ohne Dialog.
    void startExerciseDetailStartValueEdit();
    void commitExerciseDetailStartValueEdit();
    void cancelExerciseDetailStartValueEdit();

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
        ValueMode valueMode = ValueMode::Progress;

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

    // ── Übungs-Ausführung: Zustand ───────────────────────────────────────
    // Übung, die gerade einzeln ausgeführt wird.
    // 0 bedeutet, dass momentan keine Übung ausgeführt wird.
    int currentExecutionExerciseId = 0;

    // Daten der Übung, die gerade ausgeführt wird.
    struct ExerciseExecutionData
    {
        int exerciseId = 0;
        QString name;
        QString description;
        QString unit;
        double currentValue = 0.0;
        double goal = 0.0;
        ValueMode valueMode = ValueMode::Progress;

        // Der vom Nutzer eingegebene neue Wert.
        QString enteredText;
    };

    ExerciseExecutionData exerciseExecutionData;

    QTimer *exerciseExecutionTimer = nullptr;

    // Misst die echte Zeit zwischen zwei Timer-Ticks (kein Driften).
    QElapsedTimer exerciseExecutionClock;

    // Gesamtzeit, die für diese Ausführung vorgesehen ist.
    // 0 bedeutet: noch keine Zeit gesetzt -> Start/Pause/Stopp deaktiviert.
    int exerciseExecutionTotalMs = 0;

    // Verbleibende Zeit in Millisekunden.
    int exerciseExecutionRemainingMs = 0;

    // Gelaufene Zeit in Millisekunden (wird beim Speichern verwendet).
    int exerciseExecutionElapsedMs = 0;

    // Die Hobby-Seiten haben seitlich Rand. Für die Übungs-Ausführung
    // wird er ebenfalls auf 0 gesetzt.
    QMargins exerciseExecutionSavedMargins;
    bool exerciseExecutionMarginsOverridden = false;

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

    // Sound, der abgespielt wird, wenn ein Routine- oder Übungs-Timer
    // abgelaufen ist. Wird im Konstruktor aus den Ressourcen geladen.
    QSoundEffect *timerCompleteSound = nullptr;

    // Merkt sich, von welchem Tab aus die Übungs-Detailansicht geöffnet wurde.
    // Wird vom Back-Button benutzt, um dorthin zurückzukehren.
    QWidget *exerciseDetailReturnPage = nullptr;

    // Roadmap: zuletzt geladene Steps (flache Liste).
    QList<RoadmapStep> roadmapSteps;

    // In der Detail-Ansicht: vier Overlay-Buttons, die rechts in
    // der Zeile des ausgewählten Steps erscheinen.
    QToolButton *roadmapRowAddChildButton = nullptr;
    QToolButton *roadmapRowRenameButton = nullptr;
    QToolButton *roadmapRowToggleDoneButton = nullptr;
    QToolButton *roadmapRowDeleteButton = nullptr;

    // Buttons für den Filter in der Detail-Ansicht.
    QString roadmapDetailFilterText;

    // Root-Step-ID, deren Detail-Ansicht gerade angezeigt wird.
    // 0 = Übersicht sichtbar.
    int currentRoadmapDetailRootId = 0;

    // Wenn die Fortschrittsseite aus einer laufenden Ausführung geöffnet
    // wurde, merkt sich diese Flag, wohin der Back-Button zurückkehren soll.
    // 0 = keine, 1 = Routine-Ausführung, 2 = Übungs-Ausführung.
    enum class ProgressReturnSource
    {
        None,
        RoutineExecution,
        ExerciseExecution
    };

    ProgressReturnSource progressReturnSource =
        ProgressReturnSource::None;

    // Inline-Bearbeitung der Timer-Labels (Routine + Übung).
    QLineEdit *routineExecutionTimerEdit = nullptr;
    bool routineExecutionTimerEditing = false;

    QLineEdit *exerciseExecutionTimerEdit = nullptr;
    bool exerciseExecutionTimerEditing = false;

    // Inline-Bearbeitung des Startwerts in der Detailansicht.
    // Das Edit-Feld wird zur Laufzeit erzeugt und nur während der
    // Bearbeitung sichtbar geschaltet.
    QLineEdit *exerciseDetailStartValueEdit = nullptr;
    bool exerciseDetailStartValueEditing = false;
    // Baut die aktuelle Hauptziel-Card für das Hobby-Dashboard.
    // Wird als echte Goal-Card gerendert, analog zur Ziele-Seite.
    void renderDashboardGoalCard();

    // Roadmap-Detail: Titel inline bearbeiten
    QLineEdit *roadmapDetailTitleEdit = nullptr;
    bool roadmapDetailTitleEditing = false;

    void startRoadmapDetailTitleEdit();
    void commitRoadmapDetailTitleEdit();
    void cancelRoadmapDetailTitleEdit();

};
#endif // MAINWINDOW_H