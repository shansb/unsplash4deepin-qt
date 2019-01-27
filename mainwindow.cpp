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
#include <QSignalMapper>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread = new WallpaperThread(this);

    proc = new QProcess(this);
    filePath = QDir::homePath().append(tr("/.config/unplash4deepin/"));
    //读取参数
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(filePath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    QString cycleTime = configIni->value("Config/CycleTime",tr("60")).toString();
    bool autoClear = configIni->value("Config/AutoClear",false).toBool();
    delete configIni;
    //创建托盘菜单
    setting = new QMenu(tr("刷新时间"));
    halfAnHour = new QAction(this);
    oneHour = new QAction(this);
    twoHour = new QAction(this);
    fourHour = new QAction(this);
    halfAnHour->setText(tr("30分钟"));
    oneHour->setText(tr("1小时"));
    twoHour->setText(tr("2小时"));
    fourHour->setText(tr("4小时"));
    halfAnHour->setCheckable(true);
    oneHour->setCheckable(true);
    twoHour->setCheckable(true);
    fourHour->setCheckable(true);
    halfAnHour->setChecked(0 == cycleTime.compare("30"));
    oneHour->setChecked(0 == cycleTime.compare("60"));
    twoHour->setChecked(0 == cycleTime.compare("120"));
    fourHour->setChecked(0 == cycleTime.compare("240"));
    setting->addAction(halfAnHour);
    setting->addAction(oneHour);
    setting->addAction(twoHour);
    setting->addAction(fourHour);
    refresh = new QAction(this);
    refresh->setText(tr("手动刷新"));
    clear = new QAction(this);
    clear->setText("清理缓存");
    clear->setCheckable(true);
    clear->setChecked(autoClear);
    about = new QAction(this);
    about->setText("关于");
    quit = new QAction(this);
    quit->setText(tr("退出"));
    //信号连接
    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(halfAnHour, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
    connect(oneHour, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
    connect(twoHour, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
    connect(fourHour, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
    signalMapper->setMapping(halfAnHour, tr("30"));
    signalMapper->setMapping(oneHour, tr("60"));
    signalMapper->setMapping(twoHour, tr("120"));
    signalMapper->setMapping(fourHour, tr("240"));
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(setUp(QString)));
    connect(refresh, SIGNAL(triggered(bool)), this, SLOT(changeWallpaper()));
    connect(about, SIGNAL(triggered(bool)), this, SLOT(aboutMe()));
    connect(quit, SIGNAL(triggered(bool)), this, SLOT(exitApp()));
    connect(clear, SIGNAL(triggered(bool)), this, SLOT(setAutoClear(bool)));

    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    connect(mSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
    //新建托盘要显示的icon
    icon = QIcon(":/image/TrayIcon.png");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QString("WallpaperChanger"));

    trayMenu = new QMenu(this);
    trayMenu->addMenu(setting);
    trayMenu->addAction(refresh);
//    trayMenu->addAction(clear);移除缓存清理功能
    trayMenu->addAction(about);
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
void MainWindow::exitApp()
{
    delete ui;
    myApp->quit();
}

//void MainWindow::setApp(QApplication *app)
//{
//    myApp = app;
//}

void MainWindow::setAutoClear(bool flag)
{
    thread->setAutoClear(flag);
//    QString projectPath = QCoreApplication::applicationDirPath();
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(filePath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    configIni->setValue("Config/AutoClear",flag);
    delete configIni;
}

void MainWindow::changeWallpaper()
{
    thread->condtion.wakeAll();

}

void MainWindow::iconIsActived(QSystemTrayIcon::ActivationReason e)
{
    switch(e)
     {
     //双击托盘显示窗口
     case QSystemTrayIcon::DoubleClick:
     {
       changeWallpaper();
       break;
     }
     default:
      break;
     }
}

void MainWindow::aboutMe()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/shansb/unsplash4deepin-qt")));
}

void MainWindow::setUp(QString cycleTime)
{
    halfAnHour->setChecked(0 == cycleTime.compare("30"));
    oneHour->setChecked(0 == cycleTime.compare("60"));
    twoHour->setChecked(0 == cycleTime.compare("120"));
    fourHour->setChecked(0 == cycleTime.compare("240"));
    thread->minutes=cycleTime.toULong();
//    QString projectPath = QCoreApplication::applicationDirPath();
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(filePath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    configIni->setValue("Config/CycleTime",cycleTime);
    delete configIni;
//    QDesktopServices::openUrl ( QUrl::fromLocalFile(tr("%1/setting.ini").arg(QCoreApplication::applicationDirPath())) );
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
