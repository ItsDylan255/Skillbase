#include "hobbyrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool HobbyRepository::add(const QString &name, const QString &color, int &id)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO hobbies (name, color) "
        "VALUES (:name, :color)"
        );

    query.bindValue(":name", name);
    query.bindValue(":color", color);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern des Hobbys:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();

    return true;
}

QList<Hobby> HobbyRepository::getAll()
{
    QList<Hobby> hobbies;
    QSqlQuery query;

    if (!query.exec("SELECT id, name, color FROM hobbies")) {
        qDebug() << "Fehler beim Laden der Hobbys:"
                 << query.lastError().text();
        return hobbies;
    }

    while (query.next()) {
        Hobby hobby;
        hobby.id = query.value("id").toInt();
        hobby.name = query.value("name").toString();
        hobby.color = query.value("color").toString();

        hobbies.append(hobby);
    }

    return hobbies;
}
