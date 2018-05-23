#ifndef WALLPAPERTHREAD_H
#define WALLPAPERTHREAD_H
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QSystemTrayIcon>
class WallpaperThread : public QThread
{
    Q_OBJECT

public:
    explicit WallpaperThread(QObject *parent = 0);
//    ~WallpaperThread();
    void changeWallpaer();
    QWaitCondition condtion;
    QMutex localMutex;
    void init(QSystemTrayIcon *mSysTrayIcon);


private:
    QSystemTrayIcon *sysIcon;

protected:
    void run();

};

#endif // WALLPAPERTHREAD_H
