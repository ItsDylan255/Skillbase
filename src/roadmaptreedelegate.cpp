#include "roadmaptreedelegate.h"

#include <QPainter>
#include <QModelIndex>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QApplication>
#include <functional>

namespace {

constexpr int LeftPadding       = 6;
constexpr int IndentPerLevel    = 20;
constexpr int ArrowWidth        = 16;
constexpr int ArrowHeight       = 16;
constexpr int ArrowRightGap     = 8;
constexpr int DotRadius         = 4;
constexpr int DotToTextGap      = 8;
constexpr int RightPadding      = 12;
constexpr int RowHeight         = 36;

// X-Position (Mittelpunkt) des Punkts für Items der Tiefe `level`.
// Diese Formel ist die EINZIGE Quelle für alle X-Positionen der Linien:
//   - Punkt eines Items der Tiefe d          → dotXForLevel(d)
//   - senkrechte Linie der Kinder von Tiefe d → dotXForLevel(d), also genau
//     unter dem Punkt des Parents.
int dotXForLevel(int rowLeft, int level)
{
    return rowLeft
           + LeftPadding
           + level * IndentPerLevel
           + ArrowWidth
           + ArrowRightGap
           + DotRadius;
}

// true, wenn nach `index` noch ein sichtbares Geschwister kommt.
// Ausgeblendete Zeilen (Filter) werden übersprungen, damit die Linie
// nicht ins Leere läuft.
bool hasVisibleSiblingAfter(const QTreeView *view, const QModelIndex &index)
{
    const QModelIndex parent = index.parent();
    const int count = index.model()->rowCount(parent);

    for (int r = index.row() + 1; r < count; ++r) {
        if (!view || !view->isRowHidden(r, parent))
            return true;
    }
    return false;
}

// true, wenn `index` mindestens ein sichtbares Kind hat.
bool hasVisibleChildren(const QTreeView *view, const QModelIndex &index)
{
    const int count = index.model()->rowCount(index);

    for (int r = 0; r < count; ++r) {
        if (!view || !view->isRowHidden(r, index))
            return true;
    }
    return false;
}

// true, wenn `index` das letzte sichtbare Item seines Top-Level-Blocks ist.
// Ein Top-Level-Block ist das Top-Level-Item plus alle sichtbaren
// Nachkommen. Das letzte sichtbare Item ist entweder das Top-Level-Item
// selbst (wenn eingeklappt oder ohne Kinder) oder rekursiv das letzte
// sichtbare Kind.
bool isLastVisibleOfTopLevelBlock(
    const QTreeView *view,
    const QModelIndex &index
    )
{
    if (!index.isValid())
        return false;

    // Das Top-Level-Item dieses Index ermitteln.
    QModelIndex top = index;

    while (top.parent().isValid())
        top = top.parent();

    // Rekursive Hilfsfunktion: liefert das letzte sichtbare Item
    // im Teilbaum von `current`.
    std::function<QModelIndex(const QModelIndex &)> lastVisible =
        [&](const QModelIndex &current) -> QModelIndex {

        const int childCount = current.model()->rowCount(current);

        if (childCount == 0)
            return current;

        // Ist das Item expandiert?
        bool expanded = false;

        if (auto *tree = qobject_cast<const QTreeWidget *>(
                current.model()->parent())) {

            if (auto *item = tree->itemFromIndex(current))
                expanded = item->isExpanded();
        }

        if (!expanded)
            return current;

        // Letztes sichtbares Kind finden.
        for (int r = childCount - 1; r >= 0; --r) {

            if (view && view->isRowHidden(r, current))
                continue;

            return lastVisible(current.model()->index(r, 0, current));
        }

        // Alle Kinder ausgeblendet → das Item selbst.
        return current;
    };

    return lastVisible(top) == index;
}

} // namespace

RoadmapTreeDelegate::RoadmapTreeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

int RoadmapTreeDelegate::depthOf(const QModelIndex &index) const
{
    int depth = 0;
    QModelIndex parent = index.parent();

    while (parent.isValid()) {
        ++depth;
        parent = parent.parent();
    }

    return depth;
}

bool RoadmapTreeDelegate::isCompleted(const QModelIndex &index) const
{
    return index.data(Qt::UserRole + 1).toBool();
}

bool RoadmapTreeDelegate::hasChildren(const QModelIndex &index) const
{
    return index.model()->rowCount(index) > 0;
}

bool RoadmapTreeDelegate::isExpanded(const QModelIndex &index) const
{
    auto *tree = qobject_cast<const QTreeWidget *>(index.model()->parent());
    if (!tree)
        return false;

    if (auto *item = tree->itemFromIndex(index))
        return item->isExpanded();

    return false;
}

QColor RoadmapTreeDelegate::colorForDepth(int depth)
{
    if (depth <= 0)
        return QColor("#ff6f61");   // Akzent

    if (depth == 1)
        return QColor("#9a9aa0");   // Secondary

    return QColor("#6d6d73");       // Muted (Ebene 2+)
}

QColor RoadmapTreeDelegate::completedColor()
{
    return QColor("#4a4a4f");
}

QColor RoadmapTreeDelegate::lineColor()
{
    return QColor("#333338");
}

QColor RoadmapTreeDelegate::textColor(bool completed)
{
    return completed
               ? completedColor()
               : QColor("#eaeaec");
}

QColor RoadmapTreeDelegate::arrowColor(bool expanded)
{
    Q_UNUSED(expanded);
    return QColor("#9a9aa0");
}

QSize RoadmapTreeDelegate::sizeHint(
    const QStyleOptionViewItem &option,
    const QModelIndex &index
    ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize(0, RowHeight);
}

void RoadmapTreeDelegate::paint(
    QPainter *painter,
    const QStyleOptionViewItem &option,
    const QModelIndex &index
    ) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    const int depth = depthOf(index);

    // Die View wird gebraucht, um ausgeblendete Zeilen (Filter) zu erkennen.
    const auto *view = qobject_cast<const QTreeView *>(option.widget);

    // ── Selektion zeichnen ────────────────────────────────────────────────
    //
    // Dezent: dünner Accent-Rahmen um die ganze Zeile.
    // Kein vollflächiger Hintergrund.

    if (option.state & QStyle::State_Selected) {

        QPen selPen(QColor("#ff6f61"));
        selPen.setWidth(1);

        painter->setPen(selPen);
        painter->setBrush(Qt::NoBrush);

        QRect selRect = option.rect.adjusted(0, 1, -1, -1);

        painter->drawRoundedRect(selRect, 4, 4);
    }

    // ── Geometrie berechnen ───────────────────────────────────────────────

    const int rowLeft = option.rect.left();
    const int top     = option.rect.top();
    const int bottom  = option.rect.bottom();     // letzte Pixelzeile (inklusive)
    const int centerY = top + option.rect.height() / 2;

    // Pfeil (expand/collapse).
    const int arrowLeft    = rowLeft + LeftPadding + depth * IndentPerLevel;
    const int arrowCenterX = arrowLeft + ArrowWidth / 2;

    // Punkt dieses Items.
    const int dotX = dotXForLevel(rowLeft, depth);

    // Text.
    const int textX = dotX + DotRadius + DotToTextGap;

    const bool itemHasChildren = hasChildren(index);
    const bool itemExpanded    = isExpanded(index);

    // ── Verbindungslinien ─────────────────────────────────────────────────
    //
    // Alle Linien liegen auf ganzen Pixeln → Antialiasing aus,
    // damit sie scharf (1 px) und ohne Unschärfe gezeichnet werden.
    // Jede Zeile füllt ihren Bereich top..bottom komplett aus, dadurch
    // schließen die Linien benachbarter Zeilen lückenlos aneinander an.

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(QPen(lineColor(), 1));

    // (1) Durchlaufende Linien der Vorfahren-Ebenen.
    //
    // Hat ein Vorfahre auf Tiefe `a` noch ein späteres Geschwister, dann
    // muss die senkrechte Linie seiner Ebene (X = Punkt des Parents auf
    // Tiefe a-1) durch diese Zeile hindurchlaufen, sonst entstehen Lücken
    // zwischen aufgeklappten Zweigen.

    {
        QModelIndex ancestor = index.parent();            // Tiefe depth-1

        for (int a = depth - 1; a >= 1; --a) {

            if (hasVisibleSiblingAfter(view, ancestor)) {
                const int x = dotXForLevel(rowLeft, a - 1);
                painter->drawLine(x, top, x, bottom);
            }

            ancestor = ancestor.parent();
        }
    }

    // (2) Linien der eigenen Ebene (nur für Items mit Parent).
    //
    // parentDotX ist die X-Position des Parent-Punkts. Dort läuft die
    // senkrechte Geschwister-Linie, und von dort geht die waagerechte
    // Linie zum eigenen Punkt.

    if (depth > 0) {

        const int parentDotX = dotXForLevel(rowLeft, depth - 1);

        // Bei Items mit Kindern liegt der Pfeil genau auf der senkrechten
        // Linie (Pfeilmitte == parentDotX). Damit die Linie nicht durch
        // das Dreieck läuft, wird dort eine Lücke gelassen.
        const int arrowTop    = centerY - ArrowHeight / 2;
        const int arrowBottom = centerY + ArrowHeight / 2;

        // Nach oben: von der Zeilenoberkante bis zur Mitte.
        // Das gilt auch für das erste Kind — so wird es mit dem Parent-
        // Punkt verbunden (der Parent zeichnet seinen Teil nach unten).
        const int upEnd = itemHasChildren ? arrowTop - 1 : centerY;
        painter->drawLine(parentDotX, top, parentDotX, upEnd);

        // Nach unten: von der Mitte bis zur Zeilenunterkante,
        // nur wenn danach noch ein Geschwister kommt.
        if (hasVisibleSiblingAfter(view, index)) {
            const int downStart = itemHasChildren ? arrowBottom + 1 : centerY;
            painter->drawLine(parentDotX, downStart, parentDotX, bottom);
        }

        // Waagerecht: vom Parent-Punkt-X bis zum linken Rand des eigenen
        // Punkts. Hat das Item einen Pfeil, beginnt die Linie rechts davon.
        const int hStart = itemHasChildren
                               ? arrowLeft + ArrowWidth
                               : parentDotX;

        painter->drawLine(hStart, centerY, dotX - DotRadius, centerY);
    }

    // (3) Linie vom eigenen Punkt nach unten zu den Kindern.
    //
    // Nur wenn das Item aufgeklappt ist und sichtbare Kinder hat.
    // X ist identisch mit parentDotX der Kinder.

    if (itemHasChildren && itemExpanded && hasVisibleChildren(view, index)) {
        painter->drawLine(dotX, centerY + DotRadius + 1, dotX, bottom);
    }

    painter->setRenderHint(QPainter::Antialiasing, true);

    // ── Expand-Pfeil zeichnen ─────────────────────────────────────────────

    if (itemHasChildren) {

        painter->setPen(Qt::NoPen);
        painter->setBrush(arrowColor(itemExpanded));

        QPolygonF triangle;

        if (itemExpanded) {
            // ▼ zeigt nach unten.
            triangle
                << QPointF(arrowLeft + 1, centerY - ArrowHeight / 3)
                << QPointF(arrowLeft + ArrowWidth - 1, centerY - ArrowHeight / 3)
                << QPointF(arrowCenterX, centerY + ArrowHeight / 3);
        } else {
            // ▶ zeigt nach rechts.
            triangle
                << QPointF(arrowLeft + 2, centerY - ArrowHeight / 2 + 1)
                << QPointF(arrowLeft + 2, centerY + ArrowHeight / 2 - 1)
                << QPointF(arrowLeft + ArrowWidth - 2, centerY);
        }

        painter->drawPolygon(triangle);
    }

    // ── Punkt zeichnen ────────────────────────────────────────────────────

    const bool completed = isCompleted(index);

    QColor dotColor = completed
                          ? completedColor()
                          : colorForDepth(depth);

    painter->setPen(Qt::NoPen);
    painter->setBrush(dotColor);
    painter->drawEllipse(
        QPoint(dotX, centerY),
        DotRadius,
        DotRadius
        );

    // ── Text zeichnen ─────────────────────────────────────────────────────

    const QString text = index.data(Qt::DisplayRole).toString();

    QFont font = option.font;
    font.setPointSize(11);
    painter->setFont(font);

    painter->setPen(textColor(completed));

    const int availableWidth =
        option.rect.right() - textX - RightPadding;

    QFontMetrics fm(font);
    const QString elided =
        fm.elidedText(text, Qt::ElideRight, availableWidth);

    painter->drawText(
        QRect(textX, option.rect.top(), availableWidth, option.rect.height()),
        Qt::AlignLeft | Qt::AlignVCenter,
        elided
        );

    painter->restore();

    // ── Trennlinie unter dem letzten sichtbaren Item eines Blocks ────────
    //
    // Wird live berechnet, damit die Linie auch bei aktivem Suchfilter
    // korrekt sitzt. Antialiasing bleibt an — die Linie liegt auf einem
    // ganzen Pixel, ist also genauso scharf wie die restlichen Elemente.

    if (isLastVisibleOfTopLevelBlock(view, index)) {

        painter->setPen(QPen(QColor("#29292d"), 1));

        const int lineY = option.rect.bottom() - 1;

        painter->drawLine(
            option.rect.left(),
            lineY,
            option.rect.right(),
            lineY
            );
    }
}
