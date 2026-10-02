#pragma once

// Gemeinsame Hilfsfunktionen für dynamisch erzeugte Cards (load*Cards).
//
// Bewusst header-only und ohne Q_OBJECT: Es muss nichts in CMake/qmake
// eingetragen werden. Die Funktionen bilden den bisherigen Code 1:1 nach
// (gleiche Eigenschaften, gleiche Reihenfolge) - es ändert sich kein Aussehen.

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLayoutItem>
#include <QSizePolicy>
#include <QString>
#include <QVBoxLayout>
#include <Qt>

namespace CardUi {

// Entfernt alle bisher erzeugten Cards aus einem Layout. Die Layout-Struktur
// selbst bleibt bestehen und kann danach wieder neu befüllt werden.
inline void clearLayout(QLayout *layout)
{
    if (!layout)
        return;

    while (layout->count() > 0) {
        QLayoutItem *item = layout->takeAt(0);

        if (item->widget())
            item->widget()->deleteLater();

        delete item;
    }
}

// Standard-Card: StyledPanel-Frame mit fester Höhe, horizontal expandierend,
// Hand-Cursor. Das Aussehen kommt ausschließlich aus theme.qss (objectName).
inline QFrame *makeCard(QWidget *parent,
                        const QString &objectName,
                        int fixedHeight)
{
    auto *card = new QFrame(parent);

    card->setObjectName(objectName);
    card->setFrameShape(QFrame::StyledPanel);
    card->setFixedHeight(fixedHeight);
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    card->setCursor(Qt::PointingHandCursor);

    return card;
}

// Vertikales Layout einer Card mit explizit übergebenen Abständen.
inline QVBoxLayout *makeCardLayout(QFrame *card,
                                   int spacing,
                                   int left, int top, int right, int bottom)
{
    auto *layout = new QVBoxLayout(card);

    layout->setSpacing(spacing);
    layout->setContentsMargins(left, top, right, bottom);

    return layout;
}

// Zeile "Label ........ Wert" (Label links, Wert rechts).
inline QHBoxLayout *makeMetaRow(QWidget *parent,
                                const QString &label,
                                const QString &value)
{
    auto *row = new QHBoxLayout();

    row->addWidget(new QLabel(label, parent));
    row->addStretch();
    row->addWidget(new QLabel(value, parent));

    return row;
}

} // namespace CardUi
