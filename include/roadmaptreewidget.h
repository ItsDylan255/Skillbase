#ifndef ROADMAPTREEWIDGET_H
#define ROADMAPTREEWIDGET_H

#include <QTreeWidget>
#include <QMouseEvent>

// Ein QTreeWidget, das die Standard-Einrückung (Pfeile, gestrichelte
// Linien) überschreibt. Stattdessen werden durchgezogene vertikale
// Linien gezeichnet, die die Hierarchie verbinden.
//
// Die Punkte und die waagerechten Linien werden vom
// RoadmapTreeDelegate gezeichnet (siehe roadmaptreedelegate.h).
class RoadmapTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit RoadmapTreeWidget(QWidget *parent = nullptr);

    // Standard-Zeilenhöhe für alle Items.
    QSize sizeHintForItem(const QModelIndex &index) const;

protected:
    void drawBranches(
        QPainter *painter,
        const QRect &rect,
        const QModelIndex &index
        ) const override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // ROADMAPTREEWIDGET_H