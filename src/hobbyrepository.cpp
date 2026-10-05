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

bool HobbyRepository::rename(int hobbyId, const QString &newName)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE hobbies SET name = :name WHERE id = :id"
        );
    query.bindValue(":name", newName);
    query.bindValue(":id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Umbenennen des Hobbys:"
                 << query.lastError().text();
        return false;
    }

    return true;
}


bool HobbyRepository::removeRecursive(int hobbyId)
{
    // Reihenfolge ist wichtig, weil Foreign Keys greifen.
    // Join-Tabellen (routine_steps) MÜSSEN vor ihren Elterntabellen
    // (exercises, routines) gelöscht werden, sonst schlägt der
    // Foreign-Key-Check fehl.

    QSqlQuery query;

    const QStringList statements = {

    // 1. Logs zuerst
    "DELETE FROM exercise_logs "
    "WHERE exercise_id IN "
    "(SELECT id FROM exercises WHERE hobby_id = :hobby_id)",

        "DELETE FROM routine_logs "
        "WHERE routine_id IN "
        "(SELECT id FROM routines WHERE hobby_id = :hobby_id)",

        // 2. Join-Tabelle VOR den Eltern
        "DELETE FROM routine_steps "
        "WHERE routine_id IN "
        "(SELECT id FROM routines WHERE hobby_id = :hobby_id)",

        // 3. Eltern-Tabellen
        "DELETE FROM routines WHERE hobby_id = :hobby_id",
        "DELETE FROM exercises WHERE hobby_id = :hobby_id",

        // 4. Unabhängige Tabellen
        "DELETE FROM roadmap_steps WHERE hobby_id = :hobby_id",
        "DELETE FROM timeline_phases WHERE hobby_id = :hobby_id",
        "DELETE FROM goals WHERE hobby_id = :hobby_id",
        "DELETE FROM hobby_notes WHERE hobby_id = :hobby_id",
        "DELETE FROM categories WHERE hobby_id = :hobby_id"
};

for (const QString &sql : statements) {

    // Debug: zeigt den aktuellen Schritt in der Konsole.
    qDebug() << "Hobby delete step:" << sql.left(60);

    query.prepare(sql);
    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen (Schritt):" << sql
                 << "—" << query.lastError().text();
        return false;
    }
}

// Zuletzt das Hobby selbst.
query.prepare("DELETE FROM hobbies WHERE id = :id");
query.bindValue(":id", hobbyId);

if (!query.exec()) {
    qDebug() << "Fehler beim Löschen des Hobbys:"
             << query.lastError().text();
    return false;
}

return true;
}