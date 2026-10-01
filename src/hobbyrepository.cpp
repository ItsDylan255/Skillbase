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
    // Reihenfolge ist wichtig, weil Foreign Keys greifen können.
    // Wir löschen zuerst die Kinder, dann die Eltern, dann das Hobby.
    //
    // Falls deine DB ON DELETE CASCADE nutzt, sind die Kinder-Löschungen
    // eigentlich überflüssig — aber sicherheitshalber explizit.
    QSqlQuery query;

    const QStringList statements = {

    // Übungs-Logs (hängen an exercises)
    "DELETE FROM exercise_logs "
    "WHERE exercise_id IN "
    "(SELECT id FROM exercises WHERE hobby_id = :hobby_id)",

        // Übungen
        "DELETE FROM exercises WHERE hobby_id = :hobby_id",

        // Routine-Logs (hängen an routines)
        "DELETE FROM routine_logs "
        "WHERE routine_id IN "
        "(SELECT id FROM routines WHERE hobby_id = :hobby_id)",

        // Routine-Steps (hängen an routines)
        "DELETE FROM routine_steps "
        "WHERE routine_id IN "
        "(SELECT id FROM routines WHERE hobby_id = :hobby_id)",

        // Routinen
        "DELETE FROM routines WHERE hobby_id = :hobby_id",

        // Roadmap-Steps
        "DELETE FROM roadmap_steps WHERE hobby_id = :hobby_id",

        // Timeline-Phasen
        "DELETE FROM timeline_phases WHERE hobby_id = :hobby_id",

        // Ziele
        "DELETE FROM goals WHERE hobby_id = :hobby_id",

        // Notizen
        "DELETE FROM hobby_notes WHERE hobby_id = :hobby_id",

        // Kategorien
        "DELETE FROM categories WHERE hobby_id = :hobby_id"
};

for (const QString &sql : statements) {

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
