#include "mainwindow.h"
#include "database.h"

#include <QApplication>
#include <QEvent>
#include <QFile>
#include <QIcon>
#include <QMouseEvent>
#include <QWidget>

namespace {

// Globaler Helfer: Wenn der Nutzer auf eine Fläche klickt, die selbst
// nicht fokussierbar ist (Labels, Frames, leere Bereiche), nimmt der
// Filter dem aktuell fokussierten Widget den Fokus. Dadurch
// verschwinden blinkender Cursor und Textselektion in Eingabefeldern,
// sobald der Nutzer irgendwo anders hinklickt.
class FocusClearFilter : public QObject
{
public:
    using QObject::QObject;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() != QEvent::MouseButtonPress) {
            return QObject::eventFilter(watched, event);
        }

        auto *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() != Qt::LeftButton) {
            return QObject::eventFilter(watched, event);
        }

        auto *clickedWidget = qobject_cast<QWidget *>(watched);

        // Ohne Widget (z. B. bei einem Popup-Kind) nichts tun.
        if (!clickedWidget) {
            return QObject::eventFilter(watched, event);
        }

        // Popups (ComboBox-Dropdown, Menüs, Tooltips) gehören zur
        // aktuellen Interaktion. Ihnen darf der Fokus nicht entzogen werden.
        if (clickedWidget->windowFlags() & Qt::Popup) {
            return QObject::eventFilter(watched, event);
        }

        // Wenn das geklickte Widget selbst den Fokus annehmen kann,
        // erledigt Qt das von allein. Wir tun nichts.
        const Qt::FocusPolicy policy = clickedWidget->focusPolicy();

        if (policy & Qt::ClickFocus) {
            return QObject::eventFilter(watched, event);
        }

        // Sonst: dem aktuell fokussierten Widget den Fokus nehmen.
        // clearFocus() sorgt dafür, dass Cursor und Textauswahl verschwinden.
        if (QWidget *focusWidget = QApplication::focusWidget()) {
            focusWidget->clearFocus();
        }

        return QObject::eventFilter(watched, event);
    }
};

} // namespace

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Skillbase");
    QApplication::setApplicationVersion("1.1.0");
    QApplication::setOrganizationName("Skillbase");
    QApplication::setOrganizationDomain("skillbase.local");

    a.setWindowIcon(QIcon(":/icons/skillbase_icon_256.png"));

    // Globaler Fokus-Filter: Klick auf nicht-fokussierbare Flächen
    // nimmt dem Eingabefeld den Fokus.
    a.installEventFilter(new FocusClearFilter(&a));

    QFile themeFile(":/theme.qss");

    if (themeFile.open(QFile::ReadOnly | QFile::Text)) {
        QByteArray css = themeFile.readAll();
        qDebug() << "theme.qss geladen, Bytes:" << css.size();
        a.setStyleSheet(QString::fromUtf8(css));
    } else {
        qWarning() << "theme.qss konnte NICHT aus den Ressourcen geöffnet werden!";
    }

    Database::connect();

    MainWindow w;
    w.show();

    return QApplication::exec();
}