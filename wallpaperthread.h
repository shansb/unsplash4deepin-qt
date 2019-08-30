#ifndef WALLPAPERTHREAD_H
#define WALLPAPERTHREAD_H
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
class WallpaperThread : public QThread
{
    Q_OBJECT

public:
    explicit WallpaperThread(QObject *parent = 0);
//    ~WallpaperThread();
    void changeWallpaer();
    QWaitCondition condtion;
    QMutex localMutex;
    void init(QSystemTrayIcon *mSysTrayIcon,unsigned long cycleTime,QString source,QString key);
    void changeIcon(QString filePath);
    void setAutoClear(bool flag);
    void changeSource(QString source);
    unsigned long minutes;
    bool autoClear;
    bool isClose;
    QString keyword;


private:
    QString sourceSite;
    QSystemTrayIcon *sysIcon;
    QDesktopWidget *desktopWidget;
    void delJpgFiles(QString filePath);

protected:
    void run();

};

#endif // WALLPAPERTHREAD_H
