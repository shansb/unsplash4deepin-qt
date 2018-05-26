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
#include <QSettings>
#include <QTextCodec>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread = new WallpaperThread(this);
    //读取参数
    QString projectPath = QCoreApplication::applicationDirPath();
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(projectPath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    QString cycleTime = configIni->value("Config/CycleTime",tr("30")).toString();
    bool autoClear = configIni->value("Config/AutoClear",false).toBool();
    delete configIni;

    proc = new QProcess(this);
    filePath = QDir::homePath().append(tr("/.config/unplash4deepin/"));
    //创建托盘菜单
    setting = new QAction(this);
    setting->setText(tr("设置"));
    refresh = new QAction(this);
    refresh->setText(tr("手动刷新"));
    clear = new QAction(this);
    clear->setText("清理缓存");
    clear->setCheckable(true);
    clear->setChecked(autoClear);
    quit = new QAction(this);
    quit->setText(tr("Quit"));
    //信号连接
    connect(setting, SIGNAL(triggered(bool)), this, SLOT(setUp()));
    connect(refresh, SIGNAL(triggered(bool)), this, SLOT(changeWallpaper()));
    connect(quit, SIGNAL(triggered(bool)), this, SIGNAL(exit()));
    connect(clear, SIGNAL(triggered(bool)), this, SLOT(setAutoClear(bool)));
    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    icon = QIcon(":/image/TrayIcon.png");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QString("WallpaperChanger"));

    trayMenu = new QMenu(this);
    trayMenu->addAction(setting);
    trayMenu->addAction(refresh);
    trayMenu->addAction(clear);
    trayMenu->addAction(quit);
    mSysTrayIcon->setContextMenu(trayMenu);
    //在系统托盘显示此对象
    mSysTrayIcon->show();


    thread->init(mSysTrayIcon,cycleTime.toULong());
    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setAutoClear(bool flag)
{
    thread->setAutoClear(flag);
    qDebug() <<flag;
    QString projectPath = QCoreApplication::applicationDirPath();
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(projectPath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    configIni->setValue("Config/AutoClear",flag);
    delete configIni;
}

void MainWindow::changeWallpaper()
{
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
}

void MainWindow::changeEvent(QEvent *e)
{
    if((e->type()==QEvent::WindowStateChange)&&this->isMinimized())
    {
        this->hide();
    }
}
