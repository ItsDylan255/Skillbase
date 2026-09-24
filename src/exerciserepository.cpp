#include "exerciserepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool ExerciseRepository::add(
    int hobbyId,
    const QString &name,
    const QString &description,
    const QString &category,
    const QString &goal,
    int &id
    )
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO exercises (hobby_id, name, description, category, goal) "
        "VALUES (:hobby_id, :name, :description, :category, :goal)"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":category", category);
    query.bindValue(":goal", goal);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern der Übung:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();

    return true;
}

QList<Exercise> ExerciseRepository::getForHobby(int hobbyId, bool includeArchived)
{
    QList<Exercise> exercises;
    QSqlQuery query;

    QString sql =
        "SELECT id, hobby_id, name, description, category, goal, archived "
        "FROM exercises "
        "WHERE hobby_id = :hobby_id";

    if (!includeArchived) {
        sql += " AND archived = 0";
    }

    query.prepare(sql);
    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Übungen:"
                 << query.lastError().text();
        return exercises;
    }

    while (query.next()) {
        Exercise exercise;
        exercise.id = query.value("id").toInt();
        exercise.hobbyId = query.value("hobby_id").toInt();
        exercise.name = query.value("name").toString();
        exercise.description = query.value("description").toString();
        exercise.category = query.value("category").toString();
        exercise.goal = query.value("goal").toString();
        exercise.archived = query.value("archived").toInt() != 0;

        exercises.append(exercise);
    }

    return exercises;
}

bool ExerciseRepository::setArchived(int exerciseId, bool archived)
{
    QSqlQuery query;

    query.prepare("UPDATE exercises SET archived = :archived WHERE id = :id");
    query.bindValue(":archived", archived ? 1 : 0);
    query.bindValue(":id", exerciseId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Archivieren der Übung:"
                 << query.lastError().text();
        return false;
    }

    return true;
}