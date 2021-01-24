#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QTWidgets/qstylefactory.h>
#include <qdebug.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    w.show();

    return a.exec();
}