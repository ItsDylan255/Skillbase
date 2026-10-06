#pragma once

// Gestapeltes Skillbase-Logo (Symbol über Schriftzug) für den Willkommens-
// Bildschirm. Gezeichnet aus den beiden SVGs in den Ressourcen:
//
//   :/icons/skillbase_symbol.svg    Elemente "s-left", "s-right"
//   :/icons/skillbase_wordmark.svg  Gruppen  "word-skill", "word-base"
//
// Jedes der vier Teile hat einen eigenen Fortschritt (0..1), damit das
// Logo beim Start nacheinander aufgebaut werden kann (siehe WelcomeIntro):
//   - Symbol: die beiden S-Hälften gleiten entlang des diagonalen Schnitts
//             zusammen und blenden ein.
//   - Schriftzug: "skill", dann "base" blenden ein.
// Im Normalzustand (und für den Designer) ist alles vollständig sichtbar.
//
// Header-only und ohne Q_OBJECT: Es muss nichts in CMake/qmake eingetragen
// werden. In mainwindow.ui als "Promoted Widget" (Header: logowidget.h).
// Benötigt das Qt-Modul Svg.

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QSize>
#include <QString>
#include <QSvgRenderer>
#include <QTransform>
#include <QWidget>

#include <QtMath>

class LogoWidget : public QWidget
{
public:
    explicit LogoWidget(QWidget *parent = nullptr)
        : QWidget(parent)
        , m_symbol(QStringLiteral(":/icons/skillbase_symbol.svg"))
        , m_word(QStringLiteral(":/icons/skillbase_wordmark.svg"))
    {
        setAccessibleName(QStringLiteral("Skillbase"));

        if (m_symbol.isValid()) {
            m_symbolLeft  = boundsOf(m_symbol, QStringLiteral("s-left"));
            m_symbolRight = boundsOf(m_symbol, QStringLiteral("s-right"));
        }

        if (m_word.isValid()) {
            m_skill = boundsOf(m_word, QStringLiteral("word-skill"));
            m_base  = boundsOf(m_word, QStringLiteral("word-base"));
        }
    }

    // ── Fortschritt (0 = unsichtbar, 1 = fertig) ────────────────────────
    void setSymbolProgress(qreal progress) { m_symbolProgress = clamp01(progress); update(); }
    void setSkillProgress(qreal progress)  { m_skillProgress  = clamp01(progress); update(); }
    void setBaseProgress(qreal progress)   { m_baseProgress   = clamp01(progress); update(); }

    void setIntroProgress(qreal symbol, qreal skill, qreal base)
    {
        m_symbolProgress = clamp01(symbol);
        m_skillProgress  = clamp01(skill);
        m_baseProgress   = clamp01(base);
        update();
    }

    qreal symbolProgress() const { return m_symbolProgress; }
    qreal skillProgress() const  { return m_skillProgress; }
    qreal baseProgress() const   { return m_baseProgress; }

    // Seitenverhältnis der gesamten Komposition.
    static constexpr qreal kCompositionWidth  = 600.0;   // Breite des Schriftzugs
    static constexpr qreal kSymbolSize        = 271.0;   // Kantenlänge des Symbols
    static constexpr qreal kGap               = 96.0;    // Abstand Symbol -> Schriftzug
    static constexpr qreal kWordHeight        = 100.0;   // Höhe des Schriftzugs
    static constexpr qreal kCompositionHeight = kSymbolSize + kGap + kWordHeight;

    QSize sizeHint() const override { return QSize(280, 218); }
    QSize minimumSizeHint() const override { return QSize(140, 109); }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        if (!m_symbol.isValid() || !m_word.isValid()) {
            // Fallback, falls die Ressourcen fehlen: schlichter Schriftzug.
            painter.setPen(palette().windowText().color());
            painter.drawText(rect(), Qt::AlignCenter, QStringLiteral("skillbase"));
            return;
        }

        const qreal scale = qMin(width() / kCompositionWidth,
                                 height() / kCompositionHeight);

        const qreal offsetX = (width()  - kCompositionWidth  * scale) / 2.0;
        const qreal offsetY = (height() - kCompositionHeight * scale) / 2.0;

        // ── Symbol ──────────────────────────────────────────────────────
        // Maßstab: Symbol-Einheiten -> Pixel.
        const qreal symbolScale = kSymbolSize / m_symbol.viewBoxF().width() * scale;
        const QPointF symbolOrigin(
            offsetX + (kCompositionWidth - kSymbolSize) / 2.0 * scale,
            offsetY);

        // Die Hälften gleiten entlang des Schnitts auseinander bzw.
        // zusammen: Richtung der Schnittkante laut SVG-Pfad,
        // von (67.455|0) nach (59.205|22), normiert.
        const QPointF cutDirection(-0.3510, 0.9364);
        // Weg: 24 von 102 Symbol-Einheiten (knapp ein Viertel der Höhe).
        const qreal travel = 24.0 * symbolScale * (1.0 - m_symbolProgress);

        // Die Deckkraft folgt dem Fortschritt fast linear und erreicht 100 %
        // erst kurz vor dem Ende. So ist das Gleiten sichtbar, das S wird
        // nicht schon fertig eingeblendet und rutscht nur noch nach.
        const qreal symbolOpacity = clamp01(m_symbolProgress * 1.15);

        drawPart(painter, m_symbol, QStringLiteral("s-left"), m_symbolLeft,
                 symbolOrigin + cutDirection * travel, symbolScale, symbolOpacity);
        drawPart(painter, m_symbol, QStringLiteral("s-right"), m_symbolRight,
                 symbolOrigin - cutDirection * travel, symbolScale, symbolOpacity);

        // ── Schriftzug ──────────────────────────────────────────────────
        const qreal wordScale = kCompositionWidth / m_word.viewBoxF().width() * scale;
        const QPointF wordOrigin(offsetX,
                                 offsetY + (kSymbolSize + kGap) * scale);

        // Aufsteigen (8 Einheiten) beim Einblenden.
        const qreal skillRise = 8.0 * scale * (1.0 - m_skillProgress);
        const qreal baseRise  = 8.0 * scale * (1.0 - m_baseProgress);

        drawPart(painter, m_word, QStringLiteral("word-skill"), m_skill,
                 wordOrigin + QPointF(0, skillRise), wordScale, m_skillProgress);
        drawPart(painter, m_word, QStringLiteral("word-base"), m_base,
                 wordOrigin + QPointF(0, baseRise), wordScale, m_baseProgress);
    }

private:
    static qreal clamp01(qreal value) { return qBound<qreal>(0.0, value, 1.0); }

    // Begrenzungsrahmen eines Elements im Dokument-Koordinatensystem.
    static QRectF boundsOf(QSvgRenderer &renderer, const QString &id)
    {
        return renderer.transformForElement(id).mapRect(renderer.boundsOnElement(id));
    }

    // Zeichnet ein Element so, dass es genau an seiner Dokumentposition
    // (verschoben um "origin") liegt.
    static void drawPart(QPainter &painter,
                         QSvgRenderer &renderer,
                         const QString &id,
                         const QRectF &documentBounds,
                         const QPointF &origin,
                         qreal scale,
                         qreal opacity)
    {
        if (opacity <= 0.0 || documentBounds.isEmpty())
            return;

        painter.save();
        painter.setOpacity(opacity);

        const QRectF target(origin + documentBounds.topLeft() * scale,
                            documentBounds.size() * scale);

        renderer.render(&painter, id, target);
        painter.restore();
    }

    QSvgRenderer m_symbol;
    QSvgRenderer m_word;

    QRectF m_symbolLeft;
    QRectF m_symbolRight;
    QRectF m_skill;
    QRectF m_base;

    qreal m_symbolProgress = 1.0;
    qreal m_skillProgress  = 1.0;
    qreal m_baseProgress   = 1.0;
};
