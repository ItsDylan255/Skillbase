#include "goalrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool GoalRepository::add(int hobbyId, const QString &title, const QString &deadline, int &id)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO goals (hobby_id, title, deadline) "
        "VALUES (:hobby_id, :title, :deadline)"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":title", title);
    query.bindValue(":deadline", deadline.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : QVariant(deadline));

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

    query.prepare(
        "SELECT id, hobby_id, title, deadline, progress_percent, status "
        "FROM goals "
        "WHERE hobby_id = :hobby_id"
        );
    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Ziele:"
                 << query.lastError().text();
        return goals;
    }

    while (query.next()) {
        Goal goal;
        goal.id = query.value("id").toInt();
        goal.hobbyId = query.value("hobby_id").toInt();
        goal.title = query.value("title").toString();
        goal.deadline = query.value("deadline").toString();
        goal.progressPercent = query.value("progress_percent").toInt();
        goal.status = query.value("status").toString();
        goals.append(goal);
    }

    return goals;
}

bool GoalRepository::updateProgress(int goalId, int progressPercent, const QString &status)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE goals SET progress_percent = :progress, status = :status WHERE id = :id"
        );
    query.bindValue(":progress", progressPercent);
    query.bindValue(":status", status);
    query.bindValue(":id", goalId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Aktualisieren des Ziel-Fortschritts:"
                 << query.lastError().text();
        return false;
    }

    return true;
}