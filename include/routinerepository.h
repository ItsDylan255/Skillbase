#ifndef ROUTINEREPOSITORY_H
#define ROUTINEREPOSITORY_H

#include <QList>
#include "routine.h"
#include "routinestep.h"
#include "routinelog.h"

// Owns all SQL for routines, routine_steps and routine_logs. These three
// tables together form the "Routine" aggregate: a step or a log never
// makes sense without its parent routine, so they're kept in one
// repository rather than split up further.
class RoutineRepository
{
public:
    static bool add(int hobbyId, const QString &name, const QString &description, int &id);
    static QList<Routine> getForHobby(int hobbyId);

    // Lädt eine einzelne Routine anhand ihrer ID.
    static bool getById(int routineId, Routine &routine);

    // Aktualisiert Name und Beschreibung einer bestehenden Routine.
    static bool update(
        int routineId,
        const QString &name,
        const QString &description
        );

    // Entfernt alle bisherigen Schritte einer Routine.
    // Beim Bearbeiten werden sie anschließend aus der aktuellen UI-Reihenfolge neu angelegt.
    static bool removeSteps(int routineId);

    // Löscht eine Routine anhand ihrer ID.
    static bool remove(int routineId);

    // Archiviert oder aktiviert eine Routine.
    static bool setArchived(int routineId, bool archived);

    // Markiert eine Routine als ausgewählt oder entfernt sie aus
    // den maximal drei ausgewählten Routinen eines Hobbys.
    static bool setCurrent(int routineId, bool current);

    // Steps: which exercises belong to a routine, and in what order.
    static bool addStep(int routineId, int exerciseId, int position, int durationSeconds, int &id);
    static QList<RoutineStep> getSteps(int routineId);

    // Logs: completed runs of a routine.
    static bool addLog(int routineId, int durationSeconds, int &id);
    static QList<RoutineLog> getLogs(int routineId);
};

#endif