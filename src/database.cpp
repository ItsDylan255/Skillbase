#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

bool Database::connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("skillbase.db");

    if (!db.open()) {
        qDebug() << "Fehler beim Öffnen der Datenbank:" << db.lastError().text();
        return false;
    }

    QSqlQuery pragma;
    pragma.exec("PRAGMA foreign_keys = ON");

    return createTables();
}

bool Database::createTables()
{
    QSqlQuery query;

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS hobbies ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL,"
            "color TEXT"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der hobbies-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS exercises ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "hobby_id INTEGER NOT NULL,"
            "name TEXT NOT NULL,"
            "description TEXT,"
            "FOREIGN KEY (hobby_id) REFERENCES hobbies(id) ON DELETE CASCADE"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der exercises-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    return true;
}
