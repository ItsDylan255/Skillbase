#include "categoryrepository.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

bool CategoryRepository::add(int hobbyId, const QString &name, int &id)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO categories (hobby_id, name) "
        "VALUES (:hobby_id, :name)"
        );

    query.bindValue(":hobby_id", hobbyId);
    query.bindValue(":name", name);

    if (!query.exec()) {
        qDebug() << "Fehler beim Speichern der Kategorie:"
                 << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();

    return true;
}

QList<Category> CategoryRepository::getForHobby(int hobbyId)
{
    QList<Category> categories;
    QSqlQuery query;

    query.prepare(
        "SELECT id, hobby_id, name "
        "FROM categories "
        "WHERE hobby_id = :hobby_id "
        "ORDER BY name"
        );

    query.bindValue(":hobby_id", hobbyId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Laden der Kategorien:"
                 << query.lastError().text();
        return categories;
    }

    while (query.next()) {
        Category category;

        category.id = query.value("id").toInt();
        category.hobbyId = query.value("hobby_id").toInt();
        category.name = query.value("name").toString();

        categories.append(category);
    }

    return categories;
}

bool CategoryRepository::rename(int categoryId, const QString &name)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE categories "
        "SET name = :name "
        "WHERE id = :id"
        );

    query.bindValue(":name", name);
    query.bindValue(":id", categoryId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Aktualisieren der Kategorie:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool CategoryRepository::remove(int categoryId)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM categories "
        "WHERE id = :id"
        );

    query.bindValue(":id", categoryId);

    if (!query.exec()) {
        qDebug() << "Fehler beim Löschen der Kategorie:"
                 << query.lastError().text();
        return false;
    }

    return true;
}
