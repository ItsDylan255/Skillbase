#ifndef ROADMAPREPOSITORY_H
#define ROADMAPREPOSITORY_H

#include <QList>
#include <QString>
#include "roadmapstep.h"

// Owns all SQL for the roadmap_steps table.
//
// Die Roadmap ist ein hierarchischer Baum von Steps. Jeder Step hat
// genau einen Parent (oder keinen, dann ist er ein Root-Step).
//
// Wichtige Regel: Wird ein Step als erledigt markiert, wird der
// gesamte Unterbaum mit erledigt. Wird er wieder auf unerledigt
// gesetzt, wird der gesamte Unterbaum mit unerledigt.
class RoadmapRepository
{
public:
    // Alle Steps eines Hobbys (Root und Children) in einer flachen Liste.
    // Die Reihenfolge ist: sort_order pro Geschwister-Ebene.
    static QList<RoadmapStep> getForHobby(int hobbyId);

    static bool getById(int stepId, RoadmapStep &step);

    // Fügt einen neuen Step hinzu. parentId == 0 erzeugt einen Root-Step.
    // Der Step landet am Ende der Geschwister-Liste.
    static bool add(
        int hobbyId,
        int parentId,
        const QString &name,
        int &id
        );

    // Ändert nur den Namen.
    static bool rename(int stepId, const QString &name);

    // Setzt den Status UND wendet ihn auf den gesamten Unterbaum an.
    // Liefert true, wenn alles erfolgreich war.
    static bool setCompletedRecursive(int stepId, bool completed);

    // Löscht den Step und alle seine Nachkommen.
    static bool removeRecursive(int stepId);

    // Markiert einen Root-Step als "aktuelle Roadmap".
    // Pro Hobby darf nur einer markiert sein.
    static bool setCurrent(int stepId, bool current);

private:
    // Nächster freier sort_order-Wert für einen neuen Step
    // unter demselben Parent.
    static int nextSortOrder(int hobbyId, int parentId);

    // Rekursive Hilfsfunktion: sammelt alle Nachkommen-IDs (inkl. self).
    static QList<int> collectSubtreeIds(int stepId);

    // Rekursive Hilfsfunktion: setzt completed für eine Liste von IDs.
    static bool setCompletedForIds(const QList<int> &ids, bool completed);
};

#endif // ROADMAPREPOSITORY_H