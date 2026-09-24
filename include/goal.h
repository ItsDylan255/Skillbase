#ifndef GOAL_H
#define GOAL_H

#include <QString>

struct Goal
{
    int id = 0;
    int hobbyId = 0;
    QString title;
    QString deadline;        // ISO date string, e.g. "2026-12-15"; empty if none set
    int progressPercent = 0;
    QString status = "in_progress";
};

#endif