#include "roadmaprepository.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QStringList>
#include <QMetaType>

namespace {

RoadmapStep stepFromQuery(const QSqlQuery &query)
{
    RoadmapStep step;
    step.id        = query.value("id").toInt();
    step.hobbyId   = query.value("hobby_id").toInt();
    step.parentId  = query.value("parent_id").toInt();   // NULL → 0
    step.name      = query.value("name").toString();
    step.completed = query.value("completed").toBool();
    step.sortOrder = query.value("sort_order").toInt();
    step.isCurrent = query.value("is_current").toBool();
    return step;
}

} // namespace

QList<RoadmapStep> RoadmapRepository::getForHobby(int hobbyId)
{
    QList<RoadmapStep> steps;
    QSqlQuery query;

    // Wir laden flach und sortieren nach Parent + sort_order.
    // Der Baum wird später im UI zusammengesetzt.
    query.prepare(
        "SELECT id, hobby_id, parent_id, name, completed, sort_order, is_current "
        "FROM roadmap_steps "
        "WHERE hobby_id = :hobby_id "
        "ORDER BY COALESCE(parent_id, 0), sort_order, id"
        );
    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Roadmap:"
                 << query.lastError().text();
        return steps;
    }

    while (query.next())
        steps.append(stepFromQuery(query));

    return steps;
}

bool RoadmapRepository::getById(int stepId, RoadmapStep &step)
{
    QSqlQuery query;

    query.prepare(
        "SELECT id, hobby_id, parent_id, name, completed, sort_order, is_current "
        "FROM roadmap_steps WHERE id = :id"
        );
    query.bindValue(":id", stepId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden des Steps:"
                 << query.lastError().text();
        return false;
    }

    if (!query.next())
        return false;

    step = stepFromQuery(query);
    return true;
}

bool RoadmapRepository::add(
    int hobbyId,
    int parentId,
    const QString &name,
    int &id
    )
{
    const int sortOrder = nextSortOrder(hobbyId, parentId);

    QSqlQuery query;

    query.prepare(
        "INSERT INTO roadmap_steps "
        "(hobby_id, parent_id, name, completed, sort_order) "
        "VALUES (:hobby_id, :parent_id, :name, 0, :sort_order)"
        );

    query.bindValue(":hobby_id", hobbyId);

    if (parentId > 0)
        query.bindValue(":parent_id", parentId);
    else
        query.bindValue(":parent_id", QVariant(QMetaType(QMetaType::Int)));

    query.bindValue(":name", name);
    query.bindValue(":sort_order", sortOrder);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern des Steps:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();
    return true;
}

bool RoadmapRepository::rename(int stepId, const QString &name)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE roadmap_steps SET name = :name WHERE id = :id"
        );
    query.bindValue(":name", name);
    query.bindValue(":id", stepId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Umbenennen des Steps:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool RoadmapRepository::setCompletedRecursive(int stepId, bool completed)
{
    const QList<int> ids = collectSubtreeIds(stepId);

    if (ids.isEmpty())
        return false;

    return setCompletedForIds(ids, completed);
}

bool RoadmapRepository::removeRecursive(int stepId)
{
    // ON DELETE CASCADE in der Datenbank erledigt das Löschen der
    // Nachkommen automatisch. Wir löschen nur den Step selbst.
    QSqlQuery query;

    query.prepare("DELETE FROM roadmap_steps WHERE id = :id");
    query.bindValue(":id", stepId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen des Steps:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

int RoadmapRepository::nextSortOrder(int hobbyId, int parentId)
{
    QSqlQuery query;

    if (parentId > 0) {

        query.prepare(
            "SELECT COALESCE(MAX(sort_order), -1) + 1 FROM roadmap_steps "
            "WHERE hobby_id = :hobby_id AND parent_id = :parent_id"
            );
        query.bindValue(":hobby_id", hobbyId);
        query.bindValue(":parent_id", parentId);

    } else {

        query.prepare(
            "SELECT COALESCE(MAX(sort_order), -1) + 1 FROM roadmap_steps "
            "WHERE hobby_id = :hobby_id AND parent_id IS NULL"
            );
        query.bindValue(":hobby_id", hobbyId);
    }

    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toInt();
}

QList<int> RoadmapRepository::collectSubtreeIds(int stepId)
{
    QList<int> result;
    result.append(stepId);

    // Rekursiv: alle direkten Children holen, dann deren Children.
    QSqlQuery query;
    query.prepare(
        "SELECT id FROM roadmap_steps WHERE parent_id = :parent_id"
        );
    query.bindValue(":parent_id", stepId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Sammeln der Children:"
                 << query.lastError().text();
        return result;
    }

    while (query.next()) {

        const int childId = query.value(0).toInt();
        result.append(collectSubtreeIds(childId));
    }

    return result;
}

bool RoadmapRepository::setCompletedForIds(
    const QList<int> &ids,
    bool completed
    )
{
    if (ids.isEmpty())
        return true;

    // Eine einzelne UPDATE-Anweisung mit IN-Klausel.
    // Wir bauen den Platzhalter-String dynamisch.
    QStringList placeholders;

    for (int i = 0; i < ids.size(); ++i)
        placeholders << "?";

    QSqlQuery query;
    query.prepare(
        "UPDATE roadmap_steps SET completed = ? "
        "WHERE id IN (" + placeholders.join(',') + ")"
        );

    query.addBindValue(completed ? 1 : 0);

    for (int id : ids)
        query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "Fehler beim Setzen des Status:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool RoadmapRepository::setCurrent(int stepId, bool current)
{

    RoadmapStep step;

    if (!getById(stepId, step))
        return false;

    // Nur Root-Steps dürfen markiert werden.
    if (step.parentId != 0)
        return false;

    QSqlQuery query;

    if (current) {

        // Erst alle anderen Root-Steps des Hobbys zurücksetzen.
        query.prepare(
            "UPDATE roadmap_steps SET is_current = 0 "
            "WHERE hobby_id = :hobby_id AND parent_id IS NULL"
            );
        query.bindValue(":hobby_id", step.hobbyId);

        if (!query.exec())
            return false;

        // Dann diesen Step markieren.
        query.prepare(
            "UPDATE roadmap_steps SET is_current = 1 WHERE id = :id"
            );
        query.bindValue(":id", stepId);

        if (!query.exec())
            return false;

    } else {

        // Stern entfernen.
        query.prepare(
            "UPDATE roadmap_steps SET is_current = 0 WHERE id = :id"
            );
        query.bindValue(":id", stepId);

        if (!query.exec())
            return false;
    }

    return true;
}