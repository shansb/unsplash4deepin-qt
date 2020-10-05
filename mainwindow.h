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
    void setAutoStart(bool flag);
    void saveWallpaper();
    void aboutMe();
    void iconIsActived(QSystemTrayIcon::ActivationReason e);
    void changeSource(QString site);
    void changeKeyword(QString key);

signals:
    void exit();

private:
    QString macOSXAppBundlePath();
    QString macOSXAppBundleName();
    Ui::MainWindow *ui;
    QSystemTrayIcon *mSysTrayIcon;
    QMenu *trayMenu;
    QMenu *setting;
    QAction *refresh;
    QAction *clear;
    QAction *quit;
    QAction *save;
    QAction *autoStart;
    QAction *about;
    QProcess *proc;
    QString filePath;
    QIcon icon;
    WallpaperThread *thread;
    QAction *halfAnHour;
    QAction *oneHour;
    QAction *twoHours;
    QAction *fourHours;
    QApplication *myApp;
    //设置源
    QMenu *sites;
    QAction *site1;
    QAction *site2;
    QAction *site3;
    //关键字
    QMenu *keywords;
    QAction *all;
    QAction *wallpaper;
    QAction *people;
    QAction *textures;
    QAction *nature;
    QAction *architecture;
    QAction *film;
    QAction *animals;
    QAction *travel;
    QAction *food;
};

#endif // MAINWINDOW_H
