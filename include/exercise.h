#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>

struct Exercise
{
    int id = 0;
    int hobbyId = 0;
    QString name;
    QString description;
    int categoryId = 0;
    double value = 0.0;
    QString unit;
    QString goal;
    bool archived = false;
};

#endif