#ifndef HOBBYNOTEREPOSITORY_H
#define HOBBYNOTEREPOSITORY_H

#include <QString>

class HobbyNoteRepository
{
public:
    static QString getContent(int hobbyId);
    static bool setContent(int hobbyId, const QString &content);
};

#endif