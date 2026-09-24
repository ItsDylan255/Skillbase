#ifndef EXERCISEREPOSITORY_H
#define EXERCISEREPOSITORY_H

#include <QList>
#include "exercise.h"

// Owns all SQL for the exercises table.
class ExerciseRepository
{
public:
    static bool add(int hobbyId, const QString &name, const QString &description, int &id);
    static QList<Exercise> getForHobby(int hobbyId);
};

#endif
