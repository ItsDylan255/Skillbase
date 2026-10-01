#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_category_management_dialog.h"
#include "ui_exercise_dialog.h"
#include "ui_goal_dialog.h"

#include "categoryrepository.h"
#include "exerciselogrepository.h"
#include "exerciserepository.h"
#include "goal.h"
#include "goalrepository.h"
#include "hobbynoterepository.h"
#include "hobbyrepository.h"
#include "routine.h"
#include "routinerepository.h"
#include "timelinephaserepository.h"
#include "roadmaprepository.h"

#include "exerciseprogresschartwidget.h"
#include "routinedialog.h"
#include "timelinebarwidget.h"
#include "timelinephasedialog.h"
#include "clickablelabel.h"
#include "roadmapminitreewidget.h"
#include "roadmaptreedelegate.h"
#include "roadmaptreewidget.h"
#include <QResizeEvent>
#include <QScrollBar>


#include <QBrush>
#include <QFont>
#include <QGridLayout>
#include <QScrollArea>
#include <QSizePolicy>
#include <QApplication>
#include <QCheckBox>
#include <QColor>
#include <QComboBox>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QHash>
#include <QIcon>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QLocale>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSet>
#include <QSignalBlocker>
#include <QSpacerItem>
#include <QTimer>
#include <QToolButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <algorithm>
#include <cmath>
#include <utility>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTreeWidgetItem>

namespace {


void refreshHobbyDashboardOverview(Ui::MainWindow *ui, int hobbyId)
{
    if (hobbyId == 0)
        return;

    const QDate today = QDate::currentDate();

    // ── Aktuelle Phase ───────────────────────────────────────────────────
    const QList<TimelinePhase> phases =
        TimelinePhaseRepository::getForHobby(hobbyId);

    bool hasActivePhase = false;
    TimelinePhase activePhase;

    for (const TimelinePhase &phase : phases) {

        const QDate start = QDate::fromString(phase.startDate, "yyyy-MM-dd");
        const QDate end   = QDate::fromString(phase.endDate, "yyyy-MM-dd");

        if (!start.isValid() || !end.isValid())
            continue;

        if (today >= start && today <= end) {
            activePhase    = phase;
            hasActivePhase = true;
            break;
        }
    }

    if (hasActivePhase) {

        ui->hobbyPhaseNameLabel->setText(activePhase.name);

        const QDate start = QDate::fromString(activePhase.startDate, "yyyy-MM-dd");
        const QDate end   = QDate::fromString(activePhase.endDate, "yyyy-MM-dd");

        const int daysLeft = today.daysTo(end);

        ui->hobbyPhaseCountdownLabel->setText(
            daysLeft > 0
                ? QString("Noch %1 Tage").arg(daysLeft)
                : "Endet heute"
            );

        const int totalDays = start.daysTo(end);

        int percent = 0;

        if (totalDays > 0) {
            percent = static_cast<int>(std::round(
                start.daysTo(today) * 100.0 / totalDays
                ));
        }

        ui->hobbyPhaseProgressBar->setValue(std::clamp(percent, 0, 100));

    } else {

        ui->hobbyPhaseNameLabel->setText("Keine aktive Phase");
        ui->hobbyPhaseCountdownLabel->setText(QString());
        ui->hobbyPhaseProgressBar->setValue(0);
    }

    // ── Aktuelles Ziel ───────────────────────────────────────────────────
    //
    // Das Dashboard zeigt ausschließlich das Ziel, das vom Benutzer
    // ausdrücklich als aktuelles Hauptziel markiert wurde.
    //
    // Es wird bewusst kein Ziel automatisch anhand der Deadline ausgewählt.
    // Dadurch bleibt die Auswahl des Hauptziels vollständig beim Benutzer.

    const QList<Goal> goals =
        GoalRepository::getForHobby(hobbyId);

    bool hasCurrentGoal = false;
    Goal currentGoal;

    for (const Goal &goal : goals) {

        if (goal.isDone())
            continue;

        if (goal.isCurrent) {
            currentGoal = goal;
            hasCurrentGoal = true;
            break;
        }
    }

    if (hasCurrentGoal) {

        ui->hobbyGoalNameLabel->setText(
            currentGoal.title
            );

        const QDate deadline =
            currentGoal.deadline.isEmpty()
                ? QDate()
                : QDate::fromString(
                      currentGoal.deadline,
                      "yyyy-MM-dd"
                      );

        ui->hobbyGoalDeadlineLabel->setText(
            deadline.isValid()
                ? "Bis " + deadline.toString("dd.MM.")
                : QString()
            );

    } else {

        ui->hobbyGoalNameLabel->setText(
            "Kein Hauptziel festgelegt"
            );

        ui->hobbyGoalDeadlineLabel->setText(
            QString()
            );
    }

    // ── Statistik ────────────────────────────────────────────────────────
    //
    // "Übungen" zählt alle geloggten Ausführungen (nicht die Anzahl
    // angelegter Übungen), "Sessions" die Anzahl unterschiedlicher Tage,
    // an denen mindestens eine Ausführung stattgefunden hat.
    const QList<Exercise> exercises = ExerciseRepository::getForHobby(hobbyId);

    int totalExecutions      = 0;
    int totalDurationSeconds = 0;
    QSet<QString> sessionDays;

    for (const Exercise &exercise : exercises) {

        const QList<ExerciseLog> logs =
            ExerciseLogRepository::getForExercise(exercise.id);

        totalExecutions += logs.size();

        for (const ExerciseLog &log : logs) {

            totalDurationSeconds += log.durationSeconds;

            const QString day = log.performedAt.left(10); // "yyyy-MM-dd"
            if (!day.isEmpty())
                sessionDays.insert(day);
        }
    }

    ui->hobbyStatExercisesValueLabel->setText(
        QString::number(totalExecutions)
        );

    const int totalMinutes = totalDurationSeconds / 60;

    ui->hobbyStatTimeValueLabel->setText(
        totalMinutes >= 60
            ? QString("%1 h %2 min").arg(totalMinutes / 60).arg(totalMinutes % 60)
            : QString("%1 min").arg(totalMinutes)
        );

    ui->hobbyStatSessionsValueLabel->setText(
        QString::number(sessionDays.size())
        );

    ui->hobbyStatGoalsValueLabel->setText(
        QString::number(goals.size())
        );
}

// Formatiert Sekunden als "MM:SS" für den Routine-Countdown.
QString formatCountdown(int seconds)
{
    seconds = std::max(0, seconds);

    return QString("%1:%2")
        .arg(seconds / 60, 2, 10, QChar('0'))
        .arg(seconds % 60, 2, 10, QChar('0'));
}

// Gleiche Darstellung wie auf den Übungs-Cards: "–", wenn kein Wert existiert.
QString formatValueWithUnit(double value, const QString &unit)
{
    if (value == 0.0)
        return QStringLiteral("–");

    QString text = QString::number(value, 'g', 15);

    if (!unit.isEmpty())
        text += " " + unit;

    return text;
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ── Übungsname in der Ausführung klickbar machen ─────────────────────
    //
    // Klick auf den Übungsnamen in der Routine- oder Übungs-Ausführung
    // öffnet die Fortschrittsseite. Der Timer wird dabei pausiert.
    // Der Back-Button führt zurück zur Ausführung.

    connect(
        qobject_cast<ClickableLabel *>(
            ui->routineExecutionExerciseNameLabel),
        &ClickableLabel::clicked,
        this,
        [this]() {

            if (currentExecutionRoutineId == 0)
                return;

            if (routineExecutionIndex < 0
                || routineExecutionIndex >= routineExecutionItems.size())
                return;

            // Timer pausieren.
            setRoutineExecutionRunning(false);

            progressReturnSource =
                ProgressReturnSource::RoutineExecution;

            const int exerciseId =
                routineExecutionItems.at(routineExecutionIndex).exerciseId;

            showExerciseDetail(exerciseId);
        }
        );

    connect(
        qobject_cast<ClickableLabel *>(
            ui->exerciseExecutionExerciseNameLabel),
        &ClickableLabel::clicked,
        this,
        [this]() {

            if (currentExecutionExerciseId == 0)
                return;

            // Timer pausieren.
            setExerciseExecutionRunning(false);

            progressReturnSource =
                ProgressReturnSource::ExerciseExecution;

            showExerciseDetail(currentExecutionExerciseId);
        }
        );

    // Die drei Dashboard-Routinen-Karten sollen immer
    // gleich breit sein, egal was der Qt Designer mit der .ui macht.
    for (int c = 0; c < 3; ++c)
        ui->hobbyCurrentRoutinesLayout->setColumnStretch(c, 1);

    // ── Routine-Ausführung ───────────────────────────────────────────────
    routineExecutionTimer = new QTimer(this);
    // 16 ms entsprechen etwa 60 Aktualisierungen pro Sekunde.
    routineExecutionTimer->setTimerType(Qt::PreciseTimer);
    routineExecutionTimer->setInterval(16);

    // Feine Auflösung (Promille) für einen flüssigen Balken.
    ui->routineExecutionProgressBar->setRange(0, 1000);
    ui->routineExecutionProgressBar->setValue(1000);

    // Stopp-Button bekommt ein Icon statt Text.
    ui->routineExecutionStopButton->setText(QString());
    ui->routineExecutionStopButton->setIcon(
        QIcon(QStringLiteral(":/icons/stop.svg"))
        );

    // Start/Pause-Button startet mit dem Play-Icon.
    ui->routineExecutionStartPauseButton->setText(QString());
    ui->routineExecutionStartPauseButton->setIcon(
        QIcon(QStringLiteral(":/icons/play.svg"))
        );

    // Die Start/Pause/Stopp-Buttons sollen keinen Windows-Fokusrahmen
    // bekommen, weil sie sonst eine kleine blaue Markierung zeigen.
    ui->routineExecutionStartPauseButton->setFocusPolicy(Qt::NoFocus);
    ui->routineExecutionStopButton->setFocusPolicy(Qt::NoFocus);

    // ── Übungs-Ausführung ────────────────────────────────────────────────
    exerciseExecutionTimer = new QTimer(this);
    exerciseExecutionTimer->setTimerType(Qt::PreciseTimer);
    exerciseExecutionTimer->setInterval(16);

    // Feine Auflösung (Promille) für einen flüssigen Balken.
    ui->exerciseExecutionProgressBar->setRange(0, 1000);
    ui->exerciseExecutionProgressBar->setValue(0);

    ui->exerciseExecutionStopButton->setText(QString());
    ui->exerciseExecutionStopButton->setIcon(
        QIcon(QStringLiteral(":/icons/stop.svg"))
        );

    ui->exerciseExecutionStartPauseButton->setText(QString());
    ui->exerciseExecutionStartPauseButton->setIcon(
        QIcon(QStringLiteral(":/icons/play.svg"))
        );

    // Die Start/Pause/Stopp-Buttons sollen keinen Windows-Fokusrahmen
    // bekommen, weil sie sonst eine kleine blaue Markierung zeigen.
    ui->exerciseExecutionStartPauseButton->setFocusPolicy(Qt::NoFocus);
    ui->exerciseExecutionStopButton->setFocusPolicy(Qt::NoFocus);

    // Ausgeblendete Pfeile behalten ihren Platz, damit die Übungsleiste
    // beim Wechsel zwischen erster/mittlerer/letzter Übung nicht springt.
    for (QPushButton *arrow : {ui->routineExecutionPreviousButton,
                               ui->routineExecutionNextButton}) {
        QSizePolicy policy = arrow->sizePolicy();
        policy.setRetainSizeWhenHidden(true);
        arrow->setSizePolicy(policy);
    }

    // Nur Zahlen mit Punkt oder Komma.
    ui->routineExecutionExerciseValueEdit->setValidator(
        new QRegularExpressionValidator(
            QRegularExpression(QStringLiteral("[0-9]*[.,]?[0-9]*")),
            this));

    // ── Timer-Labels inline bearbeitbar machen ──────────────────────────
    //
    // Klick auf das jeweilige Timer-Label öffnet ein Edit-Feld an
    // gleicher Stelle. Enter/Fokus-Verlust übernimmt die Zeit,
    // Escape verwirft sie.

    // Routine-Timer
    ui->routineExecutionTimerLabel->setCursor(Qt::PointingHandCursor);
    ui->routineExecutionTimerLabel->setToolTip(
        "Zeit antippen, um sie zu ändern");
    ui->routineExecutionTimerLabel->installEventFilter(this);

    routineExecutionTimerEdit = new QLineEdit(
        ui->routineExecutionTimerLabel->parentWidget());

    routineExecutionTimerEdit->setObjectName(
        "routineExecutionTimerEdit");

    routineExecutionTimerEdit->setAlignment(Qt::AlignCenter);
    routineExecutionTimerEdit->setVisible(false);
    routineExecutionTimerEdit->installEventFilter(this);

    // Übungs-Timer
    ui->exerciseExecutionTimerLabel->setCursor(Qt::PointingHandCursor);
    ui->exerciseExecutionTimerLabel->setToolTip(
        "Zeit antippen, um sie zu ändern");
    ui->exerciseExecutionTimerLabel->installEventFilter(this);

    exerciseExecutionTimerEdit = new QLineEdit(
        ui->exerciseExecutionTimerLabel->parentWidget());

    exerciseExecutionTimerEdit->setObjectName(
        "exerciseExecutionTimerEdit");

    exerciseExecutionTimerEdit->setAlignment(Qt::AlignCenter);
    exerciseExecutionTimerEdit->setVisible(false);
    exerciseExecutionTimerEdit->installEventFilter(this);

    connect(routineExecutionTimer, &QTimer::timeout, this, [this]() {

        if (routineExecutionIndex < 0
            || routineExecutionIndex >= routineExecutionItems.size()) {
            setRoutineExecutionRunning(false);
            return;
        }

        RoutineExecutionItem &item =
            routineExecutionItems[routineExecutionIndex];

        // Echte vergangene Zeit seit dem letzten Tick verwenden,
        // damit der Countdown nicht driftet.
        const int deltaMs = static_cast<int>(
            std::min<qint64>(routineExecutionClock.restart(), 1000));

        const int usedMs = std::min(deltaMs, item.remainingMs);

        item.remainingMs -= usedMs;
        item.elapsedMs   += usedMs;
        updateRoutineExecutionTimerDisplay();

        if (item.remainingMs <= 0) {
            setRoutineExecutionRunning(false);

            if (timerCompleteSound)
                timerCompleteSound->play();
        }
    });

    connect(ui->routineExecutionStartPauseButton, &QPushButton::clicked,
            this, [this]() {

                if (routineExecutionIndex < 0
                    || routineExecutionIndex >= routineExecutionItems.size())
                    return;

                RoutineExecutionItem &item =
                    routineExecutionItems[routineExecutionIndex];

                // Läuft gerade -> pausieren.
                if (routineExecutionTimer->isActive()) {
                    setRoutineExecutionRunning(false);
                    return;
                }

                // Keine Dauer -> nichts tun.
                if (item.durationSeconds <= 0)
                    return;

                // Wenn die Zeit schon abgelaufen ist, starten wir wieder
                // von voller Zeit (nicht bei 0 stehen bleiben).
                if (item.remainingMs <= 0)
                    item.remainingMs = item.durationSeconds * 1000;

                setRoutineExecutionRunning(true);
            });

    connect(ui->routineExecutionStopButton, &QPushButton::clicked,
            this, [this]() {

        if (routineExecutionIndex < 0
            || routineExecutionIndex >= routineExecutionItems.size())
            return;

        RoutineExecutionItem &item =
            routineExecutionItems[routineExecutionIndex];

        // Stopp setzt den Countdown zurück (gelaufene Zeit bleibt gezählt).
        item.remainingMs = item.durationSeconds * 1000;
        setRoutineExecutionRunning(false);
    });

    connect(ui->routineExecutionPreviousButton, &QPushButton::clicked,
            this, [this]() {
        selectRoutineExecutionExercise(routineExecutionIndex - 1);
    });

    connect(ui->routineExecutionNextButton, &QPushButton::clicked,
            this, [this]() {
        selectRoutineExecutionExercise(routineExecutionIndex + 1);
    });

    // Der neue Wert wird pro Übung gemerkt, damit er beim Hin- und
    // Herwechseln nicht verloren geht.
    connect(ui->routineExecutionExerciseValueEdit, &QLineEdit::textEdited,
            this, [this](const QString &text) {

        if (routineExecutionIndex >= 0
            && routineExecutionIndex < routineExecutionItems.size()) {
            routineExecutionItems[routineExecutionIndex].enteredText = text;
        }
    });

    // Zurück verwirft die Eingaben, "Routine beenden" speichert sie.
    connect(ui->routineExecutionBackButton, &QPushButton::clicked,
            this, [this]() {
        leaveRoutineExecution();
    });

    connect(ui->routineExecutionFinishButton, &QPushButton::clicked,
            this, [this]() {
        finishRoutineExecution();
    });

    // ── Übungs-Ausführung: Timer-Tick ────────────────────────────────────
    connect(exerciseExecutionTimer, &QTimer::timeout, this, [this]() {

        if (currentExecutionExerciseId == 0) {
            setExerciseExecutionRunning(false);
            return;
        }

        // Wenn keine Gesamtzeit gesetzt ist, gibt es nichts zu zählen.
        if (exerciseExecutionTotalMs <= 0) {
            setExerciseExecutionRunning(false);
            return;
        }

        const int deltaMs = static_cast<int>(
            std::min<qint64>(exerciseExecutionClock.restart(), 1000));

        const int usedMs =
            std::min(deltaMs, exerciseExecutionRemainingMs);

        exerciseExecutionRemainingMs -= usedMs;
        exerciseExecutionElapsedMs   += usedMs;
        updateExerciseExecutionTimerDisplay();

        if (exerciseExecutionRemainingMs <= 0) {
            setExerciseExecutionRunning(false);

            if (timerCompleteSound)
                timerCompleteSound->play();

            // Zeit abgelaufen -> Ausführung wird wie "Beenden" behandelt:
            // Ergebnisse speichern und zur Übersicht zurück.
            finishExerciseExecution();
        }

    });

    // ── Übungs-Ausführung: Start/Pause ───────────────────────────────────
    connect(ui->exerciseExecutionStartPauseButton, &QPushButton::clicked,
            this, [this]() {

                if (currentExecutionExerciseId == 0)
                    return;

                // Ohne gesetzte Zeit gibt es nichts zu starten.
                if (exerciseExecutionTotalMs <= 0)
                    return;

                if (exerciseExecutionTimer->isActive()) {
                    setExerciseExecutionRunning(false);
                    return;
                }

                // Nach Ablauf startet ein erneuter Klick wieder bei voller Zeit.
                if (exerciseExecutionRemainingMs <= 0) {
                    exerciseExecutionRemainingMs = exerciseExecutionTotalMs;
                    exerciseExecutionElapsedMs   = 0;
                }

                setExerciseExecutionRunning(true);
            });

    // ── Übungs-Ausführung: Stopp ─────────────────────────────────────────
    connect(ui->exerciseExecutionStopButton, &QPushButton::clicked,
            this, [this]() {

                if (currentExecutionExerciseId == 0)
                    return;

                exerciseExecutionRemainingMs = exerciseExecutionTotalMs;
                exerciseExecutionElapsedMs   = 0;
                setExerciseExecutionRunning(false);
            });

    // ── Übungs-Ausführung: Back (nichts speichern) ───────────────────────
    connect(ui->exerciseExecutionBackButton, &QPushButton::clicked,
            this, [this]() {
                leaveExerciseExecution();
            });

    // ── Übungs-Ausführung: Beenden (speichern) ───────────────────────────
    connect(ui->exerciseExecutionFinishButton, &QPushButton::clicked,
            this, [this]() {
                finishExerciseExecution();
            });

    // ── Übungs-Ausführung: Wert merken ───────────────────────────────────
    connect(ui->exerciseExecutionExerciseValueEdit, &QLineEdit::textEdited,
            this, [this](const QString &text) {
                exerciseExecutionData.enteredText = text;
            });


    // Wechselt der Nutzer mitten in der Ausführung in einen anderen Bereich
    // (Dashboard, Einstellungen, anderer Hobby-Tab), wird die Ausführung
    // nur PAUSIERT - der Zustand bleibt erhalten. Wenn der Nutzer zum
    // selben Tab zurückkehrt, erscheint die Ausführungsseite wieder.
    auto pauseExecutionWhenPageHidden = [this]() {

        if (currentExecutionRoutineId != 0
            && !ui->routinesPage->isVisibleTo(this)) {
            setRoutineExecutionRunning(false);
        }

        if (currentExecutionExerciseId != 0
            && !ui->exerciseExecutionPage->isVisibleTo(this)) {
            setExerciseExecutionRunning(false);
        }
    };

    connect(ui->hobbyPageStack, &QStackedWidget::currentChanged,
            this, pauseExecutionWhenPageHidden);
    connect(ui->pageStack, &QStackedWidget::currentChanged,
            this, pauseExecutionWhenPageHidden);

    // Die beiden Filter wechseln nur die sichtbare Seite
    // innerhalb des Routinen-Stacks.
    connect(
        ui->routinesFilterActiveButton,
        &QToolButton::clicked,
        this,
        [this]() {
            ui->routinesViewStack->setCurrentWidget(
                ui->routinesActivePage
                );
        }
        );

    connect(
        ui->routinesFilterArchiveButton,
        &QToolButton::clicked,
        this,
        [this]() {
            ui->routinesViewStack->setCurrentWidget(
                ui->routinesArchivePage
                );
        }
        );

    // Datumsfilter: Standardmäßig heute. Das Datum wird gesetzt, BEVOR
    // das Signal verbunden wird, damit beim Start kein unnötiges
    // zusätzliches Laden ausgelöst wird.
    ui->historyDateFilterEdit->setDisplayFormat("dd.MM.yyyy");
    ui->historyDateFilterEdit->setCalendarPopup(true);
    ui->historyDateFilterEdit->setDate(QDate::currentDate());

    connect(
        ui->historyDateFilterEdit,
        &QDateEdit::dateChanged,
        this,
        [this](const QDate &) {

            loadHistory();
        }
        );

    goalCompletedSound = new QSoundEffect(this);
    goalCompletedSound->setSource(
        QUrl(QStringLiteral("qrc:/audio/goal-completed.wav"))
        );
    goalCompletedSound->setVolume(0.5);
    timerCompleteSound = new QSoundEffect(this);
    timerCompleteSound->setSource(
        QUrl(QStringLiteral("qrc:/audio/timer_complete.wav"))
        );
    timerCompleteSound->setVolume(1);


    // ── Suchfelder: einheitliches Icon statt Emoji ──────────────────────────
    //
    // Alle Suchfelder im Hobby-Bereich erhalten dasselbe führende Icon
    // (icons/search.svg), statt eines Emoji-Zeichens im Placeholder-Text.
    {
        const QIcon searchIcon(":/icons/search.svg");
        for (QLineEdit *searchField : {
                 ui->exerciseSearchLineEdit,
                 ui->historySearchLineEdit,
                 ui->goalSearchLineEdit,
                 ui->routineSearchLineEdit,
                 ui->roadmapSearchLineEdit,
                 ui->roadmapDetailSearchLineEdit,
             }) {
            searchField->addAction(searchIcon, QLineEdit::LeadingPosition);
        }
    }
    // ── Dashboard: Phase und Ziel klickbar machen ─────────────────────────────
    //
    // Die Bereiche sind QFrames und keine QPushButtons.
    // Deshalb verwenden wir den bestehenden Event-Filter, um Klicks
    // auf die komplette Sektion abzufangen.
    auto installDashboardClickFilter =
        [this](QWidget *section) {

            section->installEventFilter(this);

            // Auch die enthaltenen Widgets bekommen den Filter,
            // damit ein Klick auf Text oder Fortschrittsbalken
            // ebenfalls als Klick auf die gesamte Sektion gilt.
            for (QWidget *child : section->findChildren<QWidget *>())
                child->installEventFilter(this);
        };

    installDashboardClickFilter(ui->hobbyPhaseSection);
    installDashboardClickFilter(ui->hobbyGoalSection);

    // ── Timeline: Klick auf einen Phasen-Balken ─────────────────────────────
    //
    // Phasen werden nur noch als Balken direkt in der Timeline dargestellt
    // (keine separaten Cards mehr). Ein Klick auf ein Segment soll die
    // bestehende Bearbeiten-Funktion für diese Phase öffnen.
    //
    // TODO: Sobald timelinephasedialog.h/.cpp und
    // timelinephaserepository.h/.cpp vorliegen, hier den
    // TimelinePhaseDialog im Bearbeiten-Modus öffnen, die Änderungen über
    // TimelinePhaseRepository speichern (bzw. die Phase löschen) und
    // anschließend loadTimeline() aufrufen.
    if (auto *timelineBar =
        qobject_cast<TimelineBarWidget *>(ui->timelineBarWidget)) {

        connect(
            timelineBar,
            &TimelineBarWidget::phaseClicked,
            this,
            [this](const TimelinePhase &phase) {

                TimelinePhaseDialog dialog(this);

                dialog.setWindowTitle("Phase bearbeiten");

                // Der Löschen-Button ist beim Erstellen einer neuen Phase
                // standardmäßig versteckt. Beim Bearbeiten existiert die Phase
                // bereits und kann deshalb gelöscht werden.
                dialog.deleteButton()->setVisible(true);

                // Die bestehenden Werte der angeklickten Phase
                // werden in den Dialog übernommen.
                dialog.setName(phase.name);

                dialog.setDescription(phase.description);

                dialog.setDateRange(
                    QDate::fromString(phase.startDate, "yyyy-MM-dd"),
                    QDate::fromString(phase.endDate, "yyyy-MM-dd")
                    );

                // Speichern wird hier bewusst selbst behandelt.
                // Der Dialog soll sich nur schließen, wenn das Update
                // tatsächlich erfolgreich war.
                connect(
                    dialog.saveButton(),
                    &QPushButton::clicked,
                    &dialog,
                    [&dialog, this, phase]() {

                        // Zuerst die Eingaben im Dialog prüfen.
                        if (!dialog.validateInput())
                            return;

                        // Die Änderungen in der Datenbank speichern.
                        const bool success =
                            TimelinePhaseRepository::update(
                                phase.id,
                                dialog.name(),
                                dialog.description(),
                                dialog.startDate().toString("yyyy-MM-dd"),
                                dialog.endDate().toString("yyyy-MM-dd")
                                );

                        // Bei einem Fehler bleibt derselbe Dialog geöffnet.
                        if (!success) {

                            QMessageBox::warning(
                                &dialog,
                                "Phase konnte nicht gespeichert werden",
                                "Die Phase überschneidet sich mit einer "
                                "bereits vorhandenen Phase oder konnte "
                                "nicht gespeichert werden."
                                );

                            return;
                        }

                        // Nur bei erfolgreichem Speichern schließen.
                        dialog.accept();
                    }
                    );
                connect(
                    dialog.deleteButton(),
                    &QPushButton::clicked,
                    &dialog,
                    [&dialog, this, phase]() {

                        // Das Löschen ist eine irreversible Aktion.
                        // Deshalb muss der Benutzer es zuerst bestätigen.
                        const auto answer =
                            QMessageBox::question(
                                &dialog,
                                "Phase löschen",
                                QString("Möchtest du die Phase „%1“ wirklich löschen?")
                                    .arg(phase.name),
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No
                                );

                        if (answer != QMessageBox::Yes)
                            return;

                        // Erst nach der Bestätigung wird die Phase
                        // tatsächlich aus der Datenbank entfernt.
                        if (!TimelinePhaseRepository::remove(phase.id)) {

                            QMessageBox::warning(
                                &dialog,
                                "Löschen fehlgeschlagen",
                                "Die Phase konnte nicht gelöscht werden."
                                );

                            return;
                        }

                        // Nach erfolgreichem Löschen schließen wir den Dialog.
                        dialog.accept();
                    }
                    );

                // Der Dialog bleibt geöffnet, bis der Benutzer
                // entweder abbricht oder erfolgreich speichert.
                if (dialog.exec() != QDialog::Accepted)
                    return;

                // Nach erfolgreichem Speichern die Timeline neu laden.
                loadTimeline();
                refreshHobbyDashboardOverview(ui, currentHobbyId);
            }
            );


    }

    connect(
        ui->historySearchLineEdit,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            loadHistory();
        }
        );

    for (const Hobby &hobby : HobbyRepository::getAll()) {
        QListWidgetItem *item = new QListWidgetItem(hobby.name);
        item->setData(Qt::UserRole, hobby.id);
        ui->hobbyList->addItem(item);
    }
    QListWidgetItem *addHobbyItem = new QListWidgetItem("+ hinzufügen");
    addHobbyItem->setFlags(addHobbyItem->flags() & ~Qt::ItemIsSelectable);
    ui->hobbyList->addItem(addHobbyItem);

    ui->pageStack->setCurrentWidget(ui->dashboardPage);

    // ── Sidebar-Navigation ──────────────────────────────────────────────────

    connect(ui->dashboardButton, &QPushButton::clicked, this, [this]() {
        ui->hobbyList->clearSelection();
        ui->pageStack->setCurrentWidget(ui->dashboardPage);
    });


    connect(ui->settingsButton, &QPushButton::clicked, this, [this]() {
        ui->hobbyList->clearSelection();
        ui->pageStack->setCurrentWidget(ui->settingsPage);
    });

    // ── Hobby-Tab-Navigation ────────────────────────────────────────────────
    // Die Hobby-Tabs verhalten sich wie eine feste Navigation:
    // Immer genau ein Tab bleibt ausgewählt.
    // Ein erneuter Klick auf den bereits aktiven Tab darf ihn nicht abwählen.
    const QList<QToolButton*> hobbyTabs = {
        ui->dashboardTab,
        ui->routinesTab,
        ui->exercisesTab,
        ui->historyTab,
        ui->goalsTab,
        ui->roadmapTab
    };

    for (QToolButton *tab : hobbyTabs) {
        tab->setCheckable(true);
        tab->setAutoExclusive(true);
    }
    // Aktiviert den ausgewählten Hobby-Tab.
    // Der aktuelle Tab bleibt dadurch auch bei einem erneuten Klick markiert.
    auto selectHobbyTab = [this](QToolButton *tab, QWidget *page) {
        tab->setChecked(true);
        ui->hobbyPageStack->setCurrentWidget(page);
    };


    connect(ui->dashboardTab, &QToolButton::clicked, this, [this, selectHobbyTab]() {
        selectHobbyTab(
            ui->dashboardTab,
            ui->dashboardHobbyPage
            );
    });

    connect(ui->routinesTab, &QToolButton::clicked, this, [this]() {
        ui->routinesTab->setChecked(true);
        ui->hobbyPageStack->setCurrentWidget(ui->routinesPage);

        // Wenn eine Routine-Ausführung aktiv ist, zeigen wir sie wieder.
        // Der pausierte Zustand bleibt erhalten.
        if (currentExecutionRoutineId != 0) {
            ui->routineViewStack->setCurrentWidget(
                ui->routineExecutionPage
                );
        }
    });


    connect(ui->exercisesTab, &QToolButton::clicked, this, [this]() {
        ui->exercisesTab->setChecked(true);

        // Wenn eine Übungs-Ausführung aktiv ist, zeigen wir sie wieder.
        // Der pausierte Zustand bleibt erhalten.
        if (currentExecutionExerciseId != 0) {
            ui->hobbyPageStack->setCurrentWidget(
                ui->exerciseExecutionPage
                );
            return;
        }

        // Der Reiter "Übungen" zeigt immer zunächst die normale
        // Kartenübersicht - unabhängig davon, ob zuvor eine
        // Detail-/Fortschrittsansicht geöffnet war.
        showExerciseOverview();

        ui->hobbyPageStack->setCurrentWidget(ui->exercisesPage);
    });

    connect(ui->historyTab, &QToolButton::clicked, this, [this]() {
        ui->historyTab->setChecked(true);
        ui->hobbyPageStack->setCurrentWidget(ui->historyPage);
    });

    connect(ui->goalsTab, &QToolButton::clicked, this, [this]() {
        ui->goalsTab->setChecked(true);
        ui->hobbyPageStack->setCurrentWidget(ui->goalsPage);

        // Den "Offen"-Button wirklich auslösen, damit nicht nur
        // der visuelle Zustand gesetzt wird, sondern auch der Filter
        // tatsächlich angewendet wird.
        ui->goalsFilterOpenButton->click();
    });

    connect(ui->roadmapTab, &QToolButton::clicked, this, [this]() {
        ui->roadmapTab->setChecked(true);
        ui->hobbyPageStack->setCurrentWidget(ui->roadmapPage);

        // Immer mit der Übersicht starten.
        showRoadmapOverview();
        loadRoadmapCards();
    });
    // ── Roadmap-Detail-Ansicht: Suchleiste ───────────────────────────────
    //
    // Der Suchtext wird in roadmapDetailFilterText gespeichert.
    // loadRoadmapDetail() wertet ihn aus und blendet nicht passende
    // Items aus.

    connect(
        ui->roadmapDetailSearchLineEdit,
        &QLineEdit::textChanged,
        this,
        [this](const QString &text) {

            roadmapDetailFilterText = text.trimmed();

            // Nur neu laden, wenn eine Roadmap geöffnet ist.
            if (currentRoadmapDetailRootId > 0)
                loadRoadmapDetail(currentRoadmapDetailRootId);
        }
        );

    // ── Roadmap-Detail-Ansicht: Overlay-Buttons rechts in der Zeile ──────
    //
    // Vier Icon-Buttons, die rechts in der Zeile des ausgewählten Steps
    // erscheinen. Sie sind Kinder des Tree-Widgets und werden per
    // updateRoadmapRowButtons() positioniert.
    //
    // Die Buttons existieren von Anfang an, sind aber unsichtbar.
    // Sobald ein Step selektiert wird, werden sie sichtbar geschaltet
    // und an die richtige Stelle verschoben.

    auto createRoadmapRowButton =
        [this](const QString &objectName,
               const QString &iconPath,
               const QString &tooltip) -> QToolButton * {

        auto *button = new QToolButton(ui->roadmapTreeWidget);

        button->setObjectName(objectName);
        button->setFixedSize(24, 24);
        button->setIconSize(QSize(16, 16));
        button->setAutoRaise(true);
        button->setCursor(Qt::PointingHandCursor);
        button->setIcon(QIcon(iconPath));
        button->setToolTip(tooltip);
        button->setVisible(false);

        return button;
    };

    roadmapRowAddChildButton = createRoadmapRowButton(
        "roadmapRowAddChildButton",
        ":/icons/roadmap-add.svg",
        "Unter-Step erstellen"
        );

    roadmapRowRenameButton = createRoadmapRowButton(
        "roadmapRowRenameButton",
        ":/icons/roadmap-rename.svg",
        "Umbenennen"
        );

    roadmapRowToggleDoneButton = createRoadmapRowButton(
        "roadmapRowToggleDoneButton",
        ":/icons/roadmap-check.svg",
        "Als erledigt markieren"
        );

    roadmapRowDeleteButton = createRoadmapRowButton(
        "roadmapRowDeleteButton",
        ":/icons/roadmap-delete.svg",
        "Löschen"
        );

    // ── Verbindungen ────────────────────────────────────────────────────

    connect(
        roadmapRowAddChildButton,
        &QToolButton::clicked,
        this,
        [this]() { onRoadmapAddChildStep(); }
        );

    connect(
        roadmapRowRenameButton,
        &QToolButton::clicked,
        this,
        [this]() { onRoadmapRenameSelectedStep(); }
        );

    connect(
        roadmapRowToggleDoneButton,
        &QToolButton::clicked,
        this,
        [this]() { onRoadmapToggleSelectedStepDone(); }
        );

    connect(
        roadmapRowDeleteButton,
        &QToolButton::clicked,
        this,
        [this]() { onRoadmapDeleteSelectedStep(); }
        );

    // ── Sichtbarkeit / Position aktualisieren ───────────────────────────
    //
    // Bei jeder Auswahl-Änderung und jedem Scroll müssen die Buttons
    // neu positioniert werden.

    connect(
        ui->roadmapTreeWidget,
        &QTreeWidget::itemSelectionChanged,
        this,
        [this]() {
            updateRoadmapRowButtons();
            updateRoadmapRowButtonsState();
        }
        );

    connect(
        ui->roadmapTreeWidget->verticalScrollBar(),
        &QScrollBar::valueChanged,
        this,
        [this]() {
            updateRoadmapRowButtons();
        }
        );

    // Initial deaktiviert, bis ein Step ausgewählt ist.
    updateRoadmapRowButtonsState();

    // ── Roadmap-Tree: Delegate für Linien und Punkte ─────────────────────
    //
    // Der Delegate zeichnet Pfeile, Punkte und Linien selbst.
    // Das RoadmapTreeWidget zeichnet die vertikalen Verbindungslinien.

    if (auto *roadmapTree = ui->roadmapTreeWidget) {

        roadmapTree->setItemDelegate(
            new RoadmapTreeDelegate(roadmapTree)
            );

        // Indentation und rootIsDecorated werden in RoadmapTreeWidget
        // gesetzt. Nicht überschreiben.
        roadmapTree->setUniformRowHeights(true);
        roadmapTree->setExpandsOnDoubleClick(false);
        roadmapTree->setSelectionMode(QAbstractItemView::SingleSelection);
    }

    // Auto-Einklappen in der Detail-Ansicht:
    // Wenn ein Step eingeklappt wird, werden auch alle seine
    // Nachkommen eingeklappt.
    connect(
        ui->roadmapTreeWidget,
        &QTreeWidget::itemCollapsed,
        this,
        [](QTreeWidgetItem *item) {

            std::function<void(QTreeWidgetItem *)> collapseAll =
                [&](QTreeWidgetItem *parent) {

                    for (int i = 0; i < parent->childCount(); ++i) {

                        QTreeWidgetItem *child = parent->child(i);

                        if (child->isExpanded())
                            child->setExpanded(false);

                        collapseAll(child);
                    }
                };

            collapseAll(item);
        }
        );

    // Detail-Ansicht: Alle Steps auf- bzw. zuklappen.
    connect(
        ui->roadmapExpandAllButton,
        &QToolButton::clicked,
        this,
        [this]() {
            ui->roadmapTreeWidget->expandAll();
        }
        );

    connect(
        ui->roadmapCollapseAllButton,
        &QToolButton::clicked,
        this,
        [this]() {
            ui->roadmapTreeWidget->collapseAll();
        }
        );

    // Detail-Ansicht: "+ Step" erstellt einen neuen Step direkt unter
    // dem Root der aktuell geöffneten Roadmap.
    connect(
        ui->addRoadmapChildStepButton,
        &QPushButton::clicked,
        this,
        [this]() {
            onRoadmapAddChildOfRootStep();
        }
        );

    // ── Notizen ─────────────────────────────────────────────────────────────

    connect(ui->hobbyNotesTextEdit, &QPlainTextEdit::textChanged, this, [this]() {
        if (currentHobbyId == 0)
            return;
        HobbyNoteRepository::setContent(
            currentHobbyId,
            ui->hobbyNotesTextEdit->toPlainText()
            );
    });

    // ── Hobby-Liste ─────────────────────────────────────────────────────────

    connect(ui->hobbyList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {

        if (item->text() == "+ hinzufügen") {
            bool ok;
            QString hobbyName = QInputDialog::getText(
                this, "Neues Hobby", "Name des Hobbys:",
                QLineEdit::Normal, "", &ok);

            if (ok && !hobbyName.isEmpty()) {
                int hobbyId;
                if (HobbyRepository::add(hobbyName, "", hobbyId)) {
                    QListWidgetItem *newItem = new QListWidgetItem(hobbyName);
                    newItem->setData(Qt::UserRole, hobbyId);
                    int addHobbyIndex = ui->hobbyList->count() - 1;
                    ui->hobbyList->insertItem(addHobbyIndex, newItem);
                }
            }
            return;
        }

        // Die Hobby-Auswahl ist die aktive Navigation - die
        // obersten Sidebar-Buttons dürfen dann nicht mehr als
        // "aktiv" markiert bleiben (immer nur ein Indikator).
        ui->dashboardButton->setChecked(false);
        ui->settingsButton->setChecked(false);

        // Eine laufende Routine- oder Übungs-Ausführung gehört zum
        // bisherigen Hobby. Beim Hobby-Wechsel wird sie komplett
        // beendet (nicht nur pausiert), weil der Zustand nicht mit
        // dem neuen Hobby vereinbar ist.
        if (currentExecutionRoutineId != 0)
            leaveRoutineExecution();

        if (currentExecutionExerciseId != 0)
            leaveExerciseExecution();

        currentHobby   = item->text();
        currentHobbyId = item->data(Qt::UserRole).toInt();

        // Beim Hobby-Wechsel einen evtl. gemerkten Rückkehrpunkt der
        // Übungs-Detailansicht zurücksetzen, damit der Back-Button
        // nicht ins vorherige Hobby zurückspringt.
        exerciseDetailReturnPage = nullptr;

        ui->hobbyLabel->setText(currentHobby);

        {
            QSignalBlocker blocker(ui->hobbyNotesTextEdit);
            ui->hobbyNotesTextEdit->setPlainText(
                HobbyNoteRepository::getContent(currentHobbyId));
        }

        refreshHobbyDashboardOverview(ui, currentHobbyId);

        // Kategorien des ausgewählten Hobbys in den Filter laden
        ui->exerciseCategoryComboBox->clear();

        ui->exerciseCategoryComboBox->addItem(
            "Alle Kategorien",
            0
            );

        ui->exerciseCategoryComboBox->addItem(
            "Archiv",
            -1
            );

        for (const Category &cat :
             CategoryRepository::getForHobby(currentHobbyId)) {

            ui->exerciseCategoryComboBox->addItem(
                cat.name,
                cat.id
                );
        }

        // Ein Hobby-Wechsel soll nicht mitten in der Detailansicht
        // einer Übung des vorherigen Hobbys landen.
        showExerciseOverview();

        loadExerciseCards();
        loadHistory();
        loadTimeline();
        loadRoutineCards();
        ui->goalsFilterOpenButton->setChecked(true);
        ui->goalsViewStack->setCurrentWidget(ui->goalsOpenPage);
        loadGoalCards();
        loadDashboardRoutineCards();
        showRoadmapOverview();
        loadRoadmapCards();


        ui->dashboardTab->setChecked(true);
        ui->hobbyPageStack->setCurrentWidget(ui->dashboardHobbyPage);
        ui->pageStack->setCurrentWidget(ui->hobbyPage);
    });

    // ── Übung hinzufügen ────────────────────────────────────────────────────

    connect(ui->addExerciseButton, &QPushButton::clicked, this, [this]() {
        QDialog dialog(this);
        Ui::ExerciseDialog dialogUi;
        dialogUi.setupUi(&dialog);

        // Buttons auf Deutsch (Qt zeigt sonst "Save" / "Cancel").
        if (auto *saveButton =
            dialogUi.buttonBox->button(QDialogButtonBox::Save)) {
            saveButton->setText("Erstellen");
            saveButton->setDefault(true);
        }
        if (auto *cancelButton =
            dialogUi.buttonBox->button(QDialogButtonBox::Cancel)) {
            cancelButton->setText("Abbrechen");
        }

        dialogUi.categoryComboBox->clear();
        dialogUi.categoryComboBox->addItem("– keine –", -1);
        for (const Category &cat : CategoryRepository::getForHobby(currentHobbyId))
            dialogUi.categoryComboBox->addItem(cat.name, cat.id);
        dialogUi.categoryComboBox->addItem("+ Kategorie erstellen", -2);

        connect(dialogUi.categoryComboBox, &QComboBox::activated, &dialog,
                [&dialog, &dialogUi, this](int index) {
                    if (dialogUi.categoryComboBox->itemData(index).toInt() != -2)
                        return;

                    bool ok = false;
                    QString categoryName = QInputDialog::getText(
                                               &dialog, "Neue Kategorie", "Name der Kategorie:",
                                               QLineEdit::Normal, "", &ok).trimmed();

                    if (!ok || categoryName.isEmpty()) {
                        dialogUi.categoryComboBox->setCurrentIndex(0);
                        return;
                    }

                    int categoryId = 0;
                    if (!CategoryRepository::add(currentHobbyId, categoryName, categoryId)) {
                        qDebug() << "Kategorie konnte nicht gespeichert werden.";
                        dialogUi.categoryComboBox->setCurrentIndex(0);
                        return;
                    }

                    const int createIndex = dialogUi.categoryComboBox->findData(-2);
                    dialogUi.categoryComboBox->insertItem(createIndex, categoryName, categoryId);
                    dialogUi.categoryComboBox->setCurrentIndex(createIndex);
                });

        if (dialog.exec() != QDialog::Accepted)
            return;

        const QString name =
            dialogUi.nameLineEdit->text().trimmed();

        if (name.isEmpty()) {

            QMessageBox::warning(
                &dialog,
                "Name fehlt",
                "Bitte gib einen Namen für die Übung ein."
                );

            return;
        }

        QString description = dialogUi.descriptionLineEdit->text().trimmed();
        int     categoryId  = dialogUi.categoryComboBox->currentData().toInt();
        QString unit        = dialogUi.unitLineEdit->text().trimmed();
        double  value       = dialogUi.valueLineEdit->text().replace(',', '.').toDouble();
        QString goal        = dialogUi.goalLineEdit->text().trimmed();

        int exerciseId;
        if (ExerciseRepository::add(
                currentHobbyId, name, description,
                categoryId, value, unit, goal, exerciseId)) {
            loadExerciseCards();
        } else {
            qDebug() << "Übung konnte nicht gespeichert werden.";
        }
    });

    // ── Suche & Kategoriefilter ──────────────────────────────────────────────

    connect(ui->exerciseSearchLineEdit, &QLineEdit::textChanged,
            this, [this]() { loadExerciseCards(); });

    connect(ui->exerciseCategoryComboBox, &QComboBox::currentIndexChanged,
            this, [this]() { loadExerciseCards(); });

    // ── Kategorien verwalten ────────────────────────────────────────────────

    connect(ui->manageCategoriesButton, &QPushButton::clicked, this, [this]() {
        QDialog dialog(this);
        Ui::CategoryManagementDialog dialogUi;
        dialogUi.setupUi(&dialog);

        // Kategorien des aktuellen Hobbys laden
        for (const Category &cat : CategoryRepository::getForHobby(currentHobbyId)) {
            auto *item = new QListWidgetItem(cat.name);
            item->setData(Qt::UserRole, cat.id);
            dialogUi.categoryListWidget->addItem(item);
        }

        // Buttons erst aktiv wenn eine Kategorie ausgewählt ist
        connect(dialogUi.categoryListWidget, &QListWidget::currentItemChanged, &dialog,
                [&dialogUi](QListWidgetItem *current) {
                    bool has = current != nullptr;
                    dialogUi.renameButton->setEnabled(has);
                    dialogUi.deleteButton->setEnabled(has);
                });

        // Umbenennen
        connect(dialogUi.renameButton, &QPushButton::clicked, &dialog, [&]() {
            auto *item = dialogUi.categoryListWidget->currentItem();
            if (!item) return;
            bool ok;
            QString newName = QInputDialog::getText(
                                  &dialog, "Kategorie umbenennen", "Neuer Name:",
                                  QLineEdit::Normal, item->text(), &ok).trimmed();
            if (ok && !newName.isEmpty()) {
                CategoryRepository::rename(item->data(Qt::UserRole).toInt(), newName);
                item->setText(newName);
            }
        });

        // Löschen
        connect(dialogUi.deleteButton, &QPushButton::clicked, &dialog, [&]() {
            auto *item = dialogUi.categoryListWidget->currentItem();
            if (!item) return;
            auto res = QMessageBox::question(
                &dialog, "Kategorie löschen",
                QString("\"%1\" wirklich löschen?\n\nZugehörige Übungen bleiben erhalten.")
                    .arg(item->text()));
            if (res == QMessageBox::Yes) {
                CategoryRepository::remove(item->data(Qt::UserRole).toInt());
                delete dialogUi.categoryListWidget->takeItem(
                    dialogUi.categoryListWidget->row(item));
            }
        });

        connect(dialogUi.addButton, &QPushButton::clicked, &dialog, [&]() {
            bool ok;
            QString name = QInputDialog::getText(
                               &dialog, "Neue Kategorie", "Name der Kategorie:",
                               QLineEdit::Normal, "", &ok).trimmed();

            if (!ok || name.isEmpty())
                return;

            int categoryId = 0;
            if (!CategoryRepository::add(currentHobbyId, name, categoryId)) {
                qDebug() << "Kategorie konnte nicht gespeichert werden.";
                return;
            }

            auto *item = new QListWidgetItem(name);
            item->setData(Qt::UserRole, categoryId);
            dialogUi.categoryListWidget->addItem(item);
            dialogUi.categoryListWidget->setCurrentItem(item);
        });

        dialog.exec();

        // Nach dem Schließen Kategoriefilter und Cards neu laden
        ui->exerciseCategoryComboBox->clear();

        ui->exerciseCategoryComboBox->addItem(
            "Alle Kategorien",
            0
            );

        ui->exerciseCategoryComboBox->addItem(
            "Archiv",
            -1
            );

        const QList<Category> categories =
            CategoryRepository::getForHobby(currentHobbyId);

        for (const Category &category : categories) {
            ui->exerciseCategoryComboBox->addItem(
                category.name,
                category.id
                );
        }
    });
    connect(ui->progressBackButton, &QPushButton::clicked, this, [this]() {

        showExerciseOverview();

        // ── Sonderfall: Fortschrittsseite wurde aus einer laufenden
        //    Ausführung geöffnet. Zurück zur Ausführung.
        if (progressReturnSource == ProgressReturnSource::RoutineExecution) {

            progressReturnSource = ProgressReturnSource::None;

            ui->routinesTab->setChecked(true);
            ui->hobbyPageStack->setCurrentWidget(ui->routinesPage);
            ui->routineViewStack->setCurrentWidget(ui->routineExecutionPage);

            return;
        }

        if (progressReturnSource == ProgressReturnSource::ExerciseExecution) {

            progressReturnSource = ProgressReturnSource::None;

            ui->exercisesTab->setChecked(true);
            ui->hobbyPageStack->setCurrentWidget(ui->exerciseExecutionPage);

            return;
        }

        // ── Normalfall: Detailansicht wurde aus Übersicht/Verlauf geöffnet.
        QWidget *targetPage =
            exerciseDetailReturnPage
                ? exerciseDetailReturnPage
                : ui->exercisesPage;

        ui->hobbyPageStack->setCurrentWidget(targetPage);

        if (targetPage == ui->historyPage) {
            ui->historyTab->setChecked(true);
        } else if (targetPage == ui->goalsPage) {
            ui->goalsTab->setChecked(true);
        } else if (targetPage == ui->routinesPage) {
            ui->routinesTab->setChecked(true);
        } else if (targetPage == ui->dashboardHobbyPage) {
            ui->dashboardTab->setChecked(true);
        } else {
            ui->exercisesTab->setChecked(true);
        }

        exerciseDetailReturnPage = nullptr;
    });

    connect(ui->timelineBackButton, &QPushButton::clicked, this, [this]() {
        ui->hobbyPageStack->setCurrentWidget(ui->dashboardHobbyPage);
        ui->dashboardTab->setChecked(true);
    });


    // ── Übungs-Detailansicht: Startwert bearbeiten ──────────────────────────
    //
    // Der Startwert entspricht dem Wert, mit dem die Übung ursprünglich
    // angelegt wurde, und wird bewusst nicht im normalen
    // Übung-bearbeiten-Dialog verändert, sondern über diese kleine,
    // eigenständige Aktion direkt in der Fortschrittsansicht.
    // ── Übungs-Detailansicht: Startwert inline bearbeitbar machen ───────
    //
    // Der "Bearbeiten"-Button wird ausgeblendet; stattdessen wird der
    // Startwert selbst klickbar. Ein QLineEdit wird zur Laufzeit erzeugt
    // und nur während der Bearbeitung eingeblendet.

    ui->exerciseDetailEditStartValueButton->setVisible(false);

    ui->exerciseDetailStartValueLabel->setCursor(Qt::PointingHandCursor);
    ui->exerciseDetailStartValueLabel->installEventFilter(this);

    // Das Edit-Feld wird als Kind des übergeordneten WIDGETS angelegt,
    // nicht des Layouts. Dadurch zählt es nicht als Layout-Element und
    // schiebt das Label nicht nach unten.
    exerciseDetailStartValueEdit = new QLineEdit(
        ui->exerciseDetailStartValueLabel->parentWidget()
        );

    exerciseDetailStartValueEdit->setObjectName(
        "exerciseDetailStartValueEdit"
        );

    exerciseDetailStartValueEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    exerciseDetailStartValueEdit->setVisible(false);
    exerciseDetailStartValueEdit->installEventFilter(this);

    // ── Ziele ────────────────────────────────────────────────────────────

    connect(ui->goalsFilterOpenButton, &QToolButton::clicked, this, [this]() {
        ui->goalsViewStack->setCurrentWidget(ui->goalsOpenPage);
    });

    connect(ui->goalsFilterDoneButton, &QToolButton::clicked, this, [this]() {
        ui->goalsViewStack->setCurrentWidget(ui->goalsDonePage);
    });

    connect(ui->addGoalButton, &QPushButton::clicked, this, [this]() {
        openGoalDialog(0);
    });

    connect(ui->goalSearchLineEdit, &QLineEdit::textChanged,
            this, [this]() { loadGoalCards(); });

    // Der Container für offene Ziel-Cards nimmt Drops per Drag & Drop an,
    // um die freie Reihenfolge (siehe Feature-Konzept "Ziele", Abschnitt 9)
    // zu ermöglichen.
    ui->goalOpenCardsWidget->setAcceptDrops(true);
    ui->goalOpenCardsWidget->installEventFilter(this);

    // ── Roadmap ──────────────────────────────────────────────────────────

    // "+ Neuer Step" in der Übersicht → Root-Step erstellen.
    connect(
        ui->addRoadmapGoalButton,
        &QPushButton::clicked,
        this,
        [this]() {
            onRoadmapAddRootStep();
        }
        );

    // Aktiv/Archiv-Filter.
    connect(
        ui->roadmapFilterActiveButton,
        &QToolButton::clicked,
        this,
        [this]() {
            ui->roadmapFilterStack->setCurrentWidget(
                ui->roadmapActivePage
                );
        }
        );

    connect(
        ui->roadmapFilterArchiveButton,
        &QToolButton::clicked,
        this,
        [this]() {
            ui->roadmapFilterStack->setCurrentWidget(
                ui->roadmapArchivePage
                );
        }
        );

    // Suchfeld.
    connect(
        ui->roadmapSearchLineEdit,
        &QLineEdit::textChanged,
        this,
        [this]() {
            loadRoadmapCards();
        }
        );

    // Back-Button in der Detail-Ansicht.
    connect(
        ui->roadmapDetailBackButton,
        &QPushButton::clicked,
        this,
        [this]() {
            showRoadmapOverview();
        }
        );

    // Aktionen in der Detail-Ansicht.
    connect(
        ui->roadmapTreeWidget,
        &QTreeWidget::itemSelectionChanged,
        this,
        [this]() {
            updateRoadmapActionButtons();
        }
        );

    // TODO: In Nachricht 2 werden die 4 Buttons in die Zeile
    // des ausgewählten Steps integriert. Dann werden die Connects
    // wieder aktiviert.

    /*
    connect(
        ui->roadmapAddChildButton,
        &QToolButton::clicked,
        this,
        [this]() {
            onRoadmapAddChildStep();
        }
        );

    connect(
        ui->roadmapRenameButton,
        &QToolButton::clicked,
        this,
        [this]() {
            onRoadmapRenameSelectedStep();
        }
        );

    connect(
        ui->roadmapToggleDoneButton,
        &QToolButton::clicked,
        this,
        [this]() {
            onRoadmapToggleSelectedStepDone();
        }
        );

    connect(
        ui->roadmapDeleteButton,
        &QToolButton::clicked,
        this,
        [this]() {
            onRoadmapDeleteSelectedStep();
        }
        );
    */

    updateRoadmapActionButtons();
}
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // ── Routine-Timer inline bearbeiten ──────────────────────────────────
    if (watched == ui->routineExecutionTimerLabel &&
        event->type() == QEvent::MouseButtonRelease) {

        auto *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton) {
            startRoutineExecutionTimerEdit();
            return true;
        }
    }

    if (watched == routineExecutionTimerEdit) {

        if (event->type() == QEvent::KeyPress) {

            const int key = static_cast<QKeyEvent *>(event)->key();

            if (key == Qt::Key_Return || key == Qt::Key_Enter) {
                commitRoutineExecutionTimerEdit();
                return true;
            }

            if (key == Qt::Key_Escape) {
                cancelRoutineExecutionTimerEdit();
                return true;
            }
        }

        if (event->type() == QEvent::FocusOut) {
            commitRoutineExecutionTimerEdit();
            return true;
        }
    }

    // ── Übungs-Timer inline bearbeiten ───────────────────────────────────
    if (watched == ui->exerciseExecutionTimerLabel &&
        event->type() == QEvent::MouseButtonRelease) {

        auto *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton) {
            startExerciseExecutionTimerEdit();
            return true;
        }
    }

    if (watched == exerciseExecutionTimerEdit) {

        if (event->type() == QEvent::KeyPress) {

            const int key = static_cast<QKeyEvent *>(event)->key();

            if (key == Qt::Key_Return || key == Qt::Key_Enter) {
                commitExerciseExecutionTimerEdit();
                return true;
            }

            if (key == Qt::Key_Escape) {
                cancelExerciseExecutionTimerEdit();
                return true;
            }
        }

        if (event->type() == QEvent::FocusOut) {
            commitExerciseExecutionTimerEdit();
            return true;
        }
    }

    // ── Übungs-Detailansicht: Startwert inline bearbeiten ────────────────
    //
    // Klick auf das Startwert-Label öffnet das Edit-Feld an gleicher Stelle.
    // Enter/Fokus-Verlust übernimmt den Wert, Escape verwirft ihn.
    if (watched == ui->exerciseDetailStartValueLabel &&
        event->type() == QEvent::MouseButtonRelease) {

        auto *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton) {

            startExerciseDetailStartValueEdit();
            return true;
        }
    }

    if (watched == exerciseDetailStartValueEdit) {

        if (event->type() == QEvent::KeyPress) {

            const int key = static_cast<QKeyEvent *>(event)->key();

            if (key == Qt::Key_Return || key == Qt::Key_Enter) {
                commitExerciseDetailStartValueEdit();
                return true;
            }

            if (key == Qt::Key_Escape) {
                cancelExerciseDetailStartValueEdit();
                return true;
            }
        }

        if (event->type() == QEvent::FocusOut) {
            commitExerciseDetailStartValueEdit();
            return true;
        }
    }

    if (qobject_cast<QToolButton *>(watched)) {

        if (watched->objectName() == "goalCurrentButton") {

            if (event->type() == QEvent::MouseButtonPress ||
                event->type() == QEvent::MouseButtonRelease) {

                return false;
            }
        }
    }
    // ── Hobby-Dashboard: aktuelle Phase / aktuelles Ziel ──────────────────
    //
    // Die Dashboard-Sektionen sind QFrames und besitzen kein clicked()-Signal.
    // Der Event-Filter wird deshalb sowohl auf die Sektion selbst als auch
    // auf deren enthaltene Widgets installiert.
    if (event->type() == QEvent::MouseButtonRelease) {

        auto *mouseEvent =
            static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton) {

            // Prüfen, ob auf die aktuelle Phase oder eines ihrer
            // enthaltenen Widgets geklickt wurde.
            const bool phaseClicked =
                watched == ui->hobbyPhaseSection ||
                ui->hobbyPhaseSection->isAncestorOf(
                    qobject_cast<QWidget *>(watched)
                    );

            if (phaseClicked) {

                ui->roadmapTab->setChecked(true);
                ui->hobbyPageStack->setCurrentWidget(
                    ui->timelinePage
                    );

                return true;
            }

            // Prüfen, ob auf das aktuelle Ziel oder eines seiner
            // enthaltenen Widgets geklickt wurde.
            const bool goalClicked =
                watched == ui->hobbyGoalSection ||
                ui->hobbyGoalSection->isAncestorOf(
                    qobject_cast<QWidget *>(watched)
                    );

            if (goalClicked) {
                ui->goalsTab->setChecked(true);
                ui->hobbyPageStack->setCurrentWidget(ui->goalsPage);

                // Den "Offen"-Button wirklich auslösen, damit der Filter
                // genauso angewendet wird wie bei einem normalen Benutzerklick.
                ui->goalsFilterOpenButton->click();

                return true;
            }
        }
    }



    // ── Dashboard-Routine-Card: Klick startet die Routine-Ausführung ────────
    if (event->type() == QEvent::MouseButtonRelease) {

        auto *card =
            qobject_cast<QFrame *>(watched);

        if (card &&
            card->objectName() == "dashboardRoutineCard") {

            auto *mouseEvent =
                static_cast<QMouseEvent *>(event);

            if (mouseEvent->button() == Qt::LeftButton) {

                const int routineId =
                    card->property("routineId").toInt();

                if (routineId > 0) {
                    ui->routinesTab->setChecked(true);
                    showRoutineExecution(routineId);
                }

                return true;
            }
        }
    }

    // ── Routine-Cards: Hover für den Auswahl-Stern ───────────────────────────
    //
    // Ausgewählte Routinen zeigen ihren Stern dauerhaft.
    // Bei nicht ausgewählten Routinen erscheint der Stern nur,
    // solange die Maus über der Card liegt.
    if (auto *card = qobject_cast<QFrame *>(watched)) {

        const int routineId =
            card->property("routineId").toInt();

        if (routineId > 0) {

            if (event->type() == QEvent::Enter) {

                if (!card->property("routineIsCurrent").toBool()) {

                    auto *favoriteButton =
                        card->findChild<QToolButton *>(
                            "routineFavoriteButton"
                            );

                    if (favoriteButton)
                        favoriteButton->setVisible(true);
                }
            }

            if (event->type() == QEvent::Leave) {

                if (!card->property("routineIsCurrent").toBool()) {

                    auto *favoriteButton =
                        card->findChild<QToolButton *>(
                            "routineFavoriteButton"
                            );

                    if (favoriteButton)
                        favoriteButton->setVisible(false);
                }
            }
        }
    }

    // ── Ziel-Cards: Klick öffnet die Bearbeitung, offene Ziele lassen ───────
    // sich zusätzlich per Drag & Drop neu sortieren.
    if (auto *card = qobject_cast<QFrame *>(watched)) {

        const int goalId = card->property("goalId").toInt();

        if (goalId > 0) {

            // ── Hover für den Hauptziel-Stern ────────────────────────────────
            //
            // Das aktuelle Hauptziel zeigt seinen Stern dauerhaft.
            // Bei allen anderen offenen Zielen erscheint der Stern nur,
            // solange die Maus über der Karte liegt.

            if (event->type() == QEvent::Enter) {

                if (!card->property("goalIsCurrent").toBool()) {

                    auto *currentButton =
                        card->findChild<QToolButton *>(
                            "goalCurrentButton"
                            );

                    if (currentButton)
                        currentButton->setVisible(true);
                }
            }

            if (event->type() == QEvent::Leave) {

                if (!card->property("goalIsCurrent").toBool()) {

                    auto *currentButton =
                        card->findChild<QToolButton *>(
                            "goalCurrentButton"
                            );

                    if (currentButton)
                        currentButton->setVisible(false);
                }
            }

            if (event->type() == QEvent::MouseButtonPress) {

                auto *mouseEvent = static_cast<QMouseEvent *>(event);

                if (mouseEvent->button() == Qt::LeftButton) {
                    goalDragCandidateId = goalId;
                    goalDragStartPos    = mouseEvent->pos();
                }

            } else if (event->type() == QEvent::MouseMove) {

                auto *mouseEvent = static_cast<QMouseEvent *>(event);

                const bool isDraggable = card->property("goalDraggable").toBool();

                if (isDraggable &&
                    goalDragCandidateId == goalId &&
                    (mouseEvent->buttons() & Qt::LeftButton) &&
                    (mouseEvent->pos() - goalDragStartPos).manhattanLength() >=
                        QApplication::startDragDistance()) {

                    // Ein Drag beginnt jetzt - ein einfacher Klick soll
                    // danach nicht mehr die Bearbeitung öffnen.
                    goalDragCandidateId = 0;

                    auto *mimeData = new QMimeData();
                    mimeData->setData(
                        "application/x-skillbase-goal",
                        QByteArray::number(goalId)
                        );

                    auto *drag = new QDrag(card);
                    drag->setMimeData(mimeData);
                    drag->setPixmap(card->grab());
                    drag->exec(Qt::MoveAction);

                    return true;
                }

            } else if (event->type() == QEvent::MouseButtonRelease) {

                if (goalDragCandidateId == goalId) {
                    goalDragCandidateId = 0;
                    openGoalDialog(goalId);
                    return true;
                }

                goalDragCandidateId = 0;
            }
        }
    }

    // ── Roadmap-Cards: Hover für den Stern ───────────────────────────────
    //
    // Bei nicht markierten Cards erscheint der Stern nur beim Hover.
    // Bei markierten Cards bleibt er dauerhaft sichtbar.
    // Archivierte Cards haben gar keinen Stern — der bleibt unsichtbar.

    if (auto *card = qobject_cast<QFrame *>(watched)) {

        if (card->objectName() == "roadmapCard") {

            const bool isArchived =
                card->property("roadmapIsArchived").toBool();

            // Archivierte Cards haben keinen Stern — nichts zu tun.
            if (!isArchived) {

                const bool isCurrent =
                    card->property("roadmapIsCurrent").toBool();

                // Der Stern wird nur gezeigt oder versteckt, wenn die
                // Card NICHT markiert ist. Eine markierte Card behält
                // ihren Stern auch außerhalb des Hovers.
                if (!isCurrent) {

                    auto *starButton =
                        card->findChild<QToolButton *>(
                            "roadmapCardStarButton"
                            );

                    if (starButton) {

                        if (event->type() == QEvent::Enter) {
                            starButton->setVisible(true);
                        }

                        if (event->type() == QEvent::Leave) {
                            starButton->setVisible(false);
                        }
                    }
                }
            }
        }
    }


    // ── Roadmap-Card: Klick öffnet die Detail-Ansicht ────────────────────
    if (event->type() == QEvent::MouseButtonRelease) {

        auto *card = qobject_cast<QFrame *>(watched);

        if (card && card->objectName() == "roadmapCard") {

            auto *mouseEvent = static_cast<QMouseEvent *>(event);

            if (mouseEvent->button() == Qt::LeftButton) {

                const int rootId =
                    card->property("roadmapRootId").toInt();

                if (rootId > 0)
                    showRoadmapDetail(rootId);

                return true;
            }
        }
    }


    // ── Übungskarten / History-Karten: Klick öffnet die Detailansicht ───────
    if (event->type() == QEvent::MouseButtonRelease) {

        auto *card = qobject_cast<QFrame *>(watched);

        if (card) {
            const int exerciseId =
                card->property("exerciseId").toInt();

            if (exerciseId > 0) {

                // Wenn die Karte eine History-Karte ist, kehren wir
                // später zur History-Seite zurück. Bei einer normalen
                // Übungskarte bleibt der bisherige Rückkehrpunkt
                // (exercisesPage) bestehen.
                if (card->property("isHistoryCard").toBool()) {
                    exerciseDetailReturnPage = ui->historyPage;
                } else {
                    exerciseDetailReturnPage = ui->exercisesPage;
                }

                showExerciseDetail(exerciseId);
                return true;
            }
        }
    }

    // ── Ablage-Bereich für offene Ziel-Cards ────────────────────────────────
    if (watched == ui->goalOpenCardsWidget) {

        if (event->type() == QEvent::DragEnter) {

            auto *dragEvent = static_cast<QDragEnterEvent *>(event);

            if (dragEvent->mimeData()->hasFormat("application/x-skillbase-goal")) {
                dragEvent->acceptProposedAction();
                return true;
            }

        } else if (event->type() == QEvent::DragMove) {

            auto *dragEvent = static_cast<QDragMoveEvent *>(event);

            if (dragEvent->mimeData()->hasFormat("application/x-skillbase-goal")) {
                dragEvent->acceptProposedAction();
                return true;
            }

        } else if (event->type() == QEvent::Drop) {

            auto *dropEvent = static_cast<QDropEvent *>(event);

            if (!dropEvent->mimeData()->hasFormat("application/x-skillbase-goal"))
                return QMainWindow::eventFilter(watched, event);

            const int draggedGoalId =
                dropEvent->mimeData()->data("application/x-skillbase-goal").toInt();

            const QPoint dropPos = dropEvent->position().toPoint();

            // Aktuelle Reihenfolge der offenen Ziele anhand der bereits
            // angezeigten Cards ermitteln.
            QList<int> order;

            for (int i = 0; i < ui->goalOpenCardsLayout->count(); ++i) {
                QLayoutItem *item = ui->goalOpenCardsLayout->itemAt(i);
                if (item->widget())
                    order.append(item->widget()->property("goalId").toInt());
            }

            // Ziel-Card unter der Ablageposition ermitteln, um die neue
            // Einfügeposition zu bestimmen.
            int targetIndex = order.size();

            for (int i = 0; i < ui->goalOpenCardsLayout->count(); ++i) {
                QLayoutItem *item = ui->goalOpenCardsLayout->itemAt(i);
                if (item->widget() && item->geometry().contains(dropPos)) {
                    targetIndex = i;
                    break;
                }
            }

            order.removeAll(draggedGoalId);
            targetIndex = qBound(0, targetIndex, order.size());
            order.insert(targetIndex, draggedGoalId);

            if (!GoalRepository::reorder(order))
                qDebug() << "Ziel-Reihenfolge konnte nicht gespeichert werden.";

            loadGoalCards();

            dropEvent->acceptProposedAction();
            return true;
        }
    }

    // Alle anderen Events normal weiterverarbeiten.
    return QMainWindow::eventFilter(watched, event);
}
// ── Übungs-Cards laden ──────────────────────────────────────────────────────

void MainWindow::loadExerciseCards()
{
    // Alle bisherigen Cards entfernen
    while (ui->exerciseCardsLayout->count() > 0) {
        QLayoutItem *layoutItem = ui->exerciseCardsLayout->takeAt(0);

        if (layoutItem->widget())
            layoutItem->widget()->deleteLater();

        delete layoutItem;
    }

    if (currentHobbyId == 0)
        return;

    // Kategorienamen für die Tag-Anzeige vorhalten.
    QHash<int, QString> categoryNames;

    for (const Category &cat :
         CategoryRepository::getForHobby(currentHobbyId)) {

        categoryNames.insert(cat.id, cat.name);
    }

    const QList<Exercise> exercises =
        ExerciseRepository::getForHobby(currentHobbyId, true);

    const QString searchText =
        ui->exerciseSearchLineEdit->text().trimmed();

    const int selectedCategoryId =
        ui->exerciseCategoryComboBox->currentData().toInt();

    const int columnCount = 3;

    int visibleCount = 0;

    for (const Exercise &exercise : exercises) {

        // Suchfilter
        if (!searchText.isEmpty() &&
            !exercise.name.contains(searchText, Qt::CaseInsensitive)) {

            continue;
        }

        // Filter:
        // 0  = alle aktiven Übungen
        // -1 = archivierte Übungen
        // >0 = aktive Übungen einer bestimmten Kategorie
        if (selectedCategoryId == -1) {

            if (!exercise.archived)
                continue;

        } else if (selectedCategoryId == 0) {

            if (exercise.archived)
                continue;

        } else {

            if (exercise.archived ||
                exercise.categoryId != selectedCategoryId) {

                continue;
            }
        }

        // ── Card ────────────────────────────────────────────────────────────
        auto *card = new QFrame(ui->exerciseCardsWidget);

        card->setObjectName("exerciseCard");
        card->setFrameShape(QFrame::StyledPanel);

        // Alle Übungskarten haben dieselbe feste Höhe wie die Zielkarten.
        // Dadurch bleiben die Karten im 3-Spalten-Grid einheitlich.
        card->setFixedHeight(176);

        card->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Fixed
            );

        card->setCursor(Qt::PointingHandCursor);

        // Das Layout wird bewusst kompakt gehalten, damit alle Informationen
        // inklusive der beiden Buttons innerhalb der festen Kartenhöhe Platz haben.
        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(2);
        cardLayout->setContentsMargins(12, 10, 12, 10);

        // ── Name ────────────────────────────────────────────────────────────
        auto *nameLabel = new QLabel(exercise.name, card);

        nameLabel->setObjectName("exerciseCardNameLabel");
        nameLabel->setWordWrap(true);
        cardLayout->addWidget(nameLabel);

        // ── Letzten Log laden ───────────────────────────────────────────────
        ExerciseLog latestLog;

        const bool hasLatestLog =
            ExerciseLogRepository::getLatestForExercise(
                exercise.id,
                latestLog
                );

        // ── Wert ────────────────────────────────────────────────────────────
        auto *wertRow = new QHBoxLayout();

        wertRow->addWidget(new QLabel("Wert", card));
        wertRow->addStretch();

        // Der angezeigte Wert entspricht immer dem zuletzt gespeicherten Wert.
        // Wenn noch keine Ausführung existiert, wird der aktuelle Übungswert verwendet.
        QString wertText;

        if (hasLatestLog) {

            wertText =
                QString("%1%2")
                    .arg(latestLog.value, 0, 'g', 15)
                    .arg(
                        latestLog.unit.isEmpty()
                            ? ""
                            : " " + latestLog.unit
                        );

        } else if (exercise.value > 0) {

            wertText =
                QString("%1%2")
                    .arg(exercise.value, 0, 'g', 15)
                    .arg(
                        exercise.unit.isEmpty()
                            ? ""
                            : " " + exercise.unit
                        );

        } else {

            wertText = "–";
        }

        wertRow->addWidget(new QLabel(wertText, card));
        cardLayout->addLayout(wertRow);

        // ── Ziel ────────────────────────────────────────────────────────────
        auto *goalRow = new QHBoxLayout();

        goalRow->addWidget(new QLabel("Ziel", card));
        goalRow->addStretch();

        QString goalText;

        if (exercise.goal != 0.0) {

            goalText =
                QString::number(exercise.goal, 'g', 15);

            if (!exercise.unit.isEmpty())
                goalText += " " + exercise.unit;

        } else {

            goalText = "–";
        }

        goalRow->addWidget(new QLabel(goalText, card));
        cardLayout->addLayout(goalRow);

        // ── Letzte Ausführung ───────────────────────────────────────────────
        // ── Kategorie-Tag ───────────────────────────────────────────────────
        // Die Kategorie wird bewusst vor "Letzte Ausführung" angezeigt,
        // damit die Informationen in der gewünschten Reihenfolge stehen.
        if (exercise.categoryId > 0 &&
            categoryNames.contains(exercise.categoryId)) {

            auto *tagRow = new QHBoxLayout();

            auto *tag =
                new QLabel(
                    categoryNames.value(exercise.categoryId),
                    card
                    );

            tag->setObjectName(
                "exerciseCardCategoryTagLabel"
                );

            tagRow->addWidget(tag);
            tagRow->addStretch();

            cardLayout->addLayout(tagRow);
        }

        // ── Letzte Ausführung ───────────────────────────────────────────────
        auto *lastHeader =
            new QLabel("Letzte Ausführung", card);

        lastHeader->setObjectName(
            "exerciseCardMetaHeaderLabel"
            );

        lastHeader->setProperty("role", "muted");

        cardLayout->addWidget(lastHeader);

        auto *lastRow = new QHBoxLayout();

        if (hasLatestLog) {

            const QDateTime performedAt =
                QDateTime::fromString(
                    latestLog.performedAt,
                    "yyyy-MM-dd HH:mm:ss"
                    ).toLocalTime();

            lastRow->addWidget(
                new QLabel(
                    performedAt.toString("dd.MM.yyyy"),
                    card
                    )
                );

            lastRow->addStretch();

        } else {

            lastRow->addWidget(
                new QLabel("–", card)
                );

            lastRow->addStretch();
        }

        cardLayout->addLayout(lastRow);

        // ── Trennlinie ──────────────────────────────────────────────────────
        auto *separator = new QFrame(card);

        separator->setObjectName(
            "exerciseCardSeparator"
            );

        separator->setFrameShape(QFrame::HLine);

        cardLayout->addWidget(separator);

        // ── Buttons ─────────────────────────────────────────────────────────
        // Die Buttons behalten ihre normale Größe.
        // Sie werden nicht auf die gesamte verfügbare Breite gestreckt.
        auto *btnRow = new QHBoxLayout();

        auto *editBtn =
            new QPushButton("Bearbeiten", card);

        auto *runBtn =
            new QPushButton("Ausführen", card);

        runBtn->setObjectName(
            "exerciseCardRunButton"
            );

        btnRow->addWidget(editBtn);
        btnRow->addWidget(runBtn);

        cardLayout->addLayout(btnRow);

        // ── Button-Aktionen ─────────────────────────────────────────────────
        connect(
            editBtn,
            &QPushButton::clicked,
            this,
            [this, exercise]() {

                editExercise(exercise.id);
            }
            );

        connect(
            runBtn,
            &QPushButton::clicked,
            this,
            [this, exercise]() {

                // Wechselt auf die Übungs-Ausführungsseite.
                // Speichern passiert nur bei "Beenden" oder Zeitablauf.
                showExerciseExecution(exercise.id);
            }
            );

        // Die Card selbst öffnet später die Detailansicht.
        // Die Buttons innerhalb der Card werden separat behandelt.
        card->installEventFilter(this);
        card->setProperty("exerciseId", exercise.id);

        // ── Position im Grid ────────────────────────────────────────────────
        const int row =
            visibleCount / columnCount;

        const int column =
            visibleCount % columnCount;

        ui->exerciseCardsLayout->addWidget(
            card,
            row,
            column
            );

        ++visibleCount;
    }

    // Spacer sorgt dafür, dass die Cards oben im Bereich bleiben,
    // wenn nicht alle verfügbaren Zeilen gefüllt sind.
    auto *spacer = new QSpacerItem(
        0,
        0,
        QSizePolicy::Minimum,
        QSizePolicy::Expanding
        );

    ui->exerciseCardsLayout->addItem(
        spacer,
        visibleCount / columnCount + 1,
        0
        );

    // ── Leerer Zustand ──────────────────────────────────────────────────────
    if (visibleCount == 0) {

        auto *emptyLabel =
            new QLabel(
                "Keine Übungen gefunden.",
                ui->exerciseCardsWidget
                );

        emptyLabel->setAlignment(
            Qt::AlignCenter
            );

        ui->exerciseCardsLayout->addWidget(
            emptyLabel,
            0,
            0,
            1,
            columnCount
            );
    }
}

// ── Übung bearbeiten ────────────────────────────────────────────────────────

void MainWindow::editExercise(int exerciseId)
{
    Exercise exercise;
    if (!ExerciseRepository::getById(exerciseId, exercise)) {
        qDebug() << "Übung konnte nicht geladen werden.";
        return;
    }

    // Der zuletzt eingetragene Wert wird nur für die Anzeige im Eingabefeld verwendet.
    // Der gespeicherte Übungswert selbst wird dadurch nicht verändert.
    ExerciseLog latestLog;
    const bool hasLatestLog =
        ExerciseLogRepository::getLatestForExercise(
            exercise.id,
            latestLog
            );

    QDialog dialog(this);
    Ui::ExerciseDialog dialogUi;
    dialogUi.setupUi(&dialog);
    dialog.setWindowTitle("Übung bearbeiten");

    // Buttons auf Deutsch (Qt zeigt sonst "Save" / "Cancel").
    if (auto *saveButton =
        dialogUi.buttonBox->button(QDialogButtonBox::Save)) {
        saveButton->setText("Speichern");
        saveButton->setDefault(true);
    }
    if (auto *cancelButton =
        dialogUi.buttonBox->button(QDialogButtonBox::Cancel)) {
        cancelButton->setText("Abbrechen");
    }

    dialogUi.categoryComboBox->clear();
    dialogUi.categoryComboBox->addItem("– keine –", -1);

    for (const Category &cat :
         CategoryRepository::getForHobby(currentHobbyId)) {

        dialogUi.categoryComboBox->addItem(
            cat.name,
            cat.id
            );
    }

    dialogUi.categoryComboBox->addItem(
        "+ Kategorie erstellen",
        -2
        );

    connect(
        dialogUi.categoryComboBox,
        &QComboBox::activated,
        &dialog,
        [&dialog, &dialogUi, this](int index) {

            if (dialogUi.categoryComboBox->itemData(index).toInt() != -2)
                return;

            bool ok = false;

            QString categoryName = QInputDialog::getText(
                                       &dialog,
                                       "Neue Kategorie",
                                       "Name der Kategorie:",
                                       QLineEdit::Normal,
                                       "",
                                       &ok
                                       ).trimmed();

            if (!ok || categoryName.isEmpty())
                return;

            int categoryId = 0;

            if (!CategoryRepository::add(
                    currentHobbyId,
                    categoryName,
                    categoryId)) {

                qDebug() << "Kategorie konnte nicht gespeichert werden.";
                return;
            }

            const int createIndex =
                dialogUi.categoryComboBox->findData(-2);

            dialogUi.categoryComboBox->insertItem(
                createIndex,
                categoryName,
                categoryId
                );

            dialogUi.categoryComboBox->setCurrentIndex(
                createIndex
                );
        }
        );

    // Der Button zeigt abhängig vom aktuellen Status die passende Aktion an.
    dialogUi.archiveButton->setText(
        exercise.archived
            ? "Wiederherstellen"
            : "Archivieren"
        );


    // Archivieren und Wiederherstellen sind eigenständige Aktionen.
    // Deshalb wird dafür nicht der normale Speichern-Button benötigt.
    connect(
        dialogUi.archiveButton,
        &QPushButton::clicked,
        &dialog,
        [&]() {

            const bool newState = !exercise.archived;

            if (ExerciseRepository::setArchived(
                    exercise.id,
                    newState)) {

                dialog.reject();
                loadExerciseCards();
            } else {
                qDebug() << "Archivstatus konnte nicht geändert werden.";
            }
        }
        );

    connect(
        dialogUi.deleteButton,
        &QPushButton::clicked,
        &dialog,
        [&]() {

            const auto result = QMessageBox::warning(
                &dialog,
                "Übung löschen",
                "Möchtest du diese Übung wirklich löschen?\n\n"
                "Alle zugehörigen Ausführungen werden ebenfalls gelöscht.",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
                );

            if (result != QMessageBox::Yes)
                return;

            if (ExerciseRepository::remove(exercise.id)) {

                dialog.accept();
                loadExerciseCards();

            } else {

                qDebug() << "Übung konnte nicht gelöscht werden.";
            }
        }
        );

    // Bestehende Werte eintragen

    dialogUi.nameLineEdit->setText(exercise.name);
    dialogUi.descriptionLineEdit->setText(exercise.description);

    if (hasLatestLog) {
        // Wenn bereits eine Ausführung existiert, zeigen wir deren Wert an.
        dialogUi.valueLineEdit->setText(
            QString::number(latestLog.value, 'g', 15)
            );
    } else {
        // Ohne Ausführung verwenden wir den ursprünglichen Übungswert.
        dialogUi.valueLineEdit->setText(
            QString::number(exercise.value, 'g', 15)
            );
    }

    dialogUi.unitLineEdit->setText(exercise.unit);
    dialogUi.goalLineEdit->setText(
        QString::number(exercise.goal, 'g', 15)
        );

    const int categoryIndex =
        dialogUi.categoryComboBox->findData(exercise.categoryId);

    if (categoryIndex >= 0)
        dialogUi.categoryComboBox->setCurrentIndex(categoryIndex);

    if (dialog.exec() != QDialog::Accepted)
        return;

    const QString name =
        dialogUi.nameLineEdit->text().trimmed();

    if (name.isEmpty()) {

        QMessageBox::warning(
            &dialog,
            "Name fehlt",
            "Bitte gib einen Namen für die Übung ein."
            );

        return;
    }

    QString description =
        dialogUi.descriptionLineEdit->text().trimmed();

    int categoryId =
        dialogUi.categoryComboBox->currentData().toInt();

    double value =
        dialogUi.valueLineEdit->text()
            .replace(',', '.')
            .toDouble();

    QString unit =
        dialogUi.unitLineEdit->text().trimmed();

    QString goal =
        dialogUi.goalLineEdit->text().trimmed();

    if (ExerciseRepository::update(
            exerciseId,
            name,
            description,
            categoryId,
            value,
            unit,
            goal)) {

        loadExerciseCards();
        // Verlauf aktualisieren, damit auch alte Logs den neuen Übungsnamen anzeigen.
        loadHistory();
        loadTimeline();
        loadRoutineCards();
        loadDashboardRoutineCards();

    } else {

        qDebug() << "Übung konnte nicht aktualisiert werden.";
    }
}

// ── Übungs-Detailansicht: Übersicht ↔ Detail ────────────────────────────────
//
// Beide Zustände gehören zur selben Seite (exercisesPage) und werden über
// einen internen QStackedWidget umgeschaltet - es öffnet sich kein neues
// Fenster und kein Dialog.

void MainWindow::showExerciseOverview()
{
    currentDetailExerciseId = 0;
    ui->exerciseViewStack->setCurrentWidget(ui->exerciseOverviewPage);
}

void MainWindow::showExerciseDetail(int exerciseId)
{
    currentDetailExerciseId = exerciseId;
    loadExerciseDetail(exerciseId);
    ui->exerciseViewStack->setCurrentWidget(ui->exerciseDetailPage);

    // Die Detailansicht liegt innerhalb der Exercises-Seite.
    // Damit sie sichtbar wird, muss der hobbyPageStack dorthin wechseln.
    // (Sonst bleibt der Nutzer z. B. auf der History-Seite stehen
    //  und sieht nichts von der Detailansicht.)
    ui->hobbyPageStack->setCurrentWidget(ui->exercisesPage);
}

void MainWindow::loadExerciseDetail(int exerciseId)
{
    Exercise exercise;

    if (!ExerciseRepository::getById(exerciseId, exercise)) {
        // Die Übung existiert nicht mehr (z. B. gerade gelöscht) -
        // zurück zur normalen Übersicht.
        showExerciseOverview();
        loadExerciseCards();
        loadRoutineCards();
        loadDashboardRoutineCards();
        return;
    }

    // ── Kopfbereich ──────────────────────────────────────────────────────
    ui->exerciseDetailNameLabel->setText(exercise.name);

    ui->exerciseDetailDescriptionLabel->setText(exercise.description);
    ui->exerciseDetailDescriptionLabel->setVisible(
        !exercise.description.trimmed().isEmpty()
        );

    QString categoryName;

    if (exercise.categoryId > 0) {
        for (const Category &category :
             CategoryRepository::getForHobby(exercise.hobbyId)) {

            if (category.id == exercise.categoryId) {
                categoryName = category.name;
                break;
            }
        }
    }

    ui->exerciseDetailCategoryTagLabel->setText(categoryName);
    ui->exerciseDetailCategoryTagLabel->setVisible(!categoryName.isEmpty());

    // ── Werte laden ──────────────────────────────────────────────────────
    //
    // Alle Ausführungen dieser Übung, zeitlich aufsteigend sortiert -
    // sowohl für die Statistik als auch für das Entwicklungsdiagramm.
    QList<ExerciseLog> logs =
        ExerciseLogRepository::getForExercise(exerciseId);

    std::sort(
        logs.begin(),
        logs.end(),
        [](const ExerciseLog &a, const ExerciseLog &b) {
            return a.performedAt < b.performedAt;
        }
        );

    const bool hasLogs = !logs.isEmpty();

    // Der aktuelle Wert entspricht - wie auch auf der Übungskarte - dem
    // zuletzt geloggten Wert, oder dem ursprünglichen Übungswert, solange
    // noch keine Ausführung existiert.
    const double currentValue =
        hasLogs ? logs.last().value : exercise.value;

    auto formatValue = [&](double value) {

        QString text = QString::number(value, 'g', 15);

        if (!exercise.unit.isEmpty())
            text += " " + exercise.unit;

        return text;
    };

    ui->exerciseDetailStartValueLabel->setText(
        formatValue(exercise.startValue)
        );

    ui->exerciseDetailCurrentValueLabel->setText(
        formatValue(currentValue)
        );

    ui->exerciseDetailGoalValueLabel->setText(
        formatValue(exercise.goal)
        );

    // ── Fortschrittsbalken ───────────────────────────────────────────────
    //
    // Fortschritt zwischen Startwert und Ziel, auf Basis des aktuellen
    // Werts. Ein Zielwert, der dem Startwert entspricht, ergibt keinen
    // sinnvollen Fortschritt.
    double percent = 0.0;

    if (exercise.goal != exercise.startValue) {

        percent =
            (currentValue - exercise.startValue) /
            (exercise.goal - exercise.startValue) *
            100.0;

        percent = std::clamp(percent, 0.0, 100.0);
    }

    ui->exerciseDetailProgressBar->setValue(
        static_cast<int>(std::round(percent))
        );

    ui->exerciseDetailPercentLabel->setText(
        QString("%1 %").arg(static_cast<int>(std::round(percent)))
        );

    // ── Statistik ────────────────────────────────────────────────────────
    //
    // Reihenfolge in der Karte:
    //   1. Gesamtzeit
    //   2. Ø Übungszeit
    //   3. Erste Ausführung
    //   4. Letzte Ausführung
    //   5. Ausführungen
    //   6. Beste Leistung
    //   7. Ø Leistung
    //   8. Fortschritt seit Start

    // ── Gesamtzeit ───────────────────────────────────────────────────────
    int totalDurationSeconds = 0;

    for (const ExerciseLog &log : logs)
        totalDurationSeconds += log.durationSeconds;

    if (totalDurationSeconds > 0) {

        const double totalHours = totalDurationSeconds / 3600.0;

        ui->exerciseDetailTotalTimeLabel->setText(
            QLocale::system().toString(totalHours, 'f', 1) + " h"
            );

    } else {

        ui->exerciseDetailTotalTimeLabel->setText("–");
    }

    // ── Ø Übungszeit ─────────────────────────────────────────────────────
    if (hasLogs && totalDurationSeconds > 0) {

        const double avgSeconds =
            totalDurationSeconds / double(logs.size());

        const double avgMinutes = avgSeconds / 60.0;

        ui->exerciseDetailAvgDurationLabel->setText(
            QLocale::system().toString(avgMinutes, 'f', 1) + " min"
            );

    } else {

        ui->exerciseDetailAvgDurationLabel->setText("–");
    }

    // ── Erste Ausführung ─────────────────────────────────────────────────
    if (hasLogs) {

        const QDateTime firstPerformedAt =
            QDateTime::fromString(
                logs.first().performedAt,
                "yyyy-MM-dd HH:mm:ss"
                ).toLocalTime();

        ui->exerciseDetailFirstPerformedLabel->setText(
            firstPerformedAt.toString("dd.MM.yyyy")
            );

    } else {

        ui->exerciseDetailFirstPerformedLabel->setText("–");
    }

    // ── Letzte Ausführung ────────────────────────────────────────────────
    if (hasLogs) {

        const QDateTime lastPerformedAt =
            QDateTime::fromString(
                logs.last().performedAt,
                "yyyy-MM-dd HH:mm:ss"
                ).toLocalTime();

        const QDate lastDate = lastPerformedAt.date();
        const QDate today    = QDate::currentDate();

        QString lastPerformedText;

        if (lastDate == today) {
            lastPerformedText = "Heute";
        } else if (lastDate == today.addDays(-1)) {
            lastPerformedText = "Gestern";
        } else {
            lastPerformedText = lastDate.toString("dd.MM.yyyy");
        }

        ui->exerciseDetailLastPerformedLabel->setText(lastPerformedText);

    } else {

        ui->exerciseDetailLastPerformedLabel->setText("–");
    }

    // ── Ausführungen ─────────────────────────────────────────────────────
    ui->exerciseDetailExecutionCountLabel->setText(
        QString::number(logs.size())
        );

    // ── Beste Leistung ───────────────────────────────────────────────────
    if (hasLogs) {

        double bestValue = logs.first().value;

        for (const ExerciseLog &log : logs) {

            if (log.value > bestValue)
                bestValue = log.value;
        }

        QString bestText =
            QString::number(bestValue, 'g', 15);

        if (!exercise.unit.isEmpty())
            bestText += " " + exercise.unit;

        ui->exerciseDetailBestValueLabel->setText(bestText);

    } else {

        ui->exerciseDetailBestValueLabel->setText("–");
    }

    // ── Ø Leistung ───────────────────────────────────────────────────────
    if (hasLogs) {

        double sum = 0.0;

        for (const ExerciseLog &log : logs)
            sum += log.value;

        const double avg = sum / logs.size();

        // Ein Nachkommastelle, wenn der Wert nicht ganzzahlig ist.
        // Sonst ohne Nachkommastelle.
        QString avgText =
            (qFuzzyCompare(avg, qRound(avg)))
                ? QString::number(qRound(avg))
                : QLocale::system().toString(avg, 'f', 1);

        if (!exercise.unit.isEmpty())
            avgText += " " + exercise.unit;

        ui->exerciseDetailAvgValueLabel->setText(avgText);

    } else {

        ui->exerciseDetailAvgValueLabel->setText("–");
    }

    // ── Fortschritt seit Start ───────────────────────────────────────────
    //
    // Absolute Veränderung vom Startwert zum aktuellen Wert.
    // Mit Vorzeichen (+/-), damit die Richtung erkennbar ist.
    if (hasLogs) {

        const double diff =
            currentValue - exercise.startValue;

        QString diffText = QString::number(diff, 'g', 15);

        if (diff > 0.0) {
            diffText.prepend("+");
        }

        if (!exercise.unit.isEmpty())
            diffText += " " + exercise.unit;

        ui->exerciseDetailProgressSinceStartLabel->setText(diffText);

    } else {

        ui->exerciseDetailProgressSinceStartLabel->setText("–");
    }
    // ── Entwicklungsdiagramm ─────────────────────────────────────────────
    if (auto *chart =
        qobject_cast<ExerciseProgressChartWidget *>(
            ui->exerciseProgressChartWidget)) {

        chart->setLogs(
            logs,
            exercise.startValue,
            exercise.goal
            );
    }
}

void MainWindow::loadHistory()
{
    // ── Alte dynamische Inhalte entfernen ───────────────────────────────────
    //
    // Wir löschen bewusst NICHT alles im Layout, sondern nur das eine
    // Layout, das diese Methode selbst erzeugt hat. Dadurch bleiben
    // historyEmptyStateLabel und alle anderen Elemente aus der .ui unberührt.
    const QString dynamicLayoutName =
        QStringLiteral("historyDynamicContent");

    // Rekursiv, weil das dynamische Layout weitere Layouts (die Grids)
    // enthält. Ein Grid ist kein Widget, deshalb reicht item->widget() nicht.
    std::function<void(QLayout *)> clearLayout =
        [&clearLayout](QLayout *layout) {

            while (QLayoutItem *item = layout->takeAt(0)) {

                if (QWidget *widget = item->widget()) {

                    // hide(), damit das Widget bis zum Löschen
                    // (deleteLater) nicht mehr sichtbar ist.
                    widget->hide();
                    widget->deleteLater();

                } else if (QLayout *childLayout = item->layout()) {

                    clearLayout(childLayout);
                }

                // Spacer-Items und Layout-Items gehören uns nach takeAt().
                delete item;
            }
        };

    for (int i = ui->historyEntriesLayout->count() - 1; i >= 0; --i) {

        QLayoutItem *item =
            ui->historyEntriesLayout->itemAt(i);

        QLayout *layout =
            item ? item->layout() : nullptr;

        if (layout && layout->objectName() == dynamicLayoutName) {

            ui->historyEntriesLayout->takeAt(i);
            clearLayout(layout);
            delete layout;
        }
    }

    if (currentHobbyId == 0) {
        ui->historyEmptyStateLabel->setVisible(true);
        return;
    }

    // ── Verlaufseinträge sammeln ────────────────────────────────────────────
    struct HistoryItem
    {
        Exercise exercise;
        ExerciseLog log;
        QDateTime performedAt;
    };

    QList<HistoryItem> historyItems;

    const QString searchText =
        ui->historySearchLineEdit->text().trimmed();

    // Das Datum ist die obere Grenze ("bis einschließlich").
    // Verglichen wird nur das Datum, die Uhrzeit spielt keine Rolle.
    const QDate maxHistoryDate =
        ui->historyDateFilterEdit->date();

    const QList<Exercise> exercises =
        ExerciseRepository::getForHobby(currentHobbyId);

    const bool showExercises =
        historyFilter == HistoryFilter::All ||
        historyFilter == HistoryFilter::Exercises;

    if (showExercises) {

        for (const Exercise &exercise : exercises) {

            // Suche vor dem Laden der Logs prüfen, spart Datenbankzugriffe.
            if (!searchText.isEmpty() &&
                !exercise.name.contains(
                    searchText,
                    Qt::CaseInsensitive
                    )) {

                continue;
            }

            const QList<ExerciseLog> logs =
                ExerciseLogRepository::getForExercise(
                    exercise.id
                    );

            for (const ExerciseLog &log : logs) {

                const QDateTime performedAt =
                    QDateTime::fromString(
                        log.performedAt,
                        "yyyy-MM-dd HH:mm:ss"
                        ).toLocalTime();

                if (performedAt.date() > maxHistoryDate)
                    continue;

                historyItems.append({
                    exercise,
                    log,
                    performedAt
                });
            }
        }
    }

    // Neueste Ausführungen zuerst.
    std::sort(
        historyItems.begin(),
        historyItems.end(),
        [](const HistoryItem &a, const HistoryItem &b) {

            return a.performedAt > b.performedAt;
        }
        );

    // ── Dynamisches Layout anlegen ──────────────────────────────────────────
    //
    // Es wird sofort ins äußere Layout eingefügt, damit alle Widgets darin
    // von Anfang an den richtigen Parent haben. Kein QWidget als Container,
    // deshalb gibt es keine zusätzliche vertikale Layout-Ebene für die Karten.
    auto *content = new QVBoxLayout();

    content->setObjectName(dynamicLayoutName);
    content->setContentsMargins(0, 0, 0, 0);
    content->setSpacing(12);

    ui->historyEntriesLayout->addLayout(content);

    // ── Verlauf nach Datum gruppieren ───────────────────────────────────────
    //
    // Jeder Tag bekommt ein eigenes Grid. Karten verschiedener Tage
    // können dadurch nie dieselbe Reihe teilen.
    QDate currentDate;
    QGridLayout *currentGrid = nullptr;
    int cardsInCurrentGroup = 0;

    constexpr int columnCount = 6;

    for (const HistoryItem &item : historyItems) {

        const Exercise &exercise = item.exercise;
        const ExerciseLog &log = item.log;
        const QDateTime &performedAt = item.performedAt;

        const QDate entryDate = performedAt.date();

        // ── Neue Datum-Gruppe ───────────────────────────────────────────────
        if (entryDate != currentDate) {

            auto *header =
                new QLabel(ui->historyEntriesWidget);

            header->setObjectName("historyDateHeaderLabel");

            // Fixe Höhe, damit das Label nie überschüssigen Platz bekommt.
            header->setSizePolicy(
                QSizePolicy::Preferred,
                QSizePolicy::Fixed
                );

            const QDate today = QDate::currentDate();

            if (entryDate == today) {
                header->setText("HEUTE");
            } else if (entryDate == today.addDays(-1)) {
                header->setText("GESTERN");
            } else {
                header->setText(entryDate.toString("dd.MM.yyyy"));
            }

            content->addWidget(header);

            currentGrid = new QGridLayout();

            currentGrid->setContentsMargins(0, 0, 0, 0);
            currentGrid->setHorizontalSpacing(12);
            currentGrid->setVerticalSpacing(12);

            // Gleiche Spaltenstretches, damit auch eine Reihe mit nur
            // 1-2 Karten dieselbe Kartenbreite hat wie eine volle Reihe.
            for (int c = 0; c < columnCount; ++c)
                currentGrid->setColumnStretch(c, 1);

            content->addLayout(currentGrid);

            currentDate = entryDate;
            cardsInCurrentGroup = 0;
        }

        // ── Texte vorbereiten ───────────────────────────────────────────────
        QString valueText = "–";

        if (log.value != 0.0) {

            valueText =
                QString("%1%2")
                    .arg(log.value, 0, 'g', 15)
                    .arg(
                        log.unit.isEmpty()
                            ? ""
                            : " " + log.unit
                        );
        }

        // Nur ganze Minuten, Sekunden werden bewusst nicht angezeigt.
        QString durationText = "–";

        if (log.durationSeconds > 0) {

            durationText =
                QString("%1 min")
                    .arg(log.durationSeconds / 60);
        }

        // ── History-Karte ───────────────────────────────────────────────────
        // 82 + 12 + 82 = 176 = Höhe einer normalen Exercise-Karte.
        auto *card = new QFrame(ui->historyEntriesWidget);

        card->setObjectName("historyCard");
        card->setFrameShape(QFrame::StyledPanel);
        card->setFixedHeight(82);

        card->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Fixed
            );

        card->setCursor(Qt::PointingHandCursor);

        auto *cardLayout = new QVBoxLayout(card);

        cardLayout->setContentsMargins(12, 8, 12, 8);
        cardLayout->setSpacing(2);

        // Obere Zeile: Übungsname + Uhrzeit
        auto *topRow = new QHBoxLayout();
        topRow->setSpacing(8);

        auto *nameLabel = new QLabel(exercise.name, card);
        nameLabel->setObjectName("historyCardNameLabel");
        nameLabel->setWordWrap(false);

        topRow->addWidget(nameLabel, 1);

        auto *timeLabel =
            new QLabel(performedAt.toString("HH:mm"), card);

        timeLabel->setObjectName("historyCardTimeLabel");
        timeLabel->setProperty("role", "secondary");

        topRow->addWidget(
            timeLabel,
            0,
            Qt::AlignTop | Qt::AlignRight
            );

        cardLayout->addLayout(topRow);

        // Wert
        auto *valueRow = new QHBoxLayout();

        auto *valueHeader = new QLabel("Wert", card);
        valueHeader->setProperty("role", "muted");

        valueRow->addWidget(valueHeader);
        valueRow->addStretch();
        valueRow->addWidget(new QLabel(valueText, card));

        cardLayout->addLayout(valueRow);

        // Dauer
        auto *durationRow = new QHBoxLayout();

        auto *durationHeader = new QLabel("Dauer", card);
        durationHeader->setProperty("role", "muted");

        durationRow->addWidget(durationHeader);
        durationRow->addStretch();
        durationRow->addWidget(new QLabel(durationText, card));

        cardLayout->addLayout(durationRow);

        // Klick auf die Karte wird im eventFilter() behandelt.
        // Die Property "exerciseId" wird bereits vom eventFilter()
        // ausgewertet und öffnet die Übungs-Detailansicht.
        card->installEventFilter(this);
        card->setProperty("exerciseId", exercise.id);
        card->setProperty("isHistoryCard", true);


        // ── Karte ins Grid des aktuellen Tages einfügen ─────────────────────
        currentGrid->addWidget(
            card,
            cardsInCurrentGroup / columnCount,
            cardsInCurrentGroup % columnCount
            );

        ++cardsInCurrentGroup;
    }

    // Der Spacer liegt am Ende des dynamischen Layouts. Er ist das einzige
    // Element, das vertikal wachsen darf, daher bleibt alles oben.
    // Beim nächsten Aufruf wird er mit dem Layout zusammen entfernt,
    // es kann sich also kein zweiter Spacer ansammeln.
    content->addSpacerItem(
        new QSpacerItem(
            0,
            0,
            QSizePolicy::Minimum,
            QSizePolicy::Expanding
            )
        );

    // ── Empty-State ────────────────────────────────────────────────────────
    ui->historyEmptyStateLabel->setVisible(
        historyItems.isEmpty()
        );
}

// ── Timeline laden ───────────────────────────────────────────────────────────

void MainWindow::loadTimeline()
{
    // Ohne ausgewähltes Hobby gibt es keine Timeline,
    // die wir anzeigen können.
    if (currentHobbyId == 0)
        return;

    // Alle Phasen des aktuellen Hobbys aus der Datenbank laden.
    const QList<TimelinePhase> phases =
        TimelinePhaseRepository::getForHobby(currentHobbyId);

    // Die tatsächliche Farbe des aktuellen Hobbys ermitteln.
    // Fallback: Skillbase-Akzentfarbe, falls das Hobby (noch) keine
    // gültige Farbe besitzt.
    QColor hobbyColor("#ff6f61");

    for (const Hobby &hobby : HobbyRepository::getAll()) {

        if (hobby.id != currentHobbyId)
            continue;

        const QColor storedColor(hobby.color);

        if (storedColor.isValid())
            hobbyColor = storedColor;

        break;
    }

    // Die Phasen werden ausschließlich als Balken innerhalb der Timeline
    // dargestellt - eine separate Card-Liste gibt es nicht mehr.
    if (auto *timelineBar =
        qobject_cast<TimelineBarWidget *>(ui->timelineBarWidget)) {

        timelineBar->setPhases(phases);
    }

    // Empty-State nur anzeigen, wenn keine Phasen vorhanden sind.
    ui->timelineEmptyLabel->setVisible(phases.isEmpty());
}

// ── Ziele: Cards laden ───────────────────────────────────────────────────────
void MainWindow::loadGoalCards()
{
    // Entfernt alle bisher erzeugten Cards aus einem Layout.
    // Die Layout-Struktur selbst bleibt bestehen und kann anschließend
    // wieder mit neuen Cards befüllt werden.
    auto clearLayout = [](QLayout *layout) {

        while (layout->count() > 0) {
            QLayoutItem *item = layout->takeAt(0);

            if (item->widget())
                item->widget()->deleteLater();

            delete item;
        }
    };

    clearLayout(ui->goalOpenCardsLayout);
    clearLayout(ui->goalDoneCardsLayout);

    ui->goalOpenCardsLayout->setRowStretch(0, 0);
    ui->goalDoneCardsLayout->setRowStretch(0, 0);

    if (currentHobbyId == 0) {

        ui->goalsFilterOpenButton->setText("Offen");
        ui->goalsFilterDoneButton->setText("Geschafft");

        ui->goalsOpenEmptyLabel->setVisible(true);
        ui->goalsOpenScrollArea->setVisible(false);

        ui->goalsDoneEmptyLabel->setVisible(true);
        ui->goalsDoneScrollArea->setVisible(false);

        return;
    }

    const QString searchText =
        ui->goalSearchLineEdit->text().trimmed();

    QList<Goal> openGoals;
    QList<Goal> doneGoals;

    // Das Repository liefert offene Ziele bereits in ihrer gespeicherten
    // Drag-&-Drop-Reihenfolge. Erledigte Ziele werden anschließend separat
    // angezeigt.
    for (const Goal &goal : GoalRepository::getForHobby(currentHobbyId)) {

        if (!searchText.isEmpty() &&
            !goal.title.contains(searchText, Qt::CaseInsensitive) &&
            !goal.description.contains(searchText, Qt::CaseInsensitive)) {

            continue;
        }

        if (goal.isDone())
            doneGoals.append(goal);
        else
            openGoals.append(goal);
    }

    ui->goalsFilterOpenButton->setText(
        QString("Offen (%1)").arg(openGoals.size())
        );

    ui->goalsFilterDoneButton->setText(
        QString("Geschafft (%1)").arg(doneGoals.size())
        );

    // Goal-Cards verwenden bewusst dieselbe grundlegende Struktur wie
    // Exercise-Cards. Die Inhalte bleiben aber auf Ziele zugeschnitten.
    auto buildCard = [this](const Goal &goal, bool draggable) -> QFrame * {

        auto *card = new QFrame();

        card->setObjectName("goalCard");
        card->setFrameShape(QFrame::StyledPanel);

        // Die Goal-Cards haben dieselbe feste Höhe wie die Exercise-Cards.
        card->setFixedHeight(176);

        card->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Fixed
            );

        // Diese Eigenschaften werden vom bestehenden eventFilter()
        // für Klick und Drag & Drop verwendet.
        card->setProperty("goalId", goal.id);
        card->setProperty("goalDraggable", draggable);

        // Speichert, ob der Stern dauerhaft sichtbar sein soll.
        // Nur das aktuelle Hauptziel behält seinen Stern außerhalb des Hovers.
        card->setProperty(
            "goalIsCurrent",
            goal.isCurrent
            );

        card->setCursor(
            draggable
                ? Qt::OpenHandCursor
                : Qt::PointingHandCursor
            );

        card->installEventFilter(this);

        // Enter/Leave werden benötigt, damit der Stern bei normalen
        // Zielen nur während des Hovers eingeblendet wird.
        card->setAttribute(
            Qt::WA_Hover,
            true
            );

        auto *cardLayout =
            new QVBoxLayout(card);

        // Gleiche Innenabstände wie bei den Exercise-Cards.
        cardLayout->setContentsMargins(
            12,
            12,
            12,
            12
            );

        cardLayout->setSpacing(4);

        // ── Titel + Stern ────────────────────────────────────────────────

        auto *topRow =
            new QHBoxLayout();

        topRow->setSpacing(8);

        auto *titleLabel =
            new QLabel(
                goal.title,
                card
                );

        titleLabel->setObjectName(
            "goalCardTitleLabel"
            );

        titleLabel->setWordWrap(true);

        // Geschaffte Ziele werden wie bisher nur über eine gedämpfte
        // Textfarbe unterschieden und nicht durch eine zusätzliche
        // farbige Card.
        if (goal.isDone())
            titleLabel->setProperty(
                "role",
                "muted"
                );

        topRow->addWidget(
            titleLabel,
            1
            );

        // ── Hauptziel-Stern ──────────────────────────────────────────────
        //
        // Der Stern sitzt oben rechts auf der Karte.
        // Geschaffte Ziele können kein Hauptziel sein.
        //
        // Bei normalen offenen Zielen wird der Stern zunächst ausgeblendet
        // und über das bestehende Hover-Verhalten eingeblendet.
        // Das aktuelle Hauptziel bleibt dauerhaft sichtbar.

        if (!goal.isDone()) {

            auto *currentButton =
                new QToolButton(card);

            currentButton->setObjectName(
                "goalCurrentButton"
                );

            currentButton->setFixedSize(
                24,
                24
                );

            currentButton->setIconSize(
                QSize(16, 16)
                );

            currentButton->setCursor(
                Qt::PointingHandCursor
                );

            currentButton->setAutoRaise(true);

            if (!goal.isCurrent)
                currentButton->setVisible(false);

            currentButton->setIcon(
                QIcon(
                    goal.isCurrent
                        ? QStringLiteral(
                              ":/icons/star-filled.svg"
                              )
                        : QStringLiteral(
                              ":/icons/star-outline.svg"
                              )
                    )
                );

            currentButton->setToolTip(
                goal.isCurrent
                    ? QStringLiteral(
                          "Aktuelles Hauptziel"
                          )
                    : QStringLiteral(
                          "Als Hauptziel festlegen"
                          )
                );

            currentButton->installEventFilter(
                this
                );

            connect(
                currentButton,
                &QToolButton::clicked,
                this,
                [this, goalId = goal.id]() {

                    if (GoalRepository::setCurrent(
                            goalId,
                            true
                            )) {

                        loadGoalCards();

                        refreshHobbyDashboardOverview(
                            ui,
                            currentHobbyId
                            );
                    }
                }
                );

            // Der Stern wird oben rechts in der Kopfzeile platziert.
            topRow->addWidget(
                currentButton,
                0,
                Qt::AlignTop | Qt::AlignRight
                );
        }

        cardLayout->addLayout(
            topRow
            );

        // ── Beschreibung ─────────────────────────────────────────────────

        QString description =
            goal.description;

        // Die Beschreibung darf die feste Kartenhöhe nicht sprengen.
        // Deshalb wird nur die Darstellung auf der Card gekürzt;
        // der eigentliche Datenbanktext bleibt unverändert.
        constexpr int maxDescriptionChars = 90;

        if (description.length() > maxDescriptionChars) {

            description =
                description
                    .left(maxDescriptionChars)
                    .trimmed()
                + "…";
        }

        auto *descriptionLabel =
            new QLabel(
                description,
                card
                );

        descriptionLabel->setObjectName(
            "goalCardDescriptionLabel"
            );

        descriptionLabel->setProperty(
            "role",
            "secondary"
            );

        descriptionLabel->setWordWrap(true);

        descriptionLabel->setAlignment(
            Qt::AlignLeft | Qt::AlignTop
            );

        // Die Beschreibung nimmt den verfügbaren Platz zwischen
        // Kopfbereich und Deadline ein.
        cardLayout->addWidget(
            descriptionLabel,
            1
            );

        // ── Deadline ─────────────────────────────────────────────────────
        //
        // Die Deadline wird an derselben Stelle dargestellt,
        // an der bei den Exercise-Cards "Letzte Ausführung" steht:
        //
        // Deadline
        // 27.09.2026
        //
        // Es gibt bewusst keine Trennlinie.

        auto *deadlineHeader =
            new QLabel(
                "Deadline",
                card
                );

        deadlineHeader->setObjectName(
            "goalCardMetaHeaderLabel"
            );

        deadlineHeader->setProperty(
            "role",
            "muted"
            );

        cardLayout->addWidget(
            deadlineHeader
            );

        QString deadlineText =
            "–";

        if (!goal.deadline.isEmpty()) {

            const QDate deadlineDate =
                QDate::fromString(
                    goal.deadline,
                    "yyyy-MM-dd"
                    );

            if (deadlineDate.isValid()) {

                deadlineText =
                    deadlineDate.toString(
                        "dd.MM.yyyy"
                        );
            }
        }

        auto *deadlineValue =
            new QLabel(
                deadlineText,
                card
                );

        deadlineValue->setObjectName(
            "goalCardDeadlineLabel"
            );

        cardLayout->addWidget(
            deadlineValue
            );

        // ── Aktionen ─────────────────────────────────────────────────────
        //
        // Unten links befindet sich ausschließlich die Checkbox.
        // Der Stern befindet sich bereits oben rechts.

        auto *bottomRow =
            new QHBoxLayout();

        bottomRow->setContentsMargins(
            0,
            0,
            0,
            0
            );

        bottomRow->setSpacing(0);

        // ── Zielstatus ───────────────────────────────────────────────────

        auto *doneCheckBox =
            new QCheckBox(card);

        doneCheckBox->setObjectName(
            "goalDoneCheckBox"
            );

        doneCheckBox->setFixedSize(
            24,
            24
            );

        doneCheckBox->setCursor(
            Qt::PointingHandCursor
            );

        // Geschaffte Ziele starten angehakt.
        doneCheckBox->setChecked(
            goal.isDone()
            );

        connect(
            doneCheckBox,
            &QCheckBox::toggled,
            this,
            [this, goalId = goal.id](bool checked) {

                if (GoalRepository::setStatus(
                        goalId,
                        checked
                            ? QStringLiteral("done")
                            : QStringLiteral("open")
                        )) {

                    // Der Sound ertönt nur beim Abschließen des Ziels,
                    // nicht beim Zurücksetzen auf "offen".
                    if (checked &&
                        goalCompletedSound) {

                        goalCompletedSound->play();
                    }

                    loadGoalCards();

                    refreshHobbyDashboardOverview(
                        ui,
                        currentHobbyId
                        );
                }
            }
            );

        bottomRow->addWidget(
            doneCheckBox,
            0,
            Qt::AlignLeft | Qt::AlignBottom
            );

        cardLayout->addLayout(
            bottomRow
            );

        return card;
    };

    // Wie bei den Exercise-Cards werden die Goals in einem Raster
    // mit drei Cards pro Zeile dargestellt.
    constexpr int columnCount = 3;

    // Bei aktiver Suche ohne Treffer zeigen wir - analog zur
    // Übungsübersicht - ein "Keine Ziele gefunden"-Label im Grid
    // statt des zentrierten Empty-State.
    const bool isSearching = !searchText.isEmpty();

    // ── Offene Ziele ─────────────────────────────────────────────────────

    if (isSearching && openGoals.isEmpty()) {

        auto *emptyLabel =
            new QLabel(
                "Keine Ziele gefunden.",
                ui->goalOpenCardsWidget
                );

        emptyLabel->setAlignment(Qt::AlignCenter);

        ui->goalOpenCardsLayout->addWidget(
            emptyLabel,
            0,
            0,
            1,
            columnCount
            );
    }

    for (int i = 0;
         i < openGoals.size();
         ++i) {

        QFrame *card =
            buildCard(
                openGoals[i],
                true
                );

        ui->goalOpenCardsLayout->addWidget(
            card,
            i / columnCount,
            i % columnCount
            );
    }

    // Der Stretch nimmt den übrigen vertikalen Platz ein.
    // Dadurch bleiben die Goal-Cards am oberen Rand des Containers.
    const int openRowCount =
        (openGoals.size() + columnCount - 1)
        / columnCount;

    ui->goalOpenCardsLayout->setRowStretch(
        openRowCount,
        1
        );

    // ── Geschaffte Ziele ─────────────────────────────────────────────────

    if (isSearching && doneGoals.isEmpty()) {

        auto *emptyLabel =
            new QLabel(
                "Keine Ziele gefunden.",
                ui->goalDoneCardsWidget
                );

        emptyLabel->setAlignment(Qt::AlignCenter);

        ui->goalDoneCardsLayout->addWidget(
            emptyLabel,
            0,
            0,
            1,
            columnCount
            );
    }

    for (int i = 0;
         i < doneGoals.size();
         ++i) {

        QFrame *card =
            buildCard(
                doneGoals[i],
                false
                );

        ui->goalDoneCardsLayout->addWidget(
            card,
            i / columnCount,
            i % columnCount
            );
    }

    // Der zentrierte Empty-State erscheint nur, wenn es wirklich
    // keine Ziele gibt (nicht bei aktiver Suche). Bei einer Suche
    // ohne Treffer übernimmt das Label im Grid (siehe oben).
    const bool showOpenEmptyState =
        openGoals.isEmpty() && !isSearching;

    const bool showDoneEmptyState =
        doneGoals.isEmpty() && !isSearching;

    ui->goalsOpenEmptyLabel->setVisible(showOpenEmptyState);
    ui->goalsOpenScrollArea->setVisible(!showOpenEmptyState);

    ui->goalsDoneEmptyLabel->setVisible(showDoneEmptyState);
    ui->goalsDoneScrollArea->setVisible(!showDoneEmptyState);

    const int doneRowCount =
        (doneGoals.size() + columnCount - 1)
        / columnCount;

    // Der Stretch nimmt den übrigen vertikalen Platz ein.
    // Dadurch bleiben die Goal-Cards am oberen Rand des Containers.
    ui->goalDoneCardsLayout->setRowStretch(
        doneRowCount,
        1
        );
}





// ── Ziele: Erstellen / Bearbeiten ────────────────────────────────────────────

void MainWindow::openGoalDialog(int goalId)
{
    if (currentHobbyId == 0)
        return;

    const bool isNew = (goalId == 0);

    Goal goal;

    if (!isNew && !GoalRepository::getById(goalId, goal))
        return;

    QDialog dialog(this);
    Ui::GoalDialog dialogUi;
    dialogUi.setupUi(&dialog);

    dialog.setWindowTitle(isNew ? "Neues Ziel" : "Ziel bearbeiten");

    // Im Erstellen-Modus gibt es bewusst weder "Als geschafft markieren"
    // noch "Löschen" (siehe Feature-Konzept "Ziele", Abschnitt 3).
    dialogUi.statusButton->setVisible(!isNew);
    dialogUi.deleteButton->setVisible(!isNew);

    if (auto *saveButton = dialogUi.buttonBox->button(QDialogButtonBox::Save)) {
        saveButton->setText(isNew ? "Erstellen" : "Speichern");
        // Speichern/Erstellen ist die primäre Aktion des Dialogs.
        saveButton->setDefault(true);
    }

    if (auto *cancelButton =
        dialogUi.buttonBox->button(QDialogButtonBox::Cancel)) {
        cancelButton->setText("Abbrechen");
    }

    if (isNew) {

        dialogUi.hasDeadlineCheckBox->setChecked(false);
        dialogUi.deadlineDateEdit->setDate(QDate::currentDate());

    } else {

        dialogUi.titleLineEdit->setText(goal.title);
        dialogUi.descriptionTextEdit->setPlainText(goal.description);

        const bool hasDeadline = !goal.deadline.isEmpty();
        dialogUi.hasDeadlineCheckBox->setChecked(hasDeadline);
        dialogUi.deadlineDateEdit->setDate(
            hasDeadline
                ? QDate::fromString(goal.deadline, "yyyy-MM-dd")
                : QDate::currentDate()
            );

        dialogUi.statusButton->setText(
            goal.isDone()
                ? "Offen"
                : "Geschafft"
            );
    }

    connect(dialogUi.deleteButton, &QPushButton::clicked, &dialog, [&]() {

        const auto answer = QMessageBox::question(
            &dialog,
            "Ziel löschen?",
            QString("\"%1\" wirklich löschen?").arg(goal.title),
            QMessageBox::Cancel | QMessageBox::Yes,
            QMessageBox::Cancel
            );

        if (answer != QMessageBox::Yes)
            return;

        if (GoalRepository::remove(goalId)) {
            dialog.reject();
        } else {
            qDebug() << "Ziel konnte nicht gelöscht werden.";
        }
    });

    connect(dialogUi.statusButton, &QPushButton::clicked, &dialog, [&]() {

        const QString newStatus = goal.isDone() ? "open" : "done";

        if (!GoalRepository::setStatus(goalId, newStatus)) {
            qDebug() << "Ziel-Status konnte nicht geändert werden.";
            return;
        }

        // TODO: Hier könnte künftig ein kurzer, dezenter Bestätigungston
        // abgespielt werden (Feature-Konzept "Ziele", Abschnitt 5). Dafür
        // wird das Qt-Multimedia-Modul benötigt, das aktuell nicht in
        // CMakeLists.txt eingebunden ist.

        dialog.accept();
    });

    if (dialog.exec() != QDialog::Accepted) {
        // Deckt sowohl "Abbrechen" als auch das Löschen ab (das den
        // Dialog über reject() schließt) - in beiden Fällen müssen die
        // Cards ggf. neu geladen werden.
        loadGoalCards();
        return;
    }

    // Erreicht über "Erstellen"/"Speichern" oder über den Status-Button -
    // beide schließen den Dialog über accept().
    const QString title =
        dialogUi.titleLineEdit->text().trimmed();
    const QString description =
        dialogUi.descriptionTextEdit->toPlainText().trimmed();

    if (title.isEmpty()) {
        QMessageBox::warning(
            &dialog,
            "Titel fehlt",
            "Bitte gib einen Titel für das Ziel ein."
            );

        dialogUi.titleLineEdit->setFocus();
        return;
    }

    const QString deadline =
        dialogUi.hasDeadlineCheckBox->isChecked()
            ? dialogUi.deadlineDateEdit->date().toString("yyyy-MM-dd")
            : QString();

    if (isNew) {

        int newId = 0;

        if (!GoalRepository::add(
                currentHobbyId, title, description, deadline, newId)) {
            qDebug() << "Ziel konnte nicht gespeichert werden.";
        }

    } else {

        if (!GoalRepository::update(goalId, title, description, deadline))
            qDebug() << "Ziel konnte nicht aktualisiert werden.";
    }

    loadGoalCards();
    refreshHobbyDashboardOverview(ui, currentHobbyId);
}

void MainWindow::loadRoutineCards()
{
    // Entfernt alle bisher erzeugten Cards aus einem Layout.
    // Das Layout selbst bleibt bestehen und kann danach wieder
    // mit neuen Cards gefüllt werden.
    auto clearLayout = [](QLayout *layout) {

        while (layout->count() > 0) {

            QLayoutItem *item =
                layout->takeAt(0);

            if (item->widget())
                item->widget()->deleteLater();

            delete item;
        }
    };

    clearLayout(ui->routineActiveCardsLayout);
    clearLayout(ui->routineArchiveCardsLayout);

    ui->routineActiveCardsLayout->setRowStretch(0, 0);
    ui->routineArchiveCardsLayout->setRowStretch(0, 0);

    if (currentHobbyId == 0) {

        // Wenn kein Hobby ausgewählt ist, werden beide Empty States
        // angezeigt und die Scrollbereiche ausgeblendet.
        ui->routinesActiveEmptyLabel->setVisible(true);
        ui->routinesActiveScrollArea->setVisible(false);



        return;
    }

    QList<Routine> routines =
        RoutineRepository::getForHobby(
            currentHobbyId
            );

    // Ausgewählte Routinen stehen immer vor den anderen Routinen.
    // Die bisherige Reihenfolge innerhalb der beiden Gruppen bleibt erhalten.
    std::stable_sort(
        routines.begin(),
        routines.end(),
        [](const Routine &a, const Routine &b) {
            return a.isCurrent && !b.isCurrent;
        }
        );

    constexpr int columnCount = 3;

    int activeCount = 0;
    int archiveCount = 0;

    // Erstellt eine einzelne Routine-Card.
    auto buildCard =
        [this](const Routine &routine) -> QFrame * {

        auto *card =
            new QFrame();

        card->setObjectName(
            "routineCard"
            );


        card->setProperty(
            "routineId",
            routine.id
            );

        card->setProperty(
            "routineIsCurrent",
            routine.isCurrent
            );

        card->installEventFilter(this);

        card->setAttribute(
            Qt::WA_Hover,
            true
            );

        card->setFrameShape(
            QFrame::StyledPanel
            );

        // Zwei normale Card-Höhen + ein normaler Card-Abstand.
        card->setFixedHeight(364);

        card->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Fixed
            );

        card->setCursor(
            Qt::PointingHandCursor
            );

        auto *cardLayout =
            new QVBoxLayout(card);

        cardLayout->setContentsMargins(
            12,
            12,
            12,
            12
            );

        cardLayout->setSpacing(4);

        // ─────────────────────────────────────────────────────────
        // Kopf: Name + Stern
        // ─────────────────────────────────────────────────────────

        auto *topRow =
            new QHBoxLayout();

        topRow->setContentsMargins(
            0,
            0,
            0,
            0
            );

        topRow->setSpacing(8);

        auto *nameLabel =
            new QLabel(
                routine.name,
                card
                );

        nameLabel->setObjectName(
            "routineCardNameLabel"
            );

        nameLabel->setWordWrap(true);

        topRow->addWidget(
            nameLabel,
            1
            );

        if (!routine.archived) {

            auto *favoriteButton =
                new QToolButton(card);

            favoriteButton->setObjectName(
                "routineFavoriteButton"
                );

            favoriteButton->setFixedSize(
                24,
                24
                );

            favoriteButton->setIconSize(
                QSize(16, 16)
                );

            favoriteButton->setAutoRaise(true);

            favoriteButton->setCursor(
                Qt::PointingHandCursor
                );

            // Aktuelle Routinen zeigen den Stern dauerhaft.
            // Nicht ausgewählte aktive Routinen zeigen ihn erst beim Hover.
            if (!routine.isCurrent)
                favoriteButton->setVisible(false);

            favoriteButton->setIcon(
                QIcon(
                    routine.isCurrent
                        ? ":/icons/star-filled.svg"
                        : ":/icons/star-outline.svg"
                    )
                );

            favoriteButton->setToolTip(
                routine.isCurrent
                    ? "Ausgewählte Routine"
                    : "Als Routine für das Dashboard auswählen"
                );

            connect(
                favoriteButton,
                &QToolButton::clicked,
                this,
                [this, routineId = routine.id]() {

                    Routine routine;

                    if (!RoutineRepository::getById(
                            routineId,
                            routine
                            )) {
                        return;
                    }

                    const bool newCurrentState =
                        !routine.isCurrent;

                    if (!RoutineRepository::setCurrent(
                            routineId,
                            newCurrentState
                            )) {

                        if (newCurrentState) {
                            QMessageBox::information(
                                this,
                                "Routinen auswählen",
                                "Du kannst maximal 3 Routinen pro Hobby auswählen."
                                );
                        }

                        return;
                    }

                    loadRoutineCards();
                    loadDashboardRoutineCards();
                }
                );

            topRow->addWidget(
                favoriteButton,
                0,
                Qt::AlignTop | Qt::AlignRight
                );
        }

        cardLayout->addLayout(
            topRow
            );

        // ─────────────────────────────────────────────────────────
        // Beschreibung
        // ─────────────────────────────────────────────────────────

        auto *descriptionLabel =
            new QLabel(
                routine.description,
                card
                );

        descriptionLabel->setObjectName(
            "routineCardDescriptionLabel"
            );

        descriptionLabel->setProperty(
            "role",
            "secondary"
            );

        descriptionLabel->setWordWrap(true);

        descriptionLabel->setMaximumHeight(
            42
            );

        cardLayout->addWidget(
            descriptionLabel
            );

        // ─────────────────────────────────────────────────────────
        // Übungen
        // ─────────────────────────────────────────────────────────

        auto *exercisesHeader =
            new QLabel(
                "Übungen",
                card
                );

        exercisesHeader->setObjectName(
            "routineCardMetaHeaderLabel"
            );

        exercisesHeader->setProperty(
            "role",
            "muted"
            );

        cardLayout->addWidget(
            exercisesHeader
            );

        const QList<RoutineStep> steps =
            RoutineRepository::getSteps(
                routine.id
                );

        const QList<Exercise> exercises =
            ExerciseRepository::getForHobby(
                currentHobbyId,
                true
                );

        QHash<int, QString> exerciseNames;

        for (const Exercise &exercise : exercises) {

            exerciseNames.insert(
                exercise.id,
                exercise.name
                );
        }

        constexpr int visibleExerciseCount = 4;

        int shownCount = 0;

        int totalDurationSeconds = 0;

        for (const RoutineStep &step : steps) {

            totalDurationSeconds +=
                step.durationSeconds;

            if (shownCount >= visibleExerciseCount)
                continue;

            auto *exerciseRow =
                new QHBoxLayout();

            exerciseRow->setSpacing(
                8
                );

            auto *exerciseLabel =
                new QLabel(
                    exerciseNames.value(
                        step.exerciseId,
                        "Unbekannte Übung"
                        ),
                    card
                    );

            exerciseLabel->setObjectName(
                "routineCardExerciseLabel"
                );

            exerciseRow->addWidget(
                exerciseLabel
                );

            exerciseRow->addStretch();

            const int minutes =
                step.durationSeconds / 60;

            auto *durationLabel =
                new QLabel(
                    QString("%1 min")
                        .arg(minutes),
                    card
                    );

            durationLabel->setObjectName(
                "routineCardDurationLabel"
                );

            durationLabel->setProperty(
                "role",
                "secondary"
                );

            exerciseRow->addWidget(
                durationLabel
                );

            cardLayout->addLayout(
                exerciseRow
                );

            ++shownCount;
        }

        if (steps.size() > visibleExerciseCount) {

            const int remaining =
                steps.size() -
                visibleExerciseCount;

            auto *moreLabel =
                new QLabel(
                    QString("+ %1 weitere")
                        .arg(remaining),
                    card
                    );

            moreLabel->setObjectName(
                "routineCardMoreLabel"
                );

            moreLabel->setProperty(
                "role",
                "secondary"
                );

            cardLayout->addWidget(
                moreLabel
                );
        }

        // ─────────────────────────────────────────────────────────
        // Gesamtdauer
        // ─────────────────────────────────────────────────────────

        const int totalMinutes =
            totalDurationSeconds / 60;

        auto *totalDurationRow =
            new QHBoxLayout();

        auto *totalDurationLabel =
            new QLabel(
                "Gesamtdauer",
                card
                );

        totalDurationLabel->setObjectName(
            "routineCardMetaHeaderLabel"
            );

        totalDurationLabel->setProperty(
            "role",
            "muted"
            );

        totalDurationRow->addWidget(
            totalDurationLabel
            );

        totalDurationRow->addStretch();

        auto *totalDurationValue =
            new QLabel(
                QString("%1 min")
                    .arg(totalMinutes),
                card
                );

        totalDurationValue->setObjectName(
            "routineCardDurationLabel"
            );

        totalDurationRow->addWidget(
            totalDurationValue
            );

        cardLayout->addLayout(
            totalDurationRow
            );

        // ─────────────────────────────────────────────────────────
        // Letzte Ausführung
        // ─────────────────────────────────────────────────────────

        // Der Stretch sorgt dafür, dass dieser Bereich immer
        // möglichst weit unten in der Card sitzt.
        cardLayout->addStretch();

        auto *lastHeader =
            new QLabel(
                "Letzte Ausführung",
                card
                );

        lastHeader->setObjectName(
            "routineCardMetaHeaderLabel"
            );

        lastHeader->setProperty(
            "role",
            "muted"
            );

        cardLayout->addWidget(
            lastHeader
            );

        const QList<RoutineLog> logs =
            RoutineRepository::getLogs(
                routine.id
                );

        auto *lastRow =
            new QHBoxLayout();

        if (!logs.isEmpty()) {

            const QDateTime performedAt =
                QDateTime::fromString(
                    logs.first().performedAt,
                    "yyyy-MM-dd HH:mm:ss"
                    ).toLocalTime();

            lastRow->addWidget(
                new QLabel(
                    performedAt.toString(
                        "dd.MM.yyyy"
                        ),
                    card
                    )
                );

        } else {

            lastRow->addWidget(
                new QLabel(
                    "–",
                    card
                    )
                );
        }

        lastRow->addStretch();

        cardLayout->addLayout(
            lastRow
            );

        // ─────────────────────────────────────────────────────────
        // Trennlinie
        // ─────────────────────────────────────────────────────────

        auto *separator =
            new QFrame(card);

        separator->setObjectName(
            "routineCardSeparator"
            );

        separator->setFrameShape(
            QFrame::HLine
            );

        cardLayout->addWidget(
            separator
            );

        // ─────────────────────────────────────────────────────────
        // Buttons
        // ─────────────────────────────────────────────────────────

        auto *buttonRow =
            new QHBoxLayout();

        auto *editButton =
            new QPushButton(
                "Bearbeiten",
                card
                );

        auto *executeButton =
            new QPushButton(
                "Ausführen",
                card
                );

        executeButton->setEnabled(
            !routine.archived
            );

        executeButton->setObjectName(
            "routineCardRunButton"
            );

        buttonRow->addWidget(
            editButton
            );

        buttonRow->addWidget(
            executeButton
            );

        cardLayout->addLayout(
            buttonRow
            );

        // Die Aktionen werden später mit Bearbeiten/Ausführen verbunden.
        connect(
            editButton,
            &QPushButton::clicked,
            this,
            [this, routine]() {

                // Der gleiche Dialog wird im Bearbeitungsmodus geöffnet.
                // Die Routine-ID sorgt dafür, dass Name, Beschreibung
                // und Übungen aus der Datenbank geladen werden.
                RoutineDialog dialog(
                    routine.hobbyId,
                    routine.id,
                    this
                    );

                // Nach erfolgreichem Speichern wird die Routinenliste
                // neu aufgebaut, damit die Änderungen sofort sichtbar sind.
                if (dialog.exec() == QDialog::Accepted) {
                    loadRoutineCards();
                    loadDashboardRoutineCards();
                }
            }
            );

        connect(
            executeButton,
            &QPushButton::clicked,
            this,
            [this, routine]() {

                // Merkt die Routine, lädt ihre Übungen und wechselt
                // von der Übersicht auf die Ausführungsansicht.
                showRoutineExecution(routine.id);
            }
            );

        return card;
    };

    // ─────────────────────────────────────────────────────────────
    // Cards auf aktive / archivierte Ansicht verteilen
    // ─────────────────────────────────────────────────────────────

    for (const Routine &routine : routines) {

        QFrame *card =
            buildCard(routine);

        if (routine.archived) {

            ui->routineArchiveCardsLayout->addWidget(
                card,
                archiveCount / columnCount,
                archiveCount % columnCount
                );

            ++archiveCount;

        } else {

            ui->routineActiveCardsLayout->addWidget(
                card,
                activeCount / columnCount,
                activeCount % columnCount
                );

            ++activeCount;
        }
    }

    // ─────────────────────────────────────────────────────────────
    // Aktive Ansicht
    // ─────────────────────────────────────────────────────────────

    // Die aktive Seite zeigt entweder den Empty-State
    // oder die vorhandenen Routine-Karten.
    ui->routinesActiveEmptyLabel->setVisible(
        activeCount == 0
        );

    ui->routinesActiveScrollArea->setVisible(
        activeCount > 0
        );
    // ─────────────────────────────────────────────────────────────
    // Archiv-Ansicht
    // ─────────────────────────────────────────────────────────────
    // Die Archivseite zeigt entweder den Empty-State
    // oder die vorhandenen archivierten Routinen.
    ui->routinesArchiveEmptyLabel->setVisible(
        archiveCount == 0
        );

    ui->routinesArchiveScrollArea->setVisible(
        archiveCount > 0
        );

    // Stretch hält die Cards am oberen Rand.
    const int activeRowCount =
        (activeCount + columnCount - 1) /
        columnCount;

    const int archiveRowCount =
        (archiveCount + columnCount - 1) /
        columnCount;

    ui->routineActiveCardsLayout->setRowStretch(
        activeRowCount,
        1
        );

    ui->routineArchiveCardsLayout->setRowStretch(
        archiveRowCount,
        1
        );
}

void MainWindow::loadRoutineExecution()
{
    // Ohne ausgewählte Routine gibt es nichts anzuzeigen.
    if (currentExecutionRoutineId == 0)
        return;

    setRoutineExecutionRunning(false);
    routineExecutionItems.clear();
    routineExecutionIndex = -1;

    // Bisherige Einträge inklusive Stretch-Items entfernen.
    while (QLayoutItem *item =
               ui->routineExecutionExerciseListLayout->takeAt(0)) {

        if (QWidget *widget = item->widget())
            widget->deleteLater();

        delete item;
    }

    // Die gespeicherten Schritte der Routine laden.
    const QList<RoutineStep> steps =
        RoutineRepository::getSteps(
            currentExecutionRoutineId
            );

    // Die Übungen des aktuellen Hobbys laden.
    // Darüber bekommen wir Name, Beschreibung, Wert und Einheit.
    const QList<Exercise> exercises =
        ExerciseRepository::getForHobby(
            currentHobbyId,
            true
            );

    QHash<int, Exercise> exerciseById;

    for (const Exercise &exercise : exercises) {
        exerciseById.insert(
            exercise.id,
            exercise
            );
    }

    for (const RoutineStep &step : steps) {

        // Schritte, deren Übung nicht gefunden wird, werden übersprungen.
        if (!exerciseById.contains(step.exerciseId))
            continue;

        const Exercise exercise =
            exerciseById.value(step.exerciseId);

        // Aktueller Wert = zuletzt gespeicherter Wert.
        // Ohne bisherige Ausführung gilt der Übungswert.
        ExerciseLog latestLog;

        const bool hasLatestLog =
            ExerciseLogRepository::getLatestForExercise(
                exercise.id,
                latestLog
                );

        RoutineExecutionItem item;
        item.exerciseId       = exercise.id;
        item.name             = exercise.name;
        item.description      = exercise.description;
        item.unit             = exercise.unit;
        item.currentValue     = hasLatestLog ? latestLog.value : exercise.value;
        item.goal             = exercise.goal;
        item.durationSeconds  = step.durationSeconds;
        item.remainingMs      = step.durationSeconds * 1000;

        routineExecutionItems.append(item);
    }

    // Stretch vor und nach den Buttons zentriert die Übungsleiste.
    ui->routineExecutionExerciseListLayout->addStretch();

    for (int index = 0; index < routineExecutionItems.size(); ++index) {

        auto *button =
            new QPushButton(
                routineExecutionItems.at(index).name,
                ui->routineExecutionExerciseListContainer
                );

        button->setObjectName(
            "routineExecutionExerciseButton"
            );

        button->setCursor(
            Qt::PointingHandCursor
            );

        button->setCheckable(true);

        // Es ist immer genau eine Übung hervorgehoben.
        button->setAutoExclusive(true);

        // Gleiche Höhe wie die Pfeile links und rechts.
        button->setFixedHeight(36);

        // Die Position des Routine-Schritts wird am Button gespeichert.
        button->setProperty(
            "routineExerciseIndex",
            index
            );

        ui->routineExecutionExerciseListLayout->addWidget(
            button
            );

        connect(
            button,
            &QPushButton::clicked,
            this,
            [this, index]() {
                selectRoutineExecutionExercise(index);
            }
            );
    }

    ui->routineExecutionExerciseListLayout->addStretch();

    if (!routineExecutionItems.isEmpty()) {
        selectRoutineExecutionExercise(0);
        return;
    }

    // Routine ohne (auffindbare) Übungen: Anzeige leeren.
    ui->routineExecutionExerciseNameLabel->setText(
        "Diese Routine enthält keine Übungen."
        );
    ui->routineExecutionExerciseDescriptionLabel->clear();
    ui->routineExecutionExerciseTargetLabel->clear();
    ui->routineExecutionExerciseCurrentValueLabel->clear();
    ui->routineExecutionExerciseValueEdit->clear();
    ui->routineExecutionExerciseValueEdit->setEnabled(false);
    ui->routineExecutionTimerLabel->setText(formatCountdown(0));
    ui->routineExecutionProgressBar->setValue(0);
    ui->routineExecutionStartPauseButton->setEnabled(false);
    ui->routineExecutionStopButton->setEnabled(false);
    updateRoutineExecutionNavigation();
}

void MainWindow::showRoutineExecution(int routineId)
{
    // Die ID merken, damit die Ausführungsansicht weiß,
    // welche Routine gerade gestartet wurde.
    currentExecutionRoutineId = routineId;

    // Seitenrand der Hobby-Seiten merken und links/rechts entfernen,
    // damit die Trennlinie über die ganze Breite geht.
    if (!routineExecutionMarginsOverridden) {
        routineExecutionSavedMargins =
            ui->hobbyPageStackLayout->contentsMargins();
        routineExecutionMarginsOverridden = true;
    }

    ui->hobbyPageStackLayout->setContentsMargins(
        0,
        routineExecutionSavedMargins.top(),
        0,
        routineExecutionSavedMargins.bottom()
        );

    // Die Übungen dieser Routine dynamisch aus der Datenbank laden.
    loadRoutineExecution();

    // Sicherstellen, dass wir auf der Routinen-Seite sind, bevor
    // der innere Stack auf die Ausführungsansicht wechselt.
    ui->hobbyPageStack->setCurrentWidget(
        ui->routinesPage
        );

    ui->routineViewStack->setCurrentWidget(
        ui->routineExecutionPage
        );
}

void MainWindow::leaveRoutineExecution()
{
    setRoutineExecutionRunning(false);

    routineExecutionItems.clear();
    routineExecutionIndex = -1;
    currentExecutionRoutineId = 0;

    // Falls die Fortschrittsseite aus dieser Ausführung geöffnet war,
    // Merker zurücksetzen.
    if (progressReturnSource == ProgressReturnSource::RoutineExecution)
        progressReturnSource = ProgressReturnSource::None;

    // Ursprüngliche Seitenränder wiederherstellen.
    if (routineExecutionMarginsOverridden) {
        ui->hobbyPageStackLayout->setContentsMargins(
            routineExecutionSavedMargins
            );
        routineExecutionMarginsOverridden = false;
    }

    ui->routineViewStack->setCurrentWidget(
        ui->routineOverviewPage
        );
}



void MainWindow::finishRoutineExecution()
{
    setRoutineExecutionRunning(false);

    const bool saved = saveRoutineExecutionResults();

    leaveRoutineExecution();

    // Nach dem Speichern alle Ansichten aktualisieren, die Logs anzeigen.
    if (saved) {
        loadExerciseCards();
        loadHistory();
        loadTimeline();
        loadRoutineCards();
        loadDashboardRoutineCards();
    }
}

bool MainWindow::saveRoutineExecutionResults()
{
    if (currentExecutionRoutineId == 0)
        return false;

    bool savedAnything = false;
    int totalElapsedSeconds = 0;
    bool anyValueEntered = false;

    for (const RoutineExecutionItem &item :
         std::as_const(routineExecutionItems)) {

        const int elapsedSeconds = qRound(item.elapsedMs / 1000.0);
        totalElapsedSeconds += elapsedSeconds;

        // Komma und Punkt werden beide akzeptiert.
        QString text = item.enteredText.trimmed();
        text.replace(',', '.');

        bool ok = false;
        const double newValue = text.toDouble(&ok);

        // Übungen ohne eingetragenen Wert werden nicht gespeichert.
        if (!ok)
            continue;

        anyValueEntered = true;

        int logId = 0;

        // Ein Übungs-Log pro Übung: neuer Wert, Einheit und die
        // tatsächlich gelaufene Zeit. Karten, Verlauf und Fortschritt
        // lesen den letzten Log, deshalb erscheint der Wert überall.
        if (!ExerciseLogRepository::add(
                item.exerciseId,
                newValue,
                item.unit,
                elapsedSeconds,
                logId)) {

            qDebug() << "Übungs-Log der Routine konnte nicht gespeichert werden:"
                     << item.exerciseId;
            continue;
        }

        savedAnything = true;
    }

    // Das Routine-Log wird nur geschrieben, wenn wirklich etwas passiert
    // ist (Wert eingetragen oder Zeit gelaufen). Ein versehentliches
    // Öffnen und Beenden erzeugt keinen Eintrag.
    if (anyValueEntered || totalElapsedSeconds > 0) {

        int routineLogId = 0;

        if (RoutineRepository::addLog(
                currentExecutionRoutineId,
                totalElapsedSeconds,
                routineLogId)) {
            savedAnything = true;
        } else {
            qDebug() << "Routine-Log konnte nicht gespeichert werden.";
        }
    }

    return savedAnything;
}

void MainWindow::selectRoutineExecutionExercise(int index)
{
    if (index < 0 || index >= routineExecutionItems.size())
        return;

    // Beim Wechsel läuft kein Timer weiter, die Restzeit bleibt erhalten.
    setRoutineExecutionRunning(false);

    routineExecutionIndex = index;

    const RoutineExecutionItem &item =
        routineExecutionItems.at(index);

    ui->routineExecutionExerciseNameLabel->setText(item.name);
    ui->routineExecutionExerciseDescriptionLabel->setText(item.description);

    ui->routineExecutionExerciseTargetLabel->setText(
        formatValueWithUnit(item.goal, item.unit));
    ui->routineExecutionExerciseCurrentValueLabel->setText(
        formatValueWithUnit(item.currentValue, item.unit));

    ui->routineExecutionExerciseValueEdit->setEnabled(true);
    ui->routineExecutionExerciseValueEdit->setText(item.enteredText);

    // Passenden Button hervorheben und in die Leiste scrollen.
    for (int i = 0;
         i < ui->routineExecutionExerciseListLayout->count();
         ++i) {

        QWidget *widget =
            ui->routineExecutionExerciseListLayout->itemAt(i)->widget();

        if (!widget)
            continue;

        if (widget->property("routineExerciseIndex").toInt() == index) {

            if (auto *button = qobject_cast<QPushButton *>(widget))
                button->setChecked(true);

            ui->routineExecutionExerciseList->ensureWidgetVisible(widget);
            break;
        }
    }

    updateRoutineExecutionTimerDisplay();
    updateRoutineExecutionNavigation();
}

void MainWindow::updateRoutineExecutionTimerDisplay()
{
    if (routineExecutionIndex < 0
        || routineExecutionIndex >= routineExecutionItems.size())
        return;

    const RoutineExecutionItem &item =
        routineExecutionItems.at(routineExecutionIndex);

    // Aufrunden: "00:01" bleibt sichtbar, bis wirklich 0 erreicht ist.
    ui->routineExecutionTimerLabel->setText(
        formatCountdown((item.remainingMs + 999) / 1000)
        );

    // 1000 = volle Zeit verbleibt, 0 = 00:00.
    const int value = item.durationSeconds > 0
        ? qRound(1000.0 * item.remainingMs / (item.durationSeconds * 1000.0))
        : 0;

    ui->routineExecutionProgressBar->setValue(value);

    // Buttons bleiben immer aktiv.
    // Ohne Zeit machen sie aber nichts (siehe Handler).
    ui->routineExecutionStartPauseButton->setEnabled(true);
    ui->routineExecutionStopButton->setEnabled(true);
}

void MainWindow::updateRoutineExecutionNavigation()
{
    const int count = routineExecutionItems.size();

    // Nicht benötigte Pfeile werden ausgeblendet, nicht deaktiviert.
    ui->routineExecutionPreviousButton->setVisible(
        routineExecutionIndex > 0
        );

    ui->routineExecutionNextButton->setVisible(
        routineExecutionIndex >= 0
        && routineExecutionIndex < count - 1
        );
}

void MainWindow::setRoutineExecutionRunning(bool running)
{
    if (running) {
        routineExecutionClock.start();
        routineExecutionTimer->start();
    } else {
        routineExecutionTimer->stop();
    }

    ui->routineExecutionStartPauseButton->setText(QString());

    ui->routineExecutionStartPauseButton->setIcon(
        QIcon(
            running
                ? QStringLiteral(":/icons/pause.svg")
                : QStringLiteral(":/icons/play.svg")
            )
        );
    updateRoutineExecutionTimerDisplay();
}

void MainWindow::loadDashboardRoutineCards()
{
    // Der Container wird bei jedem Laden komplett neu aufgebaut.
    // Dadurch stimmt der Inhalt immer mit dem aktuell ausgewählten Hobby überein.
    while (ui->hobbyCurrentRoutinesLayout->count() > 0) {

        QLayoutItem *item =
            ui->hobbyCurrentRoutinesLayout->takeAt(0);

        if (item->widget())
            item->widget()->deleteLater();

        delete item;
    }

    if (currentHobbyId == 0)
        return;

    const QList<Routine> routines =
        RoutineRepository::getForHobby(
            currentHobbyId
            );

    // Zunächst werden nur nicht archivierte und ausgewählte Routinen benötigt.
    QList<Routine> currentRoutines;

    for (const Routine &routine : routines) {

        if (routine.archived)
            continue;

        if (!routine.isCurrent)
            continue;

        currentRoutines.append(routine);
    }

    // Erstellt eine kompakte Version der normalen Routine-Card.
    // Die Struktur bleibt gleich, aber Letzte Ausführung,
    // Trennlinie sowie Bearbeiten/Ausführen werden auf dem Dashboard nicht benötigt.
    auto buildDashboardCard =
        [this](const Routine &routine) -> QFrame * {

        auto *card =
            new QFrame();

        card->setObjectName(
            "dashboardRoutineCard"
            );

        card->setProperty(
            "routineId",
            routine.id
            );

        card->setProperty(
            "routineIsCurrent",
            routine.isCurrent
            );

        card->installEventFilter(this);

        card->setAttribute(
            Qt::WA_Hover,
            true
            );

        card->setFrameShape(
            QFrame::StyledPanel
            );

        // Die Dashboard-Card hat dieselbe Höhe wie eine Exercise-Card.
        card->setFixedHeight(176);

        // Die Grid-Spalte bestimmt die Breite der Card.
        // Dadurch füllt die Card genau ihren vorgesehenen Drittel-Bereich aus.
        card->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Fixed
            );

        card->setCursor(
            Qt::PointingHandCursor
            );

        auto *cardLayout =
            new QVBoxLayout(card);

        cardLayout->setContentsMargins(
            12,
            12,
            12,
            12
            );

        cardLayout->setSpacing(4);

        // ─────────────────────────────────────────────────────────
        // Kopf: Name + Stern
        // ─────────────────────────────────────────────────────────

        auto *topRow =
            new QHBoxLayout();

        topRow->setContentsMargins(
            0,
            0,
            0,
            0
            );

        topRow->setSpacing(8);

        auto *nameLabel =
            new QLabel(
                routine.name,
                card
                );

        nameLabel->setObjectName(
            "routineCardNameLabel"
            );

        nameLabel->setWordWrap(true);

        // Der Name soll den Klick auf die Card nicht abfangen.
        nameLabel->setAttribute(
            Qt::WA_TransparentForMouseEvents,
            true
            );

        topRow->addWidget(
            nameLabel,
            1
            );

        // ── Ausgewählte Routine ───────────────────────────────────

        auto *favoriteButton =
            new QToolButton(card);

        favoriteButton->setObjectName(
            "routineFavoriteButton"
            );

        favoriteButton->setFixedSize(
            24,
            24
            );

        favoriteButton->setIconSize(
            QSize(16, 16)
            );

        favoriteButton->setAutoRaise(true);

        favoriteButton->setCursor(
            Qt::PointingHandCursor
            );

        favoriteButton->setIcon(
            QIcon(
                ":/icons/star-filled.svg"
                )
            );

        favoriteButton->setToolTip(
            "Ausgewählte Routine"
            );

        connect(
            favoriteButton,
            &QToolButton::clicked,
            this,
            [this, routineId = routine.id]() {

                if (!RoutineRepository::setCurrent(
                        routineId,
                        false
                        )) {
                    return;
                }

                // Beide Ansichten werden aktualisiert, damit der
                // neue Auswahlstatus sofort überall sichtbar ist.
                loadRoutineCards();
                loadDashboardRoutineCards();

                refreshHobbyDashboardOverview(
                    ui,
                    currentHobbyId
                    );
            }
            );

        topRow->addWidget(
            favoriteButton,
            0,
            Qt::AlignTop | Qt::AlignRight
            );

        cardLayout->addLayout(
            topRow
            );

        // ─────────────────────────────────────────────────────────
        // Übungen
        // ─────────────────────────────────────────────────────────



        const QList<RoutineStep> steps =
            RoutineRepository::getSteps(
                routine.id
                );

        const QList<Exercise> exercises =
            ExerciseRepository::getForHobby(
                currentHobbyId,
                true
                );

        QHash<int, QString> exerciseNames;

        for (const Exercise &exercise : exercises) {

            exerciseNames.insert(
                exercise.id,
                exercise.name
                );
        }


        constexpr int visibleExerciseCount = 4;

        int shownCount = 0;

        int totalDurationSeconds = 0;

        for (const RoutineStep &step : steps) {

            totalDurationSeconds +=
                step.durationSeconds;

            if (shownCount >= visibleExerciseCount)
                continue;

            auto *exerciseRow =
                new QHBoxLayout();

            exerciseRow->setSpacing(
                8
                );

            auto *exerciseLabel =
                new QLabel(
                    exerciseNames.value(
                        step.exerciseId,
                        "Unbekannte Übung"
                        ),
                    card
                    );

            exerciseLabel->setObjectName(
                "routineCardExerciseLabel"
                );

            exerciseRow->addWidget(
                exerciseLabel
                );

            exerciseRow->addStretch();

            const int minutes =
                step.durationSeconds / 60;

            auto *durationLabel =
                new QLabel(
                    QString("%1 min")
                        .arg(minutes),
                    card
                    );

            durationLabel->setObjectName(
                "routineCardDurationLabel"
                );

            durationLabel->setProperty(
                "role",
                "secondary"
                );

            exerciseRow->addWidget(
                durationLabel
                );

            cardLayout->addLayout(
                exerciseRow
                );

            ++shownCount;
        }

        if (steps.size() > visibleExerciseCount) {

            const int remaining =
                steps.size() -
                visibleExerciseCount;

            auto *moreLabel =
                new QLabel(
                    QString("+ %1 weitere")
                        .arg(remaining),
                    card
                    );

            moreLabel->setObjectName(
                "routineCardMoreLabel"
                );

            moreLabel->setProperty(
                "role",
                "secondary"
                );

            cardLayout->addWidget(
                moreLabel
                );
        }

        // ─────────────────────────────────────────────────────────
        // Gesamtdauer
        // ─────────────────────────────────────────────────────────

        const int totalMinutes =
            totalDurationSeconds / 60;

        auto *totalDurationRow =
            new QHBoxLayout();

        auto *totalDurationLabel =
            new QLabel(
                "Gesamtdauer",
                card
                );

        totalDurationLabel->setObjectName(
            "routineCardMetaHeaderLabel"
            );

        totalDurationLabel->setProperty(
            "role",
            "muted"
            );

        totalDurationRow->addWidget(
            totalDurationLabel
            );

        totalDurationRow->addStretch();

        auto *totalDurationValue =
            new QLabel(
                QString("%1 min")
                    .arg(totalMinutes),
                card
                );

        totalDurationValue->setObjectName(
            "routineCardDurationLabel"
            );

        totalDurationRow->addWidget(
            totalDurationValue
            );

        cardLayout->addLayout(
            totalDurationRow
            );

        return card;
    };

    // Alle aktuell markierten Routinen in die drei festen Grid-Spalten
    // einfügen. Dadurch bleiben die Cards unabhängig von ihrer Anzahl gleich breit.
    for (int i = 0; i < currentRoutines.size(); ++i) {

        QFrame *card =
            buildDashboardCard(
                currentRoutines.at(i)
                );

        ui->hobbyCurrentRoutinesLayout->addWidget(
            card,
            0,
            i
            );
    }

    // Solange weniger als drei Routinen markiert sind,
    // wird der freie Platz für den Hinzufügen-Button verwendet.
    if (currentRoutines.size() < 3) {

        auto *addButton =
            new QPushButton(
                "+ Routine hinzufügen"
                );

        addButton->setObjectName(
            "dashboardRoutineAddButton"
            );

        // Der Button hat exakt dieselbe Höhe wie eine Routine-Card.
        addButton->setFixedHeight(
            176
            );

        // Die Grid-Spalte bestimmt auch hier die Breite.
        addButton->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Fixed
            );

        // Der Button wird direkt in die nächste freie der drei
        // Card-Spalten gesetzt.
        ui->hobbyCurrentRoutinesLayout->addWidget(
            addButton,
            0,
            currentRoutines.size()
            );

        connect(
            addButton,
            &QPushButton::clicked,
            this,
            [this]() {

                // Der Button führt zur vollständigen Routinen-Ansicht.
                // Dort kann die Routine über den normalen "Hinzufügen"-Ablauf
                // erstellt werden.
                ui->routinesTab->setChecked(true);

                ui->hobbyPageStack->setCurrentWidget(
                    ui->routinesPage
                    );
            }
            );
    }
}


// ── Übungs-Ausführung: Übersicht ↔ Ausführung ───────────────────────────────
//
// Analog zur Routine-Ausführung: dieselbe Seite, nur ein anderer innerer
// Zustand. Kein Dialogfenster, kein Popup.

void MainWindow::showExerciseExecution(int exerciseId)
{
    if (exerciseId == 0)
        return;

    // Übungsdaten aus der Datenbank laden.
    Exercise exercise;

    if (!ExerciseRepository::getById(exerciseId, exercise)) {
        qDebug() << "Übung konnte nicht geladen werden.";
        return;
    }

    // Der aktuelle Wert entspricht dem letzten Log oder - wenn noch
    // keine Ausführung existiert - dem ursprünglichen Übungswert.
    ExerciseLog latestLog;

    const bool hasLatestLog =
        ExerciseLogRepository::getLatestForExercise(
            exerciseId,
            latestLog
            );

    exerciseExecutionData.exerciseId    = exercise.id;
    exerciseExecutionData.name          = exercise.name;
    exerciseExecutionData.description   = exercise.description;
    exerciseExecutionData.unit          = exercise.unit;
    exerciseExecutionData.currentValue  =
        hasLatestLog ? latestLog.value : exercise.value;
    exerciseExecutionData.goal          = exercise.goal;
    exerciseExecutionData.enteredText.clear();

    currentExecutionExerciseId = exerciseId;

    // Die Hobby-Seiten haben seitlich Rand. Für die Ausführung wird er
    // auf 0 gesetzt, damit der Inhalt die volle Breite nutzen kann.
    if (!exerciseExecutionMarginsOverridden) {
        exerciseExecutionSavedMargins =
            ui->hobbyPageStackLayout->contentsMargins();
        exerciseExecutionMarginsOverridden = true;
    }

    ui->hobbyPageStackLayout->setContentsMargins(
        0,
        exerciseExecutionSavedMargins.top(),
        0,
        exerciseExecutionSavedMargins.bottom()
        );

    // ── Anzeige befüllen ────────────────────────────────────────────────
    ui->exerciseExecutionExerciseNameLabel->setText(
        exerciseExecutionData.name
        );


    ui->exerciseExecutionExerciseDescriptionLabel->setText(
        exerciseExecutionData.description
        );

    ui->exerciseExecutionExerciseDescriptionLabel->setVisible(
        !exerciseExecutionData.description.trimmed().isEmpty()
        );

    ui->exerciseExecutionExerciseCurrentValueLabel->setText(
        formatValueWithUnit(
            exerciseExecutionData.currentValue,
            exerciseExecutionData.unit
            )
        );

    ui->exerciseExecutionExerciseTargetLabel->setText(
        formatValueWithUnit(
            exerciseExecutionData.goal,
            exerciseExecutionData.unit
            )
        );


    ui->exerciseExecutionExerciseValueEdit->clear();
    ui->exerciseExecutionExerciseValueEdit->setEnabled(true);

    // ── Timer zurücksetzen ──────────────────────────────────────────────
    //
    // Es gibt (noch) keine vorgegebene Dauer. Der Timer startet bei
    // 00:00 und die Start/Pause/Stopp-Buttons sind deaktiviert, bis
    // der Nutzer eine Zeit setzt (kommt später).
    exerciseExecutionTotalMs     = 0;
    exerciseExecutionRemainingMs = 0;
    exerciseExecutionElapsedMs   = 0;

    updateExerciseExecutionTimerDisplay();
    setExerciseExecutionRunning(false);

    // ── Auf die Seite wechseln ──────────────────────────────────────────
    ui->hobbyPageStack->setCurrentWidget(
        ui->exerciseExecutionPage
        );
}

void MainWindow::leaveExerciseExecution()
{
    setExerciseExecutionRunning(false);

    // Gemerkten Rückkehrpunkt zurücksetzen.
    exerciseExecutionData = ExerciseExecutionData();
    currentExecutionExerciseId = 0;

    if (progressReturnSource == ProgressReturnSource::ExerciseExecution)
        progressReturnSource = ProgressReturnSource::None;

    // Ursprüngliche Seitenränder wiederherstellen.
    if (exerciseExecutionMarginsOverridden) {
        ui->hobbyPageStackLayout->setContentsMargins(
            exerciseExecutionSavedMargins
            );
        exerciseExecutionMarginsOverridden = false;
    }

    // Zurück zur Übungsübersicht.
    ui->exercisesTab->setChecked(true);
    ui->hobbyPageStack->setCurrentWidget(ui->exercisesPage);
    showExerciseOverview();
}

void MainWindow::finishExerciseExecution()
{
    setExerciseExecutionRunning(false);

    const bool saved = saveExerciseExecutionResults();

    leaveExerciseExecution();

    // Nach dem Speichern alle Ansichten aktualisieren, die Logs anzeigen.
    if (saved) {
        loadExerciseCards();
        loadHistory();
        loadTimeline();
        loadRoutineCards();
        loadDashboardRoutineCards();
    }
}

bool MainWindow::saveExerciseExecutionResults()
{
    if (currentExecutionExerciseId == 0)
        return false;

    // Komma und Punkt werden beide akzeptiert.
    QString text = exerciseExecutionData.enteredText.trimmed();
    text.replace(',', '.');

    bool ok = false;
    const double newValue = text.toDouble(&ok);

    // Ohne gültigen Wert wird nichts gespeichert.
    if (!ok)
        return false;

    const int elapsedSeconds = exerciseExecutionElapsedMs / 1000;

    int logId = 0;

    if (!ExerciseLogRepository::add(
            currentExecutionExerciseId,
            newValue,
            exerciseExecutionData.unit,
            elapsedSeconds,
            logId)) {

        qDebug() << "Übungs-Log konnte nicht gespeichert werden.";
        return false;
    }

    return true;
}

void MainWindow::updateExerciseExecutionTimerDisplay()
{
    if (currentExecutionExerciseId == 0)
        return;

    // Aufrunden: "00:01" bleibt sichtbar, bis wirklich 0 erreicht ist.
    ui->exerciseExecutionTimerLabel->setText(
        formatCountdown((exerciseExecutionRemainingMs + 999) / 1000)
        );

    // 1000 = volle Zeit verbleibt, 0 = 00:00.
    const int value = exerciseExecutionTotalMs > 0
                          ? qRound(1000.0 * exerciseExecutionRemainingMs / exerciseExecutionTotalMs)
                          : 0;

    ui->exerciseExecutionProgressBar->setValue(value);

    // Buttons bleiben immer aktiv.
    // Ohne Zeit machen sie aber nichts (siehe Handler).
    ui->exerciseExecutionStartPauseButton->setEnabled(true);
    ui->exerciseExecutionStopButton->setEnabled(true);
}

void MainWindow::setExerciseExecutionRunning(bool running)
{
    if (running) {
        exerciseExecutionClock.start();
        exerciseExecutionTimer->start();
    } else {
        exerciseExecutionTimer->stop();
    }
    ui->exerciseExecutionStartPauseButton->setText(QString());

    ui->exerciseExecutionStartPauseButton->setIcon(
        QIcon(
            running
                ? QStringLiteral(":/icons/pause.svg")
                : QStringLiteral(":/icons/play.svg")
            )
        );

    updateExerciseExecutionTimerDisplay();
}

// ── Übungs-Detailansicht: Startwert inline bearbeiten ───────────────────────

void MainWindow::startExerciseDetailStartValueEdit()
{
    if (currentDetailExerciseId == 0)
        return;

    if (!exerciseDetailStartValueEdit)
        return;

    Exercise exercise;

    if (!ExerciseRepository::getById(
            currentDetailExerciseId,
            exercise)) {
        return;
    }

    exerciseDetailStartValueEditing = true;

    // Der Startwert wird als reine Zahl (ohne Einheit) im Feld angezeigt.
    // Nur Zahlen mit Punkt oder Komma.
    exerciseDetailStartValueEdit->setValidator(
        new QRegularExpressionValidator(
            QRegularExpression(QStringLiteral("-?[0-9]*[.,]?[0-9]*")),
            exerciseDetailStartValueEdit));

    exerciseDetailStartValueEdit->setText(
        QString::number(exercise.startValue, 'g', 15));

    // Größe und Position exakt an das Label angleichen.
    // Die Geometrie des Labels ist relativ zu seinem Parent -
    // deshalb mappen wir die Position in das Koordinatensystem
    // des Edit-Feld-Parents.
    const QPoint labelPos =
        ui->exerciseDetailStartValueLabel->mapTo(
            exerciseDetailStartValueEdit->parentWidget(),
            QPoint(0, 0));

    exerciseDetailStartValueEdit->setGeometry(
        labelPos.x(),
        labelPos.y(),
        ui->exerciseDetailStartValueLabel->width(),
        ui->exerciseDetailStartValueLabel->height());

    // Das Edit-Feld über das Label legen, aber das Label NICHT
    // ausblenden - es bleibt an seiner Position sichtbar.
    // (Sonst würde das Layout es "zusammenklappen".)
    exerciseDetailStartValueEdit->setVisible(true);
    exerciseDetailStartValueEdit->raise();
    exerciseDetailStartValueEdit->setFocus();

    // Cursor ans Ende setzen.
    exerciseDetailStartValueEdit->setCursorPosition(
        exerciseDetailStartValueEdit->text().length());
}

void MainWindow::commitExerciseDetailStartValueEdit()
{
    if (!exerciseDetailStartValueEditing)
        return;

    exerciseDetailStartValueEditing = false;

    if (!exerciseDetailStartValueEdit)
        return;

    QString text = exerciseDetailStartValueEdit->text().trimmed();
    text.replace(',', '.');

    bool ok = false;
    const double newValue = text.toDouble(&ok);

    // Ungültige Eingabe: still verwerfen (Variante 3a).
    // Keine Fehlermeldung, alter Wert bleibt.
    if (ok && newValue >= -999.0 && newValue <= 999.0) {

        ExerciseRepository::updateStartValue(
            currentDetailExerciseId,
            newValue);
    }

    exerciseDetailStartValueEdit->setVisible(false);

    // Detailansicht neu laden, damit das Label den neuen Wert zeigt.
    loadExerciseDetail(currentDetailExerciseId);
}

void MainWindow::cancelExerciseDetailStartValueEdit()
{
    if (!exerciseDetailStartValueEditing)
        return;

    exerciseDetailStartValueEditing = false;

    if (!exerciseDetailStartValueEdit)
        return;

    exerciseDetailStartValueEdit->setVisible(false);
}


// ── Timer inline bearbeiten: Hilfsfunktion ──────────────────────────────────

namespace {

// Parst eine Zeitangabe im flexiblen Format MM:SS oder MMSS oder M.
// Liefert die Sekunden zurück, oder -1 bei ungültiger Eingabe.
int parseTimerInput(const QString &raw)
{
    QString text = raw.trimmed();

    if (text.isEmpty())
        return -1;

    // Falls ein Doppelpunkt vorhanden ist:
    //   "MM:SS" oder "M:SS" oder "MM:S" usw.
    if (text.contains(':')) {

        const QStringList parts = text.split(':');

        if (parts.size() != 2)
            return -1;

        bool minOk = false;
        bool secOk = false;

        const int minutes = parts[0].toInt(&minOk);
        const int seconds = parts[1].toInt(&secOk);

        if (!minOk || !secOk)
            return -1;

        if (minutes < 0 || seconds < 0)
            return -1;

        if (seconds >= 60)
            return -1;

        return minutes * 60 + seconds;
    }

    // Ohne Doppelpunkt:
    //   5    -> 5 Sekunden? Nein, interpretieren wir als Minuten.
    //   530  -> 5 Minuten 30 Sekunden (MMSS).
    //   1230 -> 12 Minuten 30 Sekunden.
    //
    // Wir folgen dem alten Verhalten: von rechts auffüllen auf MMSS.
    QString digits = text;

    // Nur Ziffern behalten (Komma/Punkt ignorieren wir hier).
    digits.remove(QRegularExpression("[^0-9]"));

    if (digits.isEmpty())
        return -1;

    if (digits.length() > 4)
        digits = digits.right(4);

    digits = digits.rightJustified(4, '0');

    const int minutes = digits.left(2).toInt();
    const int seconds = digits.right(2).toInt();

    if (seconds >= 60)
        return -1;

    return minutes * 60 + seconds;
}

} // namespace

// ── Routine-Timer inline bearbeiten ─────────────────────────────────────────

void MainWindow::startRoutineExecutionTimerEdit()
{
    if (routineExecutionIndex < 0
        || routineExecutionIndex >= routineExecutionItems.size())
        return;

    if (!routineExecutionTimerEdit)
        return;

    // Timer pausieren, damit während der Eingabe nichts weiterläuft.
    setRoutineExecutionRunning(false);

    routineExecutionTimerEditing = true;

    const RoutineExecutionItem &item =
        routineExecutionItems.at(routineExecutionIndex);

    // Aktuelle Restzeit als MM:SS vorbefüllen.
    routineExecutionTimerEdit->setValidator(
        new QRegularExpressionValidator(
            QRegularExpression(QStringLiteral("[0-9:]*")),
            routineExecutionTimerEdit));

    const int remainingSeconds = (item.remainingMs + 999) / 1000;

    routineExecutionTimerEdit->setText(
        formatCountdown(remainingSeconds));

    // Geometrie an das Label angleichen.
    const QPoint labelPos =
        ui->routineExecutionTimerLabel->mapTo(
            routineExecutionTimerEdit->parentWidget(),
            QPoint(0, 0));

    routineExecutionTimerEdit->setGeometry(
        labelPos.x(),
        labelPos.y(),
        ui->routineExecutionTimerLabel->width(),
        ui->routineExecutionTimerLabel->height());

    routineExecutionTimerEdit->setVisible(true);
    routineExecutionTimerEdit->raise();
    routineExecutionTimerEdit->setFocus();
    routineExecutionTimerEdit->setCursorPosition(
        routineExecutionTimerEdit->text().length());
}

void MainWindow::commitRoutineExecutionTimerEdit()
{
    if (!routineExecutionTimerEditing)
        return;

    routineExecutionTimerEditing = false;

    if (!routineExecutionTimerEdit)
        return;

    if (routineExecutionIndex >= 0
        && routineExecutionIndex < routineExecutionItems.size()) {

        const int seconds =
            parseTimerInput(routineExecutionTimerEdit->text());

        if (seconds >= 0) {

            RoutineExecutionItem &item =
                routineExecutionItems[routineExecutionIndex];

            // Temporär für diese Sitzung ändern.
            // Nicht in der Datenbank speichern.
            item.durationSeconds = seconds;

            // Volle Zeit auf den neuen Wert setzen.
            item.remainingMs = seconds * 1000;
            item.elapsedMs   = 0;
        }
    }

    routineExecutionTimerEdit->setVisible(false);

    updateRoutineExecutionTimerDisplay();
}

void MainWindow::cancelRoutineExecutionTimerEdit()
{
    if (!routineExecutionTimerEditing)
        return;

    routineExecutionTimerEditing = false;

    if (!routineExecutionTimerEdit)
        return;

    routineExecutionTimerEdit->setVisible(false);

    updateRoutineExecutionTimerDisplay();
}

// ── Übungs-Timer inline bearbeiten ──────────────────────────────────────────

void MainWindow::startExerciseExecutionTimerEdit()
{
    if (currentExecutionExerciseId == 0)
        return;

    if (!exerciseExecutionTimerEdit)
        return;

    // Timer pausieren.
    setExerciseExecutionRunning(false);

    exerciseExecutionTimerEditing = true;

    exerciseExecutionTimerEdit->setValidator(
        new QRegularExpressionValidator(
            QRegularExpression(QStringLiteral("[0-9:]*")),
            exerciseExecutionTimerEdit));

    const int remainingSeconds =
        (exerciseExecutionRemainingMs + 999) / 1000;

    exerciseExecutionTimerEdit->setText(
        formatCountdown(remainingSeconds));

    const QPoint labelPos =
        ui->exerciseExecutionTimerLabel->mapTo(
            exerciseExecutionTimerEdit->parentWidget(),
            QPoint(0, 0));

    exerciseExecutionTimerEdit->setGeometry(
        labelPos.x(),
        labelPos.y(),
        ui->exerciseExecutionTimerLabel->width(),
        ui->exerciseExecutionTimerLabel->height());

    exerciseExecutionTimerEdit->setVisible(true);
    exerciseExecutionTimerEdit->raise();
    exerciseExecutionTimerEdit->setFocus();
    exerciseExecutionTimerEdit->setCursorPosition(
        exerciseExecutionTimerEdit->text().length());
}

void MainWindow::commitExerciseExecutionTimerEdit()
{
    if (!exerciseExecutionTimerEditing)
        return;

    exerciseExecutionTimerEditing = false;

    if (!exerciseExecutionTimerEdit)
        return;

    if (currentExecutionExerciseId != 0) {

        const int seconds =
            parseTimerInput(exerciseExecutionTimerEdit->text());

        if (seconds >= 0) {

            exerciseExecutionTotalMs     = seconds * 1000;
            exerciseExecutionRemainingMs = seconds * 1000;
            exerciseExecutionElapsedMs   = 0;
        }
    }

    exerciseExecutionTimerEdit->setVisible(false);

    updateExerciseExecutionTimerDisplay();
}

void MainWindow::cancelExerciseExecutionTimerEdit()
{
    if (!exerciseExecutionTimerEditing)
        return;

    exerciseExecutionTimerEditing = false;

    if (!exerciseExecutionTimerEdit)
        return;

    exerciseExecutionTimerEdit->setVisible(false);

    updateExerciseExecutionTimerDisplay();
}

// ── Roadmap: Übersicht ──────────────────────────────────────────────────────

void MainWindow::loadRoadmapCards()
{
    // Beide Cards-Container leeren.
    auto clearLayout = [](QLayout *layout) {

        while (layout->count() > 0) {

            QLayoutItem *item = layout->takeAt(0);

            if (item->widget())
                item->widget()->deleteLater();

            delete item;
        }
    };

    clearLayout(ui->roadmapActiveCardsLayout);
    clearLayout(ui->roadmapArchiveCardsLayout);

    // Spalten-Stretch: 3 gleiche Spalten (1,1,1).
    // Damit nimmt jede Karte auch bei nur 1-2 Karten genau 1/3 ein.
    for (int c = 0; c < 3; ++c) {
        ui->roadmapActiveCardsLayout->setColumnStretch(c, 1);
        ui->roadmapArchiveCardsLayout->setColumnStretch(c, 1);
    }

    // Row-Stretch zurücksetzen (wird weiter unten neu gesetzt).
    for (int r = 0; r < 100; ++r) {
        ui->roadmapActiveCardsLayout->setRowStretch(r, 0);
        ui->roadmapArchiveCardsLayout->setRowStretch(r, 0);
    }

    if (currentHobbyId == 0) {

        ui->roadmapActiveEmptyLabel->setVisible(true);
        ui->roadmapActiveScrollArea->setVisible(false);

        ui->roadmapArchiveEmptyLabel->setVisible(true);
        ui->roadmapArchiveScrollArea->setVisible(false);

        return;
    }

    // Alle Steps laden.
    const QList<RoadmapStep> allSteps =
        RoadmapRepository::getForHobby(currentHobbyId);

    // Root-Steps extrahieren (parentId == 0).
    QList<RoadmapStep> rootSteps;

    for (const RoadmapStep &step : allSteps) {

        if (step.parentId == 0)
            rootSteps.append(step);
    }

    // Suchtext filtern.
    const QString searchText =
        ui->roadmapSearchLineEdit->text().trimmed();

    // Aktiv / Archiv aufteilen:
    // Ein Root-Step ist "archiviert", wenn er und alle seine Nachkommen
    // erledigt sind. Sonst "aktiv".
    QList<RoadmapStep> activeRoots;
    QList<RoadmapStep> archivedRoots;

    for (const RoadmapStep &root : rootSteps) {

        // Suche anwenden.
        if (!searchText.isEmpty() &&
            !root.name.contains(searchText, Qt::CaseInsensitive)) {

            continue;
        }

        // Prüfen, ob der gesamte Unterbaum erledigt ist.
        bool allDone = true;

        for (const RoadmapStep &step : allSteps) {

            if (step.id == root.id || isDescendantOf(step, root.id, allSteps)) {

                if (!step.completed) {
                    allDone = false;
                    break;
                }
            }
        }

        if (allDone)
            archivedRoots.append(root);
        else
            activeRoots.append(root);
    }

    // Sortieren: Root-Step mit Stern zuerst.
    auto sortByCurrent = [](QList<RoadmapStep> &list) {

        std::stable_sort(
            list.begin(),
            list.end(),
            [](const RoadmapStep &a, const RoadmapStep &b) {
                return a.isCurrent && !b.isCurrent;
            }
            );
    };

    sortByCurrent(activeRoots);
    sortByCurrent(archivedRoots);

    // Cards bauen.
    constexpr int columnCount = 3;

    auto buildCard = [this, allSteps](const RoadmapStep &root, bool archived) -> QFrame * {

        auto *card = new QFrame();
        card->setObjectName("roadmapCard");
        card->setFrameShape(QFrame::StyledPanel);
        card->setFixedHeight(364);
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        card->setCursor(Qt::PointingHandCursor);

        // Step-ID und Status als Properties speichern — der eventFilter()
        // braucht sie für Hover und Klick.
        card->setProperty("roadmapRootId", root.id);
        card->setProperty("roadmapIsCurrent", root.isCurrent);
        card->setProperty("roadmapIsArchived", archived);

        // Hover-Events aktivieren (für den Stern und den Hover-Effekt).
        card->setAttribute(Qt::WA_Hover, true);

        // Klick → Detail-Ansicht.
        card->installEventFilter(this);

        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(12, 12, 12, 12);
        cardLayout->setSpacing(4);

        // ── Kopf: Name + Stern ────────────────────────────────────────────
        auto *topRow = new QHBoxLayout();
        topRow->setContentsMargins(0, 0, 0, 0);
        topRow->setSpacing(8);

        auto *nameLabel = new QLabel(root.name, card);
        nameLabel->setObjectName("roadmapCardNameLabel");
        nameLabel->setWordWrap(true);
        nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        topRow->addWidget(nameLabel, 1);

        // Stern: nur bei aktiven Roadmaps.
        if (!archived) {

            auto *starButton = new QToolButton(card);
            starButton->setObjectName("roadmapCardStarButton");
            starButton->setFixedSize(24, 24);
            starButton->setIconSize(QSize(16, 16));
            starButton->setAutoRaise(true);
            starButton->setCursor(Qt::PointingHandCursor);

            starButton->setIcon(
                QIcon(
                    root.isCurrent
                        ? ":/icons/star-filled.svg"
                        : ":/icons/star-outline.svg"
                    )
                );

            starButton->setToolTip(
                root.isCurrent
                    ? "Aktuelle Roadmap"
                    : "Als aktuelle Roadmap festlegen"
                );

            // Der Stern ist nur dann dauerhaft sichtbar, wenn die
            // Roadmap als aktuelle markiert ist. Sonst erscheint er
            // nur beim Hover über die Card.
            if (!root.isCurrent)
                starButton->setVisible(false);

            connect(
                starButton,
                &QToolButton::clicked,
                this,
                [this, rootId = root.id]() {
                    onRoadmapToggleStar(rootId);
                }
                );

            topRow->addWidget(starButton, 0, Qt::AlignTop | Qt::AlignRight);
        }

        cardLayout->addLayout(topRow);

        // ── Hierarchie (Mini-Baum) ────────────────────────────────────────
        auto *hierarchyHeader = new QLabel("Hierarchie", card);
        hierarchyHeader->setObjectName("roadmapCardMetaHeaderLabel");
        cardLayout->addWidget(hierarchyHeader);

        // Custom-Widget mit echten Linien und Preorder-Traversal.
        // Die Card ist 364 px hoch. Nach Abzug von Header ("Hierarchie"),
        // Statistik-Zeile und Paddings bleiben ca. 280 px für den
        // Mini-Baum. Bei ca. 20 px Zeilenhöhe passen ca. 14 Zeilen rein.
        auto *miniTree = new RoadmapMiniTreeWidget(card);
        miniTree->setMaxVisibleNodes(14);
        miniTree->setData(root, allSteps);
        cardLayout->addWidget(miniTree);

        // Zwischen Mini-Baum und Statistik-Zeile setzen wir einen
        // Stretch. Dadurch wird die Statistik-Zeile immer an den
        // unteren Rand der Card gedrückt — unabhängig davon, wie
        // viele Steps der Mini-Baum enthält.
        cardLayout->addStretch();


        // ── Statistik ─────────────────────────────────────────────────────
        int totalSteps = 0;
        int doneSteps = 0;

        for (const RoadmapStep &step : allSteps) {

            if (step.id == root.id || isDescendantOf(step, root.id, allSteps)) {
                ++totalSteps;
                if (step.completed)
                    ++doneSteps;
            }
        }

        auto *statsLabel = new QLabel(
            QString("%1 Schritte · %2 erledigt")
                .arg(totalSteps)
                .arg(doneSteps),
            card
            );

        statsLabel->setObjectName("roadmapCardStatLabel");
        cardLayout->addWidget(statsLabel);

        return card;
    };

    // ── Aktive Roadmaps ───────────────────────────────────────────────────
    for (int i = 0; i < activeRoots.size(); ++i) {

        QFrame *card = buildCard(activeRoots[i], false);

        ui->roadmapActiveCardsLayout->addWidget(
            card,
            i / columnCount,
            i % columnCount
            );
    }

    // Empty-State / Sichtbarkeit.
    //
    // Wenn ein Suchtext aktiv ist und keine Treffer liefert, zeigen wir
    // einen anderen Text als bei komplett leeren Roadmaps.
    const bool isSearching =
        !ui->roadmapSearchLineEdit->text().trimmed().isEmpty();

    const bool showEmptyState = activeRoots.isEmpty();

    if (showEmptyState && isSearching) {
        ui->roadmapActiveEmptyLabel->setText(
            "Keine Roadmaps gefunden."
            );
    } else {
        ui->roadmapActiveEmptyLabel->setText(
            "Noch keine aktiven Roadmaps\n\n"
            "Leg mit \"+ Roadmap\" deine erste Roadmap an."
            );
    }

    ui->roadmapActiveEmptyLabel->setVisible(showEmptyState);
    ui->roadmapActiveScrollArea->setVisible(!showEmptyState);

    // Row-Stretch: Der restliche vertikale Platz wandert in eine
    // leere Zeile unter den Cards. Dadurch bleiben die Cards oben.
    const int activeRowCount =
        (activeRoots.size() + columnCount - 1) / columnCount;

    ui->roadmapActiveCardsLayout->setRowStretch(activeRowCount, 1);
    // ── Archivierte Roadmaps ──────────────────────────────────────────────
    for (int i = 0; i < archivedRoots.size(); ++i) {

        QFrame *card = buildCard(archivedRoots[i], true);

        ui->roadmapArchiveCardsLayout->addWidget(
            card,
            i / columnCount,
            i % columnCount
            );
    }

    ui->roadmapArchiveEmptyLabel->setVisible(archivedRoots.isEmpty());
    ui->roadmapArchiveScrollArea->setVisible(!archivedRoots.isEmpty());

    // Row-Stretch für Archiv.
    const int archiveRowCount =
        (archivedRoots.size() + columnCount - 1) / columnCount;

    ui->roadmapArchiveCardsLayout->setRowStretch(archiveRowCount, 1);
}

// Hilfsfunktion: Prüft, ob `step` (rekursiv) ein Nachkomme von `rootId` ist.
bool MainWindow::isDescendantOf(
    const RoadmapStep &step,
    int rootId,
    const QList<RoadmapStep> &allSteps
    )
{
    int currentId = step.parentId;

    while (currentId > 0) {

        if (currentId == rootId)
            return true;

        // Parent finden.
        bool found = false;

        for (const RoadmapStep &s : allSteps) {

            if (s.id == currentId) {
                currentId = s.parentId;
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    }

    return false;
}

// ── Roadmap: Detail-Ansicht ─────────────────────────────────────────────────

void MainWindow::loadRoadmapDetail(int rootId)
{
    ui->roadmapTreeWidget->clear();

    if (currentHobbyId == 0 || rootId <= 0)
        return;

    // Alle Steps laden.
    const QList<RoadmapStep> allSteps =
        RoadmapRepository::getForHobby(currentHobbyId);

    // Root-Step finden.
    RoadmapStep root;
    bool found = false;

    for (const RoadmapStep &step : allSteps) {

        if (step.id == rootId) {
            root = step;
            found = true;
            break;
        }
    }

    if (!found)
        return;

    // Titel anzeigen (Root-Name als Überschrift).
    ui->roadmapDetailTitleLabel->setText(root.name);

    // Root-Step NICHT als Top-Level-Item.
    // Stattdessen direkt die Kinder einfügen.
    buildRoadmapTreeItem(nullptr, root.id, allSteps);

    ui->roadmapTreeWidget->expandAll();

    // ── Filter anwenden ──────────────────────────────────────────────────
    //
    // Vorgehen:
    //   1. Zuerst alle Items auf "sichtbar" setzen (clear).
    //   2. Dann rekursiv durchgehen und prüfen:
    //      - Passt der Text zum Filter? → Item sichtbar lassen.
    //      - Hat ein Kind den Filter erfüllt? → Item auch sichtbar lassen.
    //      - Sonst → ausblenden.
    //
    // Die Funktion gibt zurück, ob das Item oder eines seiner Kinder
    // sichtbar bleiben soll. Damit können wir bequem rekursiv arbeiten.

    if (!roadmapDetailFilterText.isEmpty()) {

        std::function<bool(QTreeWidgetItem *)> applyFilter =
            [&](QTreeWidgetItem *item) -> bool {

            // Prüfen, ob der Text des Items passt.
            const bool matches =
                item->text(0).contains(
                    roadmapDetailFilterText,
                    Qt::CaseInsensitive
                    );

            // Kinder rekursiv prüfen.
            bool hasVisibleChild = false;

            for (int i = 0; i < item->childCount(); ++i) {

                if (applyFilter(item->child(i)))
                    hasVisibleChild = true;
            }

            // Das Item bleibt sichtbar, wenn es selbst passt
            // ODER mindestens ein Kind sichtbar ist.
            const bool visible = matches || hasVisibleChild;

            item->setHidden(!visible);

            // Wenn ein Kind sichtbar ist, expandieren wir das Item,
            // damit der Treffer im aufgeklappten Baum sichtbar ist.
            if (hasVisibleChild)
                item->setExpanded(true);

            return visible;
        };

        for (int i = 0; i < ui->roadmapTreeWidget->topLevelItemCount(); ++i)
            applyFilter(ui->roadmapTreeWidget->topLevelItem(i));
    }

    updateRoadmapRowButtonsState();
    updateRoadmapRowButtons();
    updateRoadmapDetailEmptyState();
}
void MainWindow::updateRoadmapDetailEmptyState()
{
    // Wenn ein Filter aktiv ist, prüfen wir, ob überhaupt ein Item
    // sichtbar ist. Wenn nicht, zeigen wir das Empty-State-Label
    // "Keine Steps gefunden." an und blenden den Tree aus.
    if (roadmapDetailFilterText.isEmpty()) {
        ui->roadmapDetailEmptyLabel->setVisible(false);
        ui->roadmapTreeWidget->setVisible(true);
        return;
    }

    bool anyVisible = false;

    std::function<void(QTreeWidgetItem *)> checkVisible =
        [&](QTreeWidgetItem *item) {

            if (!item->isHidden())
                anyVisible = true;

            for (int i = 0; i < item->childCount(); ++i)
                checkVisible(item->child(i));
        };

    for (int i = 0; i < ui->roadmapTreeWidget->topLevelItemCount(); ++i)
        checkVisible(ui->roadmapTreeWidget->topLevelItem(i));

    ui->roadmapDetailEmptyLabel->setVisible(!anyVisible);
    ui->roadmapTreeWidget->setVisible(anyVisible);
}
void MainWindow::buildRoadmapTreeItem(
    QTreeWidgetItem *parentItem,
    int parentId,
    const QList<RoadmapStep> &steps
    )
{
    // Alle direkten Kinder sammeln.
    QList<RoadmapStep> children;

    for (const RoadmapStep &step : steps) {
        if (step.parentId == parentId)
            children.append(step);
    }

    // Sortieren: offene zuerst.
    std::stable_sort(
        children.begin(),
        children.end(),
        [](const RoadmapStep &a, const RoadmapStep &b) {
            return !a.completed && b.completed;
        }
        );

    for (const RoadmapStep &step : children) {

        QTreeWidgetItem *item = nullptr;

        if (parentItem)
            item = new QTreeWidgetItem(parentItem);
        else
            item = new QTreeWidgetItem(ui->roadmapTreeWidget);

        item->setText(0, step.name);
        item->setData(0, Qt::UserRole, step.id);

        // Status als Item-Property speichern, damit der Delegate
        // ihn lesen kann.
        item->setData(0, Qt::UserRole + 1, step.completed);

        // Farbe wird vom Delegate gesetzt — nicht hier.

        // Rekursiv.
        buildRoadmapTreeItem(item, step.id, steps);
    }
}

void MainWindow::showRoadmapOverview()
{
    currentRoadmapDetailRootId = 0;
    ui->roadmapViewStack->setCurrentWidget(ui->roadmapOverviewPage);

    // Karten neu laden, damit Änderungen aus der Detail-Ansicht
    // (z. B. Root umbenannt, Step erledigt) sofort sichtbar sind.
    loadRoadmapCards();
}

void MainWindow::showRoadmapDetail(int rootId)
{
    currentRoadmapDetailRootId = rootId;
    loadRoadmapDetail(rootId);
    ui->roadmapViewStack->setCurrentWidget(ui->roadmapDetailPage);
}
// ── Roadmap: Overlay-Buttons ────────────────────────────────────────────────

void MainWindow::updateRoadmapRowButtons()
{
    // Wenn die Buttons nicht existieren, gibt's nichts zu tun.
    if (!roadmapRowAddChildButton)
        return;

    // Ausgewähltes Item ermitteln.
    QTreeWidgetItem *item = ui->roadmapTreeWidget->currentItem();

    if (!item) {
        for (QToolButton *b : {
                 roadmapRowAddChildButton,
                 roadmapRowRenameButton,
                 roadmapRowToggleDoneButton,
                 roadmapRowDeleteButton
             }) {
            if (b)
                b->setVisible(false);
        }
        return;
    }

    // Rechteck der Zeile in Koordinaten des Tree-Widgets.
    const QRect rect = ui->roadmapTreeWidget->visualItemRect(item);

    // Wenn die Zeile außerhalb des sichtbaren Bereichs ist, ausblenden.
    if (rect.bottom() < 0
        || rect.top() > ui->roadmapTreeWidget->viewport()->height()) {

        for (QToolButton *b : {
                 roadmapRowAddChildButton,
                 roadmapRowRenameButton,
                 roadmapRowToggleDoneButton,
                 roadmapRowDeleteButton
             }) {
            if (b)
                b->setVisible(false);
        }
        return;
    }

    // Position der Buttons:
    // Rechtsbündig in der Zeile, aber mit 8 px Abstand zum rechten Rand.
    const int buttonSize = 24;
    const int gap = 2;
    const int rightPadding = 8;

    int rightEdge =
        ui->roadmapTreeWidget->viewport()->width() - rightPadding;

    // Die Y-Position im Koordinatensystem des Tree-Widgets.
    // Der Viewport beginnt normalerweise bei (0, 0) — der
    // Header ist ausgeblendet.
    const int buttonY =
        ui->roadmapTreeWidget->viewport()->y()
        + rect.top()
        + (rect.height() - buttonSize) / 2;

    // Von rechts nach links positionieren.
    auto placeButton = [&](QToolButton *b) {

        if (!b)
            return;

        const int x = rightEdge - buttonSize;
        b->move(x, buttonY);
        rightEdge -= buttonSize + gap;
        b->raise();
        b->show();
    };

    placeButton(roadmapRowDeleteButton);
    placeButton(roadmapRowToggleDoneButton);
    placeButton(roadmapRowRenameButton);
    placeButton(roadmapRowAddChildButton);
}

void MainWindow::updateRoadmapRowButtonsState()
{
    QTreeWidgetItem *item = ui->roadmapTreeWidget->currentItem();

    const bool hasSelection = (item != nullptr);

    if (roadmapRowAddChildButton)
        roadmapRowAddChildButton->setEnabled(hasSelection);

    if (roadmapRowRenameButton)
        roadmapRowRenameButton->setEnabled(hasSelection);

    if (roadmapRowToggleDoneButton)
        roadmapRowToggleDoneButton->setEnabled(hasSelection);

    if (roadmapRowDeleteButton)
        roadmapRowDeleteButton->setEnabled(hasSelection);

    if (hasSelection && roadmapRowToggleDoneButton) {

        const int stepId = item->data(0, Qt::UserRole).toInt();

        RoadmapStep step;

        if (RoadmapRepository::getById(stepId, step)) {

            roadmapRowToggleDoneButton->setToolTip(
                step.completed
                    ? "Als unerledigt markieren"
                    : "Als erledigt markieren"
                );
        }
    }
}

// ── Roadmap: Stern-Handling ─────────────────────────────────────────────────

void MainWindow::onRoadmapToggleStar(int rootStepId)
{


    if (currentHobbyId == 0 || rootStepId <= 0)
        return;

    RoadmapStep step;

    if (!RoadmapRepository::getById(rootStepId, step))
        return;

    const bool newState = !step.isCurrent;

    if (!RoadmapRepository::setCurrent(rootStepId, newState))
        return;

    loadRoadmapCards();
}

// ── Roadmap: Aktionen in der Detail-Ansicht ─────────────────────────────────

void MainWindow::updateRoadmapActionButtons()
{
    // Wird von updateRoadmapRowButtonsState() abgelöst.
    // Nur noch delegieren, um Kompatibilität zu wahren.

    updateRoadmapRowButtonsState();
}

void MainWindow::onRoadmapAddRootStep()
{
    if (currentHobbyId == 0)
        return;

    bool ok = false;

    const QString name = QInputDialog::getText(
                             this,
                             "Neue Roadmap",
                             "Name:",
                             QLineEdit::Normal,
                             "",
                             &ok
                             ).trimmed();

    if (!ok || name.isEmpty())
        return;

    int newId = 0;

    if (!RoadmapRepository::add(currentHobbyId, 0, name, newId)) {

        QMessageBox::warning(
            this,
            "Fehler",
            "Der Root-Step konnte nicht erstellt werden."
            );

        return;
    }

    loadRoadmapCards();
}

void MainWindow::onRoadmapAddChildOfRootStep()
{
    if (currentHobbyId == 0 || currentRoadmapDetailRootId <= 0)
        return;

    bool ok = false;

    const QString name = QInputDialog::getText(
                             this,
                             "Neuer Step",
                             "Name:",
                             QLineEdit::Normal,
                             "",
                             &ok
                             ).trimmed();

    if (!ok || name.isEmpty())
        return;

    int newId = 0;

    // parentId = currentRoadmapDetailRootId → direkt unter dem Root.
    if (!RoadmapRepository::add(
            currentHobbyId,
            currentRoadmapDetailRootId,
            name,
            newId)) {

        QMessageBox::warning(
            this,
            "Fehler",
            "Der Step konnte nicht erstellt werden."
            );

        return;
    }

    loadRoadmapDetail(currentRoadmapDetailRootId);
}

void MainWindow::onRoadmapAddChildStep()
{
    QTreeWidgetItem *current = ui->roadmapTreeWidget->currentItem();

    if (!current)
        return;

    const int parentId = current->data(0, Qt::UserRole).toInt();

    if (parentId <= 0)
        return;

    bool ok = false;

    const QString name = QInputDialog::getText(
                             this,
                             "Neuer Unter-Step",
                             "Name:",
                             QLineEdit::Normal,
                             "",
                             &ok
                             ).trimmed();

    if (!ok || name.isEmpty())
        return;

    int newId = 0;

    if (!RoadmapRepository::add(currentHobbyId, parentId, name, newId)) {

        QMessageBox::warning(
            this,
            "Fehler",
            "Der Unter-Step konnte nicht erstellt werden."
            );

        return;
    }

    loadRoadmapDetail(currentRoadmapDetailRootId);
}

void MainWindow::onRoadmapRenameSelectedStep()
{
    QTreeWidgetItem *current = ui->roadmapTreeWidget->currentItem();

    if (!current)
        return;

    const int stepId = current->data(0, Qt::UserRole).toInt();

    if (stepId <= 0)
        return;

    bool ok = false;

    const QString name = QInputDialog::getText(
                             this,
                             "Step umbenennen",
                             "Neuer Name:",
                             QLineEdit::Normal,
                             current->text(0),
                             &ok
                             ).trimmed();

    if (!ok || name.isEmpty())
        return;

    if (!RoadmapRepository::rename(stepId, name)) {

        QMessageBox::warning(
            this,
            "Fehler",
            "Der Step konnte nicht umbenannt werden."
            );

        return;
    }

    // Wenn der Root-Step umbenannt wurde, muss auch der Titel aktualisiert werden.
    if (stepId == currentRoadmapDetailRootId) {
        ui->roadmapDetailTitleLabel->setText(name);
    }

    loadRoadmapDetail(currentRoadmapDetailRootId);
}

void MainWindow::onRoadmapToggleSelectedStepDone()
{
    QTreeWidgetItem *current = ui->roadmapTreeWidget->currentItem();

    if (!current)
        return;

    const int stepId = current->data(0, Qt::UserRole).toInt();

    if (stepId <= 0)
        return;

    RoadmapStep step;

    if (!RoadmapRepository::getById(stepId, step))
        return;

    if (!RoadmapRepository::setCompletedRecursive(
            stepId,
            !step.completed)) {

        QMessageBox::warning(
            this,
            "Fehler",
            "Der Status konnte nicht geändert werden."
            );

        return;
    }

    loadRoadmapDetail(currentRoadmapDetailRootId);
}

void MainWindow::onRoadmapDeleteSelectedStep()
{
    QTreeWidgetItem *current = ui->roadmapTreeWidget->currentItem();

    if (!current)
        return;

    const int stepId = current->data(0, Qt::UserRole).toInt();

    if (stepId <= 0)
        return;

    // Sonderfall: Root-Step löschen → zurück zur Übersicht.
    const bool isRoot = (stepId == currentRoadmapDetailRootId);

    const QString message = isRoot
                                ? QString("Möchtest du die Roadmap „%1“ und alle Unter-Steps wirklich löschen?")
                                      .arg(current->text(0))
                                : QString("Möchtest du den Step „%1“ und alle Unter-Steps wirklich löschen?")
                                      .arg(current->text(0));

    const auto answer = QMessageBox::question(
        this,
        "Löschen",
        message,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );

    if (answer != QMessageBox::Yes)
        return;

    if (!RoadmapRepository::removeRecursive(stepId)) {

        QMessageBox::warning(
            this,
            "Fehler",
            "Der Step konnte nicht gelöscht werden."
            );

        return;
    }

    if (isRoot) {
        showRoadmapOverview();
        loadRoadmapCards();
    } else {
        loadRoadmapDetail(currentRoadmapDetailRootId);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
};
