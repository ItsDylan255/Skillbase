#ifndef HOBBYREPOSITORY_H
#define HOBBYREPOSITORY_H

#include <QList>
#include "hobby.h"

// Owns all SQL for the hobbies table. Nothing outside this class
// should build a query against "hobbies" directly.
class HobbyRepository
{
public:
    static bool add(const QString &name, const QString &color, int &id);
    static QList<Hobby> getAll();
};

#endif
