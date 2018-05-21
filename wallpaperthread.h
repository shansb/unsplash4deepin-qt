#ifndef WALLPAPERTHREAD_H
#define WALLPAPERTHREAD_H
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
class WallpaperThread : public QThread
{
    Q_OBJECT

public:
    explicit WallpaperThread(QObject *parent = 0);
//    ~WallpaperThread();
private slots:
    void change();

protected:
    void run();
    void changeWallpaer();
};

#endif // WALLPAPERTHREAD_H
