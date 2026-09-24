#ifndef EXERCISELOG_H
#define EXERCISELOG_H

#include <QString>

struct ExerciseLog
{
    int id = 0;
    int exerciseId = 0;
    QString performedAt;
    double value = 0.0;
    QString unit;
    int durationSeconds = 0;
};

#endif