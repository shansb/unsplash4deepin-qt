#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QTimer>
#include <QUrl>
#include <QProcess>
#include <QDesktopWidget>
#include <QDir>
#include <QDateTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    proc = new QProcess(this);
    filePath = QDir::homePath().append(tr("/.config/unplash4deepin/"));
    //创建托盘菜单
    setting = new QAction(this);
    setting->setText(tr("Setting"));
    refresh = new QAction(this);
    refresh->setText(tr("Refresh"));
    quit = new QAction(this);
    quit->setText(tr("Quit"));
    //信号连接
    connect(setting, SIGNAL(triggered(bool)), this, SLOT(setUp()));
    connect(refresh, SIGNAL(triggered(bool)), this, SLOT(changeWallpaper()));
    connect(quit, SIGNAL(triggered(bool)), this, SIGNAL(exit()));
    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    icon = QIcon(":/image/TrayIcon16x16.png");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QString("WallpaperChanger"));

    trayMenu = new QMenu(this);
    trayMenu->addAction(setting);
    trayMenu->addAction(refresh);
    trayMenu->addAction(quit);
    mSysTrayIcon->setContextMenu(trayMenu);
    //在系统托盘显示此对象
    mSysTrayIcon->show();

    iconThread = new IconThread();
    iconThread->init(mSysTrayIcon);
    iconThread->start();
    thread = new WallpaperThread(this);
    thread->init(mSysTrayIcon,iconThread);
    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeWallpaper()
{
//    iconThread->condtion.wakeAll();
    thread->condtion.wakeAll();

}

void MainWindow::setUp()
{
    QDesktopServices::openUrl ( QUrl::fromLocalFile(tr("%1/setting.ini").arg(QCoreApplication::applicationDirPath())) );
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    e->ignore();
    this->hide();
//    qDebug() << str;
}

void MainWindow::changeEvent(QEvent *e)
{
    if((e->type()==QEvent::WindowStateChange)&&this->isMinimized())
    {
//        QTimer::singleShot(100, this, SLOT(close()));
        this->hide();
    }
}
