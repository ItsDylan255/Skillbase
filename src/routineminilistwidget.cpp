#include "routineminilistwidget.h"

#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <algorithm>

namespace {

// Farben identisch zu roadmapminitreewidget.cpp.
QColor dotColor()    { return QColor("#9a9aa0"); }   // Secondary
QColor lineColor()   { return QColor("#333338"); }
QColor textColor()   { return QColor("#eaeaec"); }
QColor rightColor()  { return QColor("#9a9aa0"); }
QColor mutedColor()  { return QColor("#6d6d73"); }

} // namespace

RoutineMiniListWidget::RoutineMiniListWidget(QWidget *parent)
    : QWidget(parent)
{
    // Wie der Mini-Baum: feste Höhe, volle Breite.
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void RoutineMiniListWidget::setItems(const QList<Item> &items, int hiddenCount)
{
    m_items = items;
    m_hiddenCount = std::max(0, hiddenCount);

    updateGeometry();
    update();
}

QSize RoutineMiniListWidget::sizeHint() const
{
    const int rows = m_items.size();
    const int extraRow = (m_hiddenCount > 0) ? 1 : 0;

    const int height =
        TopPadding
        + BottomPadding
        + rows * RowHeight
        + std::max(0, rows - 1) * RowSpacing
        + extraRow * (RowHeight + RowSpacing);

    return QSize(200, height);
}

QSize RoutineMiniListWidget::minimumSizeHint() const
{
    // Breite flexibel lassen, Höhe fest.
    return QSize(0, sizeHint().height());
}

void RoutineMiniListWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    const QFontMetrics fm(font);

    const int dotX  = LeftPadding + DotRadius;
    const int textX = dotX + DotRadius + DotToTextGap;

    const int rows = m_items.size();

    auto centerYOf = [](int row) {
        return TopPadding + row * (RowHeight + RowSpacing) + RowHeight / 2;
    };

    // ── Linien (scharf, 1 px, ohne Antialiasing) ─────────────────────────
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(lineColor(), 1));

    // Senkrechte durch alle Punkte: vom ersten bis zum letzten Mittelpunkt.
    if (rows > 1)
        painter.drawLine(dotX, centerYOf(0), dotX, centerYOf(rows - 1));

    // Waagerechte vom Punkt zum Text.
    for (int i = 0; i < rows; ++i) {
        const int cy = centerYOf(i);
        painter.drawLine(dotX + DotRadius, cy, textX - 4, cy);
    }

    // ── Punkte, Texte ────────────────────────────────────────────────────
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < rows; ++i) {

        const Item &item = m_items.at(i);
        const int rowTop = TopPadding + i * (RowHeight + RowSpacing);
        const int cy = rowTop + RowHeight / 2;

        painter.setPen(Qt::NoPen);
        painter.setBrush(dotColor());
        painter.drawEllipse(QPoint(dotX, cy), DotRadius, DotRadius);

        // Rechter Zusatztext (z. B. Dauer).
        int rightWidth = 0;

        if (!item.rightText.isEmpty()) {
            rightWidth = fm.horizontalAdvance(item.rightText);

            painter.setPen(rightColor());
            painter.drawText(
                QRect(width() - RightPadding - rightWidth, rowTop,
                      rightWidth, RowHeight),
                Qt::AlignRight | Qt::AlignVCenter,
                item.rightText
                );
        }

        const int gap = (rightWidth > 0) ? 8 : 0;
        const int availableWidth =
            std::max(0, width() - textX - RightPadding - rightWidth - gap);

        painter.setPen(textColor());
        painter.drawText(
            QRect(textX, rowTop, availableWidth, RowHeight),
            Qt::AlignLeft | Qt::AlignVCenter,
            fm.elidedText(item.text, Qt::ElideRight, availableWidth)
            );
    }

    // ── "+ N weitere" ────────────────────────────────────────────────────
    if (m_hiddenCount > 0) {

        const int rowTop = TopPadding + rows * (RowHeight + RowSpacing);

        painter.setPen(mutedColor());
        painter.drawText(
            QRect(LeftPadding, rowTop,
                  width() - LeftPadding - RightPadding, RowHeight),
            Qt::AlignLeft | Qt::AlignVCenter,
            QString("+ %1 weitere").arg(m_hiddenCount)
            );
    }
}
