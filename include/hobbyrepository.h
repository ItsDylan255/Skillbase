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

    // Ändert nur den Namen des Hobbys.
    static bool rename(int hobbyId, const QString &newName);

    // Löscht das Hobby und ALLES, was daran hängt:
    // Übungen (inkl. Logs), Ziele, Routinen (inkl. Steps + Logs),
    // Roadmap-Steps, Timeline-Phasen, Notizen.
    // Liefert true, wenn alles erfolgreich war.
    static bool removeRecursive(int hobbyId);
};

#endif
