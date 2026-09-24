#include "hobbynoterepository.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

QString HobbyNoteRepository::getContent(int hobbyId)
{
    QSqlQuery query;

    query.prepare(
        "SELECT content "
        "FROM hobby_notes "
        "WHERE hobby_id = :hobby_id"
        );

    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Hobby-Notiz:"
                 << query.lastError().text();
        return {};
    }

    if (query.next()) {
        return query.value("content").toString();
    }

    return {};
}

bool HobbyNoteRepository::setContent(int hobbyId, const QString &content)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO hobby_notes (hobby_id, content) "
        "VALUES (:hobby_id, :content) "
        "ON CONFLICT(hobby_id) DO UPDATE SET content = :content"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":content", content);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern der Hobby-Notiz:"
                 << query.lastError().text();
        return false;
    }

    return true;
}