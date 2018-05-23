#include "iconthread.h"
#include <QDir>
#include <QUrl>
#include <QProcess>
#include <QDesktopWidget>
#include <QDateTime>
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
IconThread::IconThread(QObject *parent) :
    QThread(parent)
{

}
void IconThread::init(QSystemTrayIcon *mSysTrayIcon)
{
    sysIcon = mSysTrayIcon;
}

void IconThread::run()
{

    QIcon icon2 =QIcon(":/image/TrayIcon16x16.png");
    while (true) {
        qDebug() << "Hi";
        condtion.wait(&mutex);
        sysIcon->setIcon(icon2);
    }

//    QTimer *m_pTimer = new QTimer();
//    m_pTimer->setInterval(60*60*1000);
//    connect(m_pTimer, &QTimer::timeout, this, &WallpaperThread::changeWallpaer);
//    m_pTimer->start();
//    this->exec();

}
