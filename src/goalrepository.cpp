#include "goalrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QMetaType>
#include <QDebug>

namespace {

QVariant deadlineValue(const QString &deadline)
{
    if (deadline.isEmpty())
        return QVariant(QMetaType(QMetaType::QString));

    return QVariant(deadline);
}

Goal goalFromQuery(const QSqlQuery &query)
{
    Goal goal;
    goal.id          = query.value("id").toInt();
    goal.hobbyId      = query.value("hobby_id").toInt();
    goal.title        = query.value("title").toString();
    goal.description  = query.value("description").toString();
    goal.deadline     = query.value("deadline").toString();
    goal.status       = query.value("status").toString();
    goal.sortOrder    = query.value("sort_order").toInt();
    goal.isCurrent  = query.value("is_current").toBool();
    return goal;
}

} // namespace

bool GoalRepository::add(
    int hobbyId,
    const QString &title,
    const QString &description,
    const QString &deadline,
    int &id
    )
{
    const int sortOrder = nextSortOrder(hobbyId);

    QSqlQuery query;

    query.prepare(
        "INSERT INTO goals "
        "(hobby_id, title, description, deadline, status, sort_order, is_current) "
        "VALUES (:hobby_id, :title, :description, :deadline, 'open', :sort_order, 0)"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":deadline", deadlineValue(deadline));
    query.bindValue(":sort_order", sortOrder);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern des Ziels:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();
    return true;
}

QList<Goal> GoalRepository::getForHobby(int hobbyId)
{
    QList<Goal> goals;
    QSqlQuery query;

    // Offene Ziele zuerst (nach der vom Benutzer festgelegten
    // Reihenfolge), geschaffte Ziele danach (neueste zuerst).
    query.prepare(
        "SELECT id, hobby_id, title, description, deadline, status, "
        "sort_order, is_current "
        "FROM goals "
        "WHERE hobby_id = :hobby_id "
        "ORDER BY "
        "  CASE "
        "    WHEN status = 'open' AND is_current = 1 THEN 0 "
        "    WHEN status = 'open' THEN 1 "
        "    ELSE 2 "
        "  END, "
        "  CASE status WHEN 'open' THEN sort_order ELSE -id END"
        );
    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Ziele:"
                 << query.lastError().text();
        return goals;
    }

    while (query.next())
        goals.append(goalFromQuery(query));

    return goals;
}

bool GoalRepository::getById(int goalId, Goal &goal)
{
    QSqlQuery query;

    query.prepare(
        "SELECT id, hobby_id, title, description, deadline, status, "
        "sort_order, is_current "
        "FROM goals WHERE id = :id"
        );
    query.bindValue(":id", goalId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden des Ziels:"
                 << query.lastError().text();
        return false;
    }

    if (!query.next())
        return false;

    goal = goalFromQuery(query);
    return true;
}

bool GoalRepository::update(
    int goalId,
    const QString &title,
    const QString &description,
    const QString &deadline
    )
{
    QSqlQuery query;

    query.prepare(
        "UPDATE goals SET title = :title, description = :description, "
        "deadline = :deadline WHERE id = :id"
        );
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":deadline", deadlineValue(deadline));
    query.bindValue(":id", goalId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Aktualisieren des Ziels:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool GoalRepository::setStatus(int goalId, const QString &status)
{
    // Wird ein Ziel wieder geöffnet, landet es ans Ende der offenen Liste.
    int sortOrder = 0;

    if (status == "open") {

        Goal existing;

        if (getById(goalId, existing))
            sortOrder = nextSortOrder(existing.hobbyId);
    }

    QSqlQuery query;

    query.prepare(
        "UPDATE goals SET "
        "status = :status, "
        "sort_order = CASE "
        "    WHEN :status_check = 'open' THEN :sort_order "
        "    ELSE sort_order "
        "END, "
        "is_current = CASE "
        "    WHEN :status_check = 'done' THEN 0 "
        "    ELSE is_current "
        "END "
        "WHERE id = :id"
        );
    query.bindValue(":status", status);
    query.bindValue(":status_check", status);
    query.bindValue(":sort_order", sortOrder);
    query.bindValue(":id", goalId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Ändern des Ziel-Status:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool GoalRepository::setCurrent(int goalId, bool current)
{
    Goal goal;

    if (!getById(goalId, goal)) {
        qDebug() << "Aktuelles Ziel konnte nicht geladen werden.";
        return false;
    }

    // Nur offene Ziele dürfen als Hauptziel ausgewählt werden.
    if (current && goal.status != "open") {
        qDebug() << "Ein geschafftes Ziel kann nicht als Hauptziel gesetzt werden.";
        return false;
    }

    // Ein Ziel wird zum Hauptziel.
    if (current) {

        QSqlQuery query;

        // Zuerst wird das bisherige Hauptziel dieses Hobbys entfernt.
        query.prepare(
            "UPDATE goals "
            "SET is_current = 0 "
            "WHERE hobby_id = :hobby_id"
            );

        query.bindValue(":hobby_id", goal.hobbyId);

        if (!query.exec()) {
            qDebug() << "Bisheriges Hauptziel konnte nicht zurückgesetzt werden:"
                     << query.lastError().text();
            return false;
        }

        // Danach wird genau dieses Ziel zum Hauptziel.
        query.prepare(
            "UPDATE goals "
            "SET is_current = 1 "
            "WHERE id = :id"
            );

        query.bindValue(":id", goalId);

        if (!query.exec()) {
            qDebug() << "Ziel konnte nicht als Hauptziel gesetzt werden:"
                     << query.lastError().text();
            return false;
        }

        return true;
    }

    // Aktuelles Ziel explizit entfernen.
    QSqlQuery query;

    query.prepare(
        "UPDATE goals "
        "SET is_current = 0 "
        "WHERE id = :id"
        );

    query.bindValue(":id", goalId);

    if (!query.exec()) {
        qDebug() << "Hauptziel konnte nicht entfernt werden:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool GoalRepository::reorder(const QList<int> &orderedGoalIds)
{
    for (int i = 0; i < orderedGoalIds.size(); ++i) {

        QSqlQuery query;

        query.prepare(
            "UPDATE goals SET sort_order = :sort_order WHERE id = :id"
            );
        query.bindValue(":sort_order", i);
        query.bindValue(":id", orderedGoalIds[i]);

        if (!query.exec()) {
            qDebug() << "Fehler beim Speichern der Ziel-Reihenfolge:"
                     << query.lastError().text();
            return false;
        }
    }

    return true;
}

bool GoalRepository::remove(int goalId)
{
    QSqlQuery query;

    query.prepare("DELETE FROM goals WHERE id = :id");
    query.bindValue(":id", goalId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen des Ziels:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

int GoalRepository::nextSortOrder(int hobbyId)
{
    QSqlQuery query;

    query.prepare(
        "SELECT COALESCE(MAX(sort_order), -1) + 1 FROM goals "
        "WHERE hobby_id = :hobby_id AND status = 'open'"
        );
    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec() || !query.next())
        return 0;

    return query.value(0).toInt();
}
