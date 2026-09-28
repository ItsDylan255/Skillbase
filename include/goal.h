#ifndef GOAL_H
#define GOAL_H

#include <QString>

// Ein einfaches persönliches Ziel innerhalb eines Hobbys.
//
// Bewusst simpel gehalten: keine Unterziele, keine Fortschrittsprozente,
// keine Messwerte - siehe docs/design (Feature-Konzept "Ziele").
struct Goal
{
    int id = 0;
    int hobbyId = 0;
    QString title;
    QString description;
    QString deadline;        // ISO-Datum, z. B. "2026-12-15"; leer, wenn keine Deadline gesetzt ist
    QString status = "open"; // "open" oder "done"
    int sortOrder = 0;       // vom Benutzer per Drag & Drop bestimmte Reihenfolge (nur offene Ziele)
    bool isCurrent = false; // Kennzeichnet das aktuelle Hauptziel dieses Hobbys

    bool isDone() const
    {
        return status == "done";
    }
};

#endif
