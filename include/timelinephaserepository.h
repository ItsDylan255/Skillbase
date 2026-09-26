#ifndef TIMELINEPHASEREPOSITORY_H
#define TIMELINEPHASEREPOSITORY_H

#include <QList>
#include <QString>
#include <QDate>

#include "timelinephase.h"

class TimelinePhaseRepository
{
public:
    static bool add(
        int hobbyId,
        const QString &name,
        const QString &description,
        const QString &startDate,
        const QString &endDate,
        int &id
        );

    static QList<TimelinePhase> getForHobby(int hobbyId);

    static bool getById(
        int phaseId,
        TimelinePhase &out
        );

    static bool update(
        int phaseId,
        const QString &name,
        const QString &description,
        const QString &startDate,
        const QString &endDate
        );

    static bool remove(int phaseId);

    static bool getCurrentForHobby(
        int hobbyId,
        TimelinePhase &out
        );

    // Liefert das späteste Enddatum aller Phasen eines Hobbys.
    // Wird keine Phase gefunden, ist das zurückgegebene Datum ungültig.
    static QDate getLatestEndDateForHobby(int hobbyId);

private:
    static bool hasOverlap(
        int hobbyId,
        const QString &startDate,
        const QString &endDate,
        int excludePhaseId = 0
        );
};

#endif