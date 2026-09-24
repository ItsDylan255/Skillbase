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

    // Categories belong to a specific hobby, so they are created before
    // exercises because exercises reference this table.
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS categories ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "hobby_id INTEGER NOT NULL,"
            "name TEXT NOT NULL,"
            "UNIQUE (hobby_id, name),"
            "FOREIGN KEY (hobby_id) REFERENCES hobbies(id) ON DELETE CASCADE"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der categories-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS exercises ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "hobby_id INTEGER NOT NULL,"
            "name TEXT NOT NULL,"
            "description TEXT,"
            "category_id INTEGER,"
            "value REAL,"
            "unit TEXT,"
            "goal TEXT,"
            "archived INTEGER NOT NULL DEFAULT 0,"
            "FOREIGN KEY (hobby_id) REFERENCES hobbies(id) ON DELETE CASCADE,"
            "FOREIGN KEY (category_id) REFERENCES categories(id) ON DELETE SET NULL"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der exercises-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS exercise_logs ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "exercise_id INTEGER NOT NULL,"
            "performed_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "value REAL,"
            "unit TEXT,"
            "duration_seconds INTEGER,"
            "FOREIGN KEY (exercise_id) REFERENCES exercises(id) ON DELETE CASCADE"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der exercise_logs-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS routines ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "hobby_id INTEGER NOT NULL,"
            "name TEXT NOT NULL,"
            "description TEXT,"
            "FOREIGN KEY (hobby_id) REFERENCES hobbies(id) ON DELETE CASCADE"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der routines-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS routine_steps ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "routine_id INTEGER NOT NULL,"
            "exercise_id INTEGER NOT NULL,"
            "position INTEGER NOT NULL,"
            "duration_seconds INTEGER,"
            "FOREIGN KEY (routine_id) REFERENCES routines(id) ON DELETE CASCADE,"
            "FOREIGN KEY (exercise_id) REFERENCES exercises(id) ON DELETE CASCADE"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der routine_steps-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS routine_logs ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "routine_id INTEGER NOT NULL,"
            "performed_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "duration_seconds INTEGER,"
            "FOREIGN KEY (routine_id) REFERENCES routines(id) ON DELETE CASCADE"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der routine_logs-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS goals ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "hobby_id INTEGER NOT NULL,"
            "title TEXT NOT NULL,"
            "deadline TEXT,"
            "progress_percent INTEGER NOT NULL DEFAULT 0,"
            "status TEXT NOT NULL DEFAULT 'in_progress',"
            "FOREIGN KEY (hobby_id) REFERENCES hobbies(id) ON DELETE CASCADE"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der goals-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS hobby_notes ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "hobby_id INTEGER NOT NULL UNIQUE,"
            "content TEXT,"
            "FOREIGN KEY (hobby_id) REFERENCES hobbies(id) ON DELETE CASCADE"
            ")"
            )) {
        qDebug() << "Fehler beim Erstellen der hobby_notes-Tabelle:"
                 << query.lastError().text();
        return false;
    }

    return true;
}