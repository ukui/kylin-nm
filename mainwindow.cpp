/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "oneconnform.h"
#include "onelancform.h"
#include "hot-spot/dlghotspotcreate.h"
#include "wireless-security/dlgconnhidwifi.h"

QString llname, lwname, hideWiFiConn;
int currentActWifiSignalLv, count_loop;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    checkSingle();

//    如果使用Qt::Popup 任务栏不显示且保留X事件如XCB_FOCUS_OUT, 但如果indicator点击鼠标右键触发，XCB_FOCUS_OUT事件依然会失效
//    如果使用Qt::ToolTip, Qt::Tool + Qt::WindowStaysOnTopHint, Qt::X11BypassWindowManagerHint等flag则会导致X事件失效
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);//QTool
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
//    this->setWindowFlags(Qt::FramelessWindowHint);

    //this->setWindowOpacity(0.95);
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    this->setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口
    //this->setStyleSheet("QWidget{border-top-left-radius:6px;border-top-right-radius:6px;}");
    this->setStyleSheet("QWidget{border:none;border-radius:6px;}");

    ui->centralWidget->setStyleSheet("#centralWidget{border:1px solid rgba(255, 255, 255, 0.05);border-radius:6px;background:rgba(19,19,20,0.97);}");

    lname = "-1";
    wname = "-1";
    llname = "-1";
    lwname = "-1";
    hideWiFiConn = "Connect to Hidden Wi-Fi Network";
    currentActWifiSignalLv = -1;
    count_loop = 0;

    this->confForm = new ConfForm();
    this->ksnm = new KSimpleNM();

    loading = new LoadingDiv(this);

    topLanListWidget = new QWidget(ui->centralWidget);
    topLanListWidget->move(41, 52);
    topLanListWidget->resize(435, 60 + 46);
    lbTopLanList = new QLabel(topLanListWidget);
    lbTopLanList->setText(tr("Ethernet Networks"));//"可用网络列表"
    lbTopLanList->resize(260, 46);
    lbTopLanList->move(12, 60);
    lbTopLanList->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.97);}");
    lbTopLanList->show();

    lbLoadDown = new QLabel(ui->centralWidget);
    lbLoadDown->move(41 + 132, 52 + 32);
    lbLoadDown->resize(65, 20);
    lbLoadUp = new QLabel(ui->centralWidget);
    lbLoadUp->move(41 + 215, 52 + 32);
    lbLoadUp->resize(65, 20);

    topWifiListWidget = new QWidget(ui->centralWidget);
    topWifiListWidget->move(41, 52);
    topWifiListWidget->resize(435, 60 + 46);
    lbTopWifiList = new QLabel(topWifiListWidget);
    lbTopWifiList->setText(tr("Wifi Networks"));//"可用网络列表"
    lbTopWifiList->resize(120, 46);
    lbTopWifiList->move(12, 60);
    lbTopWifiList->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.97);}");
    lbTopWifiList->show();
    btnAddNet = new QPushButton(topWifiListWidget);
    btnAddNet->resize(57, 14);
    btnAddNet->move(365, 77);
    btnAddNet->setText(tr("Add Hide Network"));
    btnAddNet->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0);color:rgba(107,142,235,0.97);font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
    btnAddNet->setFocusPolicy(Qt::NoFocus);
    btnAddNet->show();
    connect(btnAddNet,SIGNAL(clicked()),this,SLOT(onBtnAddNetClicked()));

    scrollAreal = new QScrollArea(ui->centralWidget);
    scrollAreal->move(41, 158);
    scrollAreal->resize(440, 356);
    scrollAreal->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreal->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    scrollAreaw = new QScrollArea(ui->centralWidget);
    scrollAreaw->move(41, 158);
    scrollAreaw->resize(440, 356);
    scrollAreaw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreaw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    lanListWidget = new QWidget(scrollAreal);
    wifiListWidget = new QWidget(scrollAreaw);
    lbLanList = new QLabel(lanListWidget);
    lbWifiList = new QLabel(wifiListWidget);

    btnOffQss = "QLabel{min-width: 37px; min-height: 37px;max-width:37px; max-height: 37px;border-radius: 4px; background-color:rgba(255,255,255,0)}";
    btnOnQss = "QLabel{min-width: 37px; min-height: 37px;max-width:37px; max-height: 37px;border-radius: 4px; background-color:rgba(61,107,229,1)}";
    btnBgOffQss = "QLabel{min-width: 48px; min-height: 22px;max-width:48px; max-height: 22px;border-radius: 10px; background-color:rgba(255,255,255,0.2)}";
    btnBgOnQss = "QLabel{min-width: 48px; min-height: 22px;max-width:48px; max-height: 22px;border-radius: 10px; background-color:rgba(61,107,229,1);}";

    //ui->centralWidget->setStyleSheet("#centralWidget{border:0px solid #626c6e;border-radius:6px;background-color:#151a1e;}");

    ui->wdgHead->setStyleSheet("#wdgHead{background:rgba(14,19,22,0.75);}");
    ui->wdgHead->hide();

    ui->lbNetwork->setStyleSheet("QLabel{font-size:20px;color:rgba(255,255,255,0.97);}");
    ui->lbNetwork->show();

    topLanListWidget->setStyleSheet("QWidget{border:none;}");
    topLanListWidget->setStyleSheet("background-color:transparent;");

    topWifiListWidget->setStyleSheet("QWidget{border:none;}");
    topWifiListWidget->setStyleSheet("background-color:transparent;");

    lbLoadUp->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.57);}");
    lbLoadDown->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.57);}");
    lbLoadUp->setText("0Kb/s");
    lbLoadDown->setText("0Kb/s");

    scrollAreal->setStyleSheet("QScrollArea{border:none;}");
    scrollAreal->viewport()->setStyleSheet("background-color:transparent;");
    scrollAreal->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background:transparent;border:0px;width:3px;}"
                                                    "QScrollBar::up-arrow:vertical{height:0px;}"
                                                    "QScrollBar::sub-line:vertical{border:0px solid;height:0px}"
                                                    "QScrollBar::sub-page:vertical{background:transparent;}"
                                                    "QScrollBar::handle:vertical{background:rgba(255,255,255,0.25);}"
                                                    "QScrollBar::handle:vertical:hover{background:rgba(255,255,255,1);}"
                                                    "QScrollBar::handle:vertical:pressed{background:rgba(255,255,255,1);}"
                                                    "QScrollBar::add-page:vertical{background:transparent;}"
                                                    "QScrollBar::add-line:vertical{border:0px solid;height:0px}"
                                                    "QScrollBar::down-arrow:vertical{height:0px;}");


    scrollAreaw->setStyleSheet("QScrollArea{border:none;}");
    scrollAreaw->viewport()->setStyleSheet("background-color:transparent;");
    scrollAreaw->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background:transparent;border:0px;width:10px;}"
                                                    "QScrollBar::up-arrow:vertical{height:0px;}"
                                                    "QScrollBar::sub-line:vertical{border:0px solid;height:0px}"
                                                    "QScrollBar::sub-page:vertical{background:transparent;}"
                                                    "QScrollBar::handle:vertical{background:rgba(255,255,255,0.25);}"
                                                    "QScrollBar::handle:vertical:hover{background:rgba(255,255,255,1);}"
                                                    "QScrollBar::handle:vertical:pressed{background:rgba(255,255,255,1);}"
                                                    "QScrollBar::add-page:vertical{background:transparent;}"
                                                    "QScrollBar::add-line:vertical{border:0px solid;height:0px}"
                                                    "QScrollBar::down-arrow:vertical{height:0px;}");

    ui->btnNetList->setStyleSheet("QPushButton{border:none;}");
    ui->btnNetList->setFocusPolicy(Qt::NoFocus);
    QString txtEthernet(tr("Ethernet"));
    ui->btnNetList->setToolTip("<span style=\"font-size:13px;border:0px;background-color:#3593b5;color:white;\">&nbsp; " + txtEthernet + " &nbsp;</span>");
    ui->lbNetListBG->setStyleSheet(btnOffQss);
    ui->lbNetListImg->setStyleSheet("QLabel{background-image:url(:/res/x/net-list-bg.svg);}");

    ui->btnWifiList->setStyleSheet("QPushButton{border:none;}");
    ui->btnWifiList->setFocusPolicy(Qt::NoFocus);
    QString txtWifi(tr("Wifi"));
    ui->btnWifiList->setToolTip("<span style=\"font-size:13px;border:0px;background-color:#3593b5;color:white;\">&nbsp; " + txtWifi + " &nbsp;</span>");
    ui->lbWifiListBG->setStyleSheet(btnOffQss);
    ui->lbWifiListImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-list-bg.svg);}");

    ui->btnNet->setStyleSheet("QPushButton{border:none;}");
    ui->btnNet->hide();
    ui->lbBtnNetBG->hide();

    ui->btnWifi->setStyleSheet("QPushButton{border:none;}");
    ui->btnWifi->setFocusPolicy(Qt::NoFocus);
    ui->lbBtnWifiBall->setStyleSheet("QLabel{min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;"
                                     "border-radius: 8px;  border:1px solid white;background:white;}");

    ui->btnHotspot->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
    ui->btnHotspot->setFocusPolicy(Qt::NoFocus);
    QString txtHotSpot(tr("HotSpot"));
    ui->btnHotspot->setToolTip("<span style=\"font-size:13px;border:0px;background-color:#3593b5;color:white;\">&nbsp; " + txtHotSpot + " &nbsp;</span>");
    ui->btnHotspot->hide();
    ui->lbHotImg->hide();
    ui->lbHotImg->setStyleSheet("QLabel{background-image:url(:/res/x/hot-spot-off.svg);}");
    ui->lbHotBG->hide();
    ui->lbHotBG->setStyleSheet(btnOffQss);

    ui->btnFlyMode->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
    ui->btnFlyMode->setFocusPolicy(Qt::NoFocus);
    QString txtFlyMode(tr("FlyMode"));
    ui->btnFlyMode->setToolTip("<span style=\"font-size:13px;border:0px;background-color:#3593b5;color:white;\">&nbsp; " + txtFlyMode + " &nbsp;</span>");
    ui->btnFlyMode->hide();
    ui->lbFlyImg->hide();
    ui->lbFlyImg->setStyleSheet("QLabel{background-image:url(:/res/x/fly-mode-off.svg);}");
    ui->lbFlyBG->hide();
    ui->lbFlyBG->setStyleSheet(btnOffQss);

    ui->btnAdvConf->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
    ui->btnAdvConf->setFocusPolicy(Qt::NoFocus);
    QString txtAdvanced(tr("Advanced"));
    ui->btnAdvConf->setToolTip("<span style=\"font-size:13px;border:0px;background-color:#3593b5;color:white;\">&nbsp; " + txtAdvanced + " &nbsp;</span>");
    ui->lbBtnConfImg->setStyleSheet("QLabel{background-image:url(:/res/x/setup.png);}");
    ui->lbBtnConfBG->hide();
    ui->lbBtnConfBG->setStyleSheet(btnOffQss);

    ui->line->hide();

    createTrayIcon();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    connect(mShowWindow,SIGNAL(triggered()),this,SLOT(on_showWindowAction()));
    connect(mAdvConf,SIGNAL(triggered()),this,SLOT(on_btnAdvConf_clicked()));

    connect(ksnm, SIGNAL(getLanListFinished(QStringList)), this, SLOT(getLanListDone(QStringList)));
    connect(ksnm, SIGNAL(getWifiListFinished(QStringList)), this, SLOT(getWifiListDone(QStringList)));

    objKyDBus = new KylinDBus(this);
    objNetSpeed = new NetworkSpeed();

    checkIsWirelessDeviceOn(); //检测无线网卡是否插入
    getInitLanSlist(); //初始化有线网列表
    //initTimer(); //初始化定时器
    //changeTimerState();//停止所有定时器
    initNetwork(); //初始化网络

    trayIcon->show();

    connect(ui->btnNetList, &QPushButton::clicked, this, &MainWindow::onBtnNetListClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkSingle()
{
    int fd = open("/tmp/kylin-nm-lock", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) { exit(1); }

    if (lockf(fd, F_TLOCK, 0)) {
        syslog(LOG_ERR, "Can't lock single file, kylin-network-manager is already running!");
        qDebug()<<"Can't lock single file, kylin-network-manager is already running!";
        exit(0);
    }
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(result);

    if(eventType != "xcb_generic_event_t"){
        return false;
    }

    xcb_generic_event_t *event = (xcb_generic_event_t*)message;

    switch (event->response_type & ~0x80){
    case XCB_FOCUS_OUT:
        this->hide();
        break;
    }

    return false;
}

//托盘管理
void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip(QString(tr("kylin-nm")));

    trayIconMenu = new QMenu(this);
    trayIconMenu->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    trayIconMenu->setStyleSheet("QMenu {background-color: rgba(19,19,20,0.95);"
                                    "border:1px solid rgba(255, 255, 255, 0.05);"
                                    "padding: 6px 1px;"
                                    "border-radius: 6px;}"
                                "QMenu::item {font-size: 14px;color: #ffffff;"
                                              "height: 36px;width: 248px;}"
                                "QMenu::separator{height:1px;background-color:rgba(19,19,20,0);"
                                        "margin-top:1px;margin-bottom:2px;}");

    mShowWindow = new QWidgetAction(trayIconMenu);
    mAdvConf = new QWidgetAction(trayIconMenu);

    widShowWindow = new QWidget();
    widAdvConf = new QWidget();

    mShowWindow->setDefaultWidget(widShowWindow);
    mAdvConf->setDefaultWidget(widAdvConf);

    init_widget_action(widShowWindow, "", tr("Show MainWindow"));
    init_widget_action(widAdvConf, ":/res/x/setup.png", tr("Advanced"));

    trayIconMenu->addAction(mShowWindow);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(mAdvConf);
    trayIcon->setContextMenu(trayIconMenu);

    // 初始化托盘所有Icon
    iconLanOnline = QIcon::fromTheme("nm-device-wired");
    iconLanOffline = QIcon::fromTheme("nm-no-connection");
    iconWifiFull = QIcon::fromTheme("nm-signal-100");
    iconWifiHigh = QIcon::fromTheme("nm-signal-75");
    iconWifiMedium = QIcon::fromTheme("nm-signal-50");
    iconWifiLow = QIcon::fromTheme("nm-signal-25");

    loadIcons.append(QIcon(":/res/s/conning-s/1.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/2.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/3.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/4.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/5.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/6.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/7.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/8.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/9.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/10.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/11.png"));
    loadIcons.append(QIcon(":/res/s/conning-s/12.png"));

    iconTimer = new QTimer(this);
    connect(iconTimer, SIGNAL(timeout()), this, SLOT(iconStep()));

    setTrayIcon(iconLanOnline);
}

void MainWindow::iconStep()
{
    if(currentIconIndex < 0){
        currentIconIndex = 11;
    }
    setTrayIcon(loadIcons.at(currentIconIndex));
    currentIconIndex --;
}

void MainWindow::setTrayIcon(QIcon icon){
    trayIcon->setIcon(icon);
}

void MainWindow::setTrayLoading(bool isLoading){
    if(isLoading){
        currentIconIndex = 11;
        iconTimer->start(60);
    }else{
        iconTimer->stop();
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    //    switch (reason) {
    //    case QSystemTrayIcon::Trigger:
    //    case QSystemTrayIcon::DoubleClick:
    //    case QSystemTrayIcon::MiddleClick:
    //        break;
    //    }

    switch(reason){
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
        //这里右键点击托盘图标无效
        handleIconClicked();
        if(this->isHidden()){
            this->showNormal();
        }else{
            this->hide();
        }
        break;
    default:
        break;
    }
}

void MainWindow::handleIconClicked()
{
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    QRect screenGeometry = qApp->primaryScreen()->geometry();

    //QDesktopWidget* pDesktopWidget = QApplication::desktop();
    //int nScreenCount = QApplication::desktop()->screenCount();
    //QRect deskRect = pDesktopWidget->availableGeometry();//可用区域
    //QRect screenRect = pDesktopWidget->screenGeometry();//屏幕区域

    //qDebug()<<"                                                  ";
    //qDebug()<<"trayIcon:"<<trayIcon->geometry();
    //qDebug()<<"screenGeometry: "<<screenGeometry;
    //qDebug()<<"availableGeometry: "<<availableGeometry;

    if (screenGeometry.width() == availableGeometry.width()){
        if (trayIcon->geometry().y() > availableGeometry.height()/2){
            //任务栏在下侧
            if (screenGeometry.height() != availableGeometry.height()) {
                this->move(availableGeometry.x() + availableGeometry.width() - this->width(), availableGeometry.height() - this->height());
            }else {
                this->move(availableGeometry.x() + availableGeometry.width() - this->width(), availableGeometry.height() - this->height() - 40);
            }
        }else{
            //任务栏在上侧
            this->move(availableGeometry.x() + availableGeometry.width() - this->width(), screenGeometry.height() - availableGeometry.height());
        }
    }
    if (screenGeometry.height() == availableGeometry.height()){
        if (trayIcon->geometry().x() > availableGeometry.width()/2){
            //任务栏在右侧
            this->move(availableGeometry.x() + availableGeometry.width() - this->width(), screenGeometry.height() - this->height());
        } else {
            //任务栏在左侧
            this->move(screenGeometry.width() - availableGeometry.width(), screenGeometry.height() - this->height());
        }
    }



//    QPoint pos = QCursor::pos();
//    QRect primaryGeometry;
//    for (QScreen *screen : qApp->screens()) {
//        if (screen->geometry().contains(pos)) {
//            primaryGeometry = screen->geometry();
//        }
//    }
//    if (primaryGeometry.isEmpty()) {
//        primaryGeometry = qApp->primaryScreen()->geometry();
//    }
//    this->move(primaryGeometry.x() + primaryGeometry.width() - this->width(), primaryGeometry.height() - this->height());
}

void MainWindow::on_showWindowAction()
{
    handleIconClicked();
    this->showNormal();
}

void MainWindow::init_widget_action(QWidget *wid, QString iconstr, QString textstr)
{
    QString style="QWidget{background:transparent;border:0px;border-radius: 0px;}\
    QWidget:hover{background-color:#34bed8ef;}\
    QWidget:pressed{background-color:#3abed8ef;}";

    QHBoxLayout* layout=new QHBoxLayout(wid);
    wid->setLayout(layout);
    wid->setFixedSize(248, 36);
    wid->setStyleSheet(style);
    wid->setFocusPolicy(Qt::NoFocus);

    if(!iconstr.isEmpty()) {
        QLabel* labelicon=new QLabel(wid);
        labelicon->setFixedSize(16, 16);
        labelicon->move(10, 12);
        QPixmap *pixmap = new QPixmap(":/res/x/setup.png");
        pixmap->scaled(labelicon->size(), Qt::KeepAspectRatio);
        labelicon->setScaledContents(true);
        labelicon->setPixmap(*pixmap);

//        QLabel* labelicon=new QLabel(wid);
//        QSvgRenderer* svg=new QSvgRenderer(wid); //#include <QSvgRenderer>
//        svg->load(iconstr);
//        QPixmap* pixmap=new QPixmap(22, 22);
//        pixmap->fill(Qt::transparent);
//        QPainter p(pixmap);
//        svg->render(&p);
//        labelicon->setPixmap(*pixmap);
//        labelicon->setFixedSize(pixmap->size());
//        labelicon->move(10, 14);
//        labelicon->setAlignment(Qt::AlignCenter);
//        labelicon->setStyleSheet("QLabel{background:transparent;border:0px;}");
//        labelicon->setStyleSheet("QLabel{background-image:url(:/res/x/setup.png);}");
//        layout->addWidget(labelicon);
    }

    QLabel* labeltext=new QLabel(wid);
    labeltext->setStyleSheet("background:transparent;border:0px;color:rgba(255,255,255,0.91);font-size:14px;");
    QByteArray textbyte=textstr.toLocal8Bit();
    char* text=textbyte.data();
    labeltext->setText(tr(text));
    labeltext->adjustSize();
    layout->addWidget(labeltext);

    iconstr = ""; //用QSvgRenderer的话这句要注释掉
    if(!iconstr.isEmpty()) {
        layout->setContentsMargins(10,0,wid->width()-16-labeltext->width()-20,0);
        layout->setSpacing(5);
    }
    else {
        layout->setContentsMargins(36,0,0,0);
    }
}

//加载动画
void MainWindow::startLoading()
{
    loading->startLoading();
    setTrayLoading(true);
}

void MainWindow::stopLoading()
{
    loading->stopLoading();
    setTrayLoading(false);
    getActiveInfo();
}

//获取当前连接的网络和状态并设置图标
void MainWindow::getActiveInfo()
{
    QString actLanName = "--";
    QString actWifiName = "--";

    activecon *act = kylin_network_get_activecon_info();
    int index = 0;
    while(act[index].con_name != NULL){
        if(QString(act[index].type) == "ethernet"){
            actLanName = QString(act[index].con_name);
        }
        if(QString(act[index].type) == "wifi"){
            actWifiName = QString(act[index].con_name);
        }
        index ++;
    }

    //ukui2.0中获取currentActWifiSignalLv的值
    //QList<OneConnForm *> wifiList = wifiListWidget->findChildren<OneConnForm *>();
    //for(int i = 0; i < wifiList.size(); i ++){
    //    OneConnForm *ocf = wifiList.at(i);
    //    if(ocf->isActive == true){
    //        currentActWifiSignalLv = ocf->signalLv;
    //        break;
    //    }
    //}

    //ukui3.0中获取currentActWifiSignalLv的值
    if(activeWifiSignalLv > 75){
        currentActWifiSignalLv = 1;
    }else if(activeWifiSignalLv > 55 && activeWifiSignalLv <= 75){
        currentActWifiSignalLv = 2;
    }else if(activeWifiSignalLv > 35 && activeWifiSignalLv <= 55){
        currentActWifiSignalLv = 3;
    }else if( activeWifiSignalLv <= 35){
        currentActWifiSignalLv = 4;
    }

    // 设置图标
    if(actWifiName != "--"){
        switch (currentActWifiSignalLv) {
        case 1:
            setTrayIcon(iconWifiFull);
            break;
        case 2:
            setTrayIcon(iconWifiHigh);
            break;
        case 3:
            setTrayIcon(iconWifiMedium);
            break;
        case 4:
            setTrayIcon(iconWifiLow);
            break;
        default:
            setTrayIcon(iconWifiFull);
            break;
        }
    }else{
        if(actLanName != "--"){
            setTrayIcon(iconLanOnline);
        }else{
            setTrayIcon(iconLanOffline);
        }
    }
}

//初始化有线网列表
void MainWindow::getInitLanSlist()
{
    oldLanSlist.append("TYPE      DEVICE  NAME           ");
    QString strSlist;

    system("nmcli connection show>/tmp/kylin-nm-connshow");
    QFile file("/tmp/kylin-nm-connshow");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        syslog(LOG_ERR, "Can't open the file /tmp/kylin-nm-connshow!");
        qDebug()<<"Can't open the file /tmp/kylin-nm-connshow!";
    }
    QString txt = file.readAll();
    QStringList txtLine = txt.split("\n");
    file.close();
    foreach (QString line, txtLine) {
        if(line.indexOf("ethernet") != -1){
            QStringList subLine = line.split(" ");
            if (subLine[1].size() == 1){
                strSlist =  "ethernet  --      " + subLine[0]+ " " + subLine[1] + "   ";
            }else{strSlist =  "ethernet  --      " + subLine[0] + "   "; }
            oldLanSlist.append(strSlist);
        }
    }
}

//初始化定时器
void MainWindow::initTimer()
{
    checkIfLanConnect = new QTimer(this);
    checkIfLanConnect->setTimerType(Qt::PreciseTimer);
    QObject::connect(checkIfLanConnect, SIGNAL(timeout()), this, SLOT(on_isLanConnect()));
    checkIfLanConnect->start(2000);

    checkIfWifiConnect = new QTimer(this);
    checkIfWifiConnect->setTimerType(Qt::PreciseTimer);
    QObject::connect(checkIfWifiConnect, SIGNAL(timeout()), this, SLOT(on_isWifiConnect()));
    checkIfWifiConnect->start(2000);

    checkIfNetworkOn = new QTimer(this);
    checkIfNetworkOn->setTimerType(Qt::PreciseTimer);
    QObject::connect(checkIfNetworkOn, SIGNAL(timeout()), this, SLOT(on_isNetOn()));
    checkIfNetworkOn->start(2000);
}
void MainWindow::changeTimerState()
{
    if (checkIfLanConnect->isActive()){
        checkIfLanConnect->stop();
    }

    if (checkIfNetworkOn->isActive()){
        checkIfNetworkOn->stop();
    }

    if (checkIfWifiConnect->isActive()){
        checkIfWifiConnect->stop();
    }
}

//由kylin-dbus-interface.cpp调用
void MainWindow::onPhysicalCarrierChanged(bool flag)
{
    is_stop_check_net_state = 1;
    this->startLoading();
    if (flag){
        syslog(LOG_DEBUG,"wired physical cable is already plug in");
        wiredCableUpTimer->start(2000);
    } else{
        syslog(LOG_DEBUG,"wired physical cable is already plug out");
        wiredCableDownTimer->start(6000);
    }
}
void MainWindow::onCarrierUpHandle()
{
    wiredCableUpTimer->stop();
    BackThread *up_bt = new BackThread();
    up_bt->lanDelete();
    sleep(1);
    up_bt->lanDelete();
    sleep(1);
    up_bt->lanDelete();
    up_bt->deleteLater();

    this->stopLoading();
    onBtnNetListClicked(1);
    is_stop_check_net_state = 0;
}
void MainWindow::onCarrierDownHandle()
{
    wiredCableDownTimer->stop();
    this->stopLoading();
    onBtnNetListClicked(0);
    is_stop_check_net_state = 0;
}
void MainWindow::onDeleteLan()
{
    deleteLanTimer->stop();
    BackThread *btn_bt = new BackThread();
    btn_bt->lanDelete();
    sleep(1);
    btn_bt->lanDelete();
    sleep(1);
    btn_bt->lanDelete();
    btn_bt->deleteLater();

    this->stopLoading();
    onBtnNetListClicked(0);
    is_stop_check_net_state = 0;
}

//无线网卡插拔处理
void MainWindow::onWirelessDeviceAdded(QDBusObjectPath objPath)
{
    //仅处理无线网卡插入情况
    syslog(LOG_DEBUG,"wireless device is already plug in");
//    KylinDBus kDBus1;
    objKyDBus->isWirelessCardOn = false;
    objKyDBus->getObjectPath();
    if (objKyDBus->isWirelessCardOn) {
        is_wireless_adapter_ready = 1;
    } else {
        is_wireless_adapter_ready = 0;
    }
    on_btnWifi_clicked();
}
void MainWindow::onWirelessDeviceRemoved(QDBusObjectPath objPath)
{
    //仅处理无线网卡拔出情况
    syslog(LOG_DEBUG,"wireless device is already plug out");
//    KylinDBus kDBus2;
    if (objKyDBus->wirelessPath.path() == objPath.path()){
        is_wireless_adapter_ready = 0;
    }
    on_btnWifi_clicked();
}
void MainWindow::checkIsWirelessDeviceOn()
{
    //启动时判断是否有无线网卡
//    KylinDBus kDBus3;
    if (objKyDBus->isWirelessCardOn) {
        is_wireless_adapter_ready = 1;
    } else {
        is_wireless_adapter_ready = 0;
    }
}

void MainWindow::getLanBandWidth()
{
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    lname = iface->lname;

    mwBandWidth = bt->execChkLanWidth(lname);
}

// 初始化网络
void MainWindow::initNetwork()
{
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    wname = iface->wname;
    lwname = iface->wname;
    lname = iface->lname;
    llname = iface->lname;

    mwBandWidth = bt->execChkLanWidth(lname);

    // 开关状态
    qDebug()<<"lstate ="<<iface->lstate<<"    wstate ="<<iface->wstate ;
    syslog(LOG_DEBUG, "state of switch:   lstate =%d    wstate =%d", iface->lstate, iface->wstate);

    ui->lbBtnNetBG->setStyleSheet(btnOnQss);
    if(iface->wstate == 0 || iface->wstate == 1){
        ui->lbBtnWifiBG->setStyleSheet(btnBgOnQss);
        ui->lbBtnWifiBall->move(438, 22);
    }else{
        ui->lbBtnWifiBG->setStyleSheet(btnBgOffQss);
        ui->lbBtnWifiBall->move(412, 22);
    }

    // 初始化网络列表
    if(iface->wstate != 2){
        if (iface->wstate == 0){
            connWifiDone(3);
        }else{
            if (iface->lstate == 0){
                connLanDone(3);
            }else{
                //syslog(LOG_DEBUG, "Launch kylin-nm, will check if Lan or Wifi connected circularly");
                //qDebug()<<"连接状态：启动kylin-nm, 即将循环检测 Lan或Wifi 是否连接";
                //changeTimerState();
                //checkIfNetworkOn->start(8000);
            }
        }
        on_btnWifiList_clicked();

        ui->btnNetList->setStyleSheet("QPushButton{border:0px solid rgba(255,255,255,0);background-color:rgba(255,255,255,0);}");
        ui->btnWifiList->setStyleSheet("QPushButton{border:none;}");
    } else {
        if(iface->lstate != 2){
            if (iface->lstate == 0) {
                connLanDone(3);
            } else{
                //syslog(LOG_DEBUG, "Launch kylin-nm, will check if Lan or Wifi connected circularly");
                //qDebug()<<"连接状态：启动kylin-nm, 即将循环检测 Lan或Wifi 是否连接";
                //changeTimerState();
                //checkIfNetworkOn->start(8000);
            }

            onBtnNetListClicked();

            ui->btnNetList->setStyleSheet("QPushButton{border:0px solid rgba(255,255,255,0);background-color:rgba(255,255,255,0);}");
            ui->btnWifiList->setStyleSheet("QPushButton{border:none;}");
        }else {
            BackThread *m_bt = new BackThread();
            IFace *m_iface = m_bt->execGetIface();
            qDebug()<<"m_lstate ="<<m_iface->lstate<<"    m_wstate ="<<m_iface->wstate ;

            m_bt->lanDelete();
            sleep(1);
            m_bt->lanDelete();
            sleep(1);
            m_bt->lanDelete();
            delete m_iface;
            m_bt->deleteLater();

            system("nmcli networking on");

            onBtnNetListClicked();

            ui->btnNetList->setStyleSheet("QPushButton{border:0px solid rgba(255,255,255,0);background-color:rgba(255,255,255,0);}");
            ui->btnWifiList->setStyleSheet("QPushButton{border:none;}");

            //disNetDone();
        }
    }

    //循环检测wifi列表的变化，可用于更新wifi列表
    checkWifiListChanged = new QTimer(this);
    checkWifiListChanged->setTimerType(Qt::PreciseTimer);
    QObject::connect(checkWifiListChanged, SIGNAL(timeout()), this, SLOT(on_checkWifiListChanged()));
    checkWifiListChanged->start(7000);
    //网线插入时定时执行
    wiredCableUpTimer = new QTimer(this);
    wiredCableUpTimer->setTimerType(Qt::PreciseTimer);
    QObject::connect(wiredCableUpTimer, SIGNAL(timeout()), this, SLOT(onCarrierUpHandle()));
    //网线拔出时定时执行
    wiredCableDownTimer = new QTimer(this);
    wiredCableDownTimer->setTimerType(Qt::PreciseTimer);
    QObject::connect(wiredCableDownTimer, SIGNAL(timeout()), this, SLOT(onCarrierDownHandle()));
    //定时处理异常网络，即当点击Lan列表按钮时，若lstate=2，但任然有有线网连接的情况
    deleteLanTimer = new QTimer(this);
    deleteLanTimer->setTimerType(Qt::PreciseTimer);
    QObject::connect(deleteLanTimer, SIGNAL(timeout()), this, SLOT(onDeleteLan()));
    //定时获取网速
    setNetSpeed = new QTimer(this);
    setNetSpeed->setTimerType(Qt::PreciseTimer);
    QObject::connect(setNetSpeed, SIGNAL(timeout()), this, SLOT(on_setNetSpeed()));
    setNetSpeed->start(3000);
}

bool MainWindow::checkLanOn()
{
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    if(iface->lstate == 2){
        return false;
    }else{
        return true;
    }

    delete iface;
    bt->deleteLater();
}

bool MainWindow::checkWlOn()
{
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    if(iface->wstate == 2){
        return false;
    }else{
        return true;
    }
}

// 获取lan列表回调
void MainWindow::getLanListDone(QStringList slist)
{
    if (this->ksnm->isUseOldLanSlist){
        slist = oldLanSlist;
        this->ksnm->isUseOldLanSlist = false;
    }

    // 清空top列表
    delete topLanListWidget;
    topLanListWidget = new QWidget(ui->centralWidget);
    topLanListWidget->move(41, 52);
    topLanListWidget->resize(435, 60 + 46);
    lbTopLanList = new QLabel(topLanListWidget);
    lbTopLanList->setText(tr("Ethernet Networks"));//"可用网络列表"
    lbTopLanList->resize(260, 46);
    lbTopLanList->move(12, 60);
    lbTopLanList->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.97);}");
    lbTopLanList->show();
    // 清空lan列表
    lanListWidget = new QWidget(scrollAreal);
    lanListWidget->resize(440, 60 + 108);
    scrollAreal->setWidget(lanListWidget);
    scrollAreal->move(41, 158);

    // 获取当前连接的lan name
    QString actLanName = "--";
    activecon *act = kylin_network_get_activecon_info();
    int index = 0;
    while(act[index].con_name != NULL){
        if(QString(act[index].type) == "ethernet"){
            actLanName = QString(act[index].con_name);
            break;
        }
        index ++;
    }

    // 若当前lan name为"--"，设置OneConnForm
    OneLancForm *ccf = new OneLancForm(topLanListWidget, this, confForm, ksnm);
    if(actLanName == "--"){
        ccf->setName(tr("Not connected"));//"当前未连接任何 以太网"
        ccf->setIcon(false);
        ccf->setConnedString(1, tr("Disconnected"));//"未连接"
        ccf->isConnected = false;
        ccf->setTopItem(false);
    }
    ccf->setAct(true);
    ccf->move(0, 0);
    ccf->show();

    // 填充可用网络列表
    QString headLine = slist.at(0);
    headLine = headLine.trimmed();
    int indexDevice = headLine.indexOf("DEVICE");
    int indexName = headLine.indexOf("NAME");

    for(int i = 1, j = 0; i < slist.size(); i ++) {
        QString line = slist.at(i);
        QString ltype = line.mid(0, indexDevice).trimmed();
        QString nname = line.mid(indexName).trimmed();

        if(ltype != "wifi" && ltype != "" && ltype != "--"){
            // 当前连接的lan
            objKyDBus->getLanIp(nname);
            if(nname == actLanName){
                if (mwBandWidth == "Unknown!") { getLanBandWidth(); }

                connect(ccf, SIGNAL(selectedOneLanForm(QString)), this, SLOT(oneTopLanFormSelected(QString)));
                connect(ccf, SIGNAL(disconnActiveLan()), this, SLOT(activeLanDisconn()));
                ccf->setName(nname);
                ccf->setIcon(true);
                ccf->setLanInfo(objKyDBus->dbusLanIpv4, objKyDBus->dbusLanIpv6, mwBandWidth, objKyDBus->dbusLanMac);
                ccf->setConnedString(1, tr("Connected"));//"已连接"
                ccf->isConnected = true;
                ccf->setTopItem(false);
                currSelNetName = "";
                syslog(LOG_DEBUG, "already insert an active lannet in the top of lan list");
            }else{
                lanListWidget->resize(440, lanListWidget->height() + 60);

                OneLancForm *ocf = new OneLancForm(lanListWidget, this, confForm, ksnm);
                connect(ocf, SIGNAL(selectedOneLanForm(QString)), this, SLOT(oneLanFormSelected(QString)));
                ocf->setName(nname);
                ocf->setIcon(true);
                ocf->setLine(true);
                ocf->setLanInfo(objKyDBus->dbusLanIpv4, objKyDBus->dbusLanIpv6, tr("Disconnected"), objKyDBus->dbusLanMac);
                ocf->setConnedString(0, tr("Disconnected"));//"未连接"
                ocf->move(0, j * 60);
                ocf->show();

                j ++;
            }
        }
    }

    QList<OneLancForm *> itemList = lanListWidget->findChildren<OneLancForm *>();
    int n = itemList.size();
    if (n >= 1){
        OneLancForm *lastItem = itemList.at(n-1);
        lastItem->setLine(false);
    }

    this->lanListWidget->show();
    this->topLanListWidget->show();
    this->wifiListWidget->hide();
    this->topWifiListWidget->hide();

    this->stopLoading();
    oldLanSlist = slist;
}

// 获取wifi列表回调
void MainWindow::getWifiListDone(QStringList slist)
{
    qDebug()<<"debug: oldWifiSlist.size()="<<oldWifiSlist.size()<<"   slist.size()="<<slist.size();

    if (is_update_wifi_list == 0){
        loadWifiListDone(slist);
    } else {
        updateWifiListDone(slist);
        is_update_wifi_list = 0;
    }
    oldWifiSlist = slist;
}
// 加载wifi列表
void MainWindow::loadWifiListDone(QStringList slist)
{
    //清空top列表
    delete topWifiListWidget;
    topWifiListWidget = new QWidget(ui->centralWidget);
    topWifiListWidget->move(41, 52);
    topWifiListWidget->resize(435, 60 + 46);
    lbTopWifiList = new QLabel(topWifiListWidget);
    lbTopWifiList->setText(tr("Wifi Networks"));//"可用网络列表"
    lbTopWifiList->resize(260, 46);
    lbTopWifiList->move(12, 60);
    lbTopWifiList->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.97);}");
    lbTopWifiList->show();
    btnAddNet = new QPushButton(topWifiListWidget);
    btnAddNet->resize(57, 14);
    btnAddNet->move(365, 77);
    btnAddNet->setText(tr("Add Hide Network"));
    btnAddNet->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0);color:rgba(107,142,235,0.97);font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
    btnAddNet->setFocusPolicy(Qt::NoFocus);
    btnAddNet->show();
    connect(btnAddNet,SIGNAL(clicked()),this,SLOT(onBtnAddNetClicked()));

    // 清空wifi列表
    wifiListWidget = new QWidget(scrollAreaw);
    wifiListWidget->resize(440, /*60 + */88);
    scrollAreaw->setWidget(wifiListWidget);
    scrollAreaw->move(41, 158);

    // 获取当前连接的wifi name
    QString actWifiName = "--";
    activecon *act = kylin_network_get_activecon_info();
    int index = 0;
    while(act[index].con_name != NULL){
        if(QString(act[index].type) == "wifi"){
            actWifiName = QString(act[index].con_name);
            break;
        }
        index ++;
    }

    // 根据当前连接的wifi 设置OneConnForm
    OneConnForm *ccf = new OneConnForm(topWifiListWidget, this, confForm, ksnm);
    if(actWifiName == "--"){
        ccf->setName(tr("Not connected"));//"当前未连接任何 Wifi"
        ccf->setSignal("0", "--");
        activeWifiSignalLv = 0;
        ccf->setConnedString(1, tr("Disconnected"), "");//"未连接"
        ccf->isConnected = false;
        ccf->setTopItem(false);
    }
    ccf->setAct(true);
    ccf->move(0, 0);
    ccf->show();

    // 填充可用网络列表
    QString headLine = slist.at(0);
    headLine = headLine.trimmed();
    int indexRate = headLine.indexOf("RATE");
    int indexSecu = headLine.indexOf("SECURITY");
    int indexName = headLine.indexOf("SSID");

    QStringList wnames;
    int count = 0;
    for(int i = 1, j = 0; i < slist.size(); i ++) {
        QString line = slist.at(i);
        QString wsignal = line.mid(0, indexRate).trimmed();
        QString wrate = line.mid(indexRate, indexSecu - indexRate).trimmed();
        QString wsecu = line.mid(indexSecu, indexName - indexSecu).trimmed();
        QString wname = line.mid(indexName).trimmed();

        bool isContinue = false;
        foreach (QString addName, wnames) {
            // 重复的网络名称，跳过不处理
            if(addName == wname){ isContinue = true; }
        }
        if(isContinue){ continue; }

        if(wname != "" && wname != "--"){
            // 当前连接的wifi
            if(wname == actWifiName){
                connect(ccf, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneTopWifiFormSelected(QString)));
                connect(ccf, SIGNAL(disconnActiveWifi()), this, SLOT(activeWifiDisconn()));
                ccf->setName(wname);
                ccf->setRate(wrate);
                ccf->setSignal(wsignal, wsecu);
                activeWifiSignalLv = wsignal.toInt();
                objKyDBus->getActWifiMac(wname);
                ccf->setWifiInfo(wsecu, wsignal, objKyDBus->dbusWifiMac);
                ccf->setConnedString(1, tr("Connected"), wsecu);//"已连接"
                ccf->isConnected = true;
                ccf->setTopItem(false);
                currSelNetName = "";

                syslog(LOG_DEBUG, "already insert an active wifi in the top of wifi list");
            }else{
                wifiListWidget->resize(440, wifiListWidget->height() + 60);

                OneConnForm *ocf = new OneConnForm(wifiListWidget, this, confForm, ksnm);
                connect(ocf, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
                ocf->setName(wname);
                ocf->setRate(wrate);
                ocf->setLine(true);
                ocf->setSignal(wsignal, wsecu);
                objKyDBus->getWifiMac(wname, i-1);
                ocf->setWifiInfo(wsecu, wsignal, objKyDBus->dbusWifiMac);
                ocf->setConnedString(0, tr("Disconnected"), wsecu);
                ocf->move(0, j * 60);
                ocf->show();

                j ++;
                count ++;
            }

            wnames.append(wname);
        }
    }

    QList<OneConnForm *> itemList = wifiListWidget->findChildren<OneConnForm *>();
    int n = itemList.size();
    if (n >= 1){
        OneConnForm *lastItem = itemList.at(n-1);
        lastItem->setLine(false);
    }

    this->lanListWidget->hide();
    this->topLanListWidget->hide();
    this->wifiListWidget->show();
    this->topWifiListWidget->show();
    is_stop_check_net_state = 0;
    this->stopLoading();
}
// 更新wifi列表
void MainWindow::updateWifiListDone(QStringList slist)
{
    if (this->ksnm->isExecutingGetLanList){ return;}

    //获取表头信息
    QString lastHeadLine = oldWifiSlist.at(0);
    lastHeadLine = lastHeadLine.trimmed();
    int lastIndexName = lastHeadLine.indexOf("SSID");

    QString headLine = slist.at(0);
    headLine = headLine.trimmed();
    int indexRate = headLine.indexOf("RATE");
    int indexSecu = headLine.indexOf("SECURITY");
    int indexName = headLine.indexOf("SSID");

    //列表中去除已经减少的wifi
    for (int i=1; i<oldWifiSlist.size(); i++){
        QString line = oldWifiSlist.at(i);
        QString lastWname = line.mid(lastIndexName).trimmed();
        for (int j=1; j<slist.size(); j++){
            QString line = slist.at(j);
            QString wname = line.mid(indexName).trimmed();

            if (lastWname == wname){break;} //在slist最后之前找到了lastWname，则停止
            if (j == slist.size()-1){
                syslog(LOG_DEBUG, "Will remove a Wi-Fi, it's name is: %s", lastWname.toUtf8().data());
                qDebug()<<"Will remove a Wi-Fi, it's name is: "<<lastWname;
                QList<OneConnForm *> wifiList = wifiListWidget->findChildren<OneConnForm *>();
                for(int pos = 0; pos < wifiList.size(); pos ++){
                    OneConnForm *ocf = wifiList.at(pos);
                    if (ocf->getName() == lastWname){
                        if (ocf->isActive == true){break;
                        } else { delete ocf;
                            //删除元素下面的的所有元素上移
                            for(int after_pos = pos+1; after_pos < wifiList.size(); after_pos ++){
                                OneConnForm *after_ocf = wifiList.at(after_pos);
                                if (lastWname == currSelNetName) {after_ocf->move(0, after_ocf->y() - 60 - 88);}
                                else {after_ocf->move(0, after_ocf->y() - 60);}
                            }
                            wifiListWidget->resize(440, wifiListWidget->height() - 60);
                            break;
                        }
                    }
                }

            } //end if (j == slist.size()-1)
        } //end (int j=1; j<slist.size(); j++)
    }

    //列表中插入新增的wifi
    QStringList wnames;
    int count = 0;
    for(int i = 1; i < slist.size(); i++){
        QString line = slist.at(i);
        QString wsignal = line.mid(0, indexRate).trimmed();
        QString wrate = line.mid(indexRate, indexSecu - indexRate).trimmed();
        QString wsecu = line.mid(indexSecu, indexName - indexSecu).trimmed();
        QString wname = line.mid(indexName).trimmed();

        if(wname == "" || wname == "--"){continue;}

        bool isContinue = false;
        foreach (QString addName, wnames) {
            // 重复的网络名称，跳过不处理
            if(addName == wname){isContinue = true;}
        }
        if(isContinue){continue;}
        wnames.append(wname);

        for (int j=1; j < oldWifiSlist.size(); j++){
            QString line = oldWifiSlist.at(j);
            QString lastWname = line.mid(lastIndexName).trimmed();

            if (lastWname == wname){break;} //上一次的wifi列表已经有名为wname的wifi，则停止
            if (j == oldWifiSlist.size()-1){ //到lastSlist最后一个都没找到，执行下面流程
                syslog(LOG_DEBUG, "Will add a Wi-Fi, it's name is: %s", wname.toUtf8().data());
                qDebug()<<"Will add a Wi-Fi, it's name is: "<<wname;
                QList<OneConnForm *> wifiList = wifiListWidget->findChildren<OneConnForm *>();
                int n = wifiList.size();
                int posY = 0;
                if (n >= 1){
                    OneConnForm *lastOcf = wifiList.at(n-1);
                    lastOcf->setLine(true);
                    posY = lastOcf->y()+60;
                }

                wifiListWidget->resize(440, wifiListWidget->height() + 60);
                OneConnForm *addItem = new OneConnForm(wifiListWidget, this, confForm, ksnm);
                connect(addItem, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
                addItem->setName(wname);
                addItem->setRate(wrate);
                addItem->setLine(false);
                addItem->setSignal(wsignal, wsecu);
                objKyDBus->getWifiMac(wname, i-1);
                addItem->setWifiInfo(wsecu, wsignal, objKyDBus->dbusWifiMac);
                addItem->setConnedString(0, tr("Disconnected"), wsecu);//"未连接"
                addItem->move(0, posY);
                addItem->show();

                count += 1;
            }
        }
    }

    this->lanListWidget->hide();
    this->topLanListWidget->hide();
    this->wifiListWidget->show();
    this->topWifiListWidget->show();
    this->stopLoading();
}

void MainWindow::on_btnNet_clicked()
{
    if(checkLanOn()){
        QThread *t = new QThread();
        BackThread *bt = new BackThread();
        bt->moveToThread(t);
        connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
        connect(t, SIGNAL(started()), bt, SLOT(execDisNet()));
        connect(bt, SIGNAL(disNetDone()), this, SLOT(disNetDone()));
        connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
        t->start();

    }else{
        is_stop_check_net_state = 1;
        QThread *t = new QThread();
        BackThread *bt = new BackThread();
        bt->moveToThread(t);
        connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
        connect(t, SIGNAL(started()), bt, SLOT(execEnNet()));
        connect(bt, SIGNAL(enNetDone()), this, SLOT(enNetDone()));
        connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
        t->start();
    }

    this->startLoading();
}

void MainWindow::on_btnWifi_clicked()
{
    //当连接上无线网卡时才能打开wifi开关
    if(is_wireless_adapter_ready == 1){
        // 网络开关关闭时，点击Wifi开关时，程序先打开有线开关
        if(checkLanOn()){
            if(checkWlOn()){
                QThread *t = new QThread();
                BackThread *bt = new BackThread();
                bt->moveToThread(t);
                connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
                connect(t, SIGNAL(started()), bt, SLOT(execDisWifi()));
                connect(bt, SIGNAL(disWifiDone()), this, SLOT(disWifiDone()));
                connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
                t->start();
                this->startLoading();
            }else{
                if (is_fly_mode_on == 0){
                    on_btnWifiList_clicked();
                    is_stop_check_net_state = 1;
                    QThread *t = new QThread();
                    BackThread *bt = new BackThread();
                    bt->moveToThread(t);
                    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
                    connect(t, SIGNAL(started()), bt, SLOT(execEnWifi()));
                    connect(bt, SIGNAL(enWifiDone()), this, SLOT(enWifiDone()));
                    connect(bt, SIGNAL(launchLanDone()), this, SLOT(launchLanDone()));
                    connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
                    t->start();
                    this->startLoading();
                }
            }
        }else{
            if(!checkWlOn()){
                QString txt(tr("keep wired network switch is on before turning on wireless switch"));
                QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
                system(cmd.toUtf8().data());

                on_btnWifiList_clicked();
                is_stop_check_net_state = 1;
                QThread *t = new QThread();
                BackThread *bt = new BackThread();
                bt->moveToThread(t);
                connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
                connect(t, SIGNAL(started()), bt, SLOT(execEnWifi()));
                connect(bt, SIGNAL(enWifiDone()), this, SLOT(enWifiDone()));
                connect(bt, SIGNAL(launchLanDone()), this, SLOT(launchLanDone()));
                connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
                t->start();
            }
            this->startLoading();
        }
        //this->startLoading();
    } else {
        QString txt(tr("please insert the wireless network adapter"));
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
        system(cmd.toUtf8().data());
        keepDisWifiState();
    }
}

void MainWindow::onBtnNetListClicked(int flag)
{
//    if (this->ksnm->isExecutingGetWifiList ){
//        qDebug()<<"executing update Wifi list now, try again";
//        on_btnWifiList_pressed(); //当正在更新wifi列表时，点击无效
//        QString text(tr("update Wi-Fi list now, click again")); //"正在更新 Wi-Fi列表 请再次点击"
//        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + text + "...' -t 3800";
//        system(cmd.toUtf8().data());
//        return;
//    }

    this->is_btnNetList_clicked = 1;
    this->is_btnWifiList_clicked = 0;

    ui->lbNetListBG->setStyleSheet(btnOnQss);
    ui->lbWifiListBG->setStyleSheet(btnOffQss);

    ui->lbNetwork->setText("有线网络");
    ui->btnWifi->hide();
    ui->lbBtnWifiBG->hide();
    ui->lbBtnWifiBall->hide();

    // 强行设置为打开
    if(flag == 1){
        this->startLoading();
        this->ksnm->execGetLanList();
        this->scrollAreal->show();
        this->topLanListWidget->show();
        this->scrollAreaw->hide();
        this->topWifiListWidget->hide();
        on_btnNetList_pressed();
        return;
    }

    if(checkLanOn()){
        this->startLoading();
        this->ksnm->execGetLanList();
    } else {
        QString strSlist;
        system("nmcli connection show -active>/tmp/kylin-nm-connshow");
        QFile file("/tmp/kylin-nm-connshow");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            syslog(LOG_DEBUG, "Can't open the file /tmp/kylin-nm-connshow!");
            qDebug()<<"Can't open the file /tmp/kylin-nm-connshow!";
        }
        QString txt = file.readAll();
        if (txt.indexOf("ethernet") != -1){
            QString txt(tr("Abnormal connection exist, program will delete it"));//仍然有连接异常的有线网络，断开异常连接的网络
            QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "...' -t 3800";
            system(cmd.toUtf8().data());

            is_stop_check_net_state = 1;
            this->startLoading();
            deleteLanTimer->start(1000);
            return;
        }

        // 清空top列表
        delete topLanListWidget;
        topLanListWidget = new QWidget(ui->centralWidget);
        topLanListWidget->move(41, 52);
        topLanListWidget->resize(435, 60 + 46);
        lbTopLanList = new QLabel(topLanListWidget);
        lbTopLanList->setText(tr("Ethernet Networks"));//"可用网络列表"
        lbTopLanList->resize(260, 46);
        lbTopLanList->move(12, 60);
        lbTopLanList->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.97);}");
        lbTopLanList->show();

        // 清空lan列表
        lanListWidget = new QWidget(scrollAreal);
        lanListWidget->resize(440, 60 + 108);
        scrollAreal->setWidget(lanListWidget);
        scrollAreal->move(41, 158);

        // 当前连接的lan
        OneLancForm *ccf = new OneLancForm(topLanListWidget, this, confForm, ksnm);
        ccf->setName(tr("Not connected"));//"当前未连接任何 以太网"
        ccf->setIcon(false);
        ccf->setConnedString(1, tr("Disconnected"));//"未连接"
        ccf->setAct(true);
        ccf->move(0, 0);
        ccf->show();

        this->lanListWidget->show();
        this->wifiListWidget->hide();
    }

    this->scrollAreal->show();
    this->topLanListWidget->show();
    this->scrollAreaw->hide();
    this->topWifiListWidget->hide();
    on_btnNetList_pressed();
}

void MainWindow::on_btnWifiList_clicked()
{
    this->is_btnWifiList_clicked = 1;
    this->is_btnNetList_clicked = 0;

    ui->lbNetListBG->setStyleSheet(btnOffQss);
    ui->lbWifiListBG->setStyleSheet(btnOnQss);

    ui->lbNetwork->setText("无线网络");
    ui->btnWifi->show();
    ui->lbBtnWifiBG->show();
    ui->lbBtnWifiBall->show();

    if(checkWlOn()){
        this->startLoading();
        this->ksnm->execGetWifiList();
    }else{
        delete topWifiListWidget;
        topWifiListWidget = new QWidget(ui->centralWidget);
        topWifiListWidget->move(41, 52);
        topWifiListWidget->resize(435, 60 + 46);
        lbTopWifiList = new QLabel(topWifiListWidget);
        lbTopWifiList->setText(tr("Wifi Networks"));//"可用网络列表"
        lbTopWifiList->resize(260, 46);
        lbTopWifiList->move(12, 60);
        lbTopWifiList->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.97);}");
        lbTopWifiList->show();
        btnAddNet = new QPushButton(topWifiListWidget);
        btnAddNet->resize(57, 14);
        btnAddNet->move(365, 77);
        btnAddNet->setText(tr("Add Hide Network"));
        btnAddNet->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0);color:rgba(107,142,235,0.97);font-size:14px;}"
                                   "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0);}"
                                   "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
        btnAddNet->setFocusPolicy(Qt::NoFocus);
        btnAddNet->show();
        connect(btnAddNet,SIGNAL(clicked()),this,SLOT(onBtnAddNetClicked()));

        // 清空wifi列表
        wifiListWidget = new QWidget(scrollAreaw);
        wifiListWidget->resize(440, /*60 + */88);
        scrollAreaw->setWidget(wifiListWidget);
        scrollAreaw->move(41, 158);

        // 当前连接的wifi
        OneConnForm *ccf = new OneConnForm(topWifiListWidget, this, confForm, ksnm);
        ccf->setName(tr("Not connected"));//"当前未连接任何 Wifi"
        ccf->setSignal("0", "--");
        ccf->setRate("0");
        ccf->setConnedString(1, tr("Disconnected"), "");//"未连接"
        ccf->isConnected = false;
        ccf->setTopItem(false);
        ccf->setAct(true);
        ccf->move(0, 0);
        ccf->show();

        this->lanListWidget->hide();
        this->wifiListWidget->show();
    }

    this->scrollAreal->hide();
    this->topLanListWidget->hide();
    this->scrollAreaw->show();
    this->topWifiListWidget->show();
    on_btnWifiList_pressed();
}

//**************下为循环部分************************//
void MainWindow::on_checkWifiListChanged(){
    if (is_stop_check_net_state==0 && this->is_btnWifiList_clicked==1 && this->isVisible()){
        BackThread *loop_bt = new BackThread();
        IFace *loop_iface = loop_bt->execGetIface();

        if (loop_iface->wstate != 2){
            is_update_wifi_list = 1;
            this->ksnm->execGetWifiList(); //更新wifi列表
        }

        delete loop_iface;
        loop_bt->deleteLater();
    }
}

void MainWindow::on_setNetSpeed()
{
    if (this->isVisible() && is_stop_check_net_state==0){
        if (is_btnWifiList_clicked == 1){
            objNetSpeed->getCurrentDownloadRates(objKyDBus->dbusWiFiCardName.toUtf8().data(), &start_rcv_rates, &start_tx_rates);
        }else if(is_btnNetList_clicked == 1){
            objNetSpeed->getCurrentDownloadRates(objKyDBus->dbusLanCardName.toUtf8().data(), &start_rcv_rates, &start_tx_rates);
        }

        long int delta_rcv = (start_rcv_rates - end_rcv_rates)/8;
        long int delta_tx = (start_tx_rates - end_tx_rates)/8;
        if (delta_rcv>=10000 || delta_rcv<0){delta_rcv = 0;}
        if (delta_tx>=10000 || delta_tx<0){delta_tx = 0;}

        QString str_rcv = QString::number(delta_rcv) + "kb/s";
        QString str_tx = QString::number(delta_tx) + "kb/s";

        lbLoadUp->setText(str_rcv);
        lbLoadDown->setText(str_tx);

        end_rcv_rates = start_rcv_rates;
        end_tx_rates = start_tx_rates;
    }
}

void MainWindow::connLanDone(int connFlag){
    //停止加载动画
    emit this->waitLanStop();

    // Lan连接结果，0点击连接成功 1失败 3开机启动网络工具时已经连接
    if(connFlag == 0){
        syslog(LOG_DEBUG, "Wired net already connected by clicking button");
        //syslog(LOG_DEBUG, "Wired net already connected by clicking button, will check if Lan disconnected again circularly");
        //qDebug()<<"连接状态：有线网络已经点击连接，即将重新检测 Lan 是否断开";
        this->is_wired_line_ready = 1;
        this->is_by_click_connect = 1;
        this->ksnm->execGetLanList();
        QString txt(tr("Conn Ethernet Success"));
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
        system(cmd.toUtf8().data());

        //changeTimerState();
        //checkIfLanConnect->start(8000);
    }

    if(connFlag == 1){
        qDebug()<<"without net line connect to computer";
        this->is_wired_line_ready = 0; //without net line connect to computer
        QString txt(tr("Conn Ethernet Fail"));
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
        system(cmd.toUtf8().data());
    }

    if(connFlag == 3){
        syslog(LOG_DEBUG, "Launch kylin-nm, Lan already connected");
        //syslog(LOG_DEBUG, "Launch kylin-nm, Lan already connected, will check if Lan disconnected circularly");
        //qDebug()<<"连接状态：启动kylin-nm,Lan已经开启，即将循环检测是否断开";
        this->is_wired_line_ready = 1;
        //changeTimerState();
        //checkIfLanConnect->start(8000);
    }

    this->stopLoading();
}

void MainWindow::on_isLanConnect()
{
    BackThread *loop_bt = new BackThread();
    IFace *loop_iface = loop_bt->execGetIface();

    if (is_stop_check_net_state == 0){
        if (loop_iface->lstate == 1){
            syslog(LOG_DEBUG, "Wired net is disconnected");
            qDebug()<<"连接状态：有线网络连接已经断开";
            if(this->is_btnNetList_clicked == 1) {
                this->ksnm->execGetLanList();
            }
            if(this->is_btnWifiList_clicked== 1) {
                this->ksnm->execGetWifiList();
            }
            this->is_by_click_connect = 0;
            checkIfLanConnect->stop();

            if (loop_iface->wstate != 0){
                syslog(LOG_DEBUG, "Will check if LAN or Wi-Fi connected again circularly");
                qDebug()<<"连接状态：即将循环检测是否重新连接 Lan或Wifi";
                //changeTimerState();
                //checkIfNetworkOn->start(8000);
            }
        } else if (loop_iface->wstate != 2) {
            count_loop = 1;
            if (count_loop==1 && this->is_btnWifiList_clicked == 1){
                if (this->isVisible() ){
                    is_update_wifi_list = 1;
                    this->ksnm->execGetWifiList(); //更新wifi列表
                }
            }
            if (count_loop >= 2){ count_loop = 1;}
        }
    }

    delete loop_iface;
    delete loop_bt;
}

void MainWindow::on_isNetOn()
{
    BackThread *loop_bt = new BackThread();
    IFace *loop_iface = loop_bt->execGetIface();

    if (is_stop_check_net_state == 0){
        if (loop_iface->lstate == 0 && this->is_by_click_connect == 0 && this->is_wired_line_ready == 1){
            syslog(LOG_DEBUG, "Lan already connected again, will check if Wi-Fi disconnected circularly");
            qDebug()<<"连接状态：有线网络已经重新连接，即将循环检测 Lan 是否断开";
            if(this->is_btnNetList_clicked == 1) {
                this->ksnm->execGetLanList();
            }
            if(this->is_btnWifiList_clicked== 1) {
                this->ksnm->execGetWifiList();
            }
            checkIfNetworkOn->stop();

            //changeTimerState();
            //checkIfLanConnect->start(8000);
        } else if (loop_iface->lstate == 0 && this->is_by_click_connect == 1){
            qDebug()<<"连接状态：有线网络已经点击连接，即将重新循环检测 Lan 是否断开";
            checkIfNetworkOn->stop();
        } else if (loop_iface->wstate == 0 && this->is_by_click_connect == 0){
            syslog(LOG_DEBUG, "Wi-Fi already connected again, will check if Wi-Fi disconnected circularly");
            qDebug()<<"连接状态：Wifi网络已经重新连接，即将循环检测 Wifi 是否断开";
            if(this->is_btnNetList_clicked == 1) {
                this->ksnm->execGetLanList();
            }
            if(this->is_btnWifiList_clicked== 1) {
                this->ksnm->execGetWifiList();
            }
            checkIfNetworkOn->stop();

            //changeTimerState();
            //checkIfWifiConnect->start(8000);
        } else if (loop_iface->wstate == 0 && this->is_by_click_connect == 1){
            qDebug()<<"连接状态：Wifi网络已经点击连接，即将重新循环检测 Wifi 是否断开";
            checkIfNetworkOn->stop();
        } else if (loop_iface->wstate != 2) {
            count_loop = 1;
            if (count_loop==1 && this->is_btnWifiList_clicked == 1){
                if (this->isVisible() ){
                    is_update_wifi_list = 1;
                    this->ksnm->execGetWifiList(); //更新wifi列表
                }
            }
            if (count_loop >= 2){ count_loop = 1;}
        }
    }

    delete loop_iface;
    delete loop_bt;
}

void MainWindow::connWifiDone(int connFlag)
{
    // Wifi连接结果，0点击连接成功 1失败 2没有配置文件 3开机启动网络工具时已经连接
    if(connFlag == 0){
        syslog(LOG_DEBUG, "Wi-Fi already connected by clicking button");
        //syslog(LOG_DEBUG, "Wi-Fi already connected by clicking button, will check if Wi-Fi disconnected again circularly");
        //qDebug()<<"连接状态：Wifi网络已经点击连接，即将重新循环检测 Wifi 是否断开";
        this->is_by_click_connect = 1;
        this->ksnm->execGetWifiList();
        QString txt(tr("Conn Wifi Success"));
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
        system(cmd.toUtf8().data());

        //changeTimerState();
        //checkIfWifiConnect->start(8000);
    } else if (connFlag == 1) {
        is_stop_check_net_state = 0;
    } else if (connFlag == 3) {
        syslog(LOG_DEBUG, "Launch kylin-nm, Wi-Fi already connected");
        //syslog(LOG_DEBUG, "Launch kylin-nm, Wi-Fi already connected, will check if Wi-Fi disconnected circularly");
        //qDebug()<<"连接状态：启动kylin-nm, Wifi已经连接，即将循环检测是否断开";
        //changeTimerState();
        //checkIfWifiConnect->start(8000);
    }
}

void MainWindow::on_isWifiConnect()
{
    BackThread *loop_bt = new BackThread();
    IFace *loop_iface = loop_bt->execGetIface();

    if (is_stop_check_net_state == 0){
        if (loop_iface->wstate == 1){
            syslog(LOG_DEBUG, "Wi-Fi is disconnected");
            qDebug()<<"连接状态：Wifi 网络连接已经断开";
            if(this->is_btnNetList_clicked == 1) {
                this->ksnm->execGetLanList();
            }
            if(this->is_btnWifiList_clicked== 1) {
                this->ksnm->execGetWifiList();
            }
            this->is_by_click_connect = 0;
            checkIfWifiConnect->stop();

            if (loop_iface->lstate != 0){
                syslog(LOG_DEBUG, "Will check if LAN or Wi-Fi connected again circularly");
                qDebug()<<"连接状态：即将循环检测是否重新开启 Lan或Wifi";
                //changeTimerState();
                //checkIfNetworkOn->start(8000);
            }
        } else if (loop_iface->wstate != 2){
            count_loop = 1;
            if (count_loop==1 && this->is_btnWifiList_clicked == 1){
                if (this->isVisible() ){
                    is_update_wifi_list = 1;
                    this->ksnm->execGetWifiList(); //更新wifi列表
                }
            }
            if (count_loop >= 2){ count_loop = 1;}
        }
    }
    \
    delete loop_iface;
    delete loop_bt;
}
//**************上为循环部分************************//

void MainWindow::on_btnAdvConf_clicked()
{
    system("nm-connection-editor &");
}

void MainWindow::on_btnNetList_pressed()
{
//    ui->btnNetList->setStyleSheet("#btnNetList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
//    ui->btnWifiList->setStyleSheet("#btnWifiList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
//                                  "#btnWifiList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
}

void MainWindow::on_btnWifiList_pressed()
{
//    ui->btnWifiList->setStyleSheet("#btnWifiList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
//    ui->btnNetList->setStyleSheet("#btnNetList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
//                                  "#btnNetList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
}

void MainWindow::oneLanFormSelected(QString lanName)
{
    QList<OneLancForm *> lanList = lanListWidget->findChildren<OneLancForm *>();

    // 所有元素回到原位
    for(int i = 0, j = 0;i < lanList.size(); i ++){
        OneLancForm *ocf = lanList.at(i);
        if(ocf->isActive == true){
            ocf->move(0, 0);
        }
        if(ocf->isActive == false){
            ocf->move(0, j * 60);
            j ++;
        }
    }

    //是否与上一次选中同一个网络框
    if (currSelNetName == lanName){
        // 缩小所有选项卡
        for(int i = 0;i < lanList.size(); i ++){
            OneLancForm *ocf = lanList.at(i);
            if(ocf->lanName == lanName){
                ocf->setSelected(false, true);
            }else{
                ocf->setSelected(false, false);
            }
        }

        currSelNetName = "";
    } else {
        // 选中的选项卡放大，其他选项卡缩小
        int selectY = 0;
        for(int i = 0;i < lanList.size(); i ++){
            OneLancForm *ocf = lanList.at(i);
            if(ocf->lanName == lanName){
                ocf->setSelected(true, false);
                selectY = ocf->y();
            }else{
                ocf->setSelected(false, false);
            }
        }

        // 选中元素下面的所有元素下移108
        for(int i = 0;i < lanList.size(); i ++){
            OneLancForm *ocf = lanList.at(i);
            if(ocf->y() > selectY){
                ocf->move(0, ocf->y() + 108);
            }
        }

        currSelNetName = lanName;
    }

    QList<OneLancForm *> itemList = lanListWidget->findChildren<OneLancForm *>();
    int n = itemList.size();
    if(n >= 1){
        OneLancForm *lastItem = itemList.at(n-1);
        lastItem->setLine(false);
    }

}
void MainWindow::oneTopLanFormSelected(QString lanName)
{
    QList<OneLancForm *> lanList = topLanListWidget->findChildren<OneLancForm *>();

    if (currSelNetName == lanName){
        // 与上一次选中同一个网络框，缩小当前选项卡
        topLanListWidget->resize(435, 60 + 46);
        lbTopLanList->move(12, 60);
        scrollAreal->move(41, 158);

        OneLancForm *ocf = lanList.at(0);
        ocf->setTopItem(false);

        currSelNetName = "";
    } else {
        // 没有与上一次选中同一个网络框，放大当前选项卡
        topLanListWidget->resize(435, 60 + 108 + 46);
        lbTopLanList->move(12, 60 + 108);
        scrollAreal->move(41, 158 + 108);

        OneLancForm *ocf = lanList.at(0);
        ocf->setTopItem(true);

        currSelNetName = lanName;
    }
}

void MainWindow::oneWifiFormSelected(QString wifiName)
{
    QList<OneConnForm *> wifiList = wifiListWidget->findChildren<OneConnForm *>();

    // 所有元素回到原位
    for(int i = 0, j = 0;i < wifiList.size(); i ++){
        OneConnForm *ocf = wifiList.at(i);
        if(ocf->isActive == true){
            ocf->move(0, 0);
        }
        if(ocf->isActive == false){
            ocf->move(0, j * 60);
            j ++;
        }
    }

    //是否与上一次选中同一个网络框
    if (currSelNetName == wifiName){
        // 置选中，缩小所有选项卡
        for(int i = 0;i < wifiList.size(); i ++){
            OneConnForm *ocf = wifiList.at(i);
            if(ocf->wifiName == wifiName){
                if (ocf->wifiName == hideWiFiConn){
                    ocf->setHideItem(true, true);
                }else{
                    ocf->setSelected(false, true);
                }
            } else{
                if (ocf->wifiName == hideWiFiConn){
                    ocf->setHideItem(true, true);
                }else{
                    ocf->setSelected(false, false);
                }
            }

        }
        currSelNetName = "";
    } else {
        // 设置选中，放大或缩小所有选项卡
        int selectY = 0;

        for(int i = 0;i < wifiList.size(); i ++){
            OneConnForm *ocf = wifiList.at(i);
            if(ocf->wifiName == wifiName){
                if (ocf->wifiName == hideWiFiConn){
                    ocf->setHideItem(true, true);
                }else{
                    ocf->setSelected(true, false);
                }
                selectY = ocf->y();
            }else{
                if (ocf->wifiName == hideWiFiConn){
                    ocf->setHideItem(true, true);
                }else{
                    ocf->setSelected(false, false);
                }
            }
        }

        // 选中元素下面的所有元素下移88
        for(int i = 0;i < wifiList.size(); i ++){
            OneConnForm *ocf = wifiList.at(i);
            if(ocf->y() > selectY){
                ocf->move(0, ocf->y() + 88);
            }
        }

        currSelNetName = wifiName;
    }

    QList<OneConnForm *> itemList = wifiListWidget->findChildren<OneConnForm *>();
    int n = itemList.size();
    if (n >= 1){
        OneConnForm *lastItem = itemList.at(n-1);
        lastItem->setLine(false);
    }
}
void MainWindow::oneTopWifiFormSelected(QString wifiName)
{
    QList<OneConnForm *> wifiList = topWifiListWidget->findChildren<OneConnForm *>();

    if (currSelNetName == wifiName){
        // 与上一次选中同一个网络框，缩小当前选项卡
        topWifiListWidget->resize(435, 60 + 46);
        lbTopWifiList->move(12, 60);
        btnAddNet->move(365, 77);
        scrollAreaw->move(41, 158);

        OneConnForm *ocf = wifiList.at(0);
        ocf->setTopItem(false);

        currSelNetName = "";
    } else {
        // 没有与上一次选中同一个网络框，放大当前选项卡
        topWifiListWidget->resize(435, 60 + 88 + 46);
        lbTopWifiList->move(12, 60 + 88);
        btnAddNet->move(365, 77 + 88);
        scrollAreaw->move(41, 158 + 88);

        OneConnForm *ocf = wifiList.at(0);
        ocf->setTopItem(true);

        currSelNetName = wifiName;
    }
}

void MainWindow::activeLanDisconn()
{
    syslog(LOG_DEBUG, "Wired net is disconnected");
    currSelNetName = "";
    //this->startLoading();
    emit this->waitLanStop();
    this->ksnm->execGetLanList();
}

void MainWindow::activeWifiDisconn()
{
    QThread *tt = new QThread();
    BackThread *btt = new BackThread();
    btt->moveToThread(tt);
    connect(tt, SIGNAL(finished()), tt, SLOT(deleteLater()));
    connect(tt, SIGNAL(started()), this, SLOT(activeStartLoading()));
    connect(this, SIGNAL(deleteRedundantNet()), btt, SLOT(redundantNetDeleted()));
    connect(btt, SIGNAL(disFinish()), this, SLOT(activeGetWifiList()));
    connect(btt, SIGNAL(ttFinish()), tt, SLOT(quit()));
    tt->start();
}
void MainWindow::activeStartLoading()
{
    syslog(LOG_DEBUG, "Wi-Fi is disconnected");
    currSelNetName = "";
    //this->startLoading();
    emit this->deleteRedundantNet();
}
void MainWindow::activeGetWifiList()
{
    emit this->waitWifiStop();
    this->ksnm->execGetWifiList();
}

void MainWindow::on_btnAdvConf_pressed()
{
    ui->lbBtnConfBG->setStyleSheet(btnOnQss);
}

void MainWindow::on_btnAdvConf_released()
{
    ui->lbBtnConfBG->setStyleSheet(btnOffQss);
}

void MainWindow::enNetDone()
{
    BackThread *bt = new BackThread();
    mwBandWidth = bt->execChkLanWidth(lname);

    ui->lbBtnNetBG->setStyleSheet(btnOnQss);

    // 打开网络开关时如果Wifi开关是打开的，设置其样式
    if(checkWlOn()){
        ui->lbBtnWifiBG->setStyleSheet(btnBgOnQss);
        ui->lbBtnWifiBall->move(438, 22);
    }

    onBtnNetListClicked(1);
    is_stop_check_net_state = 0;

    qDebug()<<"debug: already turn on the switch of lan network";
    syslog(LOG_DEBUG, "Already turn on the switch of lan network");
}

void MainWindow::disNetDone()
{
    this->is_btnNetList_clicked = 1;
    this->is_btnWifiList_clicked = 0;

    ui->lbNetListBG->setStyleSheet(btnOnQss);
    ui->lbWifiListBG->setStyleSheet(btnOffQss);

    ui->lbNetwork->setText("有线网络");
    ui->btnWifi->hide();
    ui->lbBtnWifiBG->hide();
    ui->lbBtnWifiBall->hide();

    // 清空top列表
    delete topLanListWidget;
    topLanListWidget = new QWidget(ui->centralWidget);
    topLanListWidget->move(41, 52);
    topLanListWidget->resize(435, 60 + 46);
    lbTopLanList = new QLabel(topLanListWidget);
    lbTopLanList->setText(tr("Ethernet Networks"));//"可用网络列表"
    lbTopLanList->resize(260, 46);
    lbTopLanList->move(12, 60);
    lbTopLanList->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.97);}");
    lbTopLanList->show();

    // 清空lan列表
    lanListWidget = new QWidget(scrollAreal);
    lanListWidget->resize(440, 60 + 108);
    scrollAreal->setWidget(lanListWidget);
    scrollAreal->move(41, 158);

    // 当前连接的lan
    OneLancForm *ccf = new OneLancForm(topLanListWidget, this, confForm, ksnm);
    ccf->setName(tr("Not connected"));//"当前未连接任何 以太网"
    ccf->setIcon(false);
    ccf->setConnedString(1, tr("Disconnected"));//"未连接"
    ccf->isConnected = false;
    ccf->setTopItem(false);
    ccf->setAct(true);
    ccf->move(0, 0);
    ccf->show();

    ui->lbBtnNetBG->setStyleSheet(btnOffQss);

    ui->lbBtnWifiBG->setStyleSheet(btnBgOffQss);
    ui->lbBtnWifiBall->move(412, 22);

    this->lanListWidget->show();
    this->wifiListWidget->hide();
    this->scrollAreal->show();
    this->topLanListWidget->show();
    this->scrollAreaw->hide();
    this->topWifiListWidget->hide();

    on_btnNetList_pressed();

    qDebug()<<"debug: already turn off the switch of lan network";
    syslog(LOG_DEBUG, "Already turn off the switch of lan network");

    this->stopLoading();
}

void MainWindow::launchLanDone()
{
    ui->lbBtnNetBG->setStyleSheet(btnOnQss);
}

void MainWindow::enWifiDone()
{
    ui->lbBtnWifiBG->setStyleSheet(btnBgOnQss);
    ui->lbBtnWifiBall->move(438, 22);

    is_update_wifi_list = 0;
    this->ksnm->execGetWifiList();

    qDebug()<<"debug: already turn on the switch of wifi network";
    syslog(LOG_DEBUG, "Already turn on the switch of wifi network");
}

void MainWindow::disWifiDone()
{
    wifiListWidget = new QWidget(scrollAreaw);
    wifiListWidget->resize(440, /*60 + */88);
    scrollAreaw->setWidget(wifiListWidget);
    scrollAreaw->move(41, 158);
    btnAddNet->move(365, 77);

    lbTopWifiList->move(12, 60);
    btnAddNet->move(365, 77);
    topWifiListWidget->resize(435, 60 + 46);

    QList<OneConnForm *> wifiList = topWifiListWidget->findChildren<OneConnForm *>();
    for(int i = 0; i < wifiList.size(); i ++){
        OneConnForm *ocf = wifiList.at(i);
        if(ocf->isActive == true){
            ocf->setSelected(false, false);
            ocf->setName(tr("Not connected"));//"当前未连接任何 Wifi"
            ocf->setSignal("0", "--");
            ocf->setConnedString(1, tr("Disconnected"), "");//"未连接"
            ocf->isConnected = false;
            ocf->setTopItem(false);
            disconnect(ocf, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneTopWifiFormSelected(QString)));
        }else{
            ocf->deleteLater();
        }
    }

    ui->lbBtnWifiBG->setStyleSheet(btnBgOffQss);
    ui->lbBtnWifiBall->move(412, 22);

    this->lanListWidget->hide();
    this->topLanListWidget->hide();
    this->wifiListWidget->show();
    this->topWifiListWidget->show();
    this->scrollAreal->hide();
    this->scrollAreaw->show();

    on_btnWifiList_pressed();

    qDebug()<<"debug: already turn off the switch of wifi network";
    syslog(LOG_DEBUG, "Already turn off the switch of wifi network");

    this->stopLoading();
}

void MainWindow::keepDisWifiState()
{
    if(this->is_btnNetList_clicked == 1) {
        ui->lbBtnWifiBG->setStyleSheet(btnBgOffQss);
        ui->lbBtnWifiBall->move(412, 22);
    }
    if(this->is_btnWifiList_clicked== 1) {
        wifiListWidget = new QWidget(scrollAreaw);
        wifiListWidget->resize(440, /*60 + */88);
        scrollAreaw->setWidget(wifiListWidget);
        scrollAreaw->move(41, 158);

        lbTopWifiList->move(12, 60);
        btnAddNet->move(365, 77);
        topWifiListWidget->resize(435, 60 + 46);

        QList<OneConnForm *> wifiList = topWifiListWidget->findChildren<OneConnForm *>();
        for(int i = 0; i < wifiList.size(); i ++){
            OneConnForm *ocf = wifiList.at(i);
            if(ocf->isActive == true){
                ocf->setSelected(false, false);
                ocf->setName(tr("Not connected"));//"当前未连接任何 Wifi"
                ocf->setSignal("0", "--");
                ocf->setConnedString(1, tr("Disconnected"), "");//"未连接"
                ocf->isConnected = false;
                ocf->setTopItem(false);
                disconnect(ocf, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneTopWifiFormSelected(QString)));
            }else{
                ocf->deleteLater();
            }
        }

        ui->lbBtnWifiBG->setStyleSheet(btnBgOffQss);
        ui->lbBtnWifiBall->move(412, 22);

        this->lanListWidget->hide();
        this->wifiListWidget->show();
        this->scrollAreal->hide();
        this->topLanListWidget->hide();
        this->scrollAreaw->show();
        this->topWifiListWidget->show();

//        this->stopLoading();
    }
}

void MainWindow::on_btnFlyMode_clicked()
{
    if(is_fly_mode_on == 0){
        ui->lbFlyImg->setStyleSheet("QLabel{background-image:url(:/res/x/fly-mode-on.svg);}");
        ui->lbFlyBG->setStyleSheet(btnOnQss);
        is_fly_mode_on = 1;

        on_btnWifi_clicked();
        on_btnWifiList_clicked();
    } else {
        ui->lbFlyImg->setStyleSheet("QLabel{background-image:url(:/res/x/fly-mode-off.svg);}");
        ui->lbFlyBG->setStyleSheet(btnOffQss);
        is_fly_mode_on = 0;
    }
}

void MainWindow::on_btnHotspot_clicked()
{
    if (is_wireless_adapter_ready == 1){
        if(is_hot_sopt_on == 0){
            ui->lbHotImg->setStyleSheet("QLabel{background-image:url(:/res/x/hot-spot-on.svg);}");
            ui->lbHotBG->setStyleSheet(btnOnQss);
            is_hot_sopt_on = 1;

            QApplication::setQuitOnLastWindowClosed(false);
            DlgHotspotCreate *hotCreate = new DlgHotspotCreate(objKyDBus->dbusWiFiCardName);
            connect(hotCreate,SIGNAL(updateHotspotList()),this,SLOT(on_btnWifiList_clicked() ));
            connect(hotCreate,SIGNAL(btnHotspotState()),this,SLOT(on_btnHotspotState() ));
            hotCreate->show();
        } else {
            on_btnHotspotState();

            QString strSlist;
            system("nmcli connection show -active>/tmp/kylin-nm-connshow");
            QFile file("/tmp/kylin-nm-connshow");
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                syslog(LOG_ERR, "Can't open the file /tmp/kylin-nm-connshow!");
                qDebug()<<"Can't open the file /tmp/kylin-nm-connshow!";
            }

            QString txt = file.readAll();
            QStringList txtLine = txt.split("\n");
            file.close();
            foreach (QString line, txtLine) {
                if(line.indexOf("wifi") != -1){
                    QStringList subLine = line.split(" ");
                    if (subLine[1].size() == 1){
                        strSlist =  subLine[0]+ " " + subLine[1];
                    }else {
                        strSlist =  subLine[0];
                    }
                    kylin_network_set_con_down(strSlist.toUtf8().data());
                }
            } //end foreach

            sleep(2);
            on_btnWifiList_clicked();
        }
    }
}

void MainWindow::on_btnHotspotState()
{
    ui->lbHotImg->setStyleSheet("QLabel{background-image:url(:/res/x/hot-spot-off.svg);}");
    ui->lbHotBG->setStyleSheet(btnOffQss);
    is_hot_sopt_on = 0;
}

void MainWindow::onBtnAddNetClicked()
{
    QApplication::setQuitOnLastWindowClosed(false);
    DlgConnHidWifi *connHidWifi = new DlgConnHidWifi(0, this);
    connect(connHidWifi, SIGNAL(reSetWifiList() ), this, SLOT(on_btnWifiList_clicked()) );
    connHidWifi->show();
}
