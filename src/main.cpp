#include "mainwindow.h"
#include "database.h"

#include <QApplication>
#include <QIcon>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/skillbase_white.svg"));

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