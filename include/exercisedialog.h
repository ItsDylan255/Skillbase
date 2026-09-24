#pragma once
#include <QDialog>
#include "ui_exercise_dialog.h"

class ExerciseDialog : public QDialog {
    Q_OBJECT
public:
    explicit ExerciseDialog(QWidget *parent = nullptr);

    QString name() const;
    QString description() const;
    QString unit() const;
    double  value() const;
    int     categoryId() const;

    void setCategories(const QList<QPair<int, QString>> &categories);

private:
    Ui::ExerciseDialog ui;
};