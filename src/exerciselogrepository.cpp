#include "exerciselogrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool ExerciseLogRepository::add(int exerciseId, double value, const QString &unit, int durationSeconds, int &id)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO exercise_logs (exercise_id, value, unit, duration_seconds) "
        "VALUES (:exercise_id, :value, :unit, :duration_seconds)"
        );

    query.bindValue(":exercise_id", exerciseId);
    query.bindValue(":value", value);
    query.bindValue(":unit", unit);
    query.bindValue(":duration_seconds", durationSeconds);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern des Übungs-Logs:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();
    return true;
}

QList<ExerciseLog> ExerciseLogRepository::getForExercise(int exerciseId)
{
    QList<ExerciseLog> logs;
    QSqlQuery query;

    query.prepare(
        "SELECT id, exercise_id, performed_at, value, unit, duration_seconds "
        "FROM exercise_logs "
        "WHERE exercise_id = :exercise_id "
        "ORDER BY performed_at DESC"
        );
    query.bindValue(":exercise_id", exerciseId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Übungs-Logs:"
                 << query.lastError().text();
        return logs;
    }

    while (query.next()) {
        ExerciseLog log;
        log.id = query.value("id").toInt();
        log.exerciseId = query.value("exercise_id").toInt();
        log.performedAt = query.value("performed_at").toString();
        log.value = query.value("value").toDouble();
        log.unit = query.value("unit").toString();
        log.durationSeconds = query.value("duration_seconds").toInt();
        logs.append(log);
    }

    return logs;
}

bool ExerciseLogRepository::getLatestForExercise(int exerciseId, ExerciseLog &out)
{
    QSqlQuery query;

    query.prepare(
        "SELECT id, exercise_id, performed_at, value, unit, duration_seconds "
        "FROM exercise_logs "
        "WHERE exercise_id = :exercise_id "
        "ORDER BY performed_at DESC "
        "LIMIT 1"
        );
    query.bindValue(":exercise_id", exerciseId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden des letzten Übungs-Logs:"
                 << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        return false; // no log yet - not an error
    }

    out.id = query.value("id").toInt();
    out.exerciseId = query.value("exercise_id").toInt();
    out.performedAt = query.value("performed_at").toString();
    out.value = query.value("value").toDouble();
    out.unit = query.value("unit").toString();
    out.durationSeconds = query.value("duration_seconds").toInt();
    return true;
}