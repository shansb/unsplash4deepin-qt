#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QLocale locale;
    if( locale.language() == QLocale::Chinese )  //获取系统语言环境
    {
        translator.load(QString(":/languages/zh.qm"));  //选择翻译文件
        a.installTranslator(&translator);
    }
//    a.connect( &w, SIGNAL( exit() ), &a, SLOT( quit() ) );
    MainWindow w;//在翻译文件装载好之后初始化
    return a.exec();
}
