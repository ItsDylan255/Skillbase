#include "clickablelabel.h"

#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget *parent)
    : QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
}

ClickableLabel::ClickableLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    setCursor(Qt::PointingHandCursor);
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        // Nur auslösen, wenn der Klick innerhalb des Labels stattfindet.
        // (Sonst würde auch ein Klick, der außerhalb startet und
        // innerhalb endet, ein Signal auslösen.)
        if (rect().contains(event->pos())) {
            emit clicked();
        }
    }

    QLabel::mouseReleaseEvent(event);
}