#ifndef CATEGORYREPOSITORY_H
#define CATEGORYREPOSITORY_H

#include <QList>
#include <QString>

#include "category.h"

class CategoryRepository
{
public:
    // Creates a category for one specific hobby.
    static bool add(int hobbyId, const QString &name, int &id);

    // Returns only the categories belonging to the selected hobby.
    static QList<Category> getForHobby(int hobbyId);

    static bool rename(int categoryId, const QString &name);
    static bool remove(int categoryId);
};

#endif
