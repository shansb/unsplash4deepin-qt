#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QProcess>
#include <wallpaperthread.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    void setApp(QApplication *app);


private slots:
    void setUp(QString cycleTime);
    void changeWallpaper();
    void exitApp();
    void closeEvent(QCloseEvent *e);
    void changeEvent(QEvent *e);
    void setAutoClear(bool flag);
    void aboutMe();
    void iconIsActived(QSystemTrayIcon::ActivationReason e);

signals:
    void exit();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *mSysTrayIcon;
    QMenu *trayMenu;
    QMenu *setting;
    QAction *refresh;
    QAction *clear;
    QAction *quit;
    QAction *about;
    QProcess *proc;
    QString filePath;
    QIcon icon;
    WallpaperThread *thread;
    QAction *halfAnHour;
    QAction *oneHour;
    QAction *twoHour;
    QAction *fourHour;
    QApplication *myApp;
};

#endif // MAINWINDOW_H
