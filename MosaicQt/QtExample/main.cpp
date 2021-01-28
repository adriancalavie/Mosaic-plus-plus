#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QTWidgets/qstylefactory.h>
#include <qdebug.h>


int main(int argc, char *argv[])
{

//#ifdef _WIN64
//    if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
//        freopen("CONOUT$", "w", stdout);
//        freopen("CONOUT$", "w", stderr);
//    }
//#endif

    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    w.show();

    return a.exec();
}