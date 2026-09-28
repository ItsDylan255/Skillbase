#ifndef GOALREPOSITORY_H
#define GOALREPOSITORY_H

#include <QList>
#include <QString>
#include "goal.h"

// Owns all SQL for the goals table.
//
// Bewusst simpel gehalten (siehe Feature-Konzept "Ziele"): keine
// Fortschrittsprozente, keine Messwerte - nur Titel, Beschreibung,
// optionale Deadline, ein einfacher offen/geschafft-Status und eine vom
// Benutzer per Drag & Drop bestimmbare Reihenfolge der offenen Ziele.
class GoalRepository
{
public:
    static bool add(
        int hobbyId,
        const QString &title,
        const QString &description,
        const QString &deadline,
        int &id
        );

    // Alle Ziele eines Hobbys - offen und geschafft gemeinsam.
    // Offene Ziele sind nach der vom Benutzer festgelegten Reihenfolge
    // sortiert, geschaffte Ziele danach (neueste zuerst).
    static QList<Goal> getForHobby(int hobbyId);

    static bool getById(int goalId, Goal &goal);

    // Aktualisiert Titel, Beschreibung und Deadline - nicht den Status.
    static bool update(
        int goalId,
        const QString &title,
        const QString &description,
        const QString &deadline
        );

    // Setzt den Status auf "open" oder "done". Wird ein Ziel wieder
    // geöffnet, landet es automatisch ans Ende der offenen Liste.
    static bool setStatus(int goalId, const QString &status);

    // Persistiert eine neue Drag & Drop-Reihenfolge der offenen Ziele.
    static bool reorder(const QList<int> &orderedGoalIds);

    static bool remove(int goalId);

    // Setzt ein Ziel als aktuelles Hauptziel bzw. entfernt diesen Status.
    // Pro Hobby kann immer nur ein offenes Ziel aktuell sein.
    static bool setCurrent(int goalId, bool current);

private:
    // Nächster freier sort_order-Wert für ein neues bzw. wieder
    // geöffnetes Ziel dieses Hobbys (ans Ende der offenen Liste).
    static int nextSortOrder(int hobbyId);
};

#endif
