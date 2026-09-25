#ifndef TIMELINEPHASE_H
#define TIMELINEPHASE_H

#include <QString>
#include <QDate>

struct TimelinePhase
{
    int id = 0;
    int hobbyId = 0;

    QString name;
    QString description;

    QString startDate;
    QString endDate;

    // Berechnet die Dauer der Phase in Tagen.
    // Start- und Endtag werden beide mitgezählt.
    int durationDays() const
    {
        const QDate start =
            QDate::fromString(startDate, "yyyy-MM-dd");

        const QDate end =
            QDate::fromString(endDate, "yyyy-MM-dd");

        if (!start.isValid() || !end.isValid() || end < start)
            return 0;

        return start.daysTo(end) + 1;
    }

    // Formatiert die Dauer für die spätere Anzeige in der UI.
    QString durationText() const
    {
        const QDate start =
            QDate::fromString(startDate, "yyyy-MM-dd");

        const QDate end =
            QDate::fromString(endDate, "yyyy-MM-dd");

        if (!start.isValid() || !end.isValid() || end < start)
            return "–";

        const int days = durationDays();
        return QString("%1 Tage").arg(days);
    }
};

#endif