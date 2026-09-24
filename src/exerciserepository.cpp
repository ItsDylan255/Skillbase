#include "exerciserepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool ExerciseRepository::add(int hobbyId, const QString &name, const QString &description, int &id)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO exercises (hobby_id, name, description) "
        "VALUES (:hobby_id, :name, :description)"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":name", name);
    query.bindValue(":description", description);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern der Übung:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();

    return true;
}

QList<Exercise> ExerciseRepository::getForHobby(int hobbyId)
{
    QList<Exercise> exercises;
    QSqlQuery query;

    query.prepare(
        "SELECT id, hobby_id, name, description "
        "FROM exercises "
        "WHERE hobby_id = :hobby_id"
        );

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

        exercises.append(exercise);
    }

    return exercises;
}
