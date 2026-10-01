#ifndef ROADMAPMINITREEWIDGET_H
#define ROADMAPMINITREEWIDGET_H

#include <QWidget>
#include <QList>
#include <QSet>

#include "roadmapstep.h"

// Zeichnet einen kompakten Mini-Baum einer Roadmap in eine Card.
//
// - Zeigt den Root-Step und dessen Nachkommen in Preorder-Reihenfolge.
// - Erledigte Parents werden nicht weiter aufgelöst (Kinder unsichtbar).
// - Linien zwischen Knoten sind durchgezogen.
// - Farben je nach Tiefe.
class RoadmapMiniTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoadmapMiniTreeWidget(QWidget *parent = nullptr);

    // Setzt den Root-Step und alle Steps des Hobbys.
    // Der Widget baut daraus selbst den sichtbaren Baum.
    void setData(
        const RoadmapStep &root,
        const QList<RoadmapStep> &allSteps
        );

    // Maximale Anzahl sichtbarer Knoten (ohne "… N weitere").
    void setMaxVisibleNodes(int count);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct VisibleNode {
        QString text;
        int depth = 0;
        bool completed = false;
    };

    // Baut die flache Liste der sichtbaren Knoten (Preorder).
    void rebuildVisibleNodes();

    RoadmapStep m_root;
    QList<RoadmapStep> m_allSteps;
    QList<VisibleNode> m_visibleNodes;
    int m_hiddenCount = 0;
    int m_maxVisibleNodes = 6;

    // Zeilen-Höhe, Padding etc.
    static constexpr int RowHeight = 18;
    static constexpr int RowSpacing = 2;
    static constexpr int LeftPadding = 8;
    static constexpr int RightPadding = 8;
    static constexpr int TopPadding = 4;
    static constexpr int BottomPadding = 4;
    static constexpr int IndentPerLevel = 14;
    static constexpr int DotRadius = 3;
};

#endif // ROADMAPMINITREEWIDGET_H