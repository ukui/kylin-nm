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
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
//    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setWindowOpacity(0.95);

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

    scrollAreal = new QScrollArea(ui->centralWidget);
    scrollAreal->move(1, 38);
    scrollAreal->resize(314, 354);
    scrollAreal->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreal->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    scrollAreaw = new QScrollArea(ui->centralWidget);
    scrollAreaw->move(1, 38);
    scrollAreaw->resize(314, 354);
    scrollAreaw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreaw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    lanListWidget = new QWidget(scrollAreal);
    wifiListWidget = new QWidget(scrollAreaw);
    lbLanList = new QLabel(lanListWidget);
    lbWifiList = new QLabel(wifiListWidget);

    btnOffQss = "QLabel{background-color:#283138;}";
    btnOnQss = "QLabel{background-color:#3593b5;}";

    ui->centralWidget->setStyleSheet("#centralWidget{border:1px solid #626c6e;background-color:#151a1e;}");

    ui->wdgHead->setStyleSheet("#wdgHead{background-color:rgba(8,10,12,0.6);}");

    scrollAreal->setStyleSheet("QScrollArea{border:none;}");
    scrollAreal->viewport()->setStyleSheet("background-color:transparent;");
    scrollAreal->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background:transparent;border:0px;width:6px;}"
                                                    "QScrollBar::up-arrow:vertical{height:0px;}"
                                                    "QScrollBar::sub-line:vertical{border:0px solid;height:0px}"
                                                    "QScrollBar::sub-page:vertical{background:transparent;}"
                                                    "QScrollBar::handle:vertical{background-color:#3593b5;}"
                                                    "QScrollBar::handle:vertical:hover{background-color:#3593b5;}"
                                                    "QScrollBar::handle:vertical:pressed{background-color:#3593b5;}"
                                                    "QScrollBar::add-page:vertical{background:transparent;}"
                                                    "QScrollBar::add-line:vertical{border:0px solid;height:0px}"
                                                    "QScrollBar::down-arrow:vertical{height:0px;}");

    scrollAreaw->setStyleSheet("QScrollArea{border:none;}");
    scrollAreaw->viewport()->setStyleSheet("background-color:transparent;");
    scrollAreaw->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background:transparent;border:0px;width:6px;}"
                                                    "QScrollBar::up-arrow:vertical{height:0px;}"
                                                    "QScrollBar::sub-line:vertical{border:0px solid;height:0px}"
                                                    "QScrollBar::sub-page:vertical{background:transparent;}"
                                                    "QScrollBar::handle:vertical{background-color:#3593b5;}"
                                                    "QScrollBar::handle:vertical:hover{background-color:#3593b5;}"
                                                    "QScrollBar::handle:vertical:pressed{background-color:#3593b5;}"
                                                    "QScrollBar::add-page:vertical{background:transparent;}"
                                                    "QScrollBar::add-line:vertical{border:0px solid;height:0px}"
                                                    "QScrollBar::down-arrow:vertical{height:0px;}");

    ui->btnNetList->setStyleSheet("#btnNetList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
                                  "#btnNetList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
    ui->btnWifiList->setStyleSheet("#btnWifiList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
                                  "#btnWifiList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");

    ui->btnNet->setStyleSheet("QPushButton{border:none;}");
    ui->lbBtnNetT1->setStyleSheet("QLabel{font-size:13px;color:#ffffff;}");
    ui->lbBtnNetT2->setStyleSheet("QLabel{font-size:12px;font-weight:100;color:#ffffff;}");

    ui->btnWifi->setStyleSheet("QPushButton{border:none;}");
    ui->lbBtnWifiT1->setStyleSheet("QLabel{font-size:13px;color:#ffffff;}");
    ui->lbBtnWifiT2->setStyleSheet("QLabel{font-size:12px;font-weight:100;color:#ffffff;}");

    ui->btnAdvConf->setStyleSheet("QPushButton{border:none;}");
    ui->lbConfImg->setStyleSheet("QLabel{background-image:url(:/res/x/setup.png);}");
    ui->lbBtnConfT2->setStyleSheet("QLabel{font-size:12px;font-weight:100;color:#ffffff;}");
    ui->lbBtnConfBG->setStyleSheet(btnOffQss);

    ui->lbBtnNetT2->setText(tr("Network"));//"网络"
    ui->lbBtnWifiT2->setText("Wifi");
    ui->lbBtnConfT2->setText(tr("Advanced"));//"高级设置"

    ui->btnNetList->setText(tr("Ethernet"));//"有线网络"
    ui->btnWifiList->setText(tr("Wifi"));//"无线网络"

    createTrayIcon();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    connect(ksnm, SIGNAL(getLanListFinished(QStringList)), this, SLOT(getLanListDone(QStringList)));
    connect(ksnm, SIGNAL(getWifiListFinished(QStringList)), this, SLOT(getWifiListDone(QStringList)));

    objKyDBus = new KylinDBus(this);

    checkIsWirelessDeviceOn(); //检测无线网卡是否插入
    getInitLanSlist(); //初始化有线网列表
    //initTimer(); //初始化定时器
    //changeTimerState();//停止所有定时器
    initNetwork(); //初始化网络

    trayIcon->show();
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

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip(QString(tr("kylin-nm")));
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

    // 确保currentActWifiSignalLv有值
    QList<OneConnForm *> wifiList = wifiListWidget->findChildren<OneConnForm *>();
    for(int i = 0; i < wifiList.size(); i ++){
        OneConnForm *ocf = wifiList.at(i);
        if(ocf->isActive == true){
            currentActWifiSignalLv = ocf->signalLv;
            break;
        }
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
        wiredCableUpTimer->start(4000);
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
    on_btnNetList_clicked(1);
    is_stop_check_net_state = 0;
}
void MainWindow::onCarrierDownHandle()
{
    wiredCableDownTimer->stop();
    this->stopLoading();
    on_btnNetList_clicked(0);
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
    BackThread *m_bt = new BackThread();
    IFace *m_iface = m_bt->execGetIface();
    qDebug()<<"lstate ="<<m_iface->lstate<<"    wstate ="<<m_iface->wstate ;
    syslog(LOG_DEBUG, "state of switch:   lstate =%d    wstate =%d", m_iface->lstate, m_iface->wstate);
    m_bt->lanDelete();
    sleep(1);
    m_bt->lanDelete();
    sleep(1);
    m_bt->lanDelete();
    delete m_iface;
    m_bt->deleteLater();

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
    if(iface->lstate == 0 || iface->lstate == 1){
        ui->lbLanImg->setStyleSheet("QLabel{background-image:url(:/res/x/network-line.png);}");
        ui->lbBtnNetBG->setStyleSheet(btnOnQss);
        ui->lbBtnNetT1->setText(tr("Enabled"));//"已开启"

        if(iface->wstate == 0 || iface->wstate == 1){
            ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-line.png);}");
            ui->lbBtnWifiBG->setStyleSheet(btnOnQss);
            ui->lbBtnWifiT1->setText(tr("Enabled"));//"已开启"
        }else{
            ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-offline.png);}");
            ui->lbBtnWifiBG->setStyleSheet(btnOffQss);
            ui->lbBtnWifiT1->setText(tr("Disabled"));//"已关闭"
        }

    }else{
        ui->lbLanImg->setStyleSheet("QLabel{background-image:url(:/res/x/network-offline.png);}");
        ui->lbBtnNetBG->setStyleSheet(btnOffQss);
        ui->lbBtnNetT1->setText(tr("Disabled"));//"已关闭"

        ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-offline.png);}");
        ui->lbBtnWifiBG->setStyleSheet(btnOffQss);
        ui->lbBtnWifiT1->setText(tr("Disabled"));//"已关闭"
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
        ui->btnWifiList->setStyleSheet("#btnWifiList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
        ui->btnNetList->setStyleSheet("#btnNetList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
                                      "#btnNetList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
    }else{
        if(iface->lstate != 2){
            if (iface->lstate == 0) {
                connLanDone(3);
            } else{
                //syslog(LOG_DEBUG, "Launch kylin-nm, will check if Lan or Wifi connected circularly");
                //qDebug()<<"连接状态：启动kylin-nm, 即将循环检测 Lan或Wifi 是否连接";
                //changeTimerState();
                //checkIfNetworkOn->start(8000);
            }
            on_btnNetList_clicked();
            ui->btnNetList->setStyleSheet("#btnNetList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
            ui->btnWifiList->setStyleSheet("#btnWifiList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
                                          "#btnWifiList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
        }else {
            disNetDone();
        }
    }

    //循环检测wifi列表的变化
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
        moveBottomRight();
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

void MainWindow::moveBottomRight()
{
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    QRect screenGeometry = qApp->primaryScreen()->geometry();

    QDesktopWidget* pDesktopWidget = QApplication::desktop();
//    int nScreenCount = QApplication::desktop()->screenCount();
//    QRect deskRect = pDesktopWidget->availableGeometry();//可用区域
    QRect screenRect = pDesktopWidget->screenGeometry();//屏幕区域

//    qDebug()<<"screenRect.x(): "<<screenRect.x()<<"   screenRect.height(): "<<screenRect.height();
//    qDebug()<<"availableGeometry.y(): "<<availableGeometry.y()<<"   availableGeometry.height(): "<<availableGeometry.height();
    if (screenRect.height() != availableGeometry.height()) {
        this->move(availableGeometry.x() + availableGeometry.width() - this->width(), availableGeometry.height() - this->height());
    }else {
        this->move(availableGeometry.x() + availableGeometry.width() - this->width(), availableGeometry.height() - this->height() - 40);
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

    // 清空lan列表
    lanListWidget = new QWidget(scrollAreal);
    lanListWidget->resize(314, 8 + 60 + 46 + 51);
    scrollAreal->setWidget(lanListWidget);
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
    OneLancForm *ccf = new OneLancForm(lanListWidget, this, confForm, ksnm);
    if(actLanName == "--"){
        ccf->setName(tr("Not connected"));//"当前未连接任何 以太网"
        ccf->setIcon(false);
        ccf->setConnedString(tr("Disconnected"));//"未连接"
        ccf->setBandWidth("--");
        ccf->setShowPoint(true);
    }
    ccf->setAct(true);
    ccf->move(0, 8);
    ccf->show();
    // 可用lan列表
    lbLanList = new QLabel(lanListWidget);
    lbLanList->setText(tr("Ethernet Networks"));//"可用网络列表"
    lbLanList->resize(260, 46);
    lbLanList->move(12, 68);
    lbLanList->setStyleSheet("QLabel{font-size:12px;color:white;}");
    lbLanList->show();
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
            if(nname == actLanName){
                if (mwBandWidth == "Unknown!") { getLanBandWidth(); }

                connect(ccf, SIGNAL(selectedOneLanForm(QString)), this, SLOT(oneLanFormSelected(QString)));
                connect(ccf, SIGNAL(disconnActiveLan()), this, SLOT(activeLanDisconn()));
                ccf->setName(nname);
                ccf->setIcon(true);
                ccf->setBandWidth(mwBandWidth);
                ccf->setShowPoint(true);
                ccf->setConnedString(tr("Connected"));//"已连接"
                currSelNetName = "";
                syslog(LOG_DEBUG, "already insert an active lannet in the top of lan list");
            }else{
                lanListWidget->resize(314, lanListWidget->height() + 60);

                OneLancForm *ocf = new OneLancForm(lanListWidget, this, confForm, ksnm);
                connect(ocf, SIGNAL(selectedOneLanForm(QString)), this, SLOT(oneLanFormSelected(QString)));
                ocf->setName(nname);
                ocf->setIcon(true);
                ocf->setBandWidth("");
                ocf->setShowPoint(false);
                ocf->move(0, 114 + j * 60);
                ocf->show();

                j ++;
            }
        }
    }

    this->lanListWidget->show();
    this->wifiListWidget->hide();

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
    // 清空wifi列表
    wifiListWidget = new QWidget(scrollAreaw);
    wifiListWidget->resize(314, 8 + 60 + 46 + 51);
    scrollAreaw->setWidget(wifiListWidget);
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
    OneConnForm *ccf = new OneConnForm(wifiListWidget, this, confForm, ksnm);
    if(actWifiName == "--"){
        ccf->setName(tr("Not connected"));//"当前未连接任何 Wifi"
        ccf->setSafe("--");
        ccf->setSignal("0");
        ccf->setSafeString("--");
        ccf->setConnedString(tr("Disconnected"));//"未连接"
    }
    ccf->setAct(true);
    ccf->move(0, 8);
    ccf->show();
    // 可用wifi列表表头
    lbWifiList = new QLabel(wifiListWidget);
    lbWifiList->setText(tr("Wifi Networks"));//"可用网络列表"
    lbWifiList->resize(260, 46);
    lbWifiList->move(12, 68);
    lbWifiList->setStyleSheet("QLabel{font-size:12px;color:white;}");
    lbWifiList->show();
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
                connect(ccf, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
                connect(ccf, SIGNAL(disconnActiveWifi()), this, SLOT(activeWifiDisconn()));
                ccf->setName(wname);
                ccf->setSafe(wsecu);
                ccf->setRate(wrate);
                ccf->setSignal(wsignal);
                ccf->setConnedString(tr("Connected"));//"已连接"
                currSelNetName = "";

                syslog(LOG_DEBUG, "already insert an active wifi in the top of wifi list");
            }else{
                wifiListWidget->resize(314, wifiListWidget->height() + 60);

                OneConnForm *ocf = new OneConnForm(wifiListWidget, this, confForm, ksnm);
                connect(ocf, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
                ocf->setName(wname);
                ocf->setSafe(wsecu);
                ocf->setRate(wrate);
                ocf->setSignal(wsignal);
                ocf->move(0, 114 + j * 60);
                ocf->show();

                j ++;
                count ++;
            }

            wnames.append(wname);
        }
    }

    //添加 连接到隐藏的Wi-Fi网络 小窗口
    wifiListWidget->resize(314, wifiListWidget->height() + 60);
    OneConnForm *hideNetButton = new OneConnForm(wifiListWidget, this, confForm, ksnm);
    connect(hideNetButton, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
    hideNetButton->setSpecialName(hideWiFiConn);
    hideNetButton->setSignal(0);
    hideNetButton->setSafe("Safe");
    hideNetButton->move(0, 114 + count * 60);
    hideNetButton->show();

    this->lanListWidget->hide();
    this->wifiListWidget->show();
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
                                if (lastWname == currSelNetName) {after_ocf->move(0, after_ocf->y() - 111);}
                                else {after_ocf->move(0, after_ocf->y() - 60);}
                            }
                            wifiListWidget->resize(314, wifiListWidget->height() - 60);
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
                OneConnForm *lastOcf = wifiList.at(n-1);
                lastOcf->setName(wname);
                lastOcf->setSafe(wsecu);
                lastOcf->setRate(wrate);
                lastOcf->setSignal(wsignal);
                lastOcf->setSelected(false);
                //lastOcf->show();

                //添加 连接到隐藏的Wi-Fi网络 小窗口
                wifiListWidget->resize(314, wifiListWidget->height() + 60);
                OneConnForm *hideNetButton = new OneConnForm(wifiListWidget, this, confForm, ksnm);
                connect(hideNetButton, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
                hideNetButton->setSpecialName(hideWiFiConn);
                hideNetButton->setSignal(0);
                hideNetButton->setSafe("Safe");
                hideNetButton->move(0, lastOcf->y()+60);
                hideNetButton->setHideSelected(false);
                if (currSelNetName == hideWiFiConn){ hideNetButton->setHideSelected(true); }
                hideNetButton->show();

                count += 1;
            }
        }
    }

    this->lanListWidget->hide();
    this->wifiListWidget->show();
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
            }else{
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
        }
        this->startLoading();
    } else {
        QString txt(tr("please insert the wireless network adapter"));
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
        system(cmd.toUtf8().data());
        keepDisWifiState();
    }
}

void MainWindow::on_btnNetList_clicked(int flag)
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

    // 强行设置为打开
    if(flag == 1){
        this->startLoading();
        this->ksnm->execGetLanList();
        this->scrollAreal->show();
        this->scrollAreaw->hide();
        on_btnNetList_pressed();
        return;
    }

    if(checkLanOn()){
        this->startLoading();
        this->ksnm->execGetLanList();
    } else {
        syslog(LOG_DEBUG, "btnNetList is clicked, but the return value of checkLanOn() is false");
        qDebug()<<"debug: btnNetList is clicked, but the return value of checkLanOn() is false";

        //if (objKyDBus->getLanConnState() == 0){
        if (true){
            BackThread *btn_bt = new BackThread();
            btn_bt->lanDelete();
            sleep(1);
            btn_bt->lanDelete();
            sleep(1);
            btn_bt->lanDelete();
            btn_bt->deleteLater();
        }

        // 清空lan列表
        lanListWidget = new QWidget(scrollAreal);
        lanListWidget->resize(314, 8 + 60 + 46 + 51);
        scrollAreal->setWidget(lanListWidget);

        // 当前连接的lan
        OneLancForm *ccf = new OneLancForm(lanListWidget, this, confForm, ksnm);
        ccf->setName(tr("Not connected"));//"当前未连接任何 以太网"
        ccf->setIcon(false);
        ccf->setConnedString(tr("Disconnected"));//"未连接"
        ccf->setBandWidth("--");
        ccf->setShowPoint(true);
        ccf->setAct(true);
        ccf->move(0, 8);
        ccf->show();

        // 可用lan列表
        lbLanList = new QLabel(lanListWidget);
        lbLanList->setText(tr("Ethernet Networks"));//"可用网络列表"
        lbLanList->resize(260, 46);
        lbLanList->move(12, 68);
        lbLanList->setStyleSheet("QLabel{font-size:12px;color:white;}");
        lbLanList->show();

        this->lanListWidget->show();
        this->wifiListWidget->hide();
    }

    this->scrollAreal->show();
    this->scrollAreaw->hide();
    on_btnNetList_pressed();
}

void MainWindow::on_btnWifiList_clicked()
{
    this->is_btnWifiList_clicked = 1;
    this->is_btnNetList_clicked = 0;

    if(checkWlOn()){
        this->startLoading();
        this->ksnm->execGetWifiList();
    }else{
        // 清空wifi列表
        wifiListWidget = new QWidget(scrollAreaw);
        wifiListWidget->resize(314, 8 + 60 + 46 + 51);
        scrollAreaw->setWidget(wifiListWidget);

        // 当前连接的wifi
        OneConnForm *ccf = new OneConnForm(wifiListWidget, this, confForm, ksnm);
        ccf->setName(tr("Not connected"));//"当前未连接任何 Wifi"
        ccf->setSafe("--");
        ccf->setSignal("0");
        ccf->setRate("0");
        ccf->setSafeString("--");
        ccf->setConnedString(tr("Disconnected"));//"未连接"
        ccf->setAct(true);
        ccf->setShowPoint(true);
        ccf->move(0, 8);
        ccf->show();

        lbWifiList = new QLabel(wifiListWidget);
        lbWifiList->setText(tr("Wifi Networks"));//"可用网络列表"
        lbWifiList->resize(260, 46);
        lbWifiList->move(12, 68);
        lbWifiList->setStyleSheet("QLabel{font-size:12px;color:white;}");
        lbWifiList->show();

        this->lanListWidget->hide();
        this->wifiListWidget->show();
    }

    this->scrollAreal->hide();
    this->scrollAreaw->show();
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

void MainWindow::connLanDone(int connFlag){
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
    ui->btnNetList->setStyleSheet("#btnNetList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
    ui->btnWifiList->setStyleSheet("#btnWifiList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
                                  "#btnWifiList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
}

void MainWindow::on_btnWifiList_pressed()
{
    ui->btnWifiList->setStyleSheet("#btnWifiList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
    ui->btnNetList->setStyleSheet("#btnNetList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
                                  "#btnNetList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
}

void MainWindow::oneLanFormSelected(QString lanName)
{
    QList<OneLancForm *> lanList = lanListWidget->findChildren<OneLancForm *>();

    // 所有元素回到原位
    for(int i = 0, j = 0;i < lanList.size(); i ++){
        OneLancForm *ocf = lanList.at(i);
        if(ocf->isActive == true){
            ocf->move(0, 8);
        }
        if(ocf->isActive == false){
            ocf->move(0, 114 + j * 60);
            j ++;
        }
    }
    lbLanList->move(12, 68);

    //是否与上一次选中同一个网络框
    if (currSelNetName == lanName){
        // 设置选中，缩小所有选项卡
        for(int i = 0;i < lanList.size(); i ++){
            OneLancForm *ocf = lanList.at(i);
                ocf->setSelected(false);
        }

        currSelNetName = "";
    } else {
        // 设置选中，放大缩小所有选项卡
        int selectY = 0;
        for(int i = 0;i < lanList.size(); i ++){
            OneLancForm *ocf = lanList.at(i);
            if(ocf->lanName == lanName){
                ocf->setSelected(true);
                selectY = ocf->y();
            }else{
                ocf->setSelected(false);
            }
        }

        // 选中元素下面的所有元素下移51
        for(int i = 0;i < lanList.size(); i ++){
            OneLancForm *ocf = lanList.at(i);
            if(ocf->y() > selectY){
                ocf->move(0, ocf->y() + 51);
            }
        }
        if(lbLanList->y() > selectY){
            lbLanList->move(12, 68 + 51);
        }

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
            ocf->move(0, 8);
        }
        if(ocf->isActive == false){
            ocf->move(0, 114 + j * 60);
            j ++;
        }
    }
    lbWifiList->move(12, 68);

    //是否与上一次选中同一个网络框
    if (currSelNetName == wifiName){
        // 设置选中，缩小所有选项卡
        for(int i = 0;i < wifiList.size(); i ++){
            OneConnForm *ocf = wifiList.at(i);
            if (ocf->wifiName == hideWiFiConn){
                ocf->setHideSelected(false);
            }else{
                ocf->setSelected(false);
            }
        }
        currSelNetName = "";
    } else {
        // 设置选中，放大或缩小所有选项卡
        int selectY = 0;
        int selectWidgetY = 0;
        for(int i = 0;i < wifiList.size(); i ++){
            OneConnForm *ocf = wifiList.at(i);
            if(ocf->wifiName == wifiName){
                if (ocf->wifiName == hideWiFiConn){
                    ocf->setHideSelected(true);
                }else{
                    ocf->setSelected(true);
                }
                selectY = ocf->y();
                selectWidgetY = wifiListWidget->y();
            }else{
                if (ocf->wifiName == hideWiFiConn){
                    ocf->setHideSelected(false);
                }else{
                    ocf->setSelected(false);
                }
            }
        }
        // wifi界面可见部分最下的一个wifi被选中，有近一半被挡住,所有元素上移51或60
        if (selectY + selectWidgetY == 294){
            wifiListWidget->move(wifiListWidget->x(), wifiListWidget->y()-51);
            if (selectY == 294){
                QScrollBar *pScrollBar = scrollAreaw->verticalScrollBar();
                pScrollBar->setSliderPosition(51);
            }
        }else if(selectY + selectWidgetY == 303){
            wifiListWidget->move(wifiListWidget->x(), wifiListWidget->y()-60);
            if (selectY == 294){
                QScrollBar *pScrollBar = scrollAreaw->verticalScrollBar();
                pScrollBar->setSliderPosition(60);
            }
        }

        // 选中元素下面的所有元素下移51
        for(int i = 0;i < wifiList.size(); i ++){
            OneConnForm *ocf = wifiList.at(i);
            if(ocf->y() > selectY){
                ocf->move(0, ocf->y() + 51);
            }
        }
        if(lbWifiList->y() > selectY){
            lbWifiList->move(12, 68 + 51);
        }

        currSelNetName = wifiName;
    }

}

void MainWindow::activeLanDisconn()
{
    syslog(LOG_DEBUG, "Wired net is disconnected");
    currSelNetName = "";
    this->startLoading();
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
    this->startLoading();
    emit this->deleteRedundantNet();
}

void MainWindow::activeGetWifiList()
{
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

    ui->lbLanImg->setStyleSheet("QLabel{background-image:url(:/res/x/network-line.png);}");
    ui->lbBtnNetBG->setStyleSheet(btnOnQss);
    ui->lbBtnNetT1->setText(tr("Enabled"));//"已开启"

    // 打开网络开关时如果Wifi开关是打开的，设置其样式
    if(checkWlOn()){
        ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-line.png);}");
        ui->lbBtnWifiBG->setStyleSheet(btnOnQss);
        ui->lbBtnWifiT1->setText(tr("Enabled"));//"已开启"
    }

    on_btnNetList_clicked(1);
    is_stop_check_net_state = 0;

    qDebug()<<"debug: already turn on the switch of lan network";
    syslog(LOG_DEBUG, "Already turn on the switch of lan network");
}

void MainWindow::disNetDone()
{
    // 清空lan列表
    lanListWidget = new QWidget(scrollAreal);
    lanListWidget->resize(314, 8 + 60 + 46 + 51);
    scrollAreal->setWidget(lanListWidget);

    // 当前连接的lan
    OneLancForm *ccf = new OneLancForm(lanListWidget, this, confForm, ksnm);
    ccf->setName(tr("Not connected"));//"当前未连接任何 以太网"
    ccf->setIcon(false);
    ccf->setConnedString(tr("Disconnected"));//"未连接"
    ccf->setBandWidth("--");
    ccf->setShowPoint(true);
    ccf->setAct(true);
    ccf->move(0, 8);
    ccf->show();

    // 名为'可用网络列表'一栏
    lbLanList = new QLabel(lanListWidget);
    lbLanList->setText(tr("Ethernet Networks"));//"可用网络列表"
    lbLanList->resize(260, 46);
    lbLanList->move(12, 68);
    lbLanList->setStyleSheet("QLabel{font-size:12px;color:white;}");
    lbLanList->show();

    ui->lbLanImg->setStyleSheet("QLabel{background-image:url(:/res/x/network-offline.png);}");
    ui->lbBtnNetBG->setStyleSheet(btnOffQss);
    ui->lbBtnNetT1->setText(tr("Disabled"));//"已关闭"

    ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-offline.png);}");
    ui->lbBtnWifiBG->setStyleSheet(btnOffQss);
    ui->lbBtnWifiT1->setText(tr("Disabled"));//"已关闭"

    this->lanListWidget->show();
    this->wifiListWidget->hide();
    this->scrollAreal->show();
    this->scrollAreaw->hide();

    on_btnNetList_pressed();

    qDebug()<<"debug: already turn off the switch of lan network";
    syslog(LOG_DEBUG, "Already turn off the switch of lan network");

    this->stopLoading();
}

void MainWindow::launchLanDone()
{
    ui->lbLanImg->setStyleSheet("QLabel{background-image:url(:/res/x/network-line.png);}");
    ui->lbBtnNetBG->setStyleSheet(btnOnQss);
    ui->lbBtnNetT1->setText(tr("Enabled"));//"已开启"
}

void MainWindow::enWifiDone()
{
    ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-line.png);}");
    ui->lbBtnWifiBG->setStyleSheet(btnOnQss);
    ui->lbBtnWifiT1->setText(tr("Enabled"));//"已开启"

    is_update_wifi_list = 0;
//    on_btnWifiList_clicked();
    this->ksnm->execGetWifiList();

    qDebug()<<"debug: already turn on the switch of wifi network";
    syslog(LOG_DEBUG, "Already turn on the switch of wifi network");
}

void MainWindow::disWifiDone()
{
    wifiListWidget->resize(314, 8 + 60 + 46 + 51);
    QList<OneConnForm *> wifiList = wifiListWidget->findChildren<OneConnForm *>();
    for(int i = 0; i < wifiList.size(); i ++){
        OneConnForm *ocf = wifiList.at(i);
        if(ocf->isActive == true){
            ocf->setSelected(false);
            ocf->setName(tr("Not connected"));//"当前未连接任何 Wifi"
            ocf->setSafe("--");
            ocf->setSignal("0");
            ocf->setSafeString("--");
            ocf->setConnedString(tr("Disconnected"));//"未连接"
            ocf->setShowPoint(false);
            disconnect(ocf, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
        }else{
            ocf->deleteLater();
        }
    }
    lbWifiList->move(12, 68);

    ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-offline.png);}");
    ui->lbBtnWifiBG->setStyleSheet(btnOffQss);
    ui->lbBtnWifiT1->setText(tr("Disabled"));//"已关闭"

    this->lanListWidget->hide();
    this->wifiListWidget->show();
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
        ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-offline.png);}");
        ui->lbBtnWifiBG->setStyleSheet(btnOffQss);
        ui->lbBtnWifiT1->setText(tr("Disabled"));//"已关闭"
    }
    if(this->is_btnWifiList_clicked== 1) {
        wifiListWidget->resize(314, 8 + 60 + 46 + 51);
        QList<OneConnForm *> wifiList = wifiListWidget->findChildren<OneConnForm *>();
        for(int i = 0; i < wifiList.size(); i ++){
            OneConnForm *ocf = wifiList.at(i);
            if(ocf->isActive == true){
                ocf->setSelected(false);
                ocf->setName(tr("Not connected"));//"当前未连接任何 Wifi"
                ocf->setSafe("--");
                ocf->setSignal("0");
                ocf->setSafeString("--");
                ocf->setConnedString(tr("Disconnected"));//"未连接"
                ocf->setShowPoint(true);
                disconnect(ocf, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
            }else{
                ocf->deleteLater();
            }
        }
        lbWifiList->move(12, 68);

        ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-offline.png);}");
        ui->lbBtnWifiBG->setStyleSheet(btnOffQss);
        ui->lbBtnWifiT1->setText(tr("Disabled"));//"已关闭"

        this->lanListWidget->hide();
        this->wifiListWidget->show();
        this->scrollAreal->hide();
        this->scrollAreaw->show();

//        this->stopLoading();
    }
}
