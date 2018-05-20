#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QProcess>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_QuickStart_triggered();
    void setUp();
    void changeWallpaper();
    void closeEvent(QCloseEvent *e);
    void changeEvent(QEvent *e);

signals:
    void exit();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *mSysTrayIcon;
    QMenu *trayMenu;
    QAction *setting;
    QAction *refresh;
    QAction *quit;
    QProcess *proc;
    QString *home;
};

#endif // MAINWINDOW_H
