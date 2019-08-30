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
    QFont f=QApplication::font();
    f.setStyleStrategy(QFont::PreferAntialias);
    QApplication::setFont(f);
//    proc = new QProcess(this);
    filePath = QDir::homePath().append("/.config/unplash4deepin/");
    //读取参数
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(filePath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    QString cycleTime = configIni->value("Config/CycleTime","60").toString();
    QString sourceSite = configIni->value("Config/Source","1").toString();
    QString keyWord = configIni->value("Config/Keyword","wallpaper").toString();
    bool autoClear = configIni->value("Config/AutoClear",false).toBool();
    bool autoBoot = configIni->value("Config/AutoStart",false).toBool();
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
    autoStart = new QAction(this);
    autoStart->setText(tr("Auto Start"));
    autoStart->setCheckable(true);
    autoStart->setChecked(autoBoot);
    //程序启动时按照设置做一次开机启动注册
    setAutoStart(autoBoot);
    about = new QAction(this);
    about->setText(tr("About"));
    quit = new QAction(this);
    quit->setText(tr("Quit"));
    //设置源
    sites = new QMenu(tr("Source"));
    site1 = new QAction(tr("Unsplash"),this);
    site2 = new QAction(tr("Picsum"),this);
    site1->setCheckable(true);
    site2->setCheckable(true);
    site1->setChecked(sourceSite.compare("1") == 0);
    site2->setChecked(sourceSite.compare("2") == 0);
    sites->addAction(site1);
    sites->addAction(site2);
    QSignalMapper *sourceMapper = new QSignalMapper(this);
    connect(site1,SIGNAL(triggered(bool)),sourceMapper, SLOT(map()));
    connect(site2,SIGNAL(triggered(bool)),sourceMapper, SLOT(map()));
    sourceMapper->setMapping(site1,"1");
    sourceMapper->setMapping(site2,"2");
    connect(sourceMapper, SIGNAL(mapped(QString)),this, SLOT(changeSource(QString)));
    //关键字
    keywords = new QMenu(tr("Category"));
    all = new QAction(tr("all"),this);
    wallpaper = new QAction(tr("wallpaper"),this);
    people = new QAction(tr("people"),this);
    textures = new QAction(tr("textures"),this);
    nature = new QAction(tr("nature"),this);
    architecture = new QAction(tr("architecture"),this);
    film = new QAction(tr("film"),this);
    animals = new QAction(tr("animals"),this);
    travel = new QAction(tr("travel"),this);
    food = new QAction(tr("food"),this);
    all->setCheckable(true);
    wallpaper->setCheckable(true);
    people->setCheckable(true);
    textures->setCheckable(true);
    nature->setCheckable(true);
    architecture->setCheckable(true);
    film->setCheckable(true);
    animals->setCheckable(true);
    travel->setCheckable(true);
    food->setCheckable(true);
    all->setChecked(keyWord.compare("all") == 0);
    wallpaper->setChecked(keyWord.compare("wallpaper") == 0);
    people->setChecked(keyWord.compare("people") == 0);
    textures->setChecked(keyWord.compare("textures") == 0);
    nature->setChecked(keyWord.compare("nature") == 0);
    architecture->setChecked(keyWord.compare("architecture") == 0);
    film->setChecked(keyWord.compare("film") == 0);
    animals->setChecked(keyWord.compare("animals") == 0);
    travel->setChecked(keyWord.compare("travel") == 0);
    food->setChecked(keyWord.compare("food") == 0);
    keywords->addAction(all);
    keywords->addAction(wallpaper);
    keywords->addAction(people);
    keywords->addAction(textures);
    keywords->addAction(nature);
    keywords->addAction(architecture);
    keywords->addAction(film);
    keywords->addAction(animals);
    keywords->addAction(travel);
    keywords->addAction(food);
    QSignalMapper *keyMapper = new QSignalMapper(this);
    connect(all,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    connect(wallpaper,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    connect(people,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    connect(textures,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    connect(nature,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    connect(architecture,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    connect(film,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    connect(animals,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    connect(travel,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    connect(food,SIGNAL(triggered(bool)),keyMapper, SLOT(map()));
    keyMapper->setMapping(all,"all");
    keyMapper->setMapping(wallpaper,"wallpaper");
    keyMapper->setMapping(people,"people");
    keyMapper->setMapping(textures,"textures");
    keyMapper->setMapping(nature,"nature");
    keyMapper->setMapping(architecture,"architecture");
    keyMapper->setMapping(film,"film");
    keyMapper->setMapping(animals,"animals");
    keyMapper->setMapping(travel,"travel");
    keyMapper->setMapping(food,"food");
    connect(keyMapper, SIGNAL(mapped(QString)),this, SLOT(changeKeyword(QString)));

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
    connect(autoStart,SIGNAL(triggered(bool)),this,SLOT(setAutoStart(bool)));
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
    trayMenu->addAction(refresh);
    trayMenu->addMenu(setting);
    trayMenu->addMenu(sites);
    trayMenu->addMenu(keywords);
    if(sourceSite.compare("1") != 0){
        keywords->menuAction()->setVisible(false);
    }
    trayMenu->addAction(save);
#ifdef Q_OS_WIN32
    trayMenu->addAction(autoStart);//win10有自启功能
#endif
//    trayMenu->addAction(clear);移除缓存清理功能
    trayMenu->addAction(about);
    trayMenu->addAction(quit);
    mSysTrayIcon->setContextMenu(trayMenu);
    //在系统托盘显示此对象
    mSysTrayIcon->show();

    thread->init(mSysTrayIcon,cycleTime.toULong(),sourceSite,keyWord);
    thread->start();// start to set wallpaper
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::exitApp()
{
    //delete ui;
    thread->isClose = true;
    changeWallpaper();
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
            #ifdef Q_OS_LINUX
                QProcess::execute(tr("notify-send -a unsplash4deepin \"").append(tip).append("\""));
            #endif
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
#ifdef Q_OS_LINUX
    QProcess::execute(tr("notify-send -a unsplash4deepin \"").append(tip).append(QDir::homePath()).append("/Pictures/unsplash\""));
#endif
}

void MainWindow::setAutoClear(bool flag)
{
    thread->setAutoClear(flag);
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(filePath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    configIni->setValue("Config/AutoClear",flag);
    delete configIni;
}

void MainWindow::setAutoStart(bool flag)
{
#ifdef Q_OS_WIN32
    QSettings  reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if(flag){
        QString strAppPath=QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        //strAppPath.replace(QChar('/'),QChar('\\'),Qt::CaseInsensitive);
        reg.setValue("wirtepad",strAppPath);
    } else {
        reg.setValue("wirtepad","");
    }
#endif
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(filePath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    configIni->setValue("Config/AutoStart",flag);
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

void MainWindow::changeSource(QString site){
    site1->setChecked(0 == site.compare("1"));
    site2->setChecked(0 == site.compare("2"));
    if(0 == site.compare("1")){
        keywords->menuAction()->setVisible(true);
    } else {
        keywords->menuAction()->setVisible(false);
    }
    thread->changeSource(site);
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(filePath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    configIni->setValue("Config/Source",site);
    delete configIni;
}

void MainWindow::changeKeyword(QString key){
    all->setChecked(key.compare("all") == 0);
    wallpaper->setChecked(key.compare("wallpaper") == 0);
    people->setChecked(key.compare("people") == 0);
    textures->setChecked(key.compare("textures") == 0);
    nature->setChecked(key.compare("nature") == 0);
    architecture->setChecked(key.compare("architecture") == 0);
    film->setChecked(key.compare("film") == 0);
    animals->setChecked(key.compare("animals") == 0);
    travel->setChecked(key.compare("travel") == 0);
    food->setChecked(key.compare("food") == 0);
    thread->keyword = key;
    QSettings *configIni = new QSettings (tr("%1/setting.ini").arg(filePath),QSettings::IniFormat);
    configIni->setIniCodec(QTextCodec::codecForName("System"));
    configIni->setValue("Config/Keyword",key);
    delete configIni;
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
