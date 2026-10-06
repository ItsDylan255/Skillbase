#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>
#include "valuemode.h"
#include <algorithm>

// Wie der Wert einer Übung interpretiert wird.
//
// Progress:   Der Wert verändert sich (Bankdrücken, Bestzeit).
//             Fortschritt = (letzter Wert - Startwert) / (Ziel - Startwert)
//
// Cumulative: Der Wert sammelt sich an (Liegestütze, wiederholte Vokabeln).
//             Fortschritt = Summe aller Werte / Ziel
//
// Time:       Lernzeit sammelt sich automatisch (Spanisch lernen).
//             Fortschritt = Summe aller Session-Dauern / Ziel
//
// TimerOnly:  Nur Timer, kein Ziel (SSH lernen).
//             Kein Fortschritt. Nur Verlauf und Statistik.

struct Exercise
{
    int id = 0;
    int hobbyId = 0;
    QString name;
    QString description;
    int categoryId = 0;
    double startValue = 0.0;
    double value = 0.0;
    QString unit;
    double goal = 0.0;
    bool archived = false;
    ValueMode valueMode = ValueMode::Progress;

    // Fortschritt für Progress-Übungen (letzter Wert vs. Startwert und Ziel).
    // Für Cumulative/Time wird der Fortschritt außerhalb berechnet
    // (aus der Summe aller Logs).
    double progressPercent() const
    {
        if (goal == startValue)
            return 0.0;

        const double progress =
            (value - startValue) /
            (goal - startValue) *
            100.0;

        return std::clamp(progress, 0.0, 100.0);
    }
};

#endif // EXERCISE_H