#include "wallpaperthread.h"
#include <windows.h>
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
#include <QtGlobal>
WallpaperThread::WallpaperThread(QObject *parent) :
    QThread(parent)
{
    desktopWidget = QApplication::desktop();
}

void WallpaperThread::init(QSystemTrayIcon *mSysTrayIcon,unsigned long cycleTime)
{
    sysIcon = mSysTrayIcon;
    minutes = cycleTime;
}
void WallpaperThread::delJpgFiles(QString filePath)
{
    qDebug() << tr("begin to clear jpg file in ").append(filePath);
    QDir fileDir = QDir(filePath);
    if ( fileDir.exists() ) {
        QFileInfoList fileList = fileDir.entryInfoList(QDir::Files);
        foreach (QFileInfo file, fileList){ //遍历文件信息
            if (0 == file.suffix().compare("jpg")){ // 是jpg，删除
                file.dir().remove(file.fileName());
            }
        }
    }
}

void WallpaperThread::changeWallpaer()
{
//    if(autoClear){
//        delCache(QDir::homePath().append("/Pictures/Wallpapers"));
//    }
    QString  filePath = QDir::homePath().append("/.config/unplash4deepin/cache");
    qDebug() << "start";    
    QDir cache(filePath);
    if(cache.exists()){
        delJpgFiles(filePath);
    } else {
        cache.mkpath(cache.absolutePath());
    }
//    QProcess::execute(tr("mkdir -p ").append(filePath).append("cache"));
    QString datestr = QDateTime::currentDateTime().toString("yyyyMMddhhmm");
    QString file = filePath.append("/").append(datestr).append(".jpg");
//    QDesktopWidget* desktopWidget = QApplication::desktop();
    int scrrenSize = desktopWidget->screenCount();
    int g_nActScreenW = 0;
    int g_nActScreenH = 0;
    for(int i = 0;i<scrrenSize;i++){
        g_nActScreenW = desktopWidget->screenGeometry(i).width() > g_nActScreenW
                ? desktopWidget->screenGeometry(i).width() : g_nActScreenW;

        g_nActScreenH = desktopWidget->screenGeometry(i).height() > g_nActScreenH
                ? desktopWidget->screenGeometry(i).height() : g_nActScreenH;
    }
//    QString site = "https://source.unsplash.com/random/";
//    site.append(QString::number(g_nActScreenW)).append("x").append(QString::number(g_nActScreenH));
    QString site = "https://picsum.photos/";
    site.append(QString::number(g_nActScreenW)).append("/").append(QString::number(g_nActScreenH));
    QString cmd = tr("wget ").append(site).append(" --output-document=").append(file);

#ifdef Q_OS_LINUX
    QProcess::execute(cmd);
    QProcess::execute(tr("gsettings set com.deepin.wrap.gnome.desktop.background picture-uri ").append(file));
#endif

#ifdef Q_OS_WIN32
    //Invoke-WebRequest -Uri 'https://source.unsplash.com/random/1280x1024' -OutFile 'test.jpg'
    cmd = tr("Invoke-WebRequest -Uri ").append(site).append(" -OutFile '").append(file).append("'");

    qDebug() << cmd;
    QStringList commands1;
    commands1.append("-Command");
    commands1.append(cmd);
    QProcess *p = new QProcess();
    QString powershell=tr("C:/Windows/system32/WindowsPowerShell/v1.0/powershell.exe");
    p->start(powershell,commands1);
    qint64 start = QDateTime::currentDateTime().toMSecsSinceEpoch();
    p->waitForFinished(600000);
    qint64 end = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(end-start > 550000){
        qDebug() << "本次下载超时，不设置壁纸";
        return;
    }
    qDebug() << "设置壁纸";
    QSettings wallPaper("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);
    QString path(file);
    //把注册表的桌面图片路径改为指定路径.
    wallPaper.setValue("Wallpaper", path);
    QByteArray byte = path.toLocal8Bit();
    //调用windows api.
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, byte.data(), SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
#endif
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
//    changeIcon(":/image/TrayIcon16x16.png");
//    qDebug() << minutes;
//    changeWallpaer();
//    sysIcon->setIcon(icon);
    while (true) {
        changeIcon(":/image/TrayIcon16x16.png");
        qDebug() << minutes;
        changeWallpaer();
        sysIcon->setIcon(icon);
        localMutex.lock();
        condtion.wait(&localMutex,minutes*60*1000);
        localMutex.unlock();
    }

}
