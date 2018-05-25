#include "wallpaperthread.h"
#include <QDir>
#include <QUrl>
#include <QProcess>
#include <QDesktopWidget>
#include <QDateTime>
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QSettings>
#include <QTextCodec>
WallpaperThread::WallpaperThread(QObject *parent) :
    QThread(parent)
{

}

void WallpaperThread::init(QSystemTrayIcon *mSysTrayIcon,IconThread *thread)
{
    sysIcon = mSysTrayIcon;
    iconThread = thread;
    QString projectPath = QCoreApplication::applicationDirPath();
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(projectPath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    QString cycleTime = configIni->value("Config/CycleTime",tr("30")).toString();
    minutes = cycleTime.toULong();
}
void WallpaperThread::changeWallpaer()
{
        QString  filePath = QDir::homePath().append(tr("/.config/unplash4deepin/"));
        qDebug() << "start";
        QProcess::execute(tr("rm -rf ").append(filePath).append(tr("cache")));
        QProcess::execute(tr("mkdir -p ").append(filePath).append(tr("cache")));
        QString datestr = QDateTime::currentDateTime().toString("yyyyMMddhhmm");
        QString file = filePath.append(tr("cache/")).append(datestr).append(tr(".jpg"));
        QDesktopWidget* desktopWidget = QApplication::desktop();
        QRect screenRect = desktopWidget->screenGeometry();
        QString site = tr("https://source.unsplash.com/random/");
        int g_nActScreenW = screenRect.width();
        int g_nActScreenH = screenRect.height();
        site.append(QString::number(g_nActScreenW)).append("x").append(QString::number(g_nActScreenH));
        QString cmd = tr("wget ").append(site).append(tr(" --output-document=")).append(file);
        qDebug() << cmd;
        QProcess::execute(cmd);
        QProcess::execute(tr("gsettings set com.deepin.wrap.gnome.desktop.background picture-uri ").append(file));
}

void WallpaperThread::run()
{
    QIcon icon = QIcon(":/image/TrayIcon.png");
    while (true) {
        iconThread->condtion.wakeAll();
        qDebug() << minutes;
        changeWallpaer();
        sysIcon->setIcon(icon);
        condtion.wait(&localMutex,minutes*60*1000);
    }

//    QTimer *m_pTimer = new QTimer();
//    m_pTimer->setInterval(60*60*1000);
//    connect(m_pTimer, &QTimer::timeout, this, &WallpaperThread::changeWallpaer);
//    m_pTimer->start();
//    this->exec();

}
