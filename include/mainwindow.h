#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPoint>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void loadExerciseCards();
    void editExercise(int exerciseId);

    // Übungen: Wechsel zwischen Kartenübersicht und Detail-/
    // Fortschrittsansicht innerhalb derselben Seite (kein neues Fenster).
    void showExerciseOverview();
    void showExerciseDetail(int exerciseId);
    void loadExerciseDetail(int exerciseId);

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

    void loadHistory();
    void loadTimeline();

    // ── Ziele ────────────────────────────────────────────────────────────
    void loadGoalCards();

    // goalId == 0 öffnet den Dialog im Erstellen-Modus (kein Status-/
    // Löschen-Button); ein bestehendes Ziel öffnet ihn im Bearbeiten-Modus.
    void openGoalDialog(int goalId);

    // Zustand für das Ziehen einer Ziel-Card per Drag & Drop (nur offene
    // Ziele lassen sich neu sortieren). 0, solange nichts gezogen wird.
    int goalDragCandidateId = 0;
    QPoint goalDragStartPos;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

};
#endif // MAINWINDOW_H
