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
    QIcon icon = QIcon(":/image/TrayIcon.png");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QString("测试系统托盘图标"));

    trayMenu = new QMenu(this);
    trayMenu->addAction(setting);
    trayMenu->addAction(refresh);
    trayMenu->addAction(quit);
    mSysTrayIcon->setContextMenu(trayMenu);
    //在系统托盘显示此对象
    mSysTrayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeWallpaper()
{
    qDebug() << QDir::homePath();
//    QProcess::execute(tr("mkdir -p ").append().append(tr("/.config/unplash4deepin/cache")));
    QString datestr = QDateTime::currentDateTime().toString("yyyyMMddhhmm");
    QString file = tr("/home/bobo/Desktop/").append(datestr).append(tr(".jpg"));
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    QString site = tr("https://source.unsplash.com/random/");
    int g_nActScreenW = screenRect.width();
    int g_nActScreenH = screenRect.height();
    site.append(QString::number(g_nActScreenW)).append("x").append(QString::number(g_nActScreenH));
    QString cmd = tr("wget ").append(site).append(tr(" --output-document=")).append(file);
    qDebug() << cmd;
    QProcess::execute(cmd);
    QProcess::execute(tr("gsettings set com.deepin.wrap.gnome.desktop.background picture-uri ").append(file));

}

void MainWindow::setUp()
{
    QProcess::execute(tr("mkdir -p ").append(QDir::homePath()).append(tr("/.config/unplash4deepin/cache")));
    qDebug()<<QDateTime::currentDateTime().toString("yyyyMMddhhmm");;
    this->show();
}

void MainWindow::on_QuickStart_triggered()
{

    QDesktopServices::openUrl(QUrl::fromLocalFile("/home/deep4win/.config/unplash4deepin/setting"));
//    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(fileName).absoluteFilePath()));
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
