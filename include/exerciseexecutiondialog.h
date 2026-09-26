#ifndef EXERCISEEXECUTIONDIALOG_H
#define EXERCISEEXECUTIONDIALOG_H

#include <QDialog>

#include "exercise.h"

namespace Ui {
class ExerciseExecutionDialog;
}

class ExerciseExecutionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExerciseExecutionDialog(
        const Exercise &exercise,
        QWidget *parent = nullptr
        );

    ~ExerciseExecutionDialog() override;

private:
    Ui::ExerciseExecutionDialog *ui;
};

#endif