#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>
#include <algorithm>

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
    // Berechnet den Fortschritt vom Startwert bis zum Zielwert.
    // Der Wert wird für die Anzeige auf 0–100 % begrenzt.
    double progressPercent() const
    {
        // Ein Zielwert, der dem Startwert entspricht,
        // kann keinen sinnvollen Fortschritt ergeben.
        if (goal == startValue)
            return 0.0;

        const double progress =
            (value - startValue) /
            (goal - startValue) *
            100.0;

        // Der tatsächliche Wert darf über das Ziel hinausgehen.
        // Für die Fortschrittsanzeige begrenzen wir ihn trotzdem auf 100 %.
        return std::clamp(progress, 0.0, 100.0);
    }
};

#endif