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

QString llname, lwname, bandWidth, hideWiFiConn;
int currentActWifiSignalLv, count_loop;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    checkSingle();

    // 如果使用Qt::Popup 任务栏不显示且保留X事件如XCB_FOCUS_OUT, 但如果indicator点击鼠标右键触发，XCB_FOCUS_OUT事件依然会失效
    // 如果使用Qt::ToolTip, Qt::Tool + Qt::WindowStaysOnTopHint, Qt::X11BypassWindowManagerHint等flag则会导致X事件失效
//    this->setWindowFlags(Qt::Popup);
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::FramelessWindowHint);

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

    getIface();// 初始化网络

    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkSingle(){
    int fd = open("/tmp/kylin-nm-lock", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        exit(1);
    }

    if (lockf(fd, F_TLOCK, 0)) {
        qDebug()<<"can't lock single file, exit.";
        exit(0);
    }
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result){
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
    trayIcon->setContextMenu(trayIconMenu);

    // 初始化托盘所有Icon
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
//    QString iconDir = "/usr/share/icons/ukui-icon-theme/48x48/status/";
//    iconLanOnline = QIcon(iconDir + "nm-device-wire.png");
//    iconLanOffline = QIcon(iconDir + "nm-no-connection.png");
//    iconWifiFull = QIcon(iconDir + "nm-signal-100.png");
//    iconWifiHigh = QIcon(iconDir + "nm-signal-75.png");
//    iconWifiMedium = QIcon(iconDir + "nm-signal-50.png");
//    iconWifiLow = QIcon(iconDir + "nm-signal-25.png");
//    iconConnecting = QIcon(iconDir + ".png");

>>>>>>> f394204a0029c50a72582ef39f3437ef9b0e3089
>>>>>>> 0dba7561c8feee9f728396f4a556ce3ae4ddbfce
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

void MainWindow::iconStep(){
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

void MainWindow::startLoading(){
    loading->startLoading();
    setTrayLoading(true);
}

void MainWindow::stopLoading(){
    loading->stopLoading();
    setTrayLoading(false);
    getActiveInfo();
}

// 获取当前连接的网络和状态并设置图标
void MainWindow::getActiveInfo(){
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

// 初始化网络
void MainWindow::getIface(){
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    wname = iface->wname;
    lwname = iface->wname;
    lname = iface->lname;
    llname = iface->lname;

    bandWidth = bt->execChkLanWidth(lname);

    // 开关状态
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
        if (iface->wstate == 0) {
            connDone(3);
        } else if(iface->lstate == 0) {
            connLanDone(3);
        } else if(iface->wstate == 1 && (iface->lstate == 1)) {
            qDebug()<<"启动软件,即将循环检测 Lan或Wifi 是否开启";
            check_isNetOn = new QTimer(this);
            check_isNetOn->setTimerType(Qt::PreciseTimer);
            QObject::connect(check_isNetOn, SIGNAL(timeout()), this, SLOT(on_isNetOn()));
            check_isNetOn->start(3000);
        }
        on_btnWifiList_clicked();
        ui->btnWifiList->setStyleSheet("#btnWifiList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
        ui->btnNetList->setStyleSheet("#btnNetList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
                                      "#btnNetList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
    }else{
        if(iface->lstate != 2){
            if (iface->lstate == 0) {
                connLanDone(3);
            }
            on_btnNetList_clicked();
            ui->btnNetList->setStyleSheet("#btnNetList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
            ui->btnWifiList->setStyleSheet("#btnWifiList{font-size:12px;color:white;border:1px solid rgba(255,255,255,0.1);background:transparent;background-color:rgba(0,0,0,0.2);}"
                                          "#btnWifiList:Pressed{border:1px solid rgba(255,255,255,0.5);background:transparent;background-color:rgba(255,255,255,0.1);}");
        }else{
            disNetDone();
        }
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    moveBottomRight();
    if(this->isHidden()){
        this->showNormal();
    }else{
        this->hide();
    }
//    switch (reason) {
//    case QSystemTrayIcon::Trigger:
//    case QSystemTrayIcon::DoubleClick:
//    case QSystemTrayIcon::MiddleClick:
//        break;
//    }
}

void MainWindow::moveBottomRight()
{
    QPoint pos = QCursor::pos();
    QRect primaryGeometry;
    for (QScreen *screen : qApp->screens()) {
        if (screen->geometry().contains(pos)) {
            primaryGeometry = screen->geometry();
        }
    }

    if (primaryGeometry.isEmpty()) {
        primaryGeometry = qApp->primaryScreen()->geometry();
    }

    this->move(primaryGeometry.x() + primaryGeometry.width() - this->width(), primaryGeometry.height() - this->height());
}

bool MainWindow::checkLanOn(){
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    if(iface->lstate == 2){
        return false;
    }else{
        return true;
    }
}

bool MainWindow::checkWlOn(){
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    if(iface->wstate == 2){
        return false;
    }else{
        return true;
    }
}

// 获取lan列表回调
void MainWindow::getLanListDone(QStringList slist){
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
                connect(ccf, SIGNAL(selectedOneLanForm(QString)), this, SLOT(oneLanFormSelected(QString)));
                connect(ccf, SIGNAL(disconnActiveLan()), this, SLOT(activeLanDisconn()));
                ccf->setName(nname);
                ccf->setIcon(true);
                ccf->setBandWidth(bandWidth);
                ccf->setShowPoint(true);
                ccf->setConnedString(tr("Connected"));//"已连接"
                currSelNetName = "";
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
}

// 获取wifi列表回调
void MainWindow::getWifiListDone(QStringList slist){
    if (updateFlag == 0){
        loadWifiListDone(slist);
    } else {
        updateWifiListDone(slist);
        updateFlag = 0;
    }
    lastSlist = slist;
}

// 加载wifi列表
void MainWindow::loadWifiListDone(QStringList slist){
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
            if(addName == wname){
                isContinue = true;
            }
        }
        if(isContinue){
            continue;
        }

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
//    connect(hideNetButton, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneHideFormSelected(QString)));
    connect(hideNetButton, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
    hideNetButton->setSpecialName(hideWiFiConn);
    hideNetButton->setSignal(0);
    hideNetButton->setSafe("Safe");
    hideNetButton->move(0, 114 + count * 60);
    hideNetButton->show();

    this->lanListWidget->hide();
    this->wifiListWidget->show();
    this->stopLoading();
}

// 更新wifi列表
void MainWindow::updateWifiListDone(QStringList slist)
{
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

    //获取表头信息
    QString lastHeadLine = lastSlist.at(0);
    lastHeadLine = lastHeadLine.trimmed();
    int lastIndexName = lastHeadLine.indexOf("SSID");

    QString headLine = slist.at(0);
    headLine = headLine.trimmed();
    int indexRate = headLine.indexOf("RATE");
    int indexSecu = headLine.indexOf("SECURITY");
    int indexName = headLine.indexOf("SSID");

    //列表中去除已经减少的wifi
    for (int i=1; i<lastSlist.size(); i++){
        QString line = lastSlist.at(i);
        QString lastWname = line.mid(lastIndexName).trimmed();
        for (int j=1; j<slist.size(); j++){
            QString line = slist.at(j);
            QString wname = line.mid(indexName).trimmed();

            if (lastWname == wname){break;} //到slist最后一个都没找到
            if (j == slist.size()-1){
                qDebug()<<"will delete a wifi named "<<lastWname;
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

        for (int j=1; j < lastSlist.size(); j++){
            QString line = lastSlist.at(j);
            QString lastWname = line.mid(lastIndexName).trimmed();

            if (lastWname == wname){break;}
            //到lastSlist最后一个都没找到
            if (j == lastSlist.size()-1){
                qDebug()<<"will insert a wifi named "<<wname;
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
                //connect(hideNetButton, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneHideFormSelected(QString)));
                connect(hideNetButton, SIGNAL(selectedOneWifiForm(QString)), this, SLOT(oneWifiFormSelected(QString)));
                hideNetButton->setSpecialName(hideWiFiConn);
                hideNetButton->setSignal(0);
                hideNetButton->setSafe("Safe");
                hideNetButton->move(0, lastOcf->y()+60);
                hideNetButton->setHideSelected(false);
                if (currSelNetName == hideWiFiConn){
                    hideNetButton->setHideSelected(true);
                }
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
    QString wlan_card = "iwconfig>/tmp/kylin-nm-iwconfig";
    system(wlan_card.toUtf8().data());

    QFile file("/tmp/kylin-nm-iwconfig");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file kylin-nm-iwconfig!"<<endl;
    }
    QString txt = file.readAll();
    file.close();

    //当连接上无线网卡时才能打开wifi开关
    if(txt.indexOf("IEEE 802.11") != -1){
        // 网络开关关闭时，点击Wifi开关无效
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
                QThread *t = new QThread();
                BackThread *bt = new BackThread();
                bt->moveToThread(t);
                connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
                connect(t, SIGNAL(started()), bt, SLOT(execEnWifi()));
                connect(bt, SIGNAL(enWifiDone()), this, SLOT(enWifiDone()));
                connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
                t->start();
            }

            this->startLoading();
        }
    } else {
        QThread *t = new QThread();
        BackThread *bt = new BackThread();
        bt->moveToThread(t);
        connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
        connect(t, SIGNAL(started()), bt, SLOT(execDisWifi()));
        connect(bt, SIGNAL(disWifiDone()), this, SLOT(keepDisWifiState()));
        connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
        t->start();
    }
}

void MainWindow::on_btnNetList_clicked(int flag)
{
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

// Lan连接结果，0点击连接成功 1失败 3开机直连成功
void MainWindow::connLanDone(int connFlag){
    if(connFlag == 0){
        this->is_NetLineReady = 1;
        this->isByClickConnect = 1;
        this->ksnm->execGetLanList();
        QString txt(tr("Conn Ethernet Success"));
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
        system(cmd.toUtf8().data());

        check_isLanConnect = new QTimer(this);
        check_isLanConnect->setTimerType(Qt::PreciseTimer);
        QObject::connect(check_isLanConnect, SIGNAL(timeout()), this, SLOT(on_isLanConnect()));
        check_isLanConnect->start(4000);
    }

    if(connFlag == 1){
        this->is_NetLineReady = 0; //without net line connect to computer
        QString txt(tr("Conn Ethernet Fail"));
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
        system(cmd.toUtf8().data());
    }

    if(connFlag == 3){
        this->is_NetLineReady = 1;
        check_isLanConnect = new QTimer(this);
        check_isLanConnect->setTimerType(Qt::PreciseTimer);
        QObject::connect(check_isLanConnect, SIGNAL(timeout()), this, SLOT(on_isLanConnect()));
        check_isLanConnect->start(4000);
    }

    this->stopLoading();
}

void MainWindow::on_isLanConnect()
{
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    if (iface->lstate == 1){
        qDebug()<<"注意：有线网络连接已经断开";
        if(this->is_btnNetList_clicked == 1) {
            this->ksnm->execGetLanList();
        }
        if(this->is_btnWifiList_clicked== 1) {
            this->ksnm->execGetWifiList();
        }
        this->isByClickConnect = 0;
        check_isLanConnect->stop();

        if (iface->wstate != 0){
            qDebug()<<"即将检测是否重新开启 Lan或Wifi";
            check_isNetOn = new QTimer(this);
            check_isNetOn->setTimerType(Qt::PreciseTimer);
            QObject::connect(check_isNetOn, SIGNAL(timeout()), this, SLOT(on_isNetOn()));
            check_isNetOn->start(4000);
        }
    } else if (iface->wstate != 2) {
        count_loop += 1;
        if (count_loop >= 2 && this->is_btnWifiList_clicked == 1){
            //period update wifilist
            updateFlag = 1;
            this->ksnm->execGetWifiList();
            count_loop = 0;
        }
    }

    if (iface->wstate == 2) {
        on_btnWifi_clicked();
    }
}

void MainWindow::on_isNetOn()
{
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    if (iface->lstate == 0 && this->isByClickConnect == 0 && this->is_NetLineReady == 1){
        qDebug()<<"注意：有线网络已经重新连接";
        if(this->is_btnNetList_clicked == 1) {
            this->ksnm->execGetLanList();
        }
        if(this->is_btnWifiList_clicked== 1) {
            this->ksnm->execGetWifiList();
        }
        check_isNetOn->stop();

        qDebug()<<"即将重新检测 Lan 是否断开";
        check_isLanConnect = new QTimer(this);
        check_isLanConnect->setTimerType(Qt::PreciseTimer);
        QObject::connect(check_isLanConnect, SIGNAL(timeout()), this, SLOT(on_isLanConnect()));
        check_isLanConnect->start(4000);
    } else if (iface->lstate == 0 && this->isByClickConnect == 1){
        qDebug()<<"注意：有线网络已经重新连接";
        check_isNetOn->stop();
    } else if (iface->wstate == 0 && this->isByClickConnect == 0){
        qDebug()<<"注意：Wifi网络已经重新连接";
        if(this->is_btnNetList_clicked == 1) {
            this->ksnm->execGetLanList();
        }
        if(this->is_btnWifiList_clicked== 1) {
            this->ksnm->execGetWifiList();
        }
        check_isNetOn->stop();

        qDebug()<<"即将重新检测 Wifi 是否断开";
        check_isWifiConnect = new QTimer(this);
        check_isWifiConnect->setTimerType(Qt::PreciseTimer);
        QObject::connect(check_isWifiConnect, SIGNAL(timeout()), this, SLOT(on_isWifiConnect()));
        check_isWifiConnect->start(4000);
    } else if (iface->wstate == 0 && this->isByClickConnect == 1){
        qDebug()<<"注意：Wifi网络已经重新连接";
        check_isNetOn->stop();
    } else if (iface->wstate != 2) {
        count_loop += 1;
        if (count_loop >= 2 && this->is_btnWifiList_clicked == 1){
            //period update wifilist
            updateFlag = 1;
            this->ksnm->execGetWifiList();
            count_loop = 0;
        }
    }

    if (iface->wstate == 2) {
        on_btnWifi_clicked();
    }
}

// Wifi连接结果，0点击连接成功 1失败 2没有配置文件 3开机直连成功
void MainWindow::connDone(int connFlag){
    if(connFlag == 0){
        qDebug()<<"Wifi connected successfully";
        this->isByClickConnect = 1;
        this->ksnm->execGetWifiList();
        QString txt(tr("Conn Wifi Success"));
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
        system(cmd.toUtf8().data());

        check_isWifiConnect = new QTimer(this);
        check_isWifiConnect->setTimerType(Qt::PreciseTimer);
        QObject::connect(check_isWifiConnect, SIGNAL(timeout()), this, SLOT(on_isWifiConnect()));
        check_isWifiConnect->start(4000);
    } else if (connFlag == 3) {
        check_isWifiConnect = new QTimer(this);
        check_isWifiConnect->setTimerType(Qt::PreciseTimer);
        QObject::connect(check_isWifiConnect, SIGNAL(timeout()), this, SLOT(on_isWifiConnect()));
        check_isWifiConnect->start(4000);
    }
}

void MainWindow::on_isWifiConnect()
{
    BackThread *bt = new BackThread();
    IFace *iface = bt->execGetIface();

    if (iface->wstate == 1){
        qDebug()<<"注意：Wifi 网络连接已经断开";
        if(this->is_btnNetList_clicked == 1) {
            this->ksnm->execGetLanList();
        }
        if(this->is_btnWifiList_clicked== 1) {
            this->ksnm->execGetWifiList();
        }
        this->isByClickConnect = 0;
        check_isWifiConnect->stop();

        if (iface->lstate != 0){
            qDebug()<<"即将检测是否重新开启 Lan或Wifi";
            check_isNetOn = new QTimer(this);
            check_isNetOn->setTimerType(Qt::PreciseTimer);
            QObject::connect(check_isNetOn, SIGNAL(timeout()), this, SLOT(on_isNetOn()));
            check_isNetOn->start(4000);
        }
    } else if (iface->wstate != 2){
        count_loop += 1;
        if (count_loop >= 2 && this->is_btnWifiList_clicked == 1){
            //period update wifilist
            updateFlag = 1;
            this->ksnm->execGetWifiList();
            count_loop = 0;
        }
    }

    if (iface->wstate == 2) {
        on_btnWifi_clicked();
    }
}

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

void MainWindow::oneLanFormSelected(QString lanName){
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

void MainWindow::oneWifiFormSelected(QString wifiName){
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
        // 设置选中，放大缩小所有选项卡
        int selectY = 0;
        for(int i = 0;i < wifiList.size(); i ++){
            OneConnForm *ocf = wifiList.at(i);
            if(ocf->wifiName == wifiName){
                if (ocf->wifiName == hideWiFiConn){
                    ocf->setHideSelected(true);
                }else{
                    ocf->setSelected(true);
                }
                selectY = ocf->y();
            }else{
                if (ocf->wifiName == hideWiFiConn){
                    ocf->setHideSelected(false);
                }else{
                    ocf->setSelected(false);
                }
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

void MainWindow::oneHideFormSelected(QString wifiName){
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

    //是否与上一次选中同一个网络框 0否 1是
    int isReSelect = 0;
    if (currSelNetName == wifiName){
        isReSelect = 1;
        currSelNetName = "";
    } else {
        isReSelect = 0;
        currSelNetName = wifiName;
    }


    // 设置选中，放大缩小所有选项卡
    int selectY = 0;
    for(int i = 0;i < wifiList.size(); i ++){
        OneConnForm *ocf = wifiList.at(i);
        if (ocf->wifiName == hideWiFiConn){
            if (isReSelect == 0){
                ocf->setHideSelected(true);
                selectY = ocf->y();
            } else if (isReSelect == 1){
                ocf->setHideSelected(false);
            }
        } else {
            ocf->setSelected(false);
        }
    }
}

void MainWindow::activeLanDisconn(){
    this->startLoading();
    this->ksnm->execGetLanList();
    currSelNetName = "";
}

void MainWindow::activeWifiDisconn(){
    this->startLoading();
    this->ksnm->execGetWifiList();
    currSelNetName = "";
}

void MainWindow::on_btnAdvConf_pressed()
{
    ui->lbBtnConfBG->setStyleSheet(btnOnQss);
}

void MainWindow::on_btnAdvConf_released()
{
    ui->lbBtnConfBG->setStyleSheet(btnOffQss);
}

void MainWindow::enNetDone(){
    BackThread *bt = new BackThread();
    bandWidth = bt->execChkLanWidth(lname);

    ui->lbLanImg->setStyleSheet("QLabel{background-image:url(:/res/x/network-line.png);}");
    ui->lbBtnNetBG->setStyleSheet(btnOnQss);
    ui->lbBtnNetT1->setText(tr("Enabled"));//"已开启"

    // 打开网络开关时如果Wifi开关是打开的，设置其样式
    if(checkWlOn()){
        ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-line.png);}");
        ui->lbBtnWifiBG->setStyleSheet(btnOnQss);
        ui->lbBtnWifiT1->setText(tr("Enabled"));//"已开启"
    }

    this->stopLoading();

    on_btnNetList_clicked(1);
}

void MainWindow::disNetDone(){
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

    // 名为可用网络列表一栏
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

    this->stopLoading();
}

void MainWindow::enWifiDone(){
    ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-line.png);}");
    ui->lbBtnWifiBG->setStyleSheet(btnOnQss);
    ui->lbBtnWifiT1->setText(tr("Enabled"));//"已开启"

    this->stopLoading();

    on_btnWifiList_clicked();
}

void MainWindow::disWifiDone(){
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

    this->stopLoading();
}

void MainWindow::keepDisWifiState()
{
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 0dba7561c8feee9f728396f4a556ce3ae4ddbfce
    if(this->is_btnNetList_clicked == 1) {
        ui->lbWifiImg->setStyleSheet("QLabel{background-image:url(:/res/x/wifi-offline.png);}");
        ui->lbBtnWifiBG->setStyleSheet(btnOffQss);
        ui->lbBtnWifiT1->setText(tr("Disabled"));//"已关闭"
    }
    if(this->is_btnWifiList_clicked== 1) {
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

        this->stopLoading();
    }
<<<<<<< HEAD
=======
=======
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

    on_btnWifiList_clicked();

    this->stopLoading();
>>>>>>> f394204a0029c50a72582ef39f3437ef9b0e3089
>>>>>>> 0dba7561c8feee9f728396f4a556ce3ae4ddbfce
}
