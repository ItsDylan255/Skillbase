#ifndef GOALREPOSITORY_H
#define GOALREPOSITORY_H

#include <QList>
#include "goal.h"

// Owns all SQL for the goals table.
class GoalRepository
{
public:
    static bool add(int hobbyId, const QString &title, const QString &deadline, int &id);
    static QList<Goal> getForHobby(int hobbyId);
    static bool updateProgress(int goalId, int progressPercent, const QString &status);
};

#endif