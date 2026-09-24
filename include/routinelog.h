#ifndef ROUTINELOG_H
#define ROUTINELOG_H

#include <QString>

struct RoutineLog
{
    int id = 0;
    int routineId = 0;
    QString performedAt;
    int durationSeconds = 0;
};

#endif