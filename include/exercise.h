#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>

struct Exercise
{
    int id = 0;
    int hobbyId = 0;
    QString name;
    QString description;
    QString category;
    QString goal;
    bool archived = false;
};

#endif