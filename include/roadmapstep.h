#ifndef ROADMAPSTEP_H
#define ROADMAPSTEP_H

#include <QString>

// Ein einzelner Step in der Roadmap.
//
// Die Roadmap ist ein hierarchischer Baum:
//   - parentId == 0 bedeutet: Root-Step (oberste Ebene)
//   - parentId >  0 bedeutet: Child von einem anderen Step
//
// Steps haben bewusst NUR einen Namen und einen Erledigungs-Status -
// keine Beschreibung, keine Deadline (siehe Feature-Konzept "Roadmap").
struct RoadmapStep
{
    int id = 0;
    int hobbyId = 0;
    int parentId = 0;       // 0 = Root-Step
    QString name;
    bool completed = false;
    int sortOrder = 0;      // Reihenfolge unter Geschwistern

    // Nur für Root-Steps relevant: markiert die "aktuelle Roadmap"
    // des Hobbys (Stern in der Übersicht).
    bool isCurrent = false;
};

#endif // ROADMAPSTEP_H