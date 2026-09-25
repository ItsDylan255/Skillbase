#include "exerciserepository.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

bool ExerciseRepository::add(
    int hobbyId,
    const QString &name,
    const QString &description,
    int categoryId,
    double value,
    const QString &unit,
    const QString &goal,
    int &id
    )
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO exercises "
        "(hobby_id, name, description, category_id, value, unit, goal) "
        "VALUES "
        "(:hobby_id, :name, :description, :category_id, :value, :unit, :goal)"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":name", name);
    query.bindValue(":description", description);

    // A category is optional. NULL means that no category was selected.
    if (categoryId > 0) {
        query.bindValue(":category_id", categoryId);
    } else {
        query.bindValue(":category_id", QVariant());
    }

    query.bindValue(":value", value);
    query.bindValue(":unit", unit);
    query.bindValue(":goal", goal);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern der Übung:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();

    return true;
}

QList<Exercise> ExerciseRepository::getForHobby(
    int hobbyId,
    bool includeArchived
    )
{
    QList<Exercise> exercises;
    QSqlQuery query;

    QString sql =
        "SELECT id, hobby_id, name, description, "
        "category_id, value, unit, goal, archived "
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
        exercise.categoryId = query.value("category_id").toInt();
        exercise.value = query.value("value").toDouble();
        exercise.unit = query.value("unit").toString();
        exercise.goal = query.value("goal").toString();
        exercise.archived = query.value("archived").toInt() != 0;

        exercises.append(exercise);
    }

    return exercises;
}

bool ExerciseRepository::getById(int exerciseId, Exercise &exercise)
{
    QSqlQuery query;

    query.prepare(
        "SELECT id, hobby_id, name, description, "
        "category_id, value, unit, goal, archived "
        "FROM exercises "
        "WHERE id = :id"
        );

    query.bindValue(":id", exerciseId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Übung:"
                 << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        return false;
    }

    exercise.id = query.value("id").toInt();
    exercise.hobbyId = query.value("hobby_id").toInt();
    exercise.name = query.value("name").toString();
    exercise.description = query.value("description").toString();
    exercise.categoryId = query.value("category_id").toInt();
    exercise.value = query.value("value").toDouble();
    exercise.unit = query.value("unit").toString();
    exercise.goal = query.value("goal").toString();
    exercise.archived = query.value("archived").toInt() != 0;

    return true;
}

bool ExerciseRepository::setArchived(int exerciseId, bool archived)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE exercises "
        "SET archived = :archived "
        "WHERE id = :id"
        );

    query.bindValue(":archived", archived ? 1 : 0);
    query.bindValue(":id", exerciseId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Archivieren der Übung:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool ExerciseRepository::update(
    int exerciseId,
    const QString &name,
    const QString &description,
    int categoryId,
    double value,
    const QString &unit,
    const QString &goal
    )
{
    QSqlQuery query;

    query.prepare(
        "UPDATE exercises SET "
        "name = :name, "
        "description = :description, "
        "category_id = :category_id, "
        "value = :value, "
        "unit = :unit, "
        "goal = :goal "
        "WHERE id = :id"
        );

    query.bindValue(":name", name);
    query.bindValue(":description", description);

    // NULL means that the exercise intentionally has no category.
    if (categoryId > 0) {
        query.bindValue(":category_id", categoryId);
    } else {
        query.bindValue(":category_id", QVariant());
    }

    query.bindValue(":value", value);
    query.bindValue(":unit", unit);
    query.bindValue(":goal", goal);
    query.bindValue(":id", exerciseId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Aktualisieren der Übung:"
                 << query.lastError().text();
        return false;
    }

    return true;


}
bool ExerciseRepository::remove(int exerciseId)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM exercises "
        "WHERE id = :id"
        );

    query.bindValue(":id", exerciseId);

    if (!query.exec()) {
        qDebug() << "Übung konnte nicht gelöscht werden:"
                 << query.lastError().text();
        return false;
    }

    return true;
}
