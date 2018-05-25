#ifndef ICONTHREAD_H
#define ICONTHREAD_H
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QSystemTrayIcon>
class IconThread : public QThread
{
    Q_OBJECT

public:
    explicit IconThread(QObject *parent = 0);
    QWaitCondition condtion;
    QMutex mutex;
    void init(QSystemTrayIcon *mSysTrayIcon);


private:
    QSystemTrayIcon *sysIcon;

protected:
    void run();

};

#endif // ICONTHREAD_H
