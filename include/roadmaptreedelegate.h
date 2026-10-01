#ifndef ROADMAPTREEDELEGATE_H
#define ROADMAPTREEDELEGATE_H

#include <QStyledItemDelegate>

// Zeichnet Roadmap-Steps im RoadmapTreeWidget:
//   ▶/▼  ●  ──  Text
//
// Der Delegate kennt die Tiefe des Items und zeichnet:
//   - einen Expand-Pfeil, wenn das Item Kinder hat
//   - einen farbigen Punkt (Farbe je Tiefe)
//   - eine waagerechte Linie vom Punkt zum Text
//   - den Namen (ausgegraut, wenn erledigt)
class RoadmapTreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit RoadmapTreeDelegate(QObject *parent = nullptr);

    void paint(
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index
        ) const override;

    QSize sizeHint(
        const QStyleOptionViewItem &option,
        const QModelIndex &index
        ) const override;

private:
    // Liefert die Tiefe des Items (0 = direktes Kind des Roots).
    int depthOf(const QModelIndex &index) const;

    // Liefert true, wenn das Item erledigt ist (Property "completed").
    bool isCompleted(const QModelIndex &index) const;

    // Liefert true, wenn das Item Kinder hat.
    bool hasChildren(const QModelIndex &index) const;

    // Liefert true, wenn das Item expandiert ist.
    bool isExpanded(const QModelIndex &index) const;

    // Farben je Tiefe.
    static QColor colorForDepth(int depth);

    // Farbe für erledigte Steps.
    static QColor completedColor();

    // Farbe für Linien.
    static QColor lineColor();

    // Farbe für Text.
    static QColor textColor(bool completed);

    // Farben für Pfeile und Punkt.
    static QColor arrowColor(bool expanded);
};

#endif // ROADMAPTREEDELEGATE_H