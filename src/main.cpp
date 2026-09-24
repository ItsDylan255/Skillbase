#include "mainwindow.h"
#include "database.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database::connect();

    MainWindow w;
    w.show();

    return QApplication::exec();
}