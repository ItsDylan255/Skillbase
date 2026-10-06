#pragma once

// Gemeinsame Basis für alle Animationen in Skillbase.
//
// Header-only und ohne Q_OBJECT: Es muss nichts in CMake/qmake eingetragen
// werden.
//
// Regeln (aus ui-guidelines.md: "unnötige Animationen vermeiden"):
//  - Dauern 120-400 ms, Easing "Out", keine Dauerschleifen.
//  - Nur Transparenz und Werte animieren, nie Layout-Geometrie.
//  - Alles ist überspringbar (Motion::skip) und respektiert die Einstellung
//    "Animationen reduzieren": Dann springen alle Animationen sofort in den
//    Endzustand, es wird nichts verzögert.
//
// Einstellung "Animationen reduzieren":
//   QSettings("Skillbase", "Skillbase"), Schlüssel  ui/motion
//                        =  "auto" (Standard) | "reduced" | "full"
//   auto    -> folgt der Systemeinstellung (derzeit nur Windows:
//              "Animationen im Fenster anzeigen")
//   Zum Testen überschreibt die Umgebungsvariable SKILLBASE_MOTION
//   (reduced/full) die Einstellung.

#include <QAbstractAnimation>
#include <QByteArray>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>
#include <QLibrary>
#include <QMetaObject>
#include <QObject>
#include <QPauseAnimation>
#include <QPointer>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QVariantAnimation>
#include <QWidget>

#include <functional>

namespace Motion {

// ── Dauern ────────────────────────────────────────────────────────────────
constexpr int Fast    = 120;   // Mikro-Interaktionen (Pop, Fehlerzeile)
constexpr int Normal  = 250;   // Standard (Einblenden, Balken)
constexpr int Slow    = 400;   // größere Flächen (Timeline, Crossfade)
constexpr int Stagger = 60;    // Versatz zwischen gestaffelten Elementen

inline QEasingCurve easeOut()   { return QEasingCurve(QEasingCurve::OutCubic); }
inline QEasingCurve easeInOut() { return QEasingCurve(QEasingCurve::InOutCubic); }

// ── Einstellung "Animationen reduzieren" ─────────────────────────────────
enum class Mode { Auto, Reduced, Full };

inline Mode mode()
{
    const QByteArray env = qgetenv("SKILLBASE_MOTION").trimmed().toLower();

    if (env == "reduced")
        return Mode::Reduced;

    if (env == "full")
        return Mode::Full;

    QSettings settings(QStringLiteral("Skillbase"), QStringLiteral("Skillbase"));

    const QString value =
        settings.value(QStringLiteral("ui/motion"), QStringLiteral("auto"))
            .toString().toLower();

    if (value == "reduced")
        return Mode::Reduced;

    if (value == "full")
        return Mode::Full;

    return Mode::Auto;
}

inline void setMode(Mode newMode)
{
    const char *value = newMode == Mode::Reduced ? "reduced"
                      : newMode == Mode::Full    ? "full"
                                                 : "auto";

    QSettings settings(QStringLiteral("Skillbase"), QStringLiteral("Skillbase"));
    settings.setValue(QStringLiteral("ui/motion"), QString::fromLatin1(value));
}

// Systemeinstellung. Windows: SystemParametersInfo(SPI_GETCLIENTAREAANIMATION).
// Die Funktion wird dynamisch geladen, damit <windows.h> nicht eingebunden
// werden muss. Auf anderen Systemen: Animationen erlaubt.
inline bool systemPrefersReducedMotion()
{
#ifdef Q_OS_WIN
    using SystemParametersInfoW_t = int (__stdcall *)(unsigned, unsigned, void *, unsigned);

    QLibrary user32(QStringLiteral("user32"));

    if (auto fn = reinterpret_cast<SystemParametersInfoW_t>(
            user32.resolve("SystemParametersInfoW"))) {

        constexpr unsigned SPI_GETCLIENTAREAANIMATION_ = 0x1042;
        int enabled = 1;

        if (fn(SPI_GETCLIENTAREAANIMATION_, 0, &enabled, 0))
            return enabled == 0;
    }
#endif
    return false;
}

inline bool reduced()
{
    switch (mode()) {
    case Mode::Reduced: return true;
    case Mode::Full:    return false;
    case Mode::Auto:    break;
    }

    return systemPrefersReducedMotion();
}

// Dauer in ms, bei reduzierter Bewegung 0.
inline int duration(int ms)
{
    return reduced() ? 0 : ms;
}

// ── Starten / Überspringen ───────────────────────────────────────────────

// Springt eine (laufende) Animation sofort in den Endzustand. Der
// Endzustand wird angewendet und finished() wird ausgelöst.
inline void skip(QAbstractAnimation *animation)
{
    if (!animation)
        return;

    if (animation->state() == QAbstractAnimation::Stopped)
        animation->start();

    animation->setCurrentTime(animation->totalDuration());
}

// Startet die Animation und löscht sie danach selbst. Bei reduzierter
// Bewegung wird sofort der Endzustand angewendet.
// Hinweis: Der Aufrufer darf den Zeiger danach nicht mehr verwenden, wenn
// die Bewegung reduziert ist (die Animation ist dann bereits gelöscht).
inline QAbstractAnimation *run(QAbstractAnimation *animation)
{
    if (!animation)
        return nullptr;

    if (reduced()) {
        skip(animation);
        animation->deleteLater();
        return nullptr;
    }

    animation->start(QAbstractAnimation::DeleteWhenStopped);
    return animation;
}

// ── Bausteine ────────────────────────────────────────────────────────────

// Animiert eine Property von ihrem aktuellen Wert auf "to".
inline QAbstractAnimation *animateProperty(QObject *target,
                                           const char *property,
                                           const QVariant &to,
                                           int ms = Normal,
                                           const QEasingCurve &curve = easeOut())
{
    if (!target)
        return nullptr;

    auto *animation = new QPropertyAnimation(target, property, target);
    animation->setStartValue(target->property(property));
    animation->setEndValue(to);
    animation->setDuration(ms);
    animation->setEasingCurve(curve);

    return run(animation);
}

// Animiert einen Zahlenwert und meldet jeden Schritt an "apply"
// (z. B. Fortschrittsbalken, hochzählende Zahlen). Bei reduzierter Bewegung
// wird "apply(to)" einmal sofort aufgerufen.
// "delayMs" verzögert den Start (gestaffelte Abläufe). Während der
// Verzögerung wird "apply" nicht aufgerufen: Der Aufrufer setzt den
// Anfangszustand selbst.
// "owner" bestimmt die Lebensdauer: Wird er gelöscht, endet die Animation.
inline QAbstractAnimation *animateValue(QObject *owner,
                                        qreal from,
                                        qreal to,
                                        int ms,
                                        std::function<void(qreal)> apply,
                                        const QEasingCurve &curve = easeOut(),
                                        int delayMs = 0)
{
    auto *animation = new QVariantAnimation(owner);
    animation->setStartValue(from);
    animation->setEndValue(to);
    animation->setDuration(ms);
    animation->setEasingCurve(curve);

    QObject::connect(animation, &QVariantAnimation::valueChanged, animation,
                     [apply](const QVariant &value) { apply(value.toReal()); });

    if (delayMs <= 0)
        return run(animation);

    auto *group = new QSequentialAnimationGroup(owner);
    group->addAnimation(new QPauseAnimation(delayMs));
    group->addAnimation(animation);

    return run(group);
}

// Blendet ein Widget ein (optional verzögert). Das Widget wird sofort
// transparent gesetzt, damit es vor dem Start nicht aufblitzt. Danach
// wird der Opacity-Effekt wieder entfernt (Effekte verlangsamen das
// Zeichnen und stören z. B. Drag-Bilder).
// Bei reduzierter Bewegung passiert nichts (das Widget bleibt sichtbar).
inline QAbstractAnimation *fadeIn(QWidget *widget,
                                  int ms = Normal,
                                  int delayMs = 0,
                                  const QEasingCurve &curve = easeOut())
{
    if (!widget)
        return nullptr;

    if (reduced())
        return nullptr;

    auto *effect = new QGraphicsOpacityEffect(widget);
    effect->setOpacity(0.0);
    widget->setGraphicsEffect(effect);          // ersetzt/löscht einen alten Effekt

    auto *group = new QSequentialAnimationGroup(effect);

    if (delayMs > 0)
        group->addAnimation(new QPauseAnimation(delayMs));

    auto *fade = new QPropertyAnimation(effect, "opacity");
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->setDuration(ms);
    fade->setEasingCurve(curve);
    group->addAnimation(fade);

    // Entfernen erst in der Event-Schleife: Der Effekt ist Elternobjekt
    // dieser Animation, sie darf nicht in ihrem eigenen Signal gelöscht
    // werden.
    QPointer<QWidget> guard(widget);
    QObject::connect(group, &QAbstractAnimation::finished, widget, [guard]() {
        if (guard) {
            QMetaObject::invokeMethod(guard.data(), [guard]() {
                if (guard)
                    guard->setGraphicsEffect(nullptr);   // löscht den Effekt
            }, Qt::QueuedConnection);
        }
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
    return group;
}

// Blendet ein Widget aus. "onFinished" wird danach aufgerufen (auch bei
// reduzierter Bewegung, dann sofort), z. B. um das Widget auszublenden
// oder die Seite zu wechseln.
inline QAbstractAnimation *fadeOut(QWidget *widget,
                                   int ms = Normal,
                                   std::function<void()> onFinished = {})
{
    if (!widget) {
        if (onFinished)
            onFinished();
        return nullptr;
    }

    if (reduced()) {
        if (onFinished)
            onFinished();
        return nullptr;
    }

    auto *effect = new QGraphicsOpacityEffect(widget);
    effect->setOpacity(1.0);
    widget->setGraphicsEffect(effect);

    auto *fade = new QPropertyAnimation(effect, "opacity", effect);
    fade->setStartValue(1.0);
    fade->setEndValue(0.0);
    fade->setDuration(ms);
    fade->setEasingCurve(easeOut());

    QPointer<QWidget> guard(widget);
    QObject::connect(fade, &QAbstractAnimation::finished, widget,
                     [guard, onFinished]() {
        if (guard) {
            QMetaObject::invokeMethod(guard.data(), [guard]() {
                if (guard)
                    guard->setGraphicsEffect(nullptr);
            }, Qt::QueuedConnection);
        }

        if (onFinished)
            onFinished();
    });

    fade->start(QAbstractAnimation::DeleteWhenStopped);
    return fade;
}

} // namespace Motion
