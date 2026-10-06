#pragma once

// Intro-Animation des Willkommens-Bildschirms (Skillbase ohne Hobby).
//
// Ablauf (insgesamt ca. 2,4 Sekunden, einmal pro App-Start):
//     0 ms  S-Hälften gleiten entlang des Schnitts zusammen (1200 ms)
//   700 ms  Schriftzug "skill" blendet ein (650 ms)
//   950 ms  Schriftzug "base" blendet ein (650 ms)
//  1500 ms  Titel, dann Untertitel, dann Button blenden gestaffelt ein
//           (je 550 ms, Versatz 150 ms)
//
// Das Intro ist bewusst langsamer als normale Oberflächen-Animationen
// (120-400 ms): Es ist ein einmaliger Marken-Moment und überspringbar.
//
// Zeit nachjustieren ohne Neubau: Umgebungsvariable
//     SKILLBASE_INTRO_TIMESCALE=1.5     (1 = Standard, 2 = doppelt so lang,
//                                         0.5 = doppelt so schnell)
// Dauerhaft ändern: die Konstanten k...Ms unten.
//
// Regeln:
//  - Startet beim ersten Anzeigen der Seite, nie erneut in derselben
//    Sitzung (spätere Besuche zeigen den Screen sofort vollständig).
//  - Klick oder Taste überspringt die Animation sofort. Das Ereignis
//    wird dabei nicht verbraucht: Ein Klick auf den Button funktioniert
//    also auch mitten im Intro.
//  - Bei "Animationen reduzieren" passiert nichts: alles ist sofort sichtbar.
//
// Header-only und ohne Q_OBJECT. Benötigt animhelpers.h und logowidget.h.

#include "animhelpers.h"
#include "logowidget.h"

#include <QApplication>
#include <QEvent>
#include <QList>
#include <QObject>
#include <QPointer>
#include <QTimer>
#include <QWidget>

class WelcomeIntro : public QObject
{
public:
    // "page":      die Willkommens-Seite (Intro startet beim ersten Show-Event)
    // "logo":      das Logo-Widget
    // "followers": Titel, Untertitel, Button ... in Einblend-Reihenfolge
    WelcomeIntro(QWidget *page,
                 LogoWidget *logo,
                 const QList<QWidget *> &followers)
        : QObject(page)
        , m_page(page)
        , m_logo(logo)
    {
        // QPointer: Die Widgets gehören dem Layout, nicht diesem Objekt.
        for (QWidget *follower : followers)
            m_followers.append(follower);

        if (m_page)
            m_page->installEventFilter(this);
    }

    ~WelcomeIntro() override { stop(); }

    // Nur für Tests: erlaubt, das Intro erneut abzuspielen.
    static void resetForTests() { s_played = false; }

    bool isRunning() const { return m_running; }

    // Zeiten (ms) bei Zeitfaktor 1.
    static constexpr int kSymbolMs          = 1200;
    static constexpr int kSkillDelayMs      = 700;
    static constexpr int kBaseDelayMs       = 950;
    static constexpr int kWordMs            = 650;
    static constexpr int kFollowersAtMs     = 1500;
    static constexpr int kFollowerStaggerMs = 150;
    static constexpr int kFollowerMs        = 550;

    // Zeitfaktor aus SKILLBASE_INTRO_TIMESCALE (ungültig/außerhalb 0.2..5 -> 1).
    static qreal timeScale()
    {
        bool ok = false;
        const qreal value =
            qEnvironmentVariable("SKILLBASE_INTRO_TIMESCALE").toDouble(&ok);

        return (ok && value >= 0.2 && value <= 5.0) ? value : 1.0;
    }

    static int scaled(int ms) { return qRound(ms * timeScale()); }

    // Gesamtdauer für "followerCount" gestaffelte Elemente.
    static int totalMs(int followerCount)
    {
        const int count = followerCount > 0 ? followerCount : 1;

        return scaled(kFollowersAtMs
                      + (count - 1) * kFollowerStaggerMs
                      + kFollowerMs);
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == m_page && event->type() == QEvent::Show) {
            play();
        } else if (m_running
                   && (event->type() == QEvent::MouseButtonPress
                       || event->type() == QEvent::KeyPress)) {
            skip();
        }

        return false;   // Ereignisse nie verbrauchen
    }

private:
    void play()
    {
        if (s_played)
            return;

        s_played = true;

        // Bei reduzierter Bewegung bleibt alles unverändert sichtbar.
        if (Motion::reduced() || !m_logo)
            return;

        m_running = true;

        // Anfangszustand sofort setzen, bevor das erste Bild gezeichnet wird.
        m_logo->setIntroProgress(0.0, 0.0, 0.0);

        LogoWidget *logo = m_logo;

        // Sanftes Ein- und Ausgleiten (InOutSine) statt "Out": Bei "Out"
        // ist der Großteil der Bewegung nach der halben Zeit schon vorbei
        // und das Zusammengleiten kaum zu erkennen. InOutSine hält die
        // Bewegung über den größten Teil der Dauer sichtbar.
        const QEasingCurve soft(QEasingCurve::InOutSine);

        track(Motion::animateValue(
            logo, 0.0, 1.0, scaled(kSymbolMs),
            [logo](qreal v) { logo->setSymbolProgress(v); },
            soft, 0));

        track(Motion::animateValue(
            logo, 0.0, 1.0, scaled(kWordMs),
            [logo](qreal v) { logo->setSkillProgress(v); },
            soft, scaled(kSkillDelayMs)));

        track(Motion::animateValue(
            logo, 0.0, 1.0, scaled(kWordMs),
            [logo](qreal v) { logo->setBaseProgress(v); },
            soft, scaled(kBaseDelayMs)));

        int index = 0;

        for (QWidget *follower : m_followers) {
            track(Motion::fadeIn(
                follower,
                scaled(kFollowerMs),
                scaled(kFollowersAtMs + index * kFollowerStaggerMs),
                soft));
            ++index;
        }

        // Klick/Taste irgendwo in der App überspringt das Intro.
        qApp->installEventFilter(this);

        QTimer::singleShot(totalMs(m_followers.size()) + 200, this,
                           [this]() { stop(); });
    }

    void track(QAbstractAnimation *animation)
    {
        if (animation)
            m_animations.append(animation);
    }

    void skip()
    {
        const auto animations = m_animations;

        for (const QPointer<QAbstractAnimation> &animation : animations) {
            if (animation)
                Motion::skip(animation);
        }

        stop();
    }

    void stop()
    {
        if (!m_running)
            return;

        m_running = false;
        m_animations.clear();
        qApp->removeEventFilter(this);
    }

    QPointer<QWidget> m_page;
    QPointer<LogoWidget> m_logo;
    QList<QPointer<QWidget>> m_followers;
    QList<QPointer<QAbstractAnimation>> m_animations;

    bool m_running = false;

    static inline bool s_played = false;
};
