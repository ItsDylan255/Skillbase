#ifndef DATABASE_H
#define DATABASE_H

class Database
{
public:
    // Opens the local SQLite connection and ensures all tables exist.
    // Must be called once at application startup before any repository is used.
    static bool connect();

private:
    static bool createTables();
};

#endif
