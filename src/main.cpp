#include "mainwindow.h"
#include "database.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/skillbase_white.svg"));

    Database::connect();

    MainWindow w;
    w.show();

    return QApplication::exec();
}