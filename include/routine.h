#ifndef ROUTINE_H
#define ROUTINE_H

#include <QString>

struct Routine
{
    int id = 0;
    int hobbyId = 0;
    QString name;
    QString description;

    // Gibt an, ob die Routine archiviert wurde.
    // false = aktiv, true = archiviert.
    bool archived = false;

    // Gibt an, ob die Routine als eine der maximal drei
    // ausgewählten Routinen des Hobbys markiert ist.
    bool isCurrent = false;
};

#endif