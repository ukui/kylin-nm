/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "vpnmainwindow.h"
#include "customstyle.h"
#include <KWindowEffects>
#include <QApplication>
#include <QDebug>
#include <QDBusReply>
#include <QKeyEvent>
#include <QProcess>

#include "kylinnetworkdeviceresource.h"
#include "../backend/dbus-interface/kylinagentinterface.h"

#include "ukuistylehelper/ukuistylehelper.h"
#include "windowmanager/windowmanager.h"
#include "kysdk/kysdk-system/libkysysinfo.h"

#define MAINWINDOW_WIDTH 420
#define MAINWINDOW_HEIGHT 300
#define VPN_LAYOUT_MARGINS 0,12,0,12
#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

const QString v10Sp1 = "V10SP1";
const QString intel = "V10SP1-edu";

#define KEY_PRODUCT_FEATURES "PRODUCT_FEATURES"

#include <kwindowsystem.h>
#include <kwindowsystem_export.h>

vpnMainWindow::vpnMainWindow(QWidget *parent): QMainWindow(parent)
{
    firstlyStart();
}

/**
 * @brief vpnMainWindow::showMainwindow show主窗口，同时也作为dbus接口提供给外部组件调用
 */
void vpnMainWindow::showMainwindow()
{
    if (!m_loadFinished) {
        m_secondaryStartTimer->stop();
        secondaryStart();
    }

    /**
     * 设置主界面跳过任务栏和分页器的属性，隐藏再次展示有可能辉冲刷掉该属性，需要展示时重新设置
     */
    QString platform = QGuiApplication::platformName();
    if(!platform.startsWith(QLatin1String("wayland"),Qt::CaseInsensitive))
    {
        const KWindowInfo info(this->winId(), NET::WMState);
        if (!info.hasState(NET::SkipTaskbar) || !info.hasState(NET::SkipPager)) {
            KWindowSystem::setState(this->winId(), NET::SkipTaskbar | NET::SkipPager);
        }
    }

    this->showByWaylandHelper();
    this->raise();
    this->activateWindow();
    Q_EMIT this->mainWindowVisibleChanged(true);
#ifdef WITHKYSEC
//    if (!kysec_is_disabled() && kysec_get_3adm_status() && (getuid() || geteuid())){
//        if (nullptr != m_vpnPage) {
//            m_vpnPage->hideSetting();
//        }
//    } else {
//        if (nullptr != m_vpnPage) {
//            m_vpnPage->showSetting();
//        }
//    }
#endif

}

/**
 * @brief MainWindow::hideMainwindow 隐藏主页面时要进行的操作，后续可以添加到此函数
 */
void vpnMainWindow::hideMainwindow()
{
    this->hide();
    Q_EMIT this->mainWindowVisibleChanged(false);
}

///**
// * @brief MainWindow::setWiredDefaultDevice 设置有线设备默认网卡
// */
//void MainWindow::setWiredDefaultDevice(QString deviceName)
//{
////    m_vpnPage->updateDefaultDevice(deviceName);
//}

/**
 * @brief MainWindow::firstlyStart 一级启动，执行重要且不耗时的启动操作
 */
void vpnMainWindow::firstlyStart()
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
}

/**
 * @brief vpnMainWindow::secondaryStart 二级启动，可以将较耗时的初始化操作放到此处执行
 */
void vpnMainWindow::secondaryStart()
{
    if (m_loadFinished)
        return;
    m_loadFinished = true;
}

/**
 * @brief vpnMainWindow::initPlatform 初始化平台信息
 */
void vpnMainWindow::initPlatform()
{
    char* projectName = kdk_system_get_projectName();
    QString strProjectName(projectName);
    free(projectName);
    projectName = NULL;
    if(v10Sp1.compare(strProjectName,Qt::CaseInsensitive) == 0) {
        unsigned int feature = kdk_system_get_productFeatures();
        if (feature == 3) {
            m_isShowInCenter = true;
        }
    } else if (intel.compare(strProjectName,Qt::CaseInsensitive) == 0) {
        m_isShowInCenter = true;
    }
    qDebug() << "projectName" << projectName << m_isShowInCenter;
}

/**
 * @brief vpnMainWindow::initWindowProperties 初始化一些窗口属性
 */
void vpnMainWindow::initWindowProperties()
{
    this->setWindowTitle(tr("kylin-vpn"));
//    this->setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
//    //绘制毛玻璃特效
//    this->setAttribute(Qt::WA_TranslucentBackground, true);  //透明
    this->setFocusPolicy(Qt::NoFocus);

    QString platform = QGuiApplication::platformName();
    if(!platform.startsWith(QLatin1String("wayland"),Qt::CaseInsensitive))
    {
        QPainterPath path;
        auto rect = this->rect();
        //    path.addRoundedRect(rect, 12, 12);
        path.addRect(rect);
        KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));   //背景模糊
    }
}

void vpnMainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
//    auto rect = this->rect();
//    painter.drawRoundedRect(rect, 12, 12);      //窗口圆角
}

void vpnMainWindow::initTransparency()
{
    if(QGSettings::isSchemaInstalled(TRANSPARENCY_GSETTINGS)) {
        m_transGsettings = new QGSettings(TRANSPARENCY_GSETTINGS);
        if(m_transGsettings->keys().contains(QString("transparency"))) {
            m_transparency = m_transGsettings->get("transparency").toDouble() + 0.15;
            m_transparency = (m_transparency > 1) ? 1 : m_transparency;
            connect(m_transGsettings, &QGSettings::changed, this, &vpnMainWindow::onTransChanged);
        }
    }
}

void vpnMainWindow::onTransChanged()
{
    m_transparency = m_transGsettings->get("transparency").toDouble() + 0.15;
    m_transparency = (m_transparency > 1) ? 1 : m_transparency;
    paintWithTrans();
}

void vpnMainWindow::paintWithTrans()
{
    QPalette pal = this->palette();
    QColor color = qApp->palette().base().color();
    color.setAlphaF(m_transparency);
    pal.setColor(QPalette::Window, color);
    this->setPalette(pal);
}

/**
 * @brief vpnMainWindow::initUI 初始化窗口内控件
 */
void vpnMainWindow::initUI()
{
    m_vpnWidget = new QWidget(this);
    this->setCentralWidget(m_vpnWidget);
    m_vpnLayout = new QVBoxLayout(m_vpnWidget);
    m_vpnLayout->setContentsMargins(VPN_LAYOUT_MARGINS);
    m_vpnWidget->setLayout(m_vpnLayout);
    m_vpnWidget->setAttribute(Qt::WA_TranslucentBackground, true);          // 背景透明 解决切换黑屏问题

    m_vpnPage = new VpnPage(m_vpnWidget);
    m_vpnLayout->addWidget(m_vpnPage);

    paintWithTrans();
}

/**
 * @brief vpnMainWindow::initTrayIcon 初始化托盘图标和托盘右键菜单
 */
void vpnMainWindow::initTrayIcon()
{
    m_vpnTrayIcon = new QSystemTrayIcon(this);
    m_vpnTrayIconMenu = new QMenu();
//    m_showMainwindowAction = new QAction(tr("Show MainWindow"),this);
//    m_showSettingsAction = new QAction(tr("Settings"),this);

    m_vpnTrayIcon->setToolTip(QString(tr("vpn tool")));
    m_vpnTrayIcon->setIcon(QIcon::fromTheme("ukui-vpn-symbolic"));
//    m_showSettingsAction->setIcon(QIcon::fromTheme("document-page-setup-symbolic", QIcon(":/res/x/setup.png")) );
////    m_vpnTrayIconMenu->addAction(m_showMainwindowAction);
//    m_vpnTrayIconMenu->addAction(m_showSettingsAction);
//    m_vpnTrayIcon->setContextMenu(m_vpnTrayIconMenu);
//    m_vpnTrayIcon->setIcon(QIcon::fromTheme("network-wired-signal-excellent-symbolic"));
//    m_iconStatus = IconActiveType::LAN_CONNECTED;
//    onRefreshTrayIcon();

    connect(m_vpnTrayIcon, &QSystemTrayIcon::activated, this, &vpnMainWindow::onTrayIconActivated);
////    connect(m_showMainwindowAction, &QAction::triggered, this, &MainWindow::onShowMainwindowActionTriggled);
//    connect(m_showSettingsAction, &QAction::triggered, this, &MainWindow::onShowSettingsActionTriggled);
    m_vpnTrayIcon->show();
}

void vpnMainWindow::initDbusConnnect()
{
//    connect(m_vpnPage, &LanPage::deviceStatusChanged, this, &MainWindow::deviceStatusChanged);
//    connect(m_vpnPage, &LanPage::deviceNameChanged, this, &MainWindow::deviceNameChanged);
//    connect(m_vpnPage, &LanPage::activateFailed, this, &MainWindow::activateFailed);
//    connect(m_vpnPage, &LanPage::deactivateFailed, this, &MainWindow::deactivateFailed);

    connect(m_vpnPage, &VpnPage::vpnAdd, this, &vpnMainWindow::vpnAdd);
    connect(m_vpnPage, &VpnPage::vpnRemove, this, &vpnMainWindow::vpnRemove);
    connect(m_vpnPage, &VpnPage::vpnUpdate, this, &vpnMainWindow::vpnUpdate);
    connect(m_vpnPage, &VpnPage::vpnActiveConnectionStateChanged, this, &vpnMainWindow::vpnActiveConnectionStateChanged);
//    connect(m_vpnPage, &LanPage::lanConnectChanged, this, &MainWindow::onLanConnectStatusToChangeTrayIcon);

//    //模式切换
//    QDBusConnection::sessionBus().connect(QString("com.kylin.statusmanager.interface"),
//                                         QString("/"),
//                                         QString("com.kylin.statusmanager.interface"),
//                                         QString("mode_change_signal"), this, SLOT(onTabletModeChanged(bool)));
}

/**
 * @brief vpnMainWindow::resetWindowPosition 重新计算窗口位置
 */
void vpnMainWindow::resetWindowPosition()
{

    if (m_isShowInCenter) {
        QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
        QRect rect((availableGeometry.width() - this->width())/2, (availableGeometry.height() - this->height())/2,
                   this->width(), this->height());
        kdk::WindowManager::setGeometry(this->windowHandle(), rect);

        return;
    }

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
    QRect rect;
    QDBusReply<QVariantList> reply = m_positionInterface->call("GetPrimaryScreenGeometry");
    //reply获取的参数共5个，分别是 主屏可用区域的起点x坐标，主屏可用区域的起点y坐标，主屏可用区域的宽度，主屏可用区域高度，任务栏位置
    if (!m_positionInterface->isValid() || !reply.isValid() || reply.value().size() < 5) {
        qCritical() << QDBusConnection::sessionBus().lastError().message();
        kdk::WindowManager::setGeometry(this->windowHandle(), QRect(0, 0, this->width(), this->height()));
        return;
    }
    QVariantList position_list = reply.value();
    int position = position_list.at(4).toInt();
    switch(position){
    case PANEL_TOP:
        //任务栏位于上方
        rect = QRect(position_list.at(0).toInt() + position_list.at(2).toInt() - this->width() - MARGIN,
                     position_list.at(1).toInt() + MARGIN,
                     this->width(), this->height());
        break;
        //任务栏位于左边
    case PANEL_LEFT:
        rect = QRect(position_list.at(0).toInt() + MARGIN,
                     position_list.at(1).toInt() + reply.value().at(3).toInt() - this->height() - MARGIN,
                     this->width(), this->height());
        break;
        //任务栏位于右边
    case PANEL_RIGHT:
        rect = QRect(position_list.at(0).toInt() + position_list.at(2).toInt() - this->width() - MARGIN,
                     position_list.at(1).toInt() + reply.value().at(3).toInt() - this->height() - MARGIN,
                     this->width(), this->height());
        break;
        //任务栏位于下方
    default:
        rect = QRect(position_list.at(0).toInt() + position_list.at(2).toInt() - this->width() - MARGIN,
                     position_list.at(1).toInt() + reply.value().at(3).toInt() - this->height() - MARGIN,
                     this->width(), this->height());
        break;
    }
    kdk::WindowManager::setGeometry(this->windowHandle(), rect);
    qDebug() << " Position of ukui-panel is " << position << "; Position of mainwindow is " << this->geometry() << "." << Q_FUNC_INFO << __LINE__;
}

///**
// * @brief MainWindow::resetTrayIconTool 重新获取网络连接状态并重新设置图标和tooltip
// */
//void MainWindow::resetTrayIconTool()
//{
//    //ZJP_TODO 检测当前连接的是有线还是无线，是否可用，设置图标和tooltip,图标最好提前define
////    int connectivity = objKyDBus->getNetworkConectivity();
////    qDebug() << "Value of current network Connectivity property : "<< connectivity;
////    switch (connectivity) {
////    case UnknownConnectivity:
////    case Portal:
////    case Limited:
////        setTrayIcon(iconLanOnlineNoInternet);
////        trayIcon->setToolTip(QString(tr("Network Connected But Can Not Access Internet")));
////        break;
////    case NoConnectivity:
////    case Full:
////        setTrayIcon(iconLanOnline);
////        trayIcon->setToolTip(QString(tr("kylin-nm")));
////        break;
////    }
//    qDebug() << "Has set tray icon to be XXX." << Q_FUNC_INFO << __LINE__;
//}


/**
 * @brief vpnMainWindow::initWindowTheme 初始化窗口主题并创建信号槽
 */
void vpnMainWindow::initWindowTheme()
{
    const QByteArray style_id(THEME_SCHAME);
    if (QGSettings::isSchemaInstalled(style_id)) {
        m_styleGsettings = new QGSettings(style_id);
        connect(m_styleGsettings, &QGSettings::changed, this, &vpnMainWindow::onThemeChanged);
    } else {
        qWarning() << "Gsettings interface \"org.ukui.style\" is not exist!" << Q_FUNC_INFO << __LINE__;
    }
}

///**
// * @brief MainWindow::resetWindowTheme 读取和设置窗口主题
// */
//void MainWindow::resetWindowTheme()
//{
//    if (!m_styleGsettings) { return; }
//    QString currentTheme = m_styleGsettings->get(COLOR_THEME).toString();
//    auto app = static_cast<QApplication*>(QCoreApplication::instance());
//    if(currentTheme == "ukui-dark" || currentTheme == "ukui-black"){
//        app->setStyle(new CustomStyle("ukui-dark"));
//        qDebug() << "Has set color theme to ukui-dark." << Q_FUNC_INFO << __LINE__;
//        Q_EMIT qApp->paletteChanged(qApp->palette());
//        return;
//    }
//    app->setStyle(new CustomStyle("ukui-light"));
//    qDebug() << "Has set color theme to " << currentTheme << Q_FUNC_INFO << __LINE__;
//    Q_EMIT qApp->paletteChanged(qApp->palette());
//    return;
//}

///**
// * @brief MainWindow::showControlCenter 打开控制面板网络界面
// */
//void MainWindow::showControlCenter()
//{
//    QProcess process;
//    if (!m_vpnPage->lanIsConnected() && m_wlanWidget->checkWlanStatus(NetworkManager::ActiveConnection::State::Activated)){
//        process.startDetached("ukui-control-center -m wlanconnect");
//    } else {
//        process.startDetached("ukui-control-center -m netconnect");
//    }
//}

void vpnMainWindow::showByWaylandHelper()
{
    //去除窗管标题栏，传入参数为QWidget*
    kdk::UkuiStyleHelper::self()->removeHeader(this);
    this->show();
    getTabletMode();
    resetWindowPosition();
    //设置窗体位置，传入参数为QWindow*，QRect

}

void vpnMainWindow::setCentralWidgetType(IconActiveType iconStatus)
{
    if (iconStatus == WLAN_CONNECTED || iconStatus == WLAN_CONNECTED_LIMITED) {
//         m_vpnWidget->setCurrentIndex(WLAN_PAGE_INDEX);
     } else if (iconStatus == ACTIVATING) {
//         if (m_wlanWidget->checkWlanStatus(NetworkManager::ActiveConnection::State::Activating)) {
//             m_vpnWidget->setCurrentIndex(WLAN_PAGE_INDEX);
//         } else {
//             m_vpnWidget->setCurrentIndex(LAN_PAGE_INDEX);
//         }
     } else {
//         m_vpnWidget->setCurrentIndex(LAN_PAGE_INDEX);
     }
}

void vpnMainWindow::getTabletMode()
{
    QDBusInterface interface(QString("com.kylin.statusmanager.interface"),
                             QString("/"),
                             QString("com.kylin.statusmanager.interface"),
                             QDBusConnection::sessionBus());
    if(!interface.isValid()) {
        m_isShowInCenter = true;
        return;
    }
    QDBusReply<bool> reply = interface.call("get_current_tabletmode");
    if (!reply.isValid()) {
        m_isShowInCenter = true;
        return;
    }
    m_isShowInCenter = reply.value();
}

/**
 * @brief vpnMainWindow::onTrayIconActivated 点击托盘图标的槽函数
 */
void vpnMainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    setCentralWidgetType(m_iconStatus);
    switch(reason) {
        case QSystemTrayIcon::Context:
            m_vpnTrayIconMenu->popup(QCursor::pos());
            break;
        case QSystemTrayIcon::Trigger:
            if (this->isVisible()) {
                qDebug() << "Received signal of tray icon activated, will hide mainwindow." << Q_FUNC_INFO << __LINE__;
                hideMainwindow();
                return;
            }
            qDebug() << "Received signal of tray icon activated, will show mainwindow." << Q_FUNC_INFO << __LINE__;
            this->showMainwindow();
            break;

        default:
            break;
    }
//    if (reason == QSystemTrayIcon::ActivationReason::Context) {
//            m_vpnTrayIconMenu->popup(QCursor::pos());
//    } else {
//        if (this->isVisible()) {
//            qDebug() << "Received signal of tray icon activated, will hide mainwindow." << Q_FUNC_INFO << __LINE__;
//            hideMainwindow();
//            return;
//        }
//        qDebug() << "Received signal of tray icon activated, will show mainwindow." << Q_FUNC_INFO << __LINE__;
//        this->showMainwindow();
//    }
}

//void MainWindow::onShowMainwindowActionTriggled()
//{
//    showMainwindow();
//}

//void MainWindow::onShowSettingsActionTriggled()
//{
//    showControlCenter();
//}

void vpnMainWindow::onThemeChanged(const QString &key)
{
    if (key == COLOR_THEME) {
        qDebug() << "Received signal of theme changed, will reset theme." << Q_FUNC_INFO << __LINE__;
        paintWithTrans();
        Q_EMIT qApp->paletteChanged(qApp->palette());
    } else {
        qDebug() << "Received signal of theme changed, key=" << key << " will do nothing." << Q_FUNC_INFO << __LINE__;
    }
}

void vpnMainWindow::onRefreshTrayIcon()
{
    //更新托盘图标显示
//    m_iconTimer->stop();
//    if (m_vpnPage->lanIsConnected()) {
//        m_vpnTrayIcon->setIcon(QIcon::fromTheme("network-wired-connected-symbolic"));
//        m_iconStatus = IconActiveType::LAN_CONNECTED;
//    } else {
//        m_vpnTrayIcon->setIcon(QIcon::fromTheme("network-wired-disconnected-symbolic"));
//        m_iconStatus = IconActiveType::NOT_CONNECTED;
//    }

    NetworkManager::Connectivity connecttivity;
    if (connecttivity != NetworkManager::Connectivity::Full) {
        if (m_iconStatus == IconActiveType::LAN_CONNECTED) {
            m_vpnTrayIcon->setIcon(QIcon::fromTheme("network-error-symbolic"));
            m_iconStatus = IconActiveType::LAN_CONNECTED_LIMITED;
        }
    }
}

//void vpnMainWindow::onSetTrayIconLoading()
//{
//    if (m_currentIconIndex > 11) {
//        m_currentIconIndex = 0;
//    }
//    m_vpnTrayIcon->setIcon(m_loadIcons.at(m_currentIconIndex));
//    m_iconStatus = IconActiveType::ACTIVATING;
//    m_currentIconIndex ++;
//}

//void MainWindow::onLanConnectStatusToChangeTrayIcon(int state)
//{
//    qDebug() << "lan state:" << state << Q_FUNC_INFO << __LINE__;
//    if (state==1 || state==3){
//        m_lanIsLoading = true;
//        m_iconTimer->start(LOADING_TRAYICON_TIMER_MS);
//    } else {
//        m_lanIsLoading = false;
//        if (m_wlanIsLoading == false) {
//            onRefreshTrayIcon();
//        }
//    }
//}

//void MainWindow::onTabletModeChanged(bool mode)
//{
//    qDebug() << "TabletMode change" << mode;
//    Q_UNUSED(mode)
//    //模式切换时，隐藏主界面
//    hideMainwindow();
//}

//void MainWindow::onShowMainWindow(int type)
//{
//    m_vpnWidget->setCurrentIndex(type);

//    if(QApplication::activeWindow() != this) {
//        this->showMainwindow();
//    }
//}

//void MainWindow::onConnectivityChanged(NetworkManager::Connectivity connectivity)
//{
//    if (!m_vpnTrayIcon) {
//        return;
//    }

//    if (m_iconStatus == ACTIVATING) {
//        return;
//    }

//    onRefreshTrayIcon();
//}

///**
// * @brief MainWindow::keyPressEvent 按esc键关闭主界面
// * @param event
// */
//void MainWindow::keyPressEvent(QKeyEvent *event)
//{
//    if (event->key() == Qt::Key_Escape) {
//        hideMainwindow();
//    }
//    return QWidget::keyPressEvent(event);
//}

///**
// * @brief MainWindow::eventFilter 事件过滤器
// * @param watched
// * @param event
// * @return
// */
bool vpnMainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::ActivationChange) {
        if(QApplication::activeWindow() != this) {
            hideMainwindow();
        }
    }
    return QMainWindow::eventFilter(watched,event);
}

void vpnMainWindow::getVirtualList(QMap<QString, QVector<QStringList>> &map)
{
    map.clear();
    if (nullptr != m_vpnPage) {
        m_vpnPage->getVirtualList(map);
    }
}

//void MainWindow::setWiredDeviceEnable(const QString& devName, bool enable)
//{
//    m_vpnPage->setWiredDeviceEnable(devName, enable);
//}
//void MainWindow::showPropertyWidget(QString devName, QString ssid)
//{
//    KyNetworkDeviceResourse *devResourse = new KyNetworkDeviceResourse();
//    QStringList wiredDeviceList;
//    wiredDeviceList.clear();
//    devResourse->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, wiredDeviceList);
//    if (wiredDeviceList.contains(devName)) {
//      qDebug() <<   "showPropertyWidget device type wired device name " << devName << " uuid " << ssid;
//      m_vpnPage->showDetailPage(devName, ssid);
//      delete devResourse;
//      devResourse = nullptr;
//      return;
//    }

//    QStringList wirelessDeviceList;
//    wirelessDeviceList.clear();
//    devResourse->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, wirelessDeviceList);
//    if (wirelessDeviceList.contains(devName)) {
//      qDebug() <<   "showPropertyWidget device type wireless device name " << devName << " ssid " << ssid;
//      m_wlanWidget->showDetailPage(devName, ssid);
//      delete devResourse;
//      devResourse = nullptr;
//      return;
//    }

//    qWarning() <<   "showPropertyWidget no such device " << devName;
//    delete devResourse;
//    devResourse = nullptr;
//}

//void MainWindow::showCreateWiredConnectWidget(const QString devName)
//{
//    qDebug() << "showCreateWiredConnectWidget! devName = " << devName;
//    if (m_createPagePtrMap.contains(devName)) {
//        if (m_createPagePtrMap[devName] != nullptr) {
//            qDebug() << "showCreateWiredConnectWidget" << devName << "already create,just raise";

//            KWindowSystem::raiseWindow(m_createPagePtrMap[devName]->winId());
//            return;
//        }
//    }
//    NetDetail *netDetail = new NetDetail(devName, "", "", false, false, true, this);
//    connect(netDetail, &NetDetail::createPageClose, [&](QString interfaceName){
//        if (m_createPagePtrMap.contains(interfaceName)) {
//            m_createPagePtrMap[interfaceName] = nullptr;
//        }
//    });
//    m_createPagePtrMap.insert(devName, netDetail);
//    netDetail->show();
//}

//void MainWindow::showAddOtherWlanWidget(QString devName)
//{
//    qDebug() << "showAddOtherWlanWidget! devName = " << devName;
//        if (m_addOtherPagePtrMap.contains(devName)) {
//            if (m_addOtherPagePtrMap[devName] != nullptr) {
//                qDebug() << "showAddOtherWlanWidget" << devName << "already create,just raise";

//                KWindowSystem::raiseWindow(m_addOtherPagePtrMap[devName]->winId());
//                return;
//            }
//        }

//#if 0
//        NetDetail *netDetail = new NetDetail(devName, "", "", false, true, true, this);
//        connect(netDetail, &NetDetail::createPageClose, [&](QString interfaceName){
//            if (m_addOtherPagePtrMap.contains(interfaceName)) {
//                m_addOtherPagePtrMap[interfaceName] = nullptr;
//            }
//        });
//        m_addOtherPagePtrMap.insert(devName, netDetail);
//        netDetail->show();
//#endif

//        JoinHiddenWiFiPage *hiddenWiFi =new JoinHiddenWiFiPage(devName);
//        connect(hiddenWiFi, &JoinHiddenWiFiPage::hiddenWiFiPageClose, [&](QString interfaceName){
//            if (m_addOtherPagePtrMap.contains(interfaceName)) {
//                m_addOtherPagePtrMap[interfaceName] = nullptr;
//            }
//        });
//        m_addOtherPagePtrMap.insert(devName, hiddenWiFi);
//        connect(hiddenWiFi, &JoinHiddenWiFiPage::showWlanList, this, &MainWindow::onShowMainWindow);
//        hiddenWiFi->show();
//}

//有线连接断开
void vpnMainWindow::activateVpn(const QString& connUuid)
{
    m_vpnPage->activateVpn(connUuid);
}
void vpnMainWindow::deactivateVpn(const QString& connUuid)
{
    m_vpnPage->deactivateVpn(connUuid);
}

