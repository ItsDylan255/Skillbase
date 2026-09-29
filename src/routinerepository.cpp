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

    query.prepare(
        "SELECT id, hobby_id, name, description, archived, is_current "
        "FROM routines "
        "WHERE hobby_id = :hobby_id "
        "ORDER BY id ASC"
        );

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

        // SQLite speichert boolesche Werte als INTEGER:
        // 0 = aktiv, 1 = archiviert.
        routine.archived =
            query.value("archived").toBool();

        // SQLite speichert boolesche Werte als INTEGER:
        // 0 = nicht ausgewählt, 1 = ausgewählt.
        routine.isCurrent =
            query.value("is_current").toBool();

        routines.append(routine);
    }

    return routines;
}

bool RoutineRepository::getById(int routineId, Routine &routine)
{
    QSqlQuery query;

    query.prepare(
        "SELECT id, hobby_id, name, description, archived, is_current "
        "FROM routines "
        "WHERE id = :id"
        );

    query.bindValue(":id", routineId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Routine:"
                 << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qDebug() << "Routine nicht gefunden:" << routineId;
        return false;
    }

    routine.id = query.value("id").toInt();
    routine.hobbyId = query.value("hobby_id").toInt();
    routine.name = query.value("name").toString();
    routine.description = query.value("description").toString();

    // SQLite speichert boolesche Werte als INTEGER:
    // 0 = aktiv, 1 = archiviert.
    routine.archived = query.value("archived").toBool();

    // SQLite speichert boolesche Werte als INTEGER:
    // 0 = nicht ausgewählt, 1 = ausgewählt.
    routine.isCurrent = query.value("is_current").toBool();

    return true;
}

bool RoutineRepository::update(
    int routineId,
    const QString &name,
    const QString &description
    )
{
    QSqlQuery query;

    query.prepare(
        "UPDATE routines "
        "SET name = :name, description = :description "
        "WHERE id = :id"
        );

    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":id", routineId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Aktualisieren der Routine:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool RoutineRepository::removeSteps(int routineId)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM routine_steps "
        "WHERE routine_id = :routine_id"
        );

    query.bindValue(":routine_id", routineId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Entfernen der Routine-Schritte:"
                 << query.lastError().text();
        return false;
    }

    return true;
}


bool RoutineRepository::remove(int routineId)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM routines "
        "WHERE id = :id"
        );

    query.bindValue(":id", routineId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen der Routine:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool RoutineRepository::setArchived(int routineId, bool archived)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE routines "
        "SET archived = :archived, "
        "    is_current = CASE "
        "        WHEN :archived = 1 THEN 0 "
        "        ELSE is_current "
        "    END "
        "WHERE id = :id"
        );

    query.bindValue(":archived", archived ? 1 : 0);
    query.bindValue(":id", routineId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Ändern des Archivstatus der Routine:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool RoutineRepository::setCurrent(int routineId, bool current)
{
    // Zuerst bestimmen wir, zu welchem Hobby die Routine gehört.
    // Die maximale Anzahl von drei ausgewählten Routinen gilt immer
    // innerhalb eines einzelnen Hobbys.
    QSqlQuery hobbyQuery;

    hobbyQuery.prepare(
        "SELECT hobby_id "
        "FROM routines "
        "WHERE id = :id"
        );

    hobbyQuery.bindValue(":id", routineId);

    if (!hobbyQuery.exec()) {
        qDebug() << "Fehler beim Laden des Hobbys der Routine:"
                 << hobbyQuery.lastError().text();
        return false;
    }

    if (!hobbyQuery.next()) {
        qDebug() << "Routine nicht gefunden:" << routineId;
        return false;
    }

    const int hobbyId =
        hobbyQuery.value("hobby_id").toInt();

    // Wenn die Routine abgewählt werden soll,
    // müssen wir die Anzahl der bereits ausgewählten Routinen
    // nicht prüfen.
    if (!current) {

        QSqlQuery query;

        query.prepare(
            "UPDATE routines "
            "SET is_current = 0 "
            "WHERE id = :id"
            );

        query.bindValue(":id", routineId);

        if (!query.exec()) {
            qDebug() << "Fehler beim Abwählen der Routine:"
                     << query.lastError().text();
            return false;
        }

        return true;
    }

    // Prüfen, wie viele Routinen dieses Hobbys bereits ausgewählt sind.
    QSqlQuery countQuery;

    countQuery.prepare(
        "SELECT COUNT(*) "
        "FROM routines "
        "WHERE hobby_id = :hobby_id "
        "AND is_current = 1"
        );

    countQuery.bindValue(":hobby_id", hobbyId);

    if (!countQuery.exec()) {
        qDebug() << "Fehler beim Zählen der ausgewählten Routinen:"
                 << countQuery.lastError().text();
        return false;
    }

    if (!countQuery.next())
        return false;

    const int currentCount =
        countQuery.value(0).toInt();

    // Eine bereits ausgewählte Routine darf weiterhin ausgewählt bleiben.
    // Deshalb prüfen wir die Grenze nur, wenn bereits drei andere
    // Routinen ausgewählt sind.
    QSqlQuery selectedQuery;

    selectedQuery.prepare(
        "SELECT is_current "
        "FROM routines "
        "WHERE id = :id"
        );

    selectedQuery.bindValue(":id", routineId);

    if (!selectedQuery.exec()) {
        qDebug() << "Fehler beim Prüfen des Auswahlstatus der Routine:"
                 << selectedQuery.lastError().text();
        return false;
    }

    if (!selectedQuery.next())
        return false;

    const bool alreadyCurrent =
        selectedQuery.value("is_current").toBool();

    if (alreadyCurrent)
        return true;

    if (currentCount >= 3)
        return false;

    // Jetzt darf die Routine ausgewählt werden.
    QSqlQuery query;

    query.prepare(
        "UPDATE routines "
        "SET is_current = 1 "
        "WHERE id = :id"
        );

    query.bindValue(":id", routineId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Auswählen der Routine:"
                 << query.lastError().text();
        return false;
    }

    return true;
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