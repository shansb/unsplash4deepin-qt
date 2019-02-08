#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QTimer>
#include <QUrl>
#include <QProcess>
#include <QDebug>
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

//    proc = new QProcess(this);
    filePath = QDir::homePath().append("/.config/unplash4deepin/");
    //读取参数
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(filePath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    QString cycleTime = configIni->value("Config/CycleTime","60").toString();
    bool autoClear = configIni->value("Config/AutoClear",false).toBool();
    delete configIni;
    //创建托盘菜单
    setting = new QMenu(tr("Interval"));
    halfAnHour = new QAction(this);
    oneHour = new QAction(this);
    twoHour = new QAction(this);
    fourHour = new QAction(this);
    halfAnHour->setText(tr("30 mins"));
    oneHour->setText(tr("1 hour"));
    twoHour->setText(tr("2 hours"));
    fourHour->setText(tr("4 hours"));
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
    refresh->setText(tr("Refresh"));
    clear = new QAction(this);
    clear->setText(tr("Clear cache"));
    clear->setCheckable(true);
    clear->setChecked(autoClear);
    save = new QAction(this);
    save->setText(tr("Save it"));
    about = new QAction(this);
    about->setText(tr("About"));
    quit = new QAction(this);
    quit->setText(tr("Quit"));
    //信号连接
    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(halfAnHour, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
    connect(oneHour, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
    connect(twoHour, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
    connect(fourHour, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
    signalMapper->setMapping(halfAnHour, "30");
    signalMapper->setMapping(oneHour, "60");
    signalMapper->setMapping(twoHour, "120");
    signalMapper->setMapping(fourHour, "240");
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(setUp(QString)));
    connect(refresh, SIGNAL(triggered(bool)), this, SLOT(changeWallpaper()));
    connect(save, SIGNAL(triggered(bool)), this, SLOT(saveWallpaper()));
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
    trayMenu->addAction(save);
//    trayMenu->addAction(clear);移除缓存清理功能
    trayMenu->addAction(about);
    trayMenu->addAction(quit);
    mSysTrayIcon->setContextMenu(trayMenu);
    //在系统托盘显示此对象
    mSysTrayIcon->show();

    thread->init(mSysTrayIcon,cycleTime.toULong());
    thread->start();// start to set wallpaper
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

void MainWindow::saveWallpaper()
{
    QDir target(QDir::homePath().append("/Pictures/unsplash"));
    QDir sourceDir(QDir::homePath().append("/.config/unplash4deepin/cache"));
    QString tip = tr("Current wallpaper has been saved to ");
    if(!target.exists()){
        if(!target.mkdir(target.absolutePath())){
            tip = tr("Cannot create target floder");
            QProcess::execute(tr("notify-send -a unsplash4deepin \"").append(tip).append("\""));
            return;
        }
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        qDebug()<< fileInfo.fileName();
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isFile()){
            QFile::copy(fileInfo.filePath(),target.filePath(fileInfo.fileName()));
        }
    }
    QProcess::execute(tr("notify-send -a unsplash4deepin \"").append(tip).append(QDir::homePath()).append("/Pictures/unsplash\""));
}

void MainWindow::setAutoClear(bool flag)
{
    thread->setAutoClear(flag);
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
     //双击托盘更换壁纸
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
