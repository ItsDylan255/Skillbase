#ifndef VALUEMODE_H
#define VALUEMODE_H

#include <QString>

enum class ValueMode
{
    Progress,
    Cumulative,
    Time,
    TimerOnly
};

inline QString valueModeToString(ValueMode mode)
{
    switch (mode) {
    case ValueMode::Progress:   return QStringLiteral("progress");
    case ValueMode::Cumulative: return QStringLiteral("cumulative");
    case ValueMode::Time:       return QStringLiteral("time");
    case ValueMode::TimerOnly:  return QStringLiteral("timer_only");
    }
    return QStringLiteral("progress");
}

inline ValueMode valueModeFromString(const QString &value)
{
    if (value == QStringLiteral("cumulative"))
        return ValueMode::Cumulative;
    if (value == QStringLiteral("time"))
        return ValueMode::Time;
    if (value == QStringLiteral("timer_only"))
        return ValueMode::TimerOnly;

    return ValueMode::Progress;
}

#endif // VALUEMODE_H