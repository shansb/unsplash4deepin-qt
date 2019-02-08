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
    void init(QSystemTrayIcon *mSysTrayIcon,unsigned long cycleTime);
    void changeIcon(QString filePath);
    void setAutoClear(bool flag);
    unsigned long minutes;
    bool autoClear;


private:
    QSystemTrayIcon *sysIcon;
    void delJpgFiles(QString filePath);

protected:
    void run();

};

#endif // WALLPAPERTHREAD_H
