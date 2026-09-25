#include "timelinephaserepository.h"
#include "database.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

bool TimelinePhaseRepository::hasOverlap(
    int hobbyId,
    const QString &startDate,
    const QString &endDate,
    int excludePhaseId
    )
{
    QSqlQuery query;

    query.prepare(
        "SELECT id "
        "FROM timeline_phases "
        "WHERE hobby_id = :hobby_id "
        "AND start_date <= :end_date "
        "AND end_date >= :start_date "
        "AND id != :exclude_id"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":start_date", startDate);
    query.bindValue(":end_date", endDate);
    query.bindValue(":exclude_id", excludePhaseId);

    if (!query.exec()) {
        qDebug() << "Überschneidung konnte nicht geprüft werden:"
                 << query.lastError().text();
        return true;
    }

    return query.next();
}

bool TimelinePhaseRepository::add(
    int hobbyId,
    const QString &name,
    const QString &description,
    const QString &startDate,
    const QString &endDate,
    int &id
    )
{
    if (startDate > endDate) {
        qDebug() << "Ungültiger Zeitraum: Startdatum liegt nach dem Enddatum.";
        return false;
    }

    if (hasOverlap(hobbyId, startDate, endDate)) {
        qDebug() << "Timeline-Phase überschneidet sich mit einer bestehenden Phase.";
        return false;
    }

    QSqlQuery query;

    query.prepare(
        "INSERT INTO timeline_phases "
        "(hobby_id, name, description, start_date, end_date) "
        "VALUES (:hobby_id, :name, :description, :start_date, :end_date)"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":start_date", startDate);
    query.bindValue(":end_date", endDate);

    if (!query.exec()) {
        qDebug() << "Timeline-Phase konnte nicht erstellt werden:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();
    return true;
}

QList<TimelinePhase> TimelinePhaseRepository::getForHobby(int hobbyId)
{
    QList<TimelinePhase> phases;

    QSqlQuery query;

    query.prepare(
        "SELECT id, hobby_id, name, description, start_date, end_date "
        "FROM timeline_phases "
        "WHERE hobby_id = :hobby_id "
        "ORDER BY start_date ASC"
        );

    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Timeline-Phasen konnten nicht geladen werden:"
                 << query.lastError().text();
        return phases;
    }

    while (query.next()) {
        TimelinePhase phase;

        phase.id = query.value("id").toInt();
        phase.hobbyId = query.value("hobby_id").toInt();
        phase.name = query.value("name").toString();
        phase.description = query.value("description").toString();
        phase.startDate = query.value("start_date").toString();
        phase.endDate = query.value("end_date").toString();

        phases.append(phase);
    }

    return phases;
}

bool TimelinePhaseRepository::getCurrentForHobby(
    int hobbyId,
    TimelinePhase &out
    )
{
    const QString today =
        QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query;

    query.prepare(
        "SELECT id, hobby_id, name, description, start_date, end_date "
        "FROM timeline_phases "
        "WHERE hobby_id = :hobby_id "
        "AND start_date <= :today "
        "AND end_date >= :today "
        "LIMIT 1"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":today", today);

    if (!query.exec()) {
        qDebug() << "Aktuelle Timeline-Phase konnte nicht geladen werden:"
                 << query.lastError().text();
        return false;
    }

    if (!query.next())
        return false;

    out.id = query.value("id").toInt();
    out.hobbyId = query.value("hobby_id").toInt();
    out.name = query.value("name").toString();
    out.description = query.value("description").toString();
    out.startDate = query.value("start_date").toString();
    out.endDate = query.value("end_date").toString();

    return true;
}

bool TimelinePhaseRepository::getById(
    int phaseId,
    TimelinePhase &out
    )
{
    QSqlQuery query;

    query.prepare(
        "SELECT id, hobby_id, name, description, start_date, end_date "
        "FROM timeline_phases "
        "WHERE id = :id"
        );

    query.bindValue(":id", phaseId);

    if (!query.exec()) {
        qDebug() << "Timeline-Phase konnte nicht geladen werden:"
                 << query.lastError().text();
        return false;
    }

    if (!query.next())
        return false;

    out.id = query.value("id").toInt();
    out.hobbyId = query.value("hobby_id").toInt();
    out.name = query.value("name").toString();
    out.description = query.value("description").toString();
    out.startDate = query.value("start_date").toString();
    out.endDate = query.value("end_date").toString();

    return true;
}

bool TimelinePhaseRepository::update(
    int phaseId,
    const QString &name,
    const QString &description,
    const QString &startDate,
    const QString &endDate
    )
{
    if (startDate > endDate) {
        qDebug() << "Ungültiger Zeitraum: Startdatum liegt nach dem Enddatum.";
        return false;
    }

    TimelinePhase phase;

    if (!getById(phaseId, phase)) {
        qDebug() << "Timeline-Phase konnte nicht geladen werden.";
        return false;
    }

    QSqlQuery query;

    query.prepare(
        "UPDATE timeline_phases "
        "SET name = :name, "
        "description = :description, "
        "start_date = :start_date, "
        "end_date = :end_date "
        "WHERE id = :id"
        );

    query.bindValue(":id", phaseId);
    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":start_date", startDate);
    query.bindValue(":end_date", endDate);

    if (!query.exec()) {
        qDebug() << "Timeline-Phase konnte nicht aktualisiert werden:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool TimelinePhaseRepository::remove(int phaseId)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM timeline_phases "
        "WHERE id = :id"
        );

    query.bindValue(":id", phaseId);

    if (!query.exec()) {
        qDebug() << "Timeline-Phase konnte nicht gelöscht werden:"
                 << query.lastError().text();
        return false;
    }

    return true;
}