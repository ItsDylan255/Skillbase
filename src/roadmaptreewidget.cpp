#include "roadmaptreewidget.h"

#include <QPainter>
#include <QModelIndex>
#include <QMouseEvent>

namespace {

// Diese Werte MÜSSEN mit roadmaptreedelegate.cpp übereinstimmen,
// weil der Klick-Bereich des Pfeils aus derselben Geometrie stammt.
constexpr int LeftPadding    = 6;
constexpr int IndentPerLevel = 20;
constexpr int ArrowWidth     = 16;

} // namespace

RoadmapTreeWidget::RoadmapTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    // rootIsDecorated bleibt true, indentation ist 0:
    // Qt reserviert dadurch keinen eigenen Platz für Pfeile/Linien.
    // Pfeile, Punkte und ALLE Linien zeichnet der RoadmapTreeDelegate.
    setRootIsDecorated(true);

    setIndentation(0);
    setUniformRowHeights(true);
    setAllColumnsShowFocus(false);

    // Kompakt ohne Rahmen um die Items — der Delegate zeichnet selbst.
    setFrameShape(QFrame::NoFrame);
}

QSize RoadmapTreeWidget::sizeHintForItem(const QModelIndex &index) const
{
    Q_UNUSED(index);

    // Die tatsächliche Zeilenhöhe kommt aus RoadmapTreeDelegate::sizeHint().
    return QSize(0, 28);
}

void RoadmapTreeWidget::drawBranches(
    QPainter *painter,
    const QRect &rect,
    const QModelIndex &index
    ) const
{
    // Bewusst leer: Der Delegate zeichnet alle Linien selbst.
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(index);
}

void RoadmapTreeWidget::mousePressEvent(QMouseEvent *event)
{
    // Klick auf den Expand-Pfeil → Step auf-/zuklappen.
    // Alles andere (Klick auf Name/Punkt/Zeile) geht an die Basisklasse
    // und selektiert normal.

    if (event->button() == Qt::LeftButton) {

        const QPoint pos = event->position().toPoint();
        const QModelIndex index = indexAt(pos);

        if (index.isValid() && model()->rowCount(index) > 0) {

            // Tiefe ermitteln.
            int depth = 0;
            QModelIndex parent = index.parent();
            while (parent.isValid()) {
                ++depth;
                parent = parent.parent();
            }

            // Gleiche Geometrie wie im Delegate:
            //   arrowLeft = Zeilenrand + LeftPadding + depth * IndentPerLevel
            const int lineLeft  = visualRect(index).left();
            const int arrowLeft = lineLeft + LeftPadding + depth * IndentPerLevel;
            const int arrowRight = arrowLeft + ArrowWidth;

            if (pos.x() >= arrowLeft && pos.x() <= arrowRight) {

                // setExpanded löst itemExpanded/itemCollapsed aus
                // (Auto-Einklappen der Nachkommen in mainwindow.cpp).
                setExpanded(index, !isExpanded(index));

                event->accept();
                return;
            }
        }
    }

    QTreeWidget::mousePressEvent(event);
}
