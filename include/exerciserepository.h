#ifndef EXERCISEREPOSITORY_H
#define EXERCISEREPOSITORY_H

#include <QList>
#include "exercise.h"

// Owns all SQL for the exercises table.
class ExerciseRepository
{
public:
    static bool add(
        int hobbyId,
        const QString &name,
        const QString &description,
        int categoryId,
        double value,
        const QString &unit,
        const QString &goal,
        int &id
        );

    // includeArchived = false (default) returns only active exercises.
    static QList<Exercise> getForHobby(int hobbyId, bool includeArchived = false);

    static bool setArchived(int exerciseId, bool archived);
    static bool update(
        int exerciseId,
        const QString &name,
        const QString &description,
        int categoryId,
        double value,
        const QString &unit,
        const QString &goal
        );

    static bool updateStartValue(
        int exerciseId,
        double startValue
        );

    static bool getById(int exerciseId, Exercise &exercise);

    static bool remove(int exerciseId);

};

#endif