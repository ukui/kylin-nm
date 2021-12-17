#include "mainwindow.h"
#include "customstyle.h"
#include <KWindowEffects>
#include <QApplication>
#include <QDebug>
#include <QDBusReply>
#include <QKeyEvent>
#include <QProcess>

#include "kylinnetworkdeviceresource.h"
#include "../backend/dbus-interface/kylinagentinterface.h"

#define MAINWINDOW_WIDTH 420
#define MAINWINDOW_HEIGHT 476
#define LAYOUT_MARGINS 0,0,0,0
#define LOADING_TRAYICON_TIMER_MS 60
#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

#define LAN_PAGE_INDEX 0
#define WLAN_PAGE_INDEX 1

#include <kwindowsystem.h>
#include <kwindowsystem_export.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    firstlyStart();
}

/**
 * @brief MainWindow::showMainwindow show主窗口，同时也作为dbus接口提供给外部组件调用
 */
void MainWindow::showMainwindow()
{
    if (!m_loadFinished) {
        m_secondaryStartTimer->stop();
        secondaryStart();
    }

    /**
     * 设置主界面跳过任务栏和分页器的属性，隐藏再次展示有可能辉冲刷掉该属性，需要展示时重新设置
     */
    const KWindowInfo info(this->winId(), NET::WMState);
    if (!info.hasState(NET::SkipTaskbar) || !info.hasState(NET::SkipPager)) {
        KWindowSystem::setState(this->winId(), NET::SkipTaskbar | NET::SkipPager);
    }

    this->resetWindowPosition();
    this->showNormal();
    this->raise();
    this->activateWindow();
    emit this->mainWindowVisibleChanged(true);
}

/**
 * @brief MainWindow::hideMainwindow 隐藏主页面时要进行的操作，后续可以添加到此函数
 */
void MainWindow::hideMainwindow()
{
    this->hide();
    emit this->mainWindowVisibleChanged(false);
}

/**
 * @brief MainWindow::setWiredDefaultDevice 设置有线设备默认网卡
 */
void MainWindow::setWiredDefaultDevice(QString deviceName)
{
//    m_lanWidget->updateDefaultDevice(deviceName);
}

/**
 * @brief MainWindow::hideMainwindow 设置无线设备默认网卡
 */
void MainWindow::setWirelessDefaultDevice(QString deviceName)
{
//    m_wlanWidget->updateDefaultDevice(deviceName);
}

/**
 * @brief MainWindow::firstlyStart 一级启动，执行重要且不耗时的启动操作
 */
void MainWindow::firstlyStart()
{
    initWindowProperties();
    initTransparency();
    initUI();
    initDbusConnnect();
    initWindowTheme();
    initTrayIcon();
    installEventFilter(this);
    m_secondaryStartTimer = new QTimer(this);
    connect(m_secondaryStartTimer, &QTimer::timeout, this, [ = ]() {
        m_secondaryStartTimer->stop();
        secondaryStart();//满足条件后执行比较耗时的二级启动
    });
    m_secondaryStartTimer->start(5 * 1000);

    m_createPagePtrMap.clear();

    //加载key ring
    agent_init();
}

/**
 * @brief MainWindow::secondaryStart 二级启动，可以将较耗时的初始化操作放到此处执行
 */
void MainWindow::secondaryStart()
{
    if (m_loadFinished)
        return;
    m_loadFinished = true;
}

/**
 * @brief MainWindow::initWindowProperties 初始化一些窗口属性
 */
void MainWindow::initWindowProperties()
{
    this->setWindowTitle(tr("kylin-nm"));
    this->setWindowIcon(QIcon::fromTheme("kylin-network", QIcon(":/res/x/setup.png")));
    this->setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
//    //绘制毛玻璃特效
    this->setAttribute(Qt::WA_TranslucentBackground, true);  //透明
    this->setFocusPolicy(Qt::NoFocus);

    QPainterPath path;
    auto rect = this->rect();
    path.addRoundedRect(rect, 12, 12);
    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));   //背景模糊
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
    auto rect = this->rect();
    painter.drawRoundedRect(rect, 12, 12);      //窗口圆角
}

void MainWindow::initTransparency()
{
    if(QGSettings::isSchemaInstalled(TRANSPARENCY_GSETTINGS)) {
        m_transGsettings = new QGSettings(TRANSPARENCY_GSETTINGS);
        if(m_transGsettings->keys().contains(QString("transparency"))) {
            m_transparency=m_transGsettings->get("transparency").toDouble() + 0.15;
            m_transparency = (m_transparency > 1) ? 1 : m_transparency;
            connect(m_transGsettings, &QGSettings::changed, this, &MainWindow::onTransChanged);
        }
    }
}

void MainWindow::onTransChanged()
{
    m_transparency = m_transGsettings->get("transparency").toDouble() + 0.15;
    m_transparency = (m_transparency > 1) ? 1 : m_transparency;
    paintWithTrans();
}

void MainWindow::paintWithTrans()
{
    QPalette pal = m_centralWidget->palette();
    QColor color = m_centralWidget->palette().base().color();
    color.setAlphaF(m_transparency);
    pal.setColor(QPalette::Base, color);
    m_centralWidget->setPalette(pal);

    QPalette tabPal = m_centralWidget->tabBar()->palette();
    tabPal.setColor(QPalette::Base, color);

    QColor inactiveColor = pal.color(QPalette::Window);
    inactiveColor.setAlphaF(0.75 *m_transparency);
    pal.setColor(QPalette::Window, inactiveColor);

    m_centralWidget->tabBar()->setPalette(pal);
}

/**
 * @brief MainWindow::initUI 初始化窗口内控件
 */
void MainWindow::initUI()
{
    m_centralWidget = new QTabWidget(this);
    this->setCentralWidget(m_centralWidget);
    m_centralWidget->tabBar()->setFixedWidth(this->width());
    m_centralWidget->tabBar()->setStyleSheet("QTabBar::tab{min-height:40px}");
    m_lanWidget = new LanPage(m_centralWidget);
    m_wlanWidget = new WlanPage(m_centralWidget);
    connect(this, &MainWindow::mainWindowVisibleChanged, m_wlanWidget, &WlanPage::onMainWindowVisibleChanged);
//    m_centralWidget->addTab(m_lanWidget, QIcon::fromTheme("network-wired-connected-symbolic", QIcon::fromTheme("network-wired-symbolic", QIcon(":/res/l/network-online.svg"))), tr("LAN"));
//    m_centralWidget->addTab(m_wlanWidget, QIcon::fromTheme("network-wireless-signal-excellent-symbolic", QIcon(":/res/x/wifi-list-bg.svg")), tr("WLAN"));

    m_centralWidget->addTab(m_lanWidget, tr(""));
    m_centralWidget->addTab(m_wlanWidget,tr(""));
    m_tabBarLayout = new QHBoxLayout(this);
    m_tabBarLayout->setContentsMargins(LAYOUT_MARGINS);
    m_lanLabel = new QLabel(tr("LAN"));
    m_lanLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_wlanLabel = new QLabel(tr("WLAN"));
    m_wlanLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_tabBarLayout->addWidget(m_lanLabel);
    m_tabBarLayout->addWidget(m_wlanLabel);
    m_centralWidget->tabBar()->setLayout(m_tabBarLayout);
    connect(m_centralWidget, &QTabWidget::currentChanged, m_wlanWidget, &WlanPage::onWlanPageVisibleChanged);
    paintWithTrans();
}

/**
 * @brief MainWindow::initTrayIcon 初始化托盘图标和托盘右键菜单
 */
void MainWindow::initTrayIcon()
{
    loadIcons.append(QIcon::fromTheme("kylin-network-1"));
    loadIcons.append(QIcon::fromTheme("kylin-network-2"));
    loadIcons.append(QIcon::fromTheme("kylin-network-3"));
    loadIcons.append(QIcon::fromTheme("kylin-network-4"));
    loadIcons.append(QIcon::fromTheme("kylin-network-5"));
    loadIcons.append(QIcon::fromTheme("kylin-network-6"));
    loadIcons.append(QIcon::fromTheme("kylin-network-7"));
    loadIcons.append(QIcon::fromTheme("kylin-network-8"));
    loadIcons.append(QIcon::fromTheme("kylin-network-9"));
    loadIcons.append(QIcon::fromTheme("kylin-network-10"));
    loadIcons.append(QIcon::fromTheme("kylin-network-11"));
    loadIcons.append(QIcon::fromTheme("kylin-network-12"));
    iconTimer = new QTimer(this);
    connect(iconTimer, &QTimer::timeout, this, &MainWindow::onSetTrayIconLoading);

    m_trayIcon = new QSystemTrayIcon();
    m_trayIconMenu = new QMenu();
    m_showMainwindowAction = new QAction(tr("Show MainWindow"),this);
    m_showSettingsAction = new QAction(tr("Settings"),this);

    m_trayIcon->setToolTip(QString(tr("kylin-nm")));
    m_showSettingsAction->setIcon(QIcon::fromTheme("document-page-setup-symbolic", QIcon(":/res/x/setup.png")) );
//    m_trayIconMenu->addAction(m_showMainwindowAction);
    m_trayIconMenu->addAction(m_showSettingsAction);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setIcon(QIcon::fromTheme("network-wired-signal-excellent-symbolic"));
    iconStatus = IconActiveType::LAN_CONNECTED;
    onRefreshTrayIcon();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
//    connect(m_showMainwindowAction, &QAction::triggered, this, &MainWindow::onShowMainwindowActionTriggled);
    connect(m_showSettingsAction, &QAction::triggered, this, &MainWindow::onShowSettingsActionTriggled);
    m_trayIcon->show();
}

void MainWindow::initDbusConnnect()
{
    connect(m_lanWidget, &LanPage::deviceStatusChanged, this, &MainWindow::deviceStatusChanged);
    connect(m_lanWidget, &LanPage::deviceNameChanged, this, &MainWindow::deviceNameChanged);
    connect(m_wlanWidget, &WlanPage::deviceStatusChanged, this, &MainWindow::deviceStatusChanged);
    connect(m_wlanWidget, &WlanPage::deviceNameChanged, this, &MainWindow::deviceNameChanged);

    connect(m_wlanWidget, &WlanPage::activateFailed, this, &MainWindow::activateFailed);
    connect(m_wlanWidget, &WlanPage::deactivateFailed, this, &MainWindow::deactivateFailed);
    connect(m_lanWidget, &LanPage::activateFailed, this, &MainWindow::activateFailed);
    connect(m_lanWidget, &LanPage::deactivateFailed, this, &MainWindow::deactivateFailed);

    connect(m_lanWidget, &LanPage::lanAdd, this, &MainWindow::lanAdd);
    connect(m_lanWidget, &LanPage::lanRemove, this, &MainWindow::lanRemove);
    connect(m_lanWidget, &LanPage::lanUpdate, this, &MainWindow::lanUpdate);
    connect(m_lanWidget, &LanPage::lanActiveConnectionStateChanged, this, &MainWindow::lanActiveConnectionStateChanged);
    connect(m_lanWidget, &LanPage::lanConnectChanged, this, &MainWindow::onLanConnectStatusToChangeTrayIcon);


    connect(m_wlanWidget, &WlanPage::wlanAdd, this, &MainWindow::wlanAdd);
    connect(m_wlanWidget, &WlanPage::wlanRemove, this, &MainWindow::wlanRemove);
    connect(m_wlanWidget, &WlanPage::wlanActiveConnectionStateChanged, this, &MainWindow::wlanactiveConnectionStateChanged);
    connect(m_wlanWidget, &WlanPage::wlanConnectChanged, this, &MainWindow::onWlanConnectStatusToChangeTrayIcon);
    connect(m_wlanWidget, &WlanPage::hotspotDeactivated, this, &MainWindow::hotspotDeactivated);
    connect(m_wlanWidget, &WlanPage::hotspotActivated, this, &MainWindow::hotspotActivated);
    connect(m_wlanWidget, &WlanPage::secuTypeChange, this, &MainWindow::secuTypeChange);
    connect(m_wlanWidget, &WlanPage::signalStrengthChange, this, &MainWindow::signalStrengthChange);
    connect(m_wlanWidget, &WlanPage::timeToUpdate , this, &MainWindow::timeToUpdate);
    connect(m_wlanWidget, &WlanPage::showMainWindow, this, &MainWindow::onShowByWlanPage);
    connect(m_wlanWidget, &WlanPage::connectivityChanged, this, &MainWindow::onConnectivityChanged);
}

/**
 * @brief MainWindow::resetWindowPosition 重新计算窗口位置
 */
void MainWindow::resetWindowPosition()
{
#define MARGIN 4
#define PANEL_TOP 1
#define PANEL_LEFT 2
#define PANEL_RIGHT 3
//#define PANEL_BOTTOM 4
    if (!m_positionInterface) {
        m_positionInterface = new QDBusInterface("org.ukui.panel",
                            "/panel/position",
                            "org.ukui.panel",
                            QDBusConnection::sessionBus());
    }
    QDBusReply<QVariantList> reply = m_positionInterface->call("GetPrimaryScreenGeometry");
    //reply获取的参数共5个，分别是 主屏可用区域的起点x坐标，主屏可用区域的起点y坐标，主屏可用区域的宽度，主屏可用区域高度，任务栏位置
    if (!m_positionInterface->isValid() || !reply.isValid() || reply.value().size() < 5) {
        qCritical() << QDBusConnection::sessionBus().lastError().message();
        this->setGeometry(0, 0, this->width(), this->height());
        return;
    }
    QVariantList position_list = reply.value();
    int position = position_list.at(4).toInt();
    switch(position){
    case PANEL_TOP:
        //任务栏位于上方
        this->setGeometry(position_list.at(0).toInt() + position_list.at(2).toInt() - this->width() - MARGIN,
                          position_list.at(1).toInt() + MARGIN,
                          this->width(), this->height());
        break;
        //任务栏位于左边
    case PANEL_LEFT:
        this->setGeometry(position_list.at(0).toInt() + MARGIN,
                          position_list.at(1).toInt() + reply.value().at(3).toInt() - this->height() - MARGIN,
                          this->width(), this->height());
        break;
        //任务栏位于右边
    case PANEL_RIGHT:
        this->setGeometry(position_list.at(0).toInt() + position_list.at(2).toInt() - this->width() - MARGIN,
                          position_list.at(1).toInt() + reply.value().at(3).toInt() - this->height() - MARGIN,
                          this->width(), this->height());
        break;
        //任务栏位于下方
    default:
        this->setGeometry(position_list.at(0).toInt() + position_list.at(2).toInt() - this->width() - MARGIN,
                          position_list.at(1).toInt() + reply.value().at(3).toInt() - this->height() - MARGIN,
                          this->width(), this->height());
        break;
    }
    qDebug() << " Position of ukui-panel is " << position << "; Position of mainwindow is " << this->geometry() << "." << Q_FUNC_INFO << __LINE__;
}

/**
 * @brief MainWindow::resetTrayIconTool 重新获取网络连接状态并重新设置图标和tooltip
 */
void MainWindow::resetTrayIconTool()
{
    //ZJP_TODO 检测当前连接的是有线还是无线，是否可用，设置图标和tooltip,图标最好提前define
//    int connectivity = objKyDBus->getNetworkConectivity();
//    qDebug() << "Value of current network Connectivity property : "<< connectivity;
//    switch (connectivity) {
//    case UnknownConnectivity:
//    case Portal:
//    case Limited:
//        setTrayIcon(iconLanOnlineNoInternet);
//        trayIcon->setToolTip(QString(tr("Network Connected But Can Not Access Internet")));
//        break;
//    case NoConnectivity:
//    case Full:
//        setTrayIcon(iconLanOnline);
//        trayIcon->setToolTip(QString(tr("kylin-nm")));
//        break;
//    }
    qDebug() << "Has set tray icon to be XXX." << Q_FUNC_INFO << __LINE__;
}


/**
 * @brief MainWindow::initWindowTheme 初始化窗口主题并创建信号槽
 */
void MainWindow::initWindowTheme()
{
    const QByteArray style_id(THEME_SCHAME);
    if (QGSettings::isSchemaInstalled(style_id)) {
        m_styleGsettings = new QGSettings(style_id);
//        resetWindowTheme();
        connect(m_styleGsettings, &QGSettings::changed, this, &MainWindow::onThemeChanged);
    } else {
        qWarning() << "Gsettings interface \"org.ukui.style\" is not exist!" << Q_FUNC_INFO << __LINE__;
    }
}

/**
 * @brief MainWindow::resetWindowTheme 读取和设置窗口主题
 */
void MainWindow::resetWindowTheme()
{
    if (!m_styleGsettings) { return; }
    QString currentTheme = m_styleGsettings->get(COLOR_THEME).toString();
    auto app = static_cast<QApplication*>(QCoreApplication::instance());
    if(currentTheme == "ukui-dark" || currentTheme == "ukui-black"){
        app->setStyle(new CustomStyle("ukui-dark"));
        qDebug() << "Has set color theme to ukui-dark." << Q_FUNC_INFO << __LINE__;
        emit qApp->paletteChanged(qApp->palette());
        return;
    }
    app->setStyle(new CustomStyle("ukui-light"));
    qDebug() << "Has set color theme to " << currentTheme << Q_FUNC_INFO << __LINE__;
    emit qApp->paletteChanged(qApp->palette());
    return;
}

/**
 * @brief MainWindow::showControlCenter 打开控制面板网络界面
 */
void MainWindow::showControlCenter()
{
    QProcess process;
    if (!m_lanWidget->lanIsConnected() && m_wlanWidget->wlanIsConnected()){
        process.startDetached("ukui-control-center -m wlanconnect");
    } else {
        process.startDetached("ukui-control-center -m netconnect");
    }
}

/**
 * @brief MainWindow::onTrayIconActivated 点击托盘图标的槽函数
 */
void MainWindow::onTrayIconActivated()
{
    if (this->isVisible()) {
        qDebug() << "Received signal of tray icon activated, will hide mainwindow." << Q_FUNC_INFO << __LINE__;
        hideMainwindow();
        return;
    }
    qDebug() << "Received signal of tray icon activated, will show mainwindow." << Q_FUNC_INFO << __LINE__;
    this->showMainwindow();
}

void MainWindow::onShowMainwindowActionTriggled()
{
    showMainwindow();
}

void MainWindow::onShowSettingsActionTriggled()
{
    showControlCenter();
}

void MainWindow::onThemeChanged(const QString &key)
{
    if (key == COLOR_THEME) {
        qDebug() << "Received signal of theme changed, will reset theme." << Q_FUNC_INFO << __LINE__;
//        resetWindowTheme();
        paintWithTrans();
        emit qApp->paletteChanged(qApp->palette());
    } else {
        qDebug() << "Received signal of theme changed, key=" << key << " will do nothing." << Q_FUNC_INFO << __LINE__;
    }
}

void MainWindow::onRefreshTrayIcon()
{
    //更新托盘图标显示
    iconTimer->stop();
    if (m_lanWidget->lanIsConnected()) {
        m_trayIcon->setIcon(QIcon::fromTheme("network-wired-connected-symbolic"));
        iconStatus = IconActiveType::LAN_CONNECTED;
    } else if (m_wlanWidget->wlanIsConnected()){
        m_trayIcon->setIcon(QIcon::fromTheme("network-wireless-connected-symbolic"));
        iconStatus = IconActiveType::WLAN_CONNECTED;
    } else {
        m_trayIcon->setIcon(QIcon::fromTheme("network-wired-disconnected-symbolic"));
        iconStatus = IconActiveType::NOT_CONNECTED;
    }

    NetworkManager::Connectivity connecttivity;
    m_wlanWidget->getConnectivity(connecttivity);
    if (connecttivity == NetworkManager::Connectivity::Portal || connecttivity == NetworkManager::Connectivity::Limited) {
        if (iconStatus == IconActiveType::LAN_CONNECTED) {
            m_trayIcon->setIcon(QIcon::fromTheme("network-error-symbolic"));
            iconStatus = IconActiveType::LAN_CONNECTED_LIMITED;
        } else if (iconStatus == IconActiveType::WLAN_CONNECTED) {
            //todo 信号强度
            m_trayIcon->setIcon(QIcon::fromTheme("network-wireless-signal-excellent-error-symbolic"));
            iconStatus = IconActiveType::WLAN_CONNECTED_LIMITED;
        }
    }
}

void MainWindow::onSetTrayIconLoading()
{
    if (currentIconIndex > 11) {
        currentIconIndex = 0;
    }
    m_trayIcon->setIcon(loadIcons.at(currentIconIndex));
    iconStatus = IconActiveType::ACTIVATING;
    currentIconIndex ++;
}

void MainWindow::onLanConnectStatusToChangeTrayIcon(int state)
{
    qDebug() << "lan state:" << state << Q_FUNC_INFO << __LINE__;
    if (state==1 || state==3){
        m_lanIsLoading = true;
        iconTimer->start(LOADING_TRAYICON_TIMER_MS);
    } else {
        m_lanIsLoading = false;
        if (m_wlanIsLoading == false) {
            onRefreshTrayIcon();
        }
    }
}

void MainWindow::onWlanConnectStatusToChangeTrayIcon(int state)
{
    qDebug() << "wlan state:" << state << Q_FUNC_INFO << __LINE__;
    if (state==1 || state==3){
        m_wlanIsLoading = true;
        iconTimer->start(LOADING_TRAYICON_TIMER_MS);
    } else {
        m_wlanIsLoading = false;
        if (m_lanIsLoading == false) {
            onRefreshTrayIcon();
        }
    }
}

void MainWindow::onShowByWlanPage()
{
    m_centralWidget->setCurrentIndex(WLAN_PAGE_INDEX);

    if(QApplication::activeWindow() != this) {
        this->resetWindowPosition();
        this->showNormal();
        this->raise();
        this->activateWindow();
        emit this->mainWindowVisibleChanged(true);
    }
}

void MainWindow::onConnectivityChanged(NetworkManager::Connectivity connectivity)
{
    if (!m_trayIcon) {
        return;
    }

    if (iconStatus == ACTIVATING) {
        return;
    }

    onRefreshTrayIcon();
}

/**
 * @brief MainWindow::keyPressEvent 按esc键关闭主界面
 * @param event
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        hideMainwindow();
    }
    return QWidget::keyPressEvent(event);
}

/**
 * @brief MainWindow::eventFilter 事件过滤器
 * @param watched
 * @param event
 * @return
 */
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::ActivationChange) {
        if(QApplication::activeWindow() != this) {
            hideMainwindow();
        }
    }
    return QMainWindow::eventFilter(watched,event);
}

/**
 * @brief MainWindow::getWirelessList 获取wifi列表，供dbus调用
 * @param map
 */
void MainWindow::getWirelessList(QMap<QString, QVector<QStringList> > &map)
{
    map.clear();
    if (nullptr != m_wlanWidget) {
        m_wlanWidget->getWirelessList(map);
    }
}

/**
 * @brief MainWindow::getWiredList 获取lan列表，供dbus调用
 * @param map
 */
void MainWindow::getWiredList(QMap<QString, QVector<QStringList>> &map)
{
    map.clear();
    if (nullptr != m_lanWidget) {
        m_lanWidget->getWiredList(map);
    }
}

/**
 * @brief MainWindow::activeWirelessAp 开启热点，供dbus调用
 * @param apName
 * @param apPassword
 * @param apDevice
 */
void MainWindow::activeWirelessAp(const QString apName, const QString apPassword, const QString band, const QString apDevice)
{
    m_wlanWidget->activeWirelessAp(apName, apPassword, band, apDevice);
}

/**
 * @brief MainWindow::activeWirelessAp 断开热点，供dbus调用
 * @param apName
 */
void MainWindow::deactiveWirelessAp(const QString apName, const QString uuid)
{
    m_wlanWidget->deactiveWirelessAp(apName, uuid);
}

/**
 * @brief MainWindow::activeWirelessAp 获取热点，供dbus调用
 * @param list
 */
void MainWindow::getApInfoBySsid(QString devName, QString ssid, QStringList &list)
{
    m_wlanWidget->getApInfoBySsid(devName, ssid, list);
}

void MainWindow::getStoredApInfo(QStringList &list)
{
    m_wlanWidget->getStoredApInfo(list);
}

//无线开关
void MainWindow::setWirelessSwitchEnable(bool enable)
{
    m_wlanWidget->setWirelessSwitchEnable(enable);
}

void MainWindow::setWiredDeviceEnable(const QString& devName, bool enable)
{
    m_lanWidget->setWiredDeviceEnable(devName, enable);
}
void MainWindow::showPropertyWidget(QString devName, QString ssid)
{
    KyNetworkDeviceResourse *devResourse = new KyNetworkDeviceResourse();
    QStringList wiredDeviceList;
    wiredDeviceList.clear();
    devResourse->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, wiredDeviceList);
    if (wiredDeviceList.contains(devName)) {
      qDebug() <<   "showPropertyWidget device type wired device name " << devName << " uuid " << ssid;
      m_lanWidget->showDetailPage(devName, ssid);
      delete devResourse;
      devResourse = nullptr;
      return;
    }

    QStringList wirelessDeviceList;
    wirelessDeviceList.clear();
    devResourse->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, wirelessDeviceList);
    if (wirelessDeviceList.contains(devName)) {
      qDebug() <<   "showPropertyWidget device type wireless device name " << devName << " ssid " << ssid;
      m_wlanWidget->showDetailPage(devName, ssid);
      delete devResourse;
      devResourse = nullptr;
      return;
    }

    qWarning() <<   "showPropertyWidget no such device " << devName;
    delete devResourse;
    devResourse = nullptr;
}

void MainWindow::showCreateWiredConnectWidget(const QString devName)
{
    qDebug() << "showCreateWiredConnectWidget! devName = " << devName;
    if (m_createPagePtrMap.contains(devName)) {
        if (m_createPagePtrMap[devName] != nullptr) {
            qDebug() << "showCreateWiredConnectWidget" << devName << "already create,just raise";

            KWindowSystem::raiseWindow(m_createPagePtrMap[devName]->winId());
            return;
        }
    }
    NetDetail *netDetail = new NetDetail(devName, "", "", false, false, true, this);
    connect(netDetail, &NetDetail::createPageClose, [&](QString interfaceName){
        if (m_createPagePtrMap.contains(interfaceName)) {
            m_createPagePtrMap[interfaceName] = nullptr;
        }
    });
    m_createPagePtrMap.insert(devName, netDetail);
    netDetail->show();
}

void MainWindow::getWirelessDeviceCap(QMap<QString, int> &map)
{
    m_wlanWidget->getWirelessDeviceCap(map);
}

//有线连接断开
void MainWindow::activateWired(const QString& devName, const QString& connUuid)
{
    m_lanWidget->activateWired(devName, connUuid);
}
void MainWindow::deactivateWired(const QString& devName, const QString& connUuid)
{
    m_lanWidget->deactivateWired(devName, connUuid);
}

//无线连接断开
void MainWindow::activateWireless(const QString& devName, const QString& ssid)
{
    m_wlanWidget->activateWirelessConnection(devName, ssid);
}

void MainWindow::deactivateWireless(const QString& devName, const QString& ssid)
{
    m_wlanWidget->deactivateWirelessConnection(devName, ssid);
}

void MainWindow::rescan()
{
    m_wlanWidget->requestScan();
}

void MainWindow::keyRingInit()
{
    agent_init();
}

void MainWindow::keyRingClear()
{
    agent_clear();
}
