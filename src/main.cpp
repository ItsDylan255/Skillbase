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
        a.setStyleSheet(QString::fromUtf8(themeFile.readAll()));
    }

    Database::connect();

    MainWindow w;
    w.show();

    return QApplication::exec();
}