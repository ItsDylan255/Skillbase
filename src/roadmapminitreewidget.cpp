#include "roadmapminitreewidget.h"
#include "roadmaptreewidget.h"
#include "roadmaptreedelegate.h"
#include <QPainter>
#include <QFontMetrics>
#include <algorithm>
#include <functional>

namespace {

// Farben für die Tiefen (Ebene 0 = Akzent, dann immer dunkler).
QColor colorForDepth(int depth)
{
    if (depth <= 0)
        return QColor("#ff6f61");   // Akzent rot

    if (depth == 1)
        return QColor("#9a9aa0");   // Secondary

    return QColor("#6d6d73");       // Muted (Ebene 2+)
}

// Farbe für erledigte Steps.
QColor completedColor()
{
    return QColor("#4a4a4f");
}

// Farbe für Linien.
QColor lineColor()
{
    return QColor("#333338");
}

} // namespace

RoadmapMiniTreeWidget::RoadmapMiniTreeWidget(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void RoadmapMiniTreeWidget::setData(
    const RoadmapStep &root,
    const QList<RoadmapStep> &allSteps
    )
{
    m_root = root;
    m_allSteps = allSteps;

    rebuildVisibleNodes();
    updateGeometry();
    update();
}

void RoadmapMiniTreeWidget::setMaxVisibleNodes(int count)
{
    if (count < 1)
        count = 1;

    m_maxVisibleNodes = count;
    rebuildVisibleNodes();
    updateGeometry();
    update();
}

void RoadmapMiniTreeWidget::rebuildVisibleNodes()
{
    m_visibleNodes.clear();
    m_hiddenCount = 0;

    // ── Schritt 1: Alle offenen Knoten in Preorder sammeln ───────────────
    //
    // Wir gehen rekursiv durch den Baum und sammeln NUR offene Steps.
    // Erledigte Steps werden an dieser Stelle nicht gesammelt.
    // Erledigte Parents werden nicht weiter aufgelöst (Kinder bleiben
    // unsichtbar).

    QList<VisibleNode> openNodes;

    std::function<void(int, int)> traverseOpen =
        [&](int parentId, int depth) {

            // Kinder sammeln.
            QList<RoadmapStep> children;

            for (const RoadmapStep &s : m_allSteps) {
                if (s.parentId == parentId)
                    children.append(s);
            }

            // Sortieren: innerhalb der Kinder die offenen zuerst.
            std::stable_sort(
                children.begin(),
                children.end(),
                [](const RoadmapStep &a, const RoadmapStep &b) {
                    return !a.completed && b.completed;
                }
                );

            for (const RoadmapStep &child : children) {

                if (child.completed)
                    continue;   // erledigte kommen später in Schritt 2

                VisibleNode node;
                node.text = child.name;
                node.depth = depth;
                node.completed = false;
                openNodes.append(node);

                traverseOpen(child.id, depth + 1);
            }
        };

    // Root zählt als offener Knoten (kommt immer zuerst).
    {
        VisibleNode rootNode;
        rootNode.text = m_root.name;
        rootNode.depth = 0;
        rootNode.completed = m_root.completed;
        openNodes.append(rootNode);
    }

    if (!m_root.completed) {
        traverseOpen(m_root.id, 1);
    }

    // ── Schritt 2: Erledigte Knoten sammeln (falls Platz übrig) ──────────

    QList<VisibleNode> completedNodes;

    // Nur wenn die offenen Knoten weniger als das Maximum belegen,
    // sammeln wir überhaupt erledigte.
    if (openNodes.size() < m_maxVisibleNodes) {

        // Wir traversieren den Baum erneut und sammeln diesmal NUR
        // erledigte Steps, aber nur solche, deren Parent-Kette bis
        // zum Root durchgehend offen war (damit erledigte Parents
        // ihre Kinder weiterhin verbergen).
        //
        // Regel: Wenn ein Parent erledigt ist, wird er selbst gesammelt,
        // aber seine Kinder nicht.

        std::function<void(int, int)> traverseCompleted =
            [&](int parentId, int depth) {

                QList<RoadmapStep> children;

                for (const RoadmapStep &s : m_allSteps) {
                    if (s.parentId == parentId)
                        children.append(s);
                }

                std::stable_sort(
                    children.begin(),
                    children.end(),
                    [](const RoadmapStep &a, const RoadmapStep &b) {
                        return !a.completed && b.completed;
                    }
                    );

                for (const RoadmapStep &child : children) {

                    if (!child.completed)
                        continue;   // offene sind schon in Schritt 1

                    // Platz prüfen.
                    if (openNodes.size() + completedNodes.size()
                        >= m_maxVisibleNodes) {
                        break;
                    }

                    VisibleNode node;
                    node.text = child.name;
                    node.depth = depth;
                    node.completed = true;
                    completedNodes.append(node);

                    // Erledigte Parents → keine Kinder anzeigen.
                }
            };

        if (!m_root.completed) {
            traverseCompleted(m_root.id, 1);
        }
    }

    // ── Schritt 3: Kombinieren + versteckte zählen ──────────────────────

    // Erst offene, dann erledigte (bis Maximum).
    for (const VisibleNode &n : openNodes) {
        if (m_visibleNodes.size() >= m_maxVisibleNodes)
            break;
        m_visibleNodes.append(n);
    }

    // Wenn nach den offenen noch Platz ist, erledigte einfügen.
    for (const VisibleNode &n : completedNodes) {
        if (m_visibleNodes.size() >= m_maxVisibleNodes)
            break;
        m_visibleNodes.append(n);
    }

    // Versteckte zählen:
    // - Alle offenen, die nicht mehr reinpassten
    // - Alle erledigten, die nicht mehr reinpassten
    // - Alle, die gar nicht gesammelt wurden, weil bereits voll

    const int totalOpen = openNodes.size();
    const int totalCompleted = completedNodes.size();

    const int shownOpen = std::min(totalOpen, m_maxVisibleNodes);
    const int shownCompleted = std::min(
        totalCompleted,
        m_maxVisibleNodes - shownOpen
        );

    // Alle erledigten, die nicht gezeigt wurden, zählen wir als versteckt.
    // Plus alle offenen, die nicht gezeigt wurden.
    m_hiddenCount =
        (totalOpen - shownOpen)
        + (totalCompleted - shownCompleted);
}

QSize RoadmapMiniTreeWidget::sizeHint() const
{
    // Höhe: jede sichtbare Zeile + "… N weitere" Zeile wenn nötig.
    const int visibleRows = m_visibleNodes.size();
    const int extraRow = (m_hiddenCount > 0) ? 1 : 0;

    const int height =
        TopPadding
        + BottomPadding
        + visibleRows * RowHeight
        + std::max(0, visibleRows - 1) * RowSpacing
        + extraRow * (RowHeight + RowSpacing);

    return QSize(200, height);
}

QSize RoadmapMiniTreeWidget::minimumSizeHint() const
{
    return sizeHint();
}

void RoadmapMiniTreeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    // Schrift.
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    const QFontMetrics fm(font);

    // ── Vorberechnung: Y-Position und X-Position jeder Zeile ─────────────
    //
    // Wir speichern für jede sichtbare Zeile:
    //   - die Y-Position (Mitte der Zeile)
    //   - die X-Position des Punkts
    //   - die Tiefe
    //
    // Damit können wir die Linien in einem separaten Durchlauf
    // sauber und durchgezogen zeichnen.

    struct RowInfo {
        int depth = 0;
        int dotX = 0;
        int centerY = 0;
    };

    QList<RowInfo> rows;
    rows.reserve(m_visibleNodes.size());

    int y = TopPadding;

    for (const VisibleNode &node : m_visibleNodes) {

        RowInfo info;
        info.depth = node.depth;
        info.dotX = LeftPadding + node.depth * IndentPerLevel + DotRadius;
        info.centerY = y + RowHeight / 2;

        rows.append(info);

        y += RowHeight + RowSpacing;
    }

    // ── Schritt 1: Alle Linien zeichnen ──────────────────────────────────
    //
    // Wir gehen durch alle Zeilen und zeichnen für jede Zeile mit
    // depth > 0 die Verbindung zum Parent.

    painter.setPen(QPen(lineColor(), 1));

    for (int i = 0; i < rows.size(); ++i) {

        const RowInfo &row = rows.at(i);

        if (row.depth <= 0)
            continue;

        // ── Parent finden ────────────────────────────────────────────────
        //
        // Der Parent ist die letzte Zeile über uns mit depth == row.depth - 1.

        int parentIndex = -1;

        for (int j = i - 1; j >= 0; --j) {
            if (rows.at(j).depth == row.depth - 1) {
                parentIndex = j;
                break;
            }
        }

        if (parentIndex < 0)
            continue;

        const RowInfo &parent = rows.at(parentIndex);

        const int parentX = parent.dotX;
        const int parentY = parent.centerY;

        // ── Vertikale Linie vom Parent-Punkt nach unten ──────────────────
        //
        // Sie läuft vom Parent-Y bis zum aktuellen Zeilen-Y.
        // Damit ergibt sich eine durchgezogene Linie vom Parent zu allen
        // seinen Kindern.

        painter.drawLine(
            parentX,
            parentY,
            parentX,
            row.centerY
            );

        // ── Horizontale Linie vom Parent-Punkt zum Kind-Punkt ────────────
        //
        // Sie läuft vom Parent-Punkt bis kurz vor den linken Rand des
        // Kind-Punkts, damit der Punkt die Linie sauber abdeckt.

        painter.drawLine(
            parentX,
            row.centerY,
            row.dotX - DotRadius,
            row.centerY
            );

        // ── Vertikale Linie vom Parent nach unten, wenn noch Geschwister ─
        //
        // Wenn es unter dem aktuellen Knoten noch einen Geschwister-Knoten
        // auf derselben Ebene gibt, verlängern wir die Linie bis zum
        // nächsten Geschwister.

        bool hasSiblingBelow = false;

        for (int j = i + 1; j < rows.size(); ++j) {

            if (rows.at(j).depth < row.depth)
                break;

            if (rows.at(j).depth == row.depth) {
                hasSiblingBelow = true;
                break;
            }
        }

        if (hasSiblingBelow) {

            const int nextRowY = row.centerY + RowHeight + RowSpacing;

            painter.drawLine(
                parentX,
                parentY,
                parentX,
                nextRowY
                );
        }
    }

    // ── Schritt 2: Punkte und Texte zeichnen ─────────────────────────────

    y = TopPadding;

    for (int i = 0; i < m_visibleNodes.size(); ++i) {

        const VisibleNode &node = m_visibleNodes.at(i);
        const RowInfo &row = rows.at(i);

        const int dotX = row.dotX;
        const int centerY = row.centerY;
        const int textX = dotX + DotRadius + 6;

        // ── Punkt zeichnen ────────────────────────────────────────────────

        QColor dotColor = node.completed
                              ? completedColor()
                              : colorForDepth(node.depth);

        painter.setBrush(dotColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(
            QPoint(dotX, centerY),
            DotRadius,
            DotRadius
            );

        // ── Text zeichnen ─────────────────────────────────────────────────

        QColor textColor = node.completed
                               ? completedColor()
                               : QColor("#eaeaec");

        painter.setPen(textColor);

        const int availableWidth = width() - textX - RightPadding;
        const QString elided = fm.elidedText(
            node.text,
            Qt::ElideRight,
            availableWidth
            );

        painter.drawText(
            QRect(textX, y, availableWidth, RowHeight),
            Qt::AlignLeft | Qt::AlignVCenter,
            elided
            );

        y += RowHeight + RowSpacing;
    }

    // ── Schritt 3: "... N weitere" ───────────────────────────────────────

    if (m_hiddenCount > 0) {

        painter.setPen(QColor("#6d6d73"));

        painter.drawText(
            QRect(LeftPadding, y, width() - LeftPadding - RightPadding, RowHeight),
            Qt::AlignLeft | Qt::AlignVCenter,
            QString("... %1 weitere").arg(m_hiddenCount)
            );
    }
}