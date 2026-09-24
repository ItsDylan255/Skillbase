#ifndef EXERCISELOGREPOSITORY_H
#define EXERCISELOGREPOSITORY_H

#include <QList>
#include "exerciselog.h"

// Owns all SQL for the exercise_logs table - one entry per completed
// run of an exercise (e.g. "110 BPM on 20.09.").
class ExerciseLogRepository
{
public:
    static bool add(int exerciseId, double value, const QString &unit, int durationSeconds, int &id);
    static QList<ExerciseLog> getForExercise(int exerciseId);

    // Convenience for progress tracking: the most recent log for an
    // exercise, used to pre-fill the value field the next time it's run.
    static bool getLatestForExercise(int exerciseId, ExerciseLog &out);
};

#endif