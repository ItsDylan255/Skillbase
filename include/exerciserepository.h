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
        const QString &category,
        const QString &goal,
        int &id
        );

    // includeArchived = false (default) returns only active exercises.
    static QList<Exercise> getForHobby(int hobbyId, bool includeArchived = false);

    static bool setArchived(int exerciseId, bool archived);
};

#endif