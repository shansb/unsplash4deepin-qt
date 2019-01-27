#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    a.connect( &w, SIGNAL( exit() ), &a, SLOT( quit() ) );
    return a.exec();
}
