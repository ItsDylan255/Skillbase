#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

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

    Ui::MainWindow *ui;
    QString currentHobby;
    int currentHobbyId = 0;

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

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};
#endif // MAINWINDOW_H
