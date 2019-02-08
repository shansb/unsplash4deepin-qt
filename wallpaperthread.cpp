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

void WallpaperThread::init(QSystemTrayIcon *mSysTrayIcon,unsigned long cycleTime)
{
    sysIcon = mSysTrayIcon;
    minutes = cycleTime;
}
void WallpaperThread::delCache(QString filePath)
{
    qDebug() << tr("begin to clear cache");
    QDir cacheDir = QDir(filePath);
    if ( cacheDir.exists() ) {
        QFileInfoList fileList = cacheDir.entryInfoList(QDir::Files);
        foreach (QFileInfo file, fileList){ //遍历文件信息
            if (0 == file.suffix().compare("jpg")){ // 是jpg，删除
                file.dir().remove(file.fileName());
            }
        }
    }
}

void WallpaperThread::changeWallpaer()
{
    if(autoClear){
        delCache(QDir::homePath().append("/Pictures/Wallpapers"));
    }
    QString  filePath = QDir::homePath().append("/.config/unplash4deepin/");
    qDebug() << "start";
    QProcess::execute(tr("rm -rf ").append(filePath).append("cache"));
    QProcess::execute(tr("mkdir -p ").append(filePath).append("cache"));
    QString datestr = QDateTime::currentDateTime().toString("yyyyMMddhhmm");
    QString file = filePath.append("cache/").append(datestr).append(".jpg");
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    QString site = "https://source.unsplash.com/random/";
    int g_nActScreenW = screenRect.width();
    int g_nActScreenH = screenRect.height();
    site.append(QString::number(g_nActScreenW)).append("x").append(QString::number(g_nActScreenH));
    QString cmd = tr("wget ").append(site).append(" --output-document=").append(file);
    qDebug() << cmd;
    QProcess::execute(cmd);
    QProcess::execute(tr("gsettings set com.deepin.wrap.gnome.desktop.background picture-uri ").append(file));
}
void WallpaperThread::setAutoClear(bool flag)
{
    autoClear = flag;
    qDebug() <<flag;
}
void WallpaperThread::changeIcon(QString filePath)
{
    sysIcon->setIcon(QIcon(filePath));
}
void WallpaperThread::run()
{
    QIcon icon = QIcon(":/image/TrayIcon.png");
    while (true) {
        changeIcon(":/image/TrayIcon16x16.png");
        qDebug() << minutes;
        changeWallpaer();
        sysIcon->setIcon(icon);
        condtion.wait(&localMutex,minutes*60*1000);
    }

}
