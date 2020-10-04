#include "wallpaperthread.h"
#ifdef Q_OS_WIN32
#include <windows.h>
#endif
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
#include <QPainter>
WallpaperThread::WallpaperThread(QObject *parent) :
    QThread(parent)
{
    desktopWidget = QApplication::desktop();
}

void WallpaperThread::init(QSystemTrayIcon *mSysTrayIcon,unsigned long cycleTime,QString source,QString key)
{
    sysIcon = mSysTrayIcon;
    minutes = cycleTime;
    sourceSite = source;
    keyword = key;
    isClose = false;
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
    QString site = "https://source.unsplash.com/random/";
    if(0 == sourceSite.compare("2")){
        site = "https://picsum.photos/";
        site.append(QString::number(g_nActScreenW)).append("/").append(QString::number(g_nActScreenH));
    } else if (0 == sourceSite.compare("1")){
       site.append(QString::number(g_nActScreenW)).append("x").append(QString::number(g_nActScreenH));
       if(keyword.compare("all") != 0){
           site.append("/?").append(keyword);
       }
    } else if (0 == sourceSite.compare("3")) {
        site = QString("http://img.nsmc.org.cn/CLOUDIMAGE/FY4A/MTCC/FY4A_DISK.JPG?").append(datestr);
    }
    QString cmd = QString("wget ").append(site).append(" --output-document=").append(file);

#ifdef Q_OS_LINUX
    QStringList options;
    QProcess::execute(cmd);
#endif
#ifdef Q_OS_WIN32
    //Invoke-WebRequest -Uri 'https://source.unsplash.com/random/1280x1024' -OutFile 'test.jpg'
    cmd = QString("Invoke-WebRequest -Uri ").append(site).append(" -OutFile '").append(file).append("'");

    qDebug() << cmd;
    QStringList commands1;
    commands1.append("-Command");
    commands1.append(cmd);
    QProcess *p = new QProcess();
    QString powershell=QString("C:/Windows/system32/WindowsPowerShell/v1.0/powershell.exe");
    p->start(powershell,commands1);
    qint64 start = QDateTime::currentDateTime().toMSecsSinceEpoch();
    p->waitForFinished(600000);
    qint64 end = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(end-start > 550000){
        qDebug() << "download time out,change wallpaper next time";
        return;
    }
    QFileInfo fileInfo(file);
    if(file.size() < 10){
        qDebug() << "download failed,change wallpaper next time";
        return;
    }
#endif
    if (0 == sourceSite.compare("3")) {
        if(g_nActScreenW > g_nActScreenH){
           int height = 2198*10/8;
           g_nActScreenW = g_nActScreenW*height/g_nActScreenH;
           g_nActScreenH =height;
        } else {
           int width = 2198*10/8;
           g_nActScreenH = g_nActScreenH*width/g_nActScreenW;
        }
        //process the earth image to fit the largest screen
        //int trueSize = g_nActScreenH*8/10 > g_nActScreenW*8/10 ? g_nActScreenW*8/10 : g_nActScreenH*8/10;
        int trueSize = 2198;
        // move the watermark
        QImage earth(file);
        QImage blackBlock(430,160,QImage::Format_RGB32);
        blackBlock.fill(QColor(0,0,0));
        QPainter imagepainter (&earth);
        imagepainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        imagepainter.drawImage(0,0,blackBlock);
        imagepainter.drawImage(2198-430,2198-160,blackBlock);
        imagepainter.end();

//        if(2198 > trueSize){
//            //resize the earth image to true size
//            earth = earth.scaled(trueSize,trueSize,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//        }
        QImage backgorund(g_nActScreenW,g_nActScreenH,QImage::Format_RGB32);
        backgorund.fill(QColor(0,0,0));
        QPainter paperPainter(&backgorund);
        paperPainter.drawImage((g_nActScreenW-trueSize)/2,(g_nActScreenH-trueSize)/2,earth);
        paperPainter.end();
        backgorund.save(file);
    }
#ifdef Q_OS_LINUX
    QString setWallpaper =  QString("for screen in  `xrandr|grep ' connected'|awk '{print $1}'`; do dbus-send --dest=com.deepin.daemon.Appearance /com/deepin/daemon/Appearance --print-reply com.deepin.daemon.Appearance.SetMonitorBackground string:$screen string:'").append(file).append("';done");
    options << "-c" << setWallpaper;
    QProcess::execute("/bin/bash",options);
//    QProcess::execute(tr("gsettings set com.deepin.wrap.gnome.desktop.background picture-uri ").append(file));
#endif

#ifdef Q_OS_WIN32
    qDebug() << "setting wallpaper";
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

void WallpaperThread::changeSource(QString source){
    sourceSite = source;
}

void WallpaperThread::run()
{
    QIcon icon = QIcon(":/image/TrayIcon.png");
//    changeIcon(":/image/TrayIcon16x16.png");
//    qDebug() << minutes;
//    changeWallpaer();
//    sysIcon->setIcon(icon);
    while (true) {
        if(isClose){
            qDebug() << "exit!!";
            this->exit(0);
            return;
        }
        changeIcon(":/image/TrayIcon16x16.png");
        qDebug() << minutes;
        changeWallpaer();
        sysIcon->setIcon(icon);
        localMutex.lock();
        condtion.wait(&localMutex,minutes*60*1000);
        localMutex.unlock();
    }

}
