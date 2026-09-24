#include "routinerepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool RoutineRepository::add(int hobbyId, const QString &name, const QString &description, int &id)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO routines (hobby_id, name, description) "
        "VALUES (:hobby_id, :name, :description)"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":name", name);
    query.bindValue(":description", description);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern der Routine:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();
    return true;
}

QList<Routine> RoutineRepository::getForHobby(int hobbyId)
{
    QList<Routine> routines;
    QSqlQuery query;

    query.prepare("SELECT id, hobby_id, name, description FROM routines WHERE hobby_id = :hobby_id");
    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Routinen:"
                 << query.lastError().text();
        return routines;
    }

    while (query.next()) {
        Routine routine;
        routine.id = query.value("id").toInt();
        routine.hobbyId = query.value("hobby_id").toInt();
        routine.name = query.value("name").toString();
        routine.description = query.value("description").toString();
        routines.append(routine);
    }

    return routines;
}

bool RoutineRepository::addStep(int routineId, int exerciseId, int position, int durationSeconds, int &id)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO routine_steps (routine_id, exercise_id, position, duration_seconds) "
        "VALUES (:routine_id, :exercise_id, :position, :duration_seconds)"
        );

    query.bindValue(":routine_id", routineId);
    query.bindValue(":exercise_id", exerciseId);
    query.bindValue(":position", position);
    query.bindValue(":duration_seconds", durationSeconds);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern des Routine-Schritts:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();
    return true;
}

QList<RoutineStep> RoutineRepository::getSteps(int routineId)
{
    QList<RoutineStep> steps;
    QSqlQuery query;

    query.prepare(
        "SELECT id, routine_id, exercise_id, position, duration_seconds "
        "FROM routine_steps "
        "WHERE routine_id = :routine_id "
        "ORDER BY position ASC"
        );
    query.bindValue(":routine_id", routineId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Routine-Schritte:"
                 << query.lastError().text();
        return steps;
    }

    while (query.next()) {
        RoutineStep step;
        step.id = query.value("id").toInt();
        step.routineId = query.value("routine_id").toInt();
        step.exerciseId = query.value("exercise_id").toInt();
        step.position = query.value("position").toInt();
        step.durationSeconds = query.value("duration_seconds").toInt();
        steps.append(step);
    }

    return steps;
}

bool RoutineRepository::addLog(int routineId, int durationSeconds, int &id)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO routine_logs (routine_id, duration_seconds) "
        "VALUES (:routine_id, :duration_seconds)"
        );

    query.bindValue(":routine_id", routineId);
    query.bindValue(":duration_seconds", durationSeconds);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern des Routine-Logs:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();
    return true;
}

QList<RoutineLog> RoutineRepository::getLogs(int routineId)
{
    QList<RoutineLog> logs;
    QSqlQuery query;

    query.prepare(
        "SELECT id, routine_id, performed_at, duration_seconds "
        "FROM routine_logs "
        "WHERE routine_id = :routine_id "
        "ORDER BY performed_at DESC"
        );
    query.bindValue(":routine_id", routineId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Routine-Logs:"
                 << query.lastError().text();
        return logs;
    }

    while (query.next()) {
        RoutineLog log;
        log.id = query.value("id").toInt();
        log.routineId = query.value("routine_id").toInt();
        log.performedAt = query.value("performed_at").toString();
        log.durationSeconds = query.value("duration_seconds").toInt();
        logs.append(log);
    }

    return logs;
}