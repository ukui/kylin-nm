/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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
#include "mainwindow.h"
#include "customstyle.h"
#include <KWindowEffects>
#include <QApplication>
#include <QDebug>
#include <QDBusReply>
#include <QKeyEvent>
#include <QProcess>
#include <QPainterPath>
#include <QDesktopWidget>
#include <QMessageBox>

#include "common.h"
#include "ukuistylehelper/ukuistylehelper.h"
#include "windowmanager/windowmanager.h"
#include "kysdk/kysdk-system/libkysysinfo.h"
#include "../netdetails/joinhiddenwifipage.h"
#include "../../backend/dbus-interface/kylinutil.h"
#include "../../backend/dbus-interface/kylinnetworkdeviceresource.h"

#define MAINWINDOW_WIDTH 420
#define MAINWINDOW_HEIGHT 476
#define LAYOUT_MARGINS 0,0,0,0
#define LOADING_TRAYICON_TIMER_MS 60
#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"
#define PANEL_SETTINGS "org.ukui.panel.settings"
#define PANEL_SIZE_KEY "panelsize"
#define PANEL_POSITION_KEY "panelposition"
#define PANEL_TYPE_KEY "paneltype"
#define UKUI_SETTINGS_ISLAND_POSITION_KEY "settingsislandposition"
#define UKUI_TOPBAR_SIZE_KEY "topbarsize"
#define UKUI_PANEL_LENGTH_KEY "panellength"

const QString v10Sp1 = "V10SP1";
const QString intel = "V10SP1-edu";

#define LANPAGE 0
#define WLANPAGE 1
#define AUTOSELET 2

#define SINGLE_TAB_WIDTH 194
#define MARGIN 8
#define PANEL_TOP 1
#define PANEL_LEFT 2
#define PANEL_RIGHT 3
#define PANEL_BOTTOM 0

#define KEY_PRODUCT_FEATURES "PRODUCT_FEATURES"


#define EXCELLENT_SIGNAL_ICON   "network-wireless-signal-excellent-symbolic"
#define GOOD_SIGNAL_ICON        "network-wireless-signal-good-symbolic"
#define OK_SIGNAL_ICON          "network-wireless-signal-ok-symbolic"
#define LOW_SIGNAL_ICON         "network-wireless-signal-weak-symbolic"
#define NONE_SIGNAL_ICON        "network-wireless-signal-none-symbolic"

#define EXCELLENT_SIGNAL_LIMIT_ICON   "ukui-network-wireless-signal-excellent-error-symbolic"
#define GOOD_SIGNAL_LIMIT_ICON        "ukui-network-wireless-signal-good-error-symbolic"
#define OK_SIGNAL_LIMIT_ICON          "ukui-network-wireless-signal-ok-error-symbolic"
#define LOW_SIGNAL_LIMIT_ICON         "ukui-network-wireless-signal-weak-error-symbolic"
#define NONE_SIGNAL_LIMIT_ICON        "ukui-network-wireless-signal-none-error-symbolic"

#define EXCELLENT_SIGNAL_INTRANET_ICON   "ukui-network-wireless-signal-excellent-intranet-symbolic"
#define GOOD_SIGNAL_INTRANET_ICON        "ukui-network-wireless-signal-good-intranet-symbolic"
#define OK_SIGNAL_INTRANET_ICON          "ukui-network-wireless-signal-ok-intranet-symbolic"
#define LOW_SIGNAL_INTRANET_ICON         "ukui-network-wireless-signal-weak-intranet-symbolic"
#define NONE_SIGNAL_INTRANET_ICON        "ukui-network-wireless-signal-none-intranet-symbolic"


#define DBUSSERVICE_UKCC        "org.ukui.ukcc.session"
#define DBUSPATH_UKCC           "/"
#define DBUSINTERFACE_UKCC      "org.ukui.ukcc.session.interface"


#include <kwindowsystem.h>
#include <kwindowsystem_export.h>

MainWindow::MainWindow(QString display, QWidget *parent) : QMainWindow(parent), m_display(display)
{
    QDir dir(CONFIG_FILE_DIR);
    if (!dir.exists()) {
        dir.mkdir(CONFIG_FILE_DIR);
    }

    QString filename = CONFIG_FILE_PATH;
    QSettings m_pConfSetting(filename, QSettings::IniFormat);
    if (!m_pConfSetting.contains(AUTO_FIRE_WALL_PERMIITTED)) {
        m_pConfSetting.setValue(AUTO_FIRE_WALL_PERMIITTED, true);
    }

    if (!m_pConfSetting.contains(FIRE_WALL_PERMISSION_SHOW)) {
        m_pConfSetting.setValue(FIRE_WALL_PERMISSION_SHOW, true);
    }

    firstlyStart();

    //去除窗管标题栏，传入参数为QWidget*
    //此方法只需要调用一次，多次调用时，除首次调用窗口正常外，其余次数调用窗口setGeometry接口失效
    kdk::UkuiStyleHelper::self()->removeHeader(this);
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
#if 0
    QString platform = QGuiApplication::platformName();
    if(!platform.startsWith(QLatin1String("wayland"),Qt::CaseInsensitive))
    {
        const KWindowInfo info(this->winId(), NET::WMState);
        if (!info.hasState(NET::SkipTaskbar) || !info.hasState(NET::SkipPager)) {
            KWindowSystem::setState(this->winId(), NET::SkipTaskbar | NET::SkipPager);
        }
    }
#endif

    this->showByWaylandHelper();
//    this->raise();
//    this->activateWindow();
    Q_EMIT this->mainWindowVisibleChanged(true);
#ifdef WITHKYSEC
    if (!kysec_is_disabled() && kysec_get_3adm_status() && (getuid() || geteuid())){
        if (nullptr != m_wlanWidget) {
            m_wlanWidget->hideSetting();
        }
        if (nullptr != m_lanWidget) {
            m_lanWidget->hideSetting();
        }
    } else {
        if (nullptr != m_wlanWidget) {
            m_wlanWidget->showSetting();
        }
        if (nullptr != m_lanWidget) {
            m_lanWidget->showSetting();
        }
    }
#endif

}

/**
 * @brief MainWindow::hideMainwindow 隐藏主页面时要进行的操作，后续可以添加到此函数
 */
void MainWindow::hideMainwindow()
{
    this->hide();
    Q_EMIT this->mainWindowVisibleChanged(false);
}

/**
 * @brief MainWindow::setWiredDefaultDevice 设置有线设备默认网卡
 */
void MainWindow::setWiredDefaultDevice(QString deviceName)
{
    m_lanWidget->updateDefaultDevice(deviceName);
}

/**
 * @brief MainWindow::hideMainwindow 设置无线设备默认网卡
 */
void MainWindow::setWirelessDefaultDevice(QString deviceName)
{
    m_wlanWidget->updateDefaultDevice(deviceName);
}

/**
 * @brief MainWindow::firstlyStart 一级启动，执行重要且不耗时的启动操作
 */
void MainWindow::firstlyStart()
{
    this->setAttribute(Qt::WA_NativeWindow);
    initWindowProperties();
    initTransparency();
    registerTrayIcon();
    initPanelGSettings();
    initUI();
    initDbusConnnect();
    initWindowTheme();
    initTrayIcon();
//    initPlatform();
    installEventFilter(this);
    initNetCtrl();
    m_secondaryStartTimer = new QTimer(this);
    connect(m_secondaryStartTimer, &QTimer::timeout, this, [ = ]() {
        m_secondaryStartTimer->stop();
        secondaryStart();//满足条件后执行比较耗时的二级启动
    });
    m_secondaryStartTimer->start(5 * 1000);

    m_createPagePtrMap.clear();

    m_networkMode = new NetworkMode(this);
    m_networkMode->initWiredNetworkMode();
    m_networkMode->initWirelessNetworkMode();

    //加载key ring
    kylinAgentInit();
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
 * @brief MainWindow::initWindowProperties 初始化平台信息
 */
void MainWindow::initPlatform()
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
 * @brief MainWindow::initWindowProperties 初始化一些窗口属性
 */
void MainWindow::initWindowProperties()
{
    this->windowHandle();
    this->setWindowTitle(tr("kylin-nm"));
    this->setWindowIcon(QIcon::fromTheme("kylin-network", QIcon(":/res/x/setup.png")));
    this->setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
//    //绘制毛玻璃特效
    this->setAttribute(Qt::WA_TranslucentBackground, true);  //透明
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

/**
 * @brief MainWindow::registerTrayIcon 注册托盘图标
 */
void MainWindow::registerTrayIcon()
{
    m_registerCount++;
    if (QSystemTrayIcon::isSystemTrayAvailable() || m_registerCount > 10) {
        m_trayIcon = new QSystemTrayIcon();
        if (nullptr == m_trayIcon) {
            qWarning()<< "分配空间trayIcon失败";
            return ;
        }
        m_trayIcon->setIcon(QIcon::fromTheme("network-wired-signal-excellent-symbolic"));
        m_trayIcon->setToolTip(QString(tr("kylin-nm")));

    } else {
        if (m_registerCount <= 10) {
            QTimer::singleShot(m_intervalTime,[this] {
                registerTrayIcon();
            });
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
//    auto rect = this->rect();
//    painter.drawRoundedRect(rect, 12, 12);      //窗口圆角
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this) {
        //失焦退出
        if (event->type() == QEvent::ActivationChange) {
            if (QApplication::activeWindow() != this) {
                hideMainwindow();
                return true;
            }
        }
    }

    return QObject::eventFilter(watched, event);
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
    QColor color = this->palette().base().color();
    color.setAlphaF(m_transparency);
    pal.setColor(QPalette::Base, color);
    m_centralWidget->setPalette(pal);

    QPalette tabPal = m_centralWidget->tabBar()->palette();
    tabPal.setColor(QPalette::Base, color);

    QColor inactiveColor = this->palette().window().color();
    inactiveColor.setAlphaF(0.86 *m_transparency);
    tabPal.setColor(QPalette::Window, inactiveColor);

    m_centralWidget->tabBar()->setPalette(tabPal);
}

/**
 * @brief MainWindow::initPanelGSettings 获取任务栏位置和大小
 */
void MainWindow::initPanelGSettings()
{
    const QByteArray id(PANEL_SETTINGS);
    if (QGSettings::isSchemaInstalled(id))
    {
        if (m_panelGSettings == nullptr)
        {
            m_panelGSettings = new QGSettings(id);
        }
        if (m_panelGSettings->keys().contains(PANEL_POSITION_KEY))
        {
            m_panelPosition = m_panelGSettings->get(PANEL_POSITION_KEY).toInt();
        }
        if (m_panelGSettings->keys().contains(PANEL_SIZE_KEY))
        {
            m_panelSize = m_panelGSettings->get(PANEL_SIZE_KEY).toInt();
        }
        if (m_panelGSettings->keys().contains(PANEL_TYPE_KEY))
        {
            m_panelType  = m_panelGSettings->get(PANEL_TYPE_KEY).toInt();
        }
        else
        {
            m_panelType = 0;
        }

        if (m_panelGSettings->keys().contains(UKUI_SETTINGS_ISLAND_POSITION_KEY))
        {
            m_settingsIslandPosition = m_panelGSettings->get(UKUI_SETTINGS_ISLAND_POSITION_KEY).toInt();
        }

        if (m_panelGSettings->keys().contains(UKUI_TOPBAR_SIZE_KEY))
        {
            m_topbarSize = m_panelGSettings->get(UKUI_TOPBAR_SIZE_KEY).toInt();
        }
        connect(m_panelGSettings, &QGSettings::changed, this, [&] (const QString &key)
        {
            if (key == PANEL_SIZE_KEY)
            {
                m_panelSize = m_panelGSettings->get(PANEL_SIZE_KEY).toInt();
            }
            else if(key == PANEL_POSITION_KEY)
            {
                m_panelPosition = m_panelGSettings->get(PANEL_SIZE_KEY).toInt();
            }
            else if (key == PANEL_TYPE_KEY)
            {
                m_panelType = m_panelGSettings->get(PANEL_TYPE_KEY).toInt();
                qDebug() << "切换任务栏类型";
                //                updateGeometry();
            }
            else if (key == UKUI_SETTINGS_ISLAND_POSITION_KEY)
            {
                m_settingsIslandPosition = m_panelGSettings->get(UKUI_SETTINGS_ISLAND_POSITION_KEY).toInt();
                qDebug() << "任务栏宽度切换" << m_settingsIslandPosition;
                //                updateGeometry();
            }
            else if (key == UKUI_TOPBAR_SIZE_KEY)
            {
                m_topbarSize = m_panelGSettings->get(UKUI_TOPBAR_SIZE_KEY).toInt();
                //                updateGeometry();
            }
            resetWindowPosition();
        });
    }
}

/**
 * @brief MainWindow::initUI 初始化窗口内控件
 */
void MainWindow::initUI()
{
    setThemePalette();
    m_centralWidget = new QTabWidget(this);
    this->setCentralWidget(m_centralWidget);
    m_centralWidget->tabBar()->setFixedWidth(this->width()+1);
//    m_centralWidget->tabBar()->setProperty("setRadius", 12);
//    m_centralWidget->tabBar()->setStyleSheet("QTabBar::tab{min-height:40px}");
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
    m_centralWidget->tabBar()->setProperty("useTabbarSeparateLine", false);     // 去掉中间的分割线
    m_centralWidget->setAttribute(Qt::WA_TranslucentBackground, true);          // 背景透明 解决切换黑屏问题
    connect(m_centralWidget, &QTabWidget::currentChanged, m_wlanWidget, &WlanPage::onWlanPageVisibleChanged);
    paintWithTrans();
}

/**
 * @brief MainWindow::initTrayIcon 初始化托盘图标和托盘右键菜单
 */
void MainWindow::initTrayIcon()
{
    loadIcons.append(QIcon::fromTheme("ukui-loading-0-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-1-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-2-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-3-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-4-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-5-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-6-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-7-symbolic"));
    iconTimer = new QTimer(this);
    connect(iconTimer, &QTimer::timeout, this, &MainWindow::onSetTrayIconLoading);

    m_iconTimeoutTimer = new QTimer(this);
    m_iconTimeoutTimer->setInterval(3*60000);//
    connect(m_iconTimeoutTimer, &QTimer::timeout, this, &MainWindow::onIconLoadingTimeout);

    m_trayIconMenu = new QMenu();
//    m_showMainwindowAction = new QAction(tr("Show MainWindow"),this);
    m_showSettingsAction = new QAction(tr("Settings"),this);
    m_showConnectivityPageAction = new QAction(tr("Network Connectivity Detection"), this);

    m_showSettingsAction->setIcon(QIcon::fromTheme("system-settings-symbolic", QIcon(":/res/x/setup.png")) );
    m_showConnectivityPageAction->setIcon(QIcon::fromTheme("gnome-netstatus-txrx"));
//    m_trayIconMenu->addAction(m_showMainwindowAction);
    m_trayIconMenu->addAction(m_showSettingsAction);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    iconStatus = IconActiveType::LAN_CONNECTED;
    onRefreshTrayIcon();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
//    connect(m_showMainwindowAction, &QAction::triggered, this, &MainWindow::onShowMainwindowActionTriggled);
    connect(m_showSettingsAction, &QAction::triggered, this, &MainWindow::onShowSettingsActionTriggled);
    connect(m_showConnectivityPageAction, &QAction::triggered, [=]() {
        if (m_connectivityPage != nullptr) {
            KWindowSystem::forceActiveWindow(m_connectivityPage->winId());
            KWindowSystem::raiseWindow(m_connectivityPage->winId());
            m_connectivityPage->activateWindow();/* 激活窗口；hotfix bug#433661 网络连通性检测界面最小化之后再也无法打开 */
            return;
        }
        QString uri = getConnectivityCheckSpareUriByGDbus();
        m_connectivityPage = new ConnectivityPage(uri, this);
        connect(m_connectivityPage, &ConnectivityPage::pageClose, [&](){
            m_connectivityPage = nullptr;
        });
        m_connectivityPage->show();
    });

    m_trayIcon->show();
}

void MainWindow::initDbusConnnect()
{
    connect(m_lanWidget, &LanPage::deviceStatusChanged, this, &MainWindow::deviceStatusChanged);
    connect(m_lanWidget, &LanPage::deviceNameChanged, this, &MainWindow::deviceNameChanged);

    connect(m_wlanWidget, &WlanPage::wirelessDeviceStatusChanged, this, &MainWindow::wirelessDeviceStatusChanged);
    connect(m_wlanWidget, &WlanPage::deviceNameChanged, this, &MainWindow::deviceNameChanged);
    connect(m_wlanWidget, &WlanPage::wirelessSwitchBtnChanged, this, &MainWindow::wirelessSwitchBtnChanged);

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

    connect(m_wlanWidget, &WlanPage::timeToUpdate , this, &MainWindow::onTimeUpdateTrayIcon);
    connect(m_wlanWidget, &WlanPage::showMainWindow, this, &MainWindow::onShowMainWindow);
    connect(m_wlanWidget, &WlanPage::connectivityChanged, this, &MainWindow::onConnectivityChanged);
    connect(m_wlanWidget, &WlanPage::connectivityCheckSpareUriChanged, this, &MainWindow::onConnectivityCheckSpareUriChanged);
    connect(m_wlanWidget, &WlanPage::sigNetworkPropChanged, this, &MainWindow::sigNetworkPropChanged);


    connect(m_lanWidget, &LanPage::lanConnectChanged, this, &MainWindow::onRefreshTrayIconTooltip);
    connect(m_lanWidget, &LanPage::deviceStatusChanged, this, &MainWindow::onRefreshTrayIconTooltip);
    connect(m_wlanWidget, &WlanPage::wlanConnectChanged, this, &MainWindow::onRefreshTrayIconTooltip);
    connect(m_wlanWidget, &WlanPage::wirelessDeviceStatusChanged, this, &MainWindow::onRefreshTrayIconTooltip);
    connect(m_lanWidget, &LanPage::wiredMainSwitchBtnChanged, this, &MainWindow::wiredMainSwitchBtnChanged);

    //模式切换
    QDBusConnection::sessionBus().connect(QString("com.kylin.statusmanager.interfacer"),
                                         QString("/"),
                                         QString("com.kylin.statusmanager.interface"),
                                         QString("mode_change_signal"), this, SLOT(onTabletModeChanged(bool)));

    connect(KWindowSystem::self(), &KWindowSystem::activeWindowChanged, this,[&](WId activeWindowId){
        if (activeWindowId != this->winId() && activeWindowId != 0) {
            qDebug() << "tray recieve KWindowSystem activeWindowChanged" << activeWindowId << this->winId();
            hideMainwindow();
        }
    });

    QDBusConnection::sessionBus().connect(QString("com.kylin.network"),
                                         QString("/com/kylin/network"),
                                         QString("com.kylin.network"),
                                          QString("showKylinNMSignal"), this, SLOT(onShowKylinNMSlot(QString,int)));

    QDBusConnection::sessionBus().connect(QString("com.kylin.network"),
                                         QString("/com/kylin/network"),
                                         QString("com.kylin.network"),
                                          QString("showPropertyWidgetSignal"), this, SLOT(onShowPropertyWidgetSlot(QString,QString,QString)));

    QDBusConnection::sessionBus().connect(QString("com.kylin.network"),
                                         QString("/com/kylin/network"),
                                         QString("com.kylin.network"),
                                          QString("showCreateWiredConnectWidgetSignal"), this, SLOT(onShowCreateWiredConnectWidgetSlot(QString,QString)));

    QDBusConnection::sessionBus().connect(QString("com.kylin.network"),
                                         QString("/com/kylin/network"),
                                         QString("com.kylin.network"),
                                          QString("showAddOtherWlanWidgetSignal"), this, SLOT(onShowAddOtherWlanWidgetSlot(QString,QString)));

}

/**
 * @brief MainWindow::resetWindowPosition 重新计算窗口位置
 */
void MainWindow::resetWindowPosition()
{

    if (m_isShowInCenter) {
        QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
        QRect rect((availableGeometry.width() - this->width())/2, (availableGeometry.height() - this->height())/2,
                   this->width(), this->height());
        kdk::WindowManager::setGeometry(this->windowHandle(), rect);

        return;
    }

#define MARGIN 8
#define PANEL_TOP 1
#define PANEL_LEFT 2
#define PANEL_RIGHT 3
//#define PANEL_BOTTOM 4

#if 0  //弃用接口 GetPrimaryScreenGeometry
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
#endif
    QRect availableGeo = QGuiApplication::screenAt(QCursor::pos())->geometry();
    int x, y;
    switch(m_panelPosition){
    case PANEL_TOP:
        x = availableGeo.x() + availableGeo.width() - this->width() - MARGIN;
        y = availableGeo.y() + m_panelSize + MARGIN;
        break;
    case PANEL_LEFT:
        x = availableGeo.x() + m_panelSize + MARGIN;
        y = availableGeo.y() + availableGeo.height() - this->height() - MARGIN;
        break;
    case PANEL_RIGHT:
        x = availableGeo.x() + availableGeo.width() - m_panelSize - this->width() - MARGIN;
        y = availableGeo.y() + availableGeo.height() - this->height() - MARGIN;
        break;
    default:
        x = availableGeo.x() + availableGeo.width() - this->width() - MARGIN;
        y = availableGeo.y() + availableGeo.height() - m_panelSize - this->height() - MARGIN;
        break;
    }
    kdk::WindowManager::setGeometry(this->windowHandle(), QRect(x, y, this->width(), this->height()));
    qDebug() << " Position of ukui-panel is " << m_panelPosition << "; Position of mainwindow is " << this->geometry() << "." << Q_FUNC_INFO << __LINE__;
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
        Q_EMIT qApp->paletteChanged(qApp->palette());
        return;
    }
    app->setStyle(new CustomStyle("ukui-light"));
    qDebug() << "Has set color theme to " << currentTheme << Q_FUNC_INFO << __LINE__;
    Q_EMIT qApp->paletteChanged(qApp->palette());
    return;
}

/**
 * @brief MainWindow::showControlCenter 打开控制面板网络界面
 */


void MainWindow::slideWindowByPanelPosition()
{
    if (m_panelType == 1) {
        if (m_settingsIslandPosition) {
            KWindowEffects::slideWindow(this->winId(), KWindowEffects::TopEdge);
        } else {
            KWindowEffects::slideWindow(this->winId(), KWindowEffects::BottomEdge);
        }
    } else {
        switch(m_panelPosition) {
        case PANEL_TOP:
            KWindowEffects::slideWindow(this->winId(), KWindowEffects::TopEdge);
            break;
        case PANEL_LEFT:
            KWindowEffects::slideWindow(this->winId(), KWindowEffects::LeftEdge);
            break;
        case PANEL_RIGHT:
            KWindowEffects::slideWindow(this->winId(), KWindowEffects::RightEdge);
            break;
        case PANEL_BOTTOM:
            KWindowEffects::slideWindow(this->winId(), KWindowEffects::BottomEdge);
            break;
        }
    }
}

void MainWindow::showByWaylandHelper()
{
    //设置窗体位置，传入参数为QWindow*，QRect


    //跳过任务栏和分页器的属性
    kdk::WindowManager::setSkipSwitcher(this->windowHandle(), true);
    kdk::WindowManager::setSkipTaskBar(this->windowHandle(), true);
    const KWindowInfo info(this->winId(), NET::WMState);
    if (!info.hasState(NET::SkipTaskbar) || !info.hasState(NET::SkipPager) || !info.hasState(NET::SkipSwitcher))
        KWindowSystem::setState(this->winId(), NET::SkipTaskbar | NET::SkipPager | NET::SkipSwitcher);

    this->show();
    this->setFocus();

    //跳过任务栏和分页器的属性
    kdk::WindowManager::setSkipSwitcher(this->windowHandle(), true);
    kdk::WindowManager::setSkipTaskBar(this->windowHandle(), true);
    if (!info.hasState(NET::SkipTaskbar) || !info.hasState(NET::SkipPager) || !info.hasState(NET::SkipSwitcher))
        KWindowSystem::setState(this->winId(), NET::SkipTaskbar | NET::SkipPager | NET::SkipSwitcher);

    //滑动弹出和窗口位置需在show函数之后调用才可正常处理窗口位置
    slideWindowByPanelPosition();
    resetWindowPosition();

}

void MainWindow::setCentralWidgetType(IconActiveType iconStatus)
{
    if (iconStatus == WLAN_CONNECTED || iconStatus == WLAN_CONNECTED_LIMITED) {
         m_centralWidget->setCurrentIndex(WLAN_PAGE_INDEX);
     } else if (iconStatus == ACTIVATING) {
         if (m_wlanWidget->checkWlanStatus(NetworkManager::ActiveConnection::State::Activating)) {
             m_centralWidget->setCurrentIndex(WLAN_PAGE_INDEX);
         } else {
             m_centralWidget->setCurrentIndex(LAN_PAGE_INDEX);
         }
     } else {
         m_centralWidget->setCurrentIndex(LAN_PAGE_INDEX);
    }
}

/**
 * @brief MainWindow::assembleTrayIconTooltip 整理托盘图标tooltip内容
 * @param map <网卡名，网络状态>
 * @param tip tooltip
 */
void MainWindow::assembleTrayIconTooltip(QMap<QString, QString> &map, QString &tip)
{
    if (map.isEmpty()) {
        tip = QString(tr("Network tool"));
        return;
    }
    QMap<QString, QString>::iterator iter = map.begin();
    if (map.size() == 1) {
        tip = map.value(iter.key());
        if (tip.indexOf(":") != -1) {
            tip = tip.mid(tip.indexOf(":") + 2); //单网卡显示时去掉“已连接: ”字样
        }
    } else if (map.size() > 1) {
        tip = "";
        int count = 0;
        while (iter != map.end()) {
            count += 1;
            tip += QString(tr("Network Card")) + QString("%1").arg(count) + "\n" + map.value(iter.key());
            ++iter;

            if (iter != map.end()) {
                tip += "\n";
            }
        }
    }
}

void MainWindow::setThemePalette()
{
    QPalette pal = qApp->palette();
    QGSettings * styleGsettings = nullptr;
    const QByteArray style_id(THEME_SCHAME);
    if (QGSettings::isSchemaInstalled(style_id)) {
       styleGsettings = new QGSettings(style_id, QByteArray(), this);
       QString currentTheme = styleGsettings->get(COLOR_THEME).toString();
       if(currentTheme == "ukui-default"){
           pal = themePalette(true, this);
       }
    }
    this->setPalette(pal);
}




/**
 * @brief MainWindow::onTrayIconActivated 点击托盘图标的槽函数
 */
void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    setCentralWidgetType(iconStatus);
    if (reason == QSystemTrayIcon::ActivationReason::Context) {
            m_trayIconMenu->popup(QCursor::pos());
    } else {
        if (this->isVisible()) {
            qDebug() << "Received signal of tray icon activated, will hide mainwindow." << Q_FUNC_INFO << __LINE__;
            hideMainwindow();
            return;
        }
        qDebug() << "Received signal of tray icon activated, will show mainwindow." << Q_FUNC_INFO << __LINE__;
//        this->showMainwindow();
        QDBusInterface iface("org.ukui.Sidebar", "/org/ukui/Sidebar", "org.ukui.Sidebar", QDBusConnection::sessionBus());
        iface.asyncCall("shortcutWidgetActive", "org.ukui.shortcut.network", false);
    }
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
        setThemePalette();
        paintWithTrans();
        Q_EMIT qApp->paletteChanged(qApp->palette());
    } else if ("themeColor" == key) {
        setThemePalette();
    } else {
        qDebug() << "Received signal of theme changed, key=" << key << " will do nothing." << Q_FUNC_INFO << __LINE__;
    }
}

void MainWindow::onRefreshTrayIcon()
{
    //更新托盘图标显示
    int signalStrength = 0;
    int currentCategory = 0;

    iconTimer->stop();
    if (m_lanWidget->lanIsConnected()) {
        m_trayIcon->setIcon(QIcon::fromTheme("network-wired-connected-symbolic"));
        iconStatus = IconActiveType::LAN_CONNECTED;
    } else {
        // wired dial-up (DSL/PPPoE) connections may not be recognized ,treat as wired connected.
        QMap<QString, QString> wiredStateMap;
        bool wiredActive = false;
        if (m_lanWidget) {
            m_lanWidget->getWiredDeviceConnectState(wiredStateMap);
            for (auto it = wiredStateMap.cbegin(); it != wiredStateMap.cend(); ++it) {
                const QString stateStr = it.value();
                // Use i18n-aware string for detecting "connected" state so translations are handled
                //英文下有线所有状态均有Connected的字段
                if (!stateStr.contains(tr("Not Connected"), Qt::CaseInsensitive) && stateStr.contains(tr("Connected"), Qt::CaseInsensitive)) {
                    wiredActive = true;
                    break;
                }
            }
        }
        if (wiredActive) {
            qDebug() << "Treating wired device as connected via fallback detection.";
            m_trayIcon->setIcon(QIcon::fromTheme("network-wired-connected-symbolic"));
            iconStatus = IconActiveType::LAN_CONNECTED;
        } else if (m_wlanWidget->checkWlanStatus(NetworkManager::ActiveConnection::State::Activated)){
    //        m_trayIcon->setIcon(QIcon::fromTheme("network-wireless-connected-symbolic"));
            signalStrength = m_wlanWidget->getActivateWifiSignal(m_wlanWidget->getCurrentDisplayDevice());
            currentCategory = m_wlanWidget->getActivateWifiCategory(m_wlanWidget->getCurrentDisplayDevice());

            qWarning() << Q_FUNC_INFO << __LINE__ << "signalStrength : " << signalStrength << " currentCategory :" << currentCategory;

            if (signalStrength == -1) {
                signalStrength = m_wlanWidget->getActivateWifiSignal();
            }
            if (currentCategory < 0)
                currentCategory = 0;

            iconStatus = IconActiveType::WLAN_CONNECTED;
        } else {
            m_trayIcon->setIcon(QIcon::fromTheme("network-wired-disconnected-symbolic"));
            iconStatus = IconActiveType::NOT_CONNECTED;
        }
    }

    NetworkManager::Connectivity connecttivity;
    m_wlanWidget->getConnectivity(connecttivity);
    if (connecttivity != NetworkManager::Connectivity::Full) {
        if (iconStatus == IconActiveType::LAN_CONNECTED) {
            //多项目提出感叹号图标为异常图标，所以任务栏图标去掉此状态显示（海关，工行，建行，中行等）
//            m_trayIcon->setIcon(QIcon::fromTheme("network-error-symbolic"));
            iconStatus = IconActiveType::LAN_CONNECTED_LIMITED;
        } else if (iconStatus == IconActiveType::WLAN_CONNECTED) {
            //todo 信号强度
//            m_trayIcon->setIcon(QIcon::fromTheme("network-wireless-signal-excellent-error-symbolic"));
            iconStatus = IconActiveType::WLAN_CONNECTED_LIMITED;
        }
    }

    qDebug() << Q_FUNC_INFO << __LINE__ << "WiFi signalStrength : " << signalStrength << "current avtive WiFi Category : " << currentCategory;

    if (iconStatus == IconActiveType::WLAN_CONNECTED
            || iconStatus == IconActiveType::WLAN_CONNECTED_LIMITED) {
        if (signalStrength > WIFI_EXCELLENT_SIGNAL){
            if (currentCategory == KyCategoryWiFi7)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi7Symbolic));
             else if (currentCategory == KyCategoryWiFi6Plus)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6PlusSymbolic));
             else if (currentCategory == KyCategoryWiFi6)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6Symbolic));
             else
                m_trayIcon->setIcon(QIcon::fromTheme(EXCELLENT_SIGNAL_ICON));

        } else if (signalStrength > WIFI_GOOD_SIGNAL) {
            if (currentCategory == KyCategoryWiFi7)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi7Good));
            else if (currentCategory == KyCategoryWiFi6Plus)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6PlusGood));
            else if (currentCategory == KyCategoryWiFi6)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6Good));
            else
                m_trayIcon->setIcon(QIcon::fromTheme(GOOD_SIGNAL_ICON));
        } else if (signalStrength > WIFI_OK_SIGNAL) {


            if (currentCategory == KyCategoryWiFi7)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi7OK));
            else if (currentCategory == KyCategoryWiFi6Plus)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6PlusOK));
            else if (currentCategory == KyCategoryWiFi6)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6OK));
            else
                m_trayIcon->setIcon(QIcon::fromTheme(OK_SIGNAL_ICON));
        } else if (signalStrength > WIFI_LOW_SIGNAL) {
            if (currentCategory == KyCategoryWiFi7)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi7Low));
            else if (currentCategory == KyCategoryWiFi6Plus)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6PlusLow));
            else if (currentCategory == KyCategoryWiFi6)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6Low));
            else
                m_trayIcon->setIcon(QIcon::fromTheme(LOW_SIGNAL_ICON));
        } else {
            if (currentCategory == KyCategoryWiFi7)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi7None));
            else if (currentCategory == KyCategoryWiFi6Plus)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6PlusNone));
            else if (currentCategory == KyCategoryWiFi6)
                m_trayIcon->setIcon(QIcon::fromTheme(KWifi6None));
            else
                m_trayIcon->setIcon(QIcon::fromTheme(NONE_SIGNAL_ICON));
        }
    }

    if(!getConnectivityCheckSpareUriByGDbus().isEmpty()) {
        if (iconStatus == IconActiveType::LAN_CONNECTED ||
            iconStatus == IconActiveType::LAN_CONNECTED_LIMITED) {
            m_trayIcon->setIcon(QIcon::fromTheme("network-intranet-symbolic"));
        } else if (iconStatus == IconActiveType::WLAN_CONNECTED ||
                   iconStatus == IconActiveType::WLAN_CONNECTED_LIMITED) {
            if (signalStrength > WIFI_EXCELLENT_SIGNAL){
                m_trayIcon->setIcon(QIcon::fromTheme(EXCELLENT_SIGNAL_INTRANET_ICON));
            } else if (signalStrength > WIFI_GOOD_SIGNAL) {
                m_trayIcon->setIcon(QIcon::fromTheme(GOOD_SIGNAL_INTRANET_ICON));
            } else if (signalStrength > WIFI_OK_SIGNAL) {
                m_trayIcon->setIcon(QIcon::fromTheme(OK_SIGNAL_INTRANET_ICON));
            } else if (signalStrength > WIFI_LOW_SIGNAL) {
                m_trayIcon->setIcon(QIcon::fromTheme(LOW_SIGNAL_INTRANET_ICON));
            } else {
                m_trayIcon->setIcon(QIcon::fromTheme(NONE_SIGNAL_INTRANET_ICON));
            }
        }
    }

    if (signalStrength == -1) {
        m_trayIcon->setIcon(QIcon::fromTheme("network-wired-disconnected-symbolic"));
    }
    onRefreshTrayIconTooltip();

    if (iconStatus > IconActiveType::NOT_CONNECTED) {
        m_trayIconMenu->addAction(m_showConnectivityPageAction);
    } else {
        m_trayIconMenu->removeAction(m_showConnectivityPageAction);
    }
}

void MainWindow::onSetTrayIconLoading()
{
    if (currentIconIndex > 7) {
        currentIconIndex = 0;
    }
    m_trayIcon->setIcon(loadIcons.at(currentIconIndex));
    //iconStatus = IconActiveType::ACTIVATING;
    currentIconIndex ++;
}

void MainWindow::onIconLoadingTimeout()
{
    if (m_iconTimeoutTimer && m_iconTimeoutTimer->isActive()) {
        m_iconTimeoutTimer->stop();
    }

    qWarning() << Q_FUNC_INFO << __LINE__ << "3min Current iconStatus: " << iconStatus ;
    if (iconStatus == IconActiveType::ACTIVATING) {
        if (m_lanIsLoading || m_wlanIsLoading) {
            //checkInternetLoading 判断了无线、有线和pppoe是否处于连接中
            if (!m_wlanWidget->checkInternetLoading()) {
                m_lanIsLoading = true;
                m_wlanIsLoading = true;
                onRefreshTrayIcon();
            }
        }
    }
}

void MainWindow::onConnectStatusToChangeTrayIcon(int state)
{
    if (state == 1 || state == 3){
        NetworkManager::Connectivity connectivity = NetworkManager::Connectivity::UnknownConnectivity;
        if (m_wlanWidget) {
            m_wlanWidget->getConnectivity(connectivity);
        }
        if (connectivity == NetworkManager::Connectivity::Full) {
            qDebug() << "Network already has full connectivity, skip loading icon";
            return;
        }

        iconStatus = IconActiveType::ACTIVATING;
        iconTimer->start(LOADING_TRAYICON_TIMER_MS);
    } else {
        if (!m_wlanWidget->checkInternetLoading()) {
            onRefreshTrayIcon();
        }
    }
}

void MainWindow::onLanConnectStatusToChangeTrayIcon(int state)
{
    qDebug() << "lan state:" << state << Q_FUNC_INFO << __LINE__;
    if (state==1 || state==3) {
        NetworkManager::Connectivity connectivity = NetworkManager::Connectivity::UnknownConnectivity;
        if (m_wlanWidget) {
            m_wlanWidget->getConnectivity(connectivity);
        }
        if (connectivity == NetworkManager::Connectivity::Full) {
            qDebug() << "LAN event but network already full, skip loading icon";
            m_lanIsLoading = false;
            return;
        }

        m_lanIsLoading = true;
        iconStatus = IconActiveType::ACTIVATING;
        currentIconIndex = 0;
        iconTimer->start(LOADING_TRAYICON_TIMER_MS);
        if (m_iconTimeoutTimer && !m_iconTimeoutTimer->isActive()) {
            m_iconTimeoutTimer->start();
        }
    } else {
        m_lanIsLoading = false;
        if (m_wlanIsLoading == false) {
            if (m_iconTimeoutTimer && m_iconTimeoutTimer->isActive())
                m_iconTimeoutTimer->stop();

            onRefreshTrayIcon();
        }
    }
}

void MainWindow::onWlanConnectStatusToChangeTrayIcon(int state)
{
    qDebug() << "wlan state:" << state << Q_FUNC_INFO << __LINE__;
    if (state==1 || state==3){
        // If network already connected, skip spinner
        NetworkManager::Connectivity connectivity = NetworkManager::Connectivity::UnknownConnectivity;
        if (m_wlanWidget) {
            m_wlanWidget->getConnectivity(connectivity);
        }
        if (connectivity == NetworkManager::Connectivity::Full) {
            qDebug() << "WLAN event but network already full, skip loading icon";
            m_wlanIsLoading = false;
            return;
        }

        m_wlanIsLoading = true;
        iconStatus = IconActiveType::ACTIVATING;
        currentIconIndex = 0;
        iconTimer->start(LOADING_TRAYICON_TIMER_MS);
        if (m_iconTimeoutTimer && !m_iconTimeoutTimer->isActive()) {
            m_iconTimeoutTimer->start();
        }
    } else {
        if (m_wlanWidget->checkWlanStatus(NetworkManager::ActiveConnection::State::Activating)) {
            //该返回逻辑有些奇怪，不知道是为了规避什么问题
            qWarning() << Q_FUNC_INFO << __LINE__ << "Current wlan network is activating!";
            return;
        }
        m_wlanIsLoading = false;
        if (m_lanIsLoading == false) {
            if (m_iconTimeoutTimer && m_iconTimeoutTimer->isActive())
                m_iconTimeoutTimer->stop();
            onRefreshTrayIcon();
        }
    }
}

void MainWindow::onTabletModeChanged(bool mode)
{
    qDebug() << "TabletMode change" << mode;
    Q_UNUSED(mode)
    //模式切换时，隐藏主界面
    hideMainwindow();
}

/**
 * @brief MainWindow::onRefreshTrayIconTooltip 根据托盘图标调整其tooltip
 */
void MainWindow::onRefreshTrayIconTooltip()
{
    if (!m_trayIcon) {
        return;
    }

    QString trayIconToolTip = "";
    QMap<QString, QString> lanMap;
    QMap<QString, QString> wlanMap;
    switch(iconStatus) {
    case IconActiveType::NOT_CONNECTED:
        trayIconToolTip = QString(tr("Not connected to the network"));
        break;

    case LAN_CONNECTED:
    case IconActiveType::LAN_CONNECTED_LIMITED:
        m_lanWidget->getWiredDeviceConnectState(lanMap);
        assembleTrayIconTooltip(lanMap, trayIconToolTip);
        break;

    case IconActiveType::WLAN_CONNECTED:
    case IconActiveType::WLAN_CONNECTED_LIMITED:
        m_wlanWidget->getWirelssDeviceConnectState(wlanMap);
        assembleTrayIconTooltip(wlanMap, trayIconToolTip);
        break;

    case IconActiveType::ACTIVATING:
    default:
        trayIconToolTip = QString(tr("Network tool"));
        break;
    }

    m_trayIcon->setToolTip(trayIconToolTip);
}

void MainWindow::onPrimaryScreenChanged()
{
    QTimer::singleShot(500, this, [=](){
        if (this->isVisible()) {
            resetWindowPosition();
        }
    });
}

void MainWindow::onShowMainWindow(int type)
{
    if (type == LANPAGE || type == WLANPAGE) {
        m_centralWidget->setCurrentIndex(type);

        if(QApplication::activeWindow() != this) {
            this->showMainwindow();
        }
    } else if (type == AUTOSELET) {
        onTrayIconActivated(QSystemTrayIcon::ActivationReason::Trigger);
    } else {
        qWarning() << "unsupport parameter";
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

void MainWindow::onConnectivityCheckSpareUriChanged()
{
    if (!m_trayIcon) {
        return;
    }

    if (iconStatus == ACTIVATING) {
        return;
    }

    onRefreshTrayIcon();
}

void MainWindow::onTimeUpdateTrayIcon()
{
    if (!m_trayIcon) {
        return;
    }

    if (iconStatus == ACTIVATING || (iconStatus != WLAN_CONNECTED && iconStatus != WLAN_CONNECTED_LIMITED)) {
        return;
    }

    onRefreshTrayIcon();
}

void MainWindow::refreshTrayIcon()
{
    onTimeUpdateTrayIcon();
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
 * @brief MainWindow::getWirelessList 获取wifi列表，供dbus调用
 * @param devName
 * @param list
 */
void MainWindow::getWirelessList(QString devName, QList<QStringList> &list)
{
    list.clear();
    if (nullptr != m_wlanWidget) {
        m_wlanWidget->getWirelessList(devName, list);
    }
}

bool MainWindow::getWirelessSwitchBtnState()
{
    if (nullptr != m_wlanWidget) {
        return m_wlanWidget->getWirelessSwitchBtnState();
    }
}

bool MainWindow::getWiredEnabledState()
{
    if (nullptr != m_lanWidget) {
        return m_lanWidget->getWiredEnabledState();
    }
}

bool MainWindow::getCableStateByDevice(const QString &deviceName)
{
    if (nullptr != m_lanWidget) {
        return m_lanWidget->getCableStateByDevice(deviceName);
    }
}

int MainWindow::getDeviceConnectivity(const QString deviceName)
{
    if (nullptr != m_lanWidget) {
        return m_lanWidget->getDeviceConnectivity(deviceName);
    }
}

/**
 * @brief MainWindow::getWiredList 获取lan列表，供dbus调用
 * @param map
 */
void MainWindow::getWiredList(QString devName, QList<QStringList> &list)
{
    list.clear();
    if (nullptr != m_lanWidget) {
        m_lanWidget->getWiredList(devName, list);
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

void MainWindow::getApConnectionPath(QString &path, QString uuid)
{
    m_wlanWidget->getApConnectionPath(path, uuid);
}

void MainWindow::getActiveConnectionPath(QString &path, QString uuid)
{
    m_wlanWidget->getActiveConnectionPath(path, uuid);
}

//无线开关
void MainWindow::setWirelessSwitchEnable(bool enable)
{
    m_wlanWidget->setWirelessSwitchEnable(enable);
}

void MainWindow::setWiredEnableStatus(bool enable)
{
    m_lanWidget->setWiredEnabledState(enable);
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
            KWindowSystem::forceActiveWindow(m_createPagePtrMap[devName]->winId());
            KWindowSystem::raiseWindow(m_createPagePtrMap[devName]->winId());
            return;
        }
    }
    NetDetail *netDetail = new NetDetail(devName, "", "", false, false, true,0,this);
    connect(netDetail, &NetDetail::createPageClose, [&](QString interfaceName){
        if (m_createPagePtrMap.contains(interfaceName)) {
            m_createPagePtrMap[interfaceName] = nullptr;
        }
    });
    m_createPagePtrMap.insert(devName, netDetail);
    netDetail->show();
}

void MainWindow::showAddOtherWlanWidget(QString devName)
{
    qDebug() << "showAddOtherWlanWidget! devName = " << devName;
    m_wlanWidget->showAddOtherPage(devName);
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

void MainWindow::deactivateWired(const QString& devName, const QString& connUuid, bool concise)
{
    m_lanWidget->deactivateWired(devName, connUuid, concise);
}

void MainWindow::setWiredDeviceAutoconnect(const QString& devName, bool state)
{
    m_lanWidget->setWiredDeviceAutoconnect(devName, state);
}
void MainWindow::setWiredConnectAutoconnect(const QString& uuid, bool state)
{
    m_lanWidget->setWiredConnectAutoconnect(uuid, state);
}

void MainWindow::deleteWiredConnect(int type, const QString& connUuid)
{
    qDebug() << Q_FUNC_INFO << __LINE__ << type << connUuid;
    if (type == 0)
        m_lanWidget->deleteWiredConnect(connUuid);

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

void MainWindow::deleteWireleeConnect(int type, const QString& connUuid)
{
    qDebug() << Q_FUNC_INFO << __LINE__ << type << connUuid;
    if (type == 1)
        m_wlanWidget->deleteWirelessConnect(connUuid);
}

void MainWindow::setWirelessConnectAutoconnect(const QString& uuid, bool state)
{
    m_wlanWidget->setWirelessConnectAutoConnectState(uuid, state);
}

void MainWindow::rescan()
{
    m_wlanWidget->requestScan();
}

void MainWindow::passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect) {

    KyWirelessConnectOperation m_wirelessConnectOperation;
    KyWirelessConnectSetting settings;
    settings.m_connectName = ssid;
    settings.m_ssid = ssid;
    settings.isAutoConnect = autoConnect;
    settings.m_psk = psk;
    if (type.isEmpty() || type == "") {
        settings.m_type = WpaNone;
    } else if (type.contains("WPA1") || type.contains("WPA2")) {
        settings.m_type = WpaPsk;
    } else if (type.contains("WPA3")) {
        settings.m_type = SAE;
    }

    m_wirelessConnectOperation.addAndActiveWirelessConnect(devName, settings, false);
}

void MainWindow::keyRingInit()
{
    //agent_init();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"agent_init!";
    m_agentThread = new QThread();
    m_agent = new KylinAgent(this);
    m_agent->moveToThread(m_agentThread);
    m_agentThread->start();

}

void MainWindow::keyRingClear()
{
    //agent_clear();
    if(m_agent){
        m_agent->stopKylinAgent();
        delete m_agent;
        m_agent = nullptr;
    }
    if(m_agentThread){
        m_agentThread->quit();      // 请求退出事件循环
        m_agentThread->wait();
        delete m_agentThread;
        m_agentThread = nullptr;
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"agent_clear!";
}

void MainWindow::onShowKylinNMSlot(QString display, int type)
{
    if (display == m_display) {
        onShowMainWindow(type);
    }
}

//唤起属性页 根据网卡类型 参数2 为ssid/uuid
void MainWindow::onShowPropertyWidgetSlot(QString display, QString devName, QString ssid)
{
    if (display == m_display) {
        showPropertyWidget(devName, ssid);
    }
}
//唤起新建有线连接界面
void MainWindow::onShowCreateWiredConnectWidgetSlot(QString display, QString devName)
{
    if (display == m_display) {
        showCreateWiredConnectWidget(devName);
    }
}
//唤起加入其他无线网络界面
void MainWindow::onShowAddOtherWlanWidgetSlot(QString display, QString devName)
{
    if (display == m_display) {
        showAddOtherWlanWidget(devName);
    }
}

/**
 * @brief MainWindow::showControlCenter 打开控制面板网络界面
 */
void MainWindow::showControlCenter()
{
    QProcess process;
    if (!m_lanWidget->lanIsConnected() && m_wlanWidget->checkWlanStatus(NetworkManager::ActiveConnection::State::Activated)){
        process.startDetached("ukui-control-center -m wlanconnect");
    } else {
        process.startDetached("ukui-control-center -m netconnect");
    }
}

/*弹窗时机应该自洽，原方案两个包之间会通过dbus交互，登录锁屏前端容易崩，直接使用后端锁屏与否的信号来决定是否在桌面注册弹窗*/
void MainWindow::kylinAgentInit()
{
    bool lockState = false;

    QDBusInterface dbusInterface("org.ukui.ScreenSaver",
                                 "/",
                                 "org.ukui.ScreenSaver",
                                 QDBusConnection::sessionBus());
    if (!dbusInterface.isValid()) {
        qWarning()<<Q_FUNC_INFO<<__LINE__<<"dbusInterface error!";
        lockState=false;
    } else {
        dbusInterface.setTimeout(2000);
        QDBusMessage result = dbusInterface.call("GetLockState");
        if(result.type() == QDBusMessage::ErrorMessage) {
            qWarning() << "[mainwindow]GetLockState error:" << result.errorMessage();
            lockState=false;
        } else {
            lockState = result.arguments().at( 0 ).toBool();
        }
    }

    if(lockState) {
        keyRingClear();
    } else {
        keyRingInit();
    }

    QDBusConnection::sessionBus().connect("org.ukui.ScreenSaver",
                                          "/",
                                          "org.ukui.ScreenSaver",
                                          "lock",
                                          this,
                                          SLOT(keyRingClear()));

    QDBusConnection::sessionBus().connect("org.ukui.ScreenSaver",
                                          "/",
                                          "org.ukui.ScreenSaver",
                                          "unlock",
                                          this,
                                          SLOT(keyRingInit()));
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"kylin agent init success!"<<lockState;
}


/*禁止双跨连接断开优先级排序
 * 有线>无线 后面拓展
*/
void MainWindow::netCtrlDiscon(QMap<QString, QString> lanMap,QMap<QString, QString> wlanMap)
{

    NetworkManager::Connection::Ptr connectPtr;
    int priority=0;
    int maxPriority=0;
    QString maxDevName;
    QString maxUuid;
    int  fristFlag=1,haveWireCon=0;
    /*多连接有线只保留优先级最高的一个连接*/
    if(lanMap.size()) {
        for (auto itLan=lanMap.cbegin();itLan != lanMap.cend(); ++itLan) {
            QString key = itLan.key();
            QString uuid = itLan.value();
            connectPtr =NetworkManager::findConnectionByUuid(uuid);
            if(connectPtr.isNull() ||  connectPtr->settings().isNull()) {
                continue;
            }
            priority = connectPtr->settings()->autoconnectPriority();

            /*缓存连接*/
            if(fristFlag) {
                maxDevName=key;
                maxUuid=uuid;
                maxPriority=priority;
                fristFlag=0;
                continue;
            }
            if(maxPriority<priority) {
                m_lanWidget->deactivateWired(maxDevName,maxUuid);
                maxDevName=key;
                maxUuid=uuid;
                maxPriority=priority;
            } else {
                m_lanWidget->deactivateWired(key,uuid);
            }
        }
        haveWireCon=1;//有有线连接需要全部关掉
    } else {
        haveWireCon=0;//无有线连接需要保留一个无线
    }
    maxDevName.clear();
    maxUuid.clear();
    maxPriority=0;
    fristFlag=1;

    if(wlanMap.size()) {
        for (auto itWlan = wlanMap.cbegin(); itWlan != wlanMap.cend(); ++itWlan) {
            QString key = itWlan.key();
            QString uuid = itWlan.value();
            connectPtr =NetworkManager::findConnectionByUuid(uuid);
            if(connectPtr.isNull() ||  connectPtr->settings().isNull()) {
                continue;
            }
            priority = connectPtr->settings()->autoconnectPriority();

            if(haveWireCon) {
                m_wlanWidget->deactivateWirelessConnectionWithUuid(key,uuid);
                continue;
            }

            /*缓存连接*/
            if(fristFlag) {
                maxDevName=key;
                maxUuid=uuid;
                maxPriority=priority;
                fristFlag=0;
                continue;
            }

            if(maxPriority<priority) {
                m_wlanWidget->deactivateWirelessConnectionWithUuid(maxDevName,maxUuid);
                maxDevName=key;
                maxUuid=uuid;
                maxPriority=priority;
            } else {
                m_wlanWidget->deactivateWirelessConnectionWithUuid(key,uuid);
            }
        }
    }

}


/*禁止双跨时虽然后端做了连接限制，但是开始在没打开管控规则时可能存在多个连接已经连上的情况，在打开管控规则时应该要主动断开*/
void MainWindow::updateNetCtrl(QString modName,QVariantMap value)
{
    QMap<QString, QString> lanMap;
    QMap<QString, QString> wlanMap;
    bool enable=false;

    if(modName!="Connect") return;

    qInfo()<<modName<<value;
    for (auto it = value.cbegin(); it != value.cend(); ++it) {
        QString key = it.key();
        QVariant value = it.value();
        if(key==QString("netWireWirelessSyncConnectCtrol")) {
            enable=value.toBool();
        }
    }

    if(!enable) return;

    m_lanWidget->getWiredDeviceConnect(lanMap);
    m_wlanWidget->getWirelssDeviceConnect(wlanMap);
    /*禁止双跨时主动断开*/

    netCtrlDiscon(lanMap,wlanMap);

    return;
}

void MainWindow::initNetCtrl()
{
    QVariantMap map;
    int errCode=0;
    QString netCtrlConnectName="Connect";
    QDBusInterface dbusInterface("com.kylin.networkCtrol",
                                 "/com/kylin/networkCtrol",
                                 "com.kylin.networkCtrol",
                                 QDBusConnection::systemBus());
    if (!dbusInterface.isValid()) {
        qWarning()<<Q_FUNC_INFO<<__LINE__<<"dbusInterface error!";
    } else {
        dbusInterface.setTimeout(2000);
        QDBusMessage result = dbusInterface.call("getNetContrlRule",netCtrlConnectName);
        if(result.type() == QDBusMessage::ErrorMessage) {
            qWarning() << "[mainwindow]getNetContrlRule error:" << result.errorMessage();
        } else {
            if( result.arguments().size()>=2) {
                const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
                dbusArg1st >> map;
                errCode = result.arguments().at( 1 ).toInt();
                qInfo()<<"mainwindows"<<map<<errCode;
                if(errCode==0) updateNetCtrl(netCtrlConnectName,map);
                map.clear();
            }
        }
	}

    //connect(m_interface,SIGNAL(sigNetContrlRuleChanged(QString ,QVariantMap )),this,SLOT(updateNetCtrl(QString ,QVariantMap)),Qt::QueuedConnection);//使用该接口连接不到信号
    QDBusConnection::systemBus().connect("com.kylin.networkCtrol",
                                         "/com/kylin/networkCtrol",
                                         "com.kylin.networkCtrol",
                                         "sigNetContrlRuleChanged",
                                         this,
                                         SLOT(updateNetCtrl(QString ,QVariantMap)));

    qInfo()<<"initNetCtrl success";
    return;
}

QString MainWindow::getDefaultDeviceName(int type)
{
    QString devName = QString("");

    if (0 == type)
        devName = m_lanWidget->getWiredDefaultDeviceName();
    else
        devName = m_wlanWidget->getWirelessDefaultDeviceName();
    return devName;
}

namespace {

static QString nmPeapPhase2AuthString(KyNoEapMethodAuth m)
{
    switch (m) {
    case KyAuthMethodMd5:
        return QStringLiteral("md5");
    case KyAuthMethodGtc:
        return QStringLiteral("gtc");
    case KyAuthMethodMschapv2:
    default:
        return QStringLiteral("mschapv2");
    }
}

static QString tlsPathToNmFileUrl(const QString &path)
{
    if (path.isEmpty()) {
        return QString();
    }
    if (path.startsWith(QLatin1String("file://"))) {
        return path;
    }
    return QStringLiteral("file://") + path;
}

} // namespace



void MainWindow::handleEnterpriseWifiReconnection(const QString &requestId, const QString &deviceName, const QString &ssid, const QString &password, const QVariantMap &enterpriseInfo)
{
    JoinHiddenWiFiPage *page = new JoinHiddenWiFiPage(deviceName);
    // 将企业WiFi配置信息传递给对话框
    page->setEnterpriseInfo(enterpriseInfo);
    // 连接page的信号到回传
    connect(page, &JoinHiddenWiFiPage::reConnectPeapConnect, this,
            [this, requestId](KyEapMethodPeapInfo info, KyWirelessConnectSetting connSetting) {
                QVariantMap secretMap;
                secretMap[IDENTITY_KEY] = info.userName;
                secretMap[PASSWORD_KEY] = info.userPWD;
                secretMap["phase2-auth"] = info.phase2AuthMethod;
                secretAgent()->onSubmitPassword(requestId, connSetting.m_ssid, secretMap);
    });
    connect(page, &JoinHiddenWiFiPage::reConnectTtlsConnect, this,
            [this, requestId](KyEapMethodTtlsInfo info, KyWirelessConnectSetting connSetting) {
                QVariantMap secretMap;
                secretMap[IDENTITY_KEY] = info.userName;
                secretMap[PASSWORD_KEY] = info.userPWD;
                secretAgent()->onSubmitPassword(requestId, connSetting.m_ssid, secretMap);
    });
    connect(page, &JoinHiddenWiFiPage::reConnectLeapConnect, this,
            [this, requestId](KyEapMethodLeapInfo info, KyWirelessConnectSetting connSetting){
                QVariantMap secretMap;
                secretMap[IDENTITY_KEY] = info.m_userName;
                secretMap[PASSWORD_KEY] = info.m_userPwd;
                secretAgent()->onSubmitPassword(requestId, connSetting.m_ssid, secretMap);
    });
    connect(page, &JoinHiddenWiFiPage::reConnectPwdConnect, this,
            [this, requestId](KyEapMethodPwdInfo info, KyWirelessConnectSetting connSetting){
            QVariantMap secretMap;
            secretMap[IDENTITY_KEY] = info.m_userName;
            secretMap[PASSWORD_KEY] = info.m_userPwd;
            secretAgent()->onSubmitPassword(requestId, connSetting.m_ssid, secretMap);
    });
    connect(page, &JoinHiddenWiFiPage::reConnectFastConnect, this,
            [this, requestId](KyEapMethodFastInfo info, KyWirelessConnectSetting connSetting){
        QVariantMap secretMap;
        secretMap[IDENTITY_KEY] = info.m_userName;
        secretMap[PASSWORD_KEY] = info.m_userPwd;
        secretMap[EAP_KEY] = QStringList(FAST_EAP);
        secretMap[PASSWORD_FLAG] = QVariant::fromValue(static_cast<uint>(info.m_passwdFlag));
        secretMap[QLatin1String("anonymous-identity")] = info.m_anonIdentity;
        secretMap[QLatin1String("phase2-auth")] = nmPeapPhase2AuthString(info.m_authMethod);
        if (info.m_allowAutoPacFlag) {
            secretMap[QLatin1String("phase1-fast-provisioning")] =
                QVariant::fromValue(static_cast<uint>(info.m_pacProvisioning));
        } else {
            secretMap[QLatin1String("phase1-fast-provisioning")] =
                QVariant::fromValue(static_cast<uint>(KyFastProvisioningDisabled));
        }
        if (!info.m_pacFilePath.isEmpty()) {
            const QByteArray pac(QStringLiteral("file://").toUtf8()
                                 + info.m_pacFilePath.toUtf8() + '\0');
            secretMap[QLatin1String("pac-file")] = pac;
        }

        secretAgent()->onSubmitPassword(requestId, connSetting.m_ssid, secretMap);
    });
    connect(page, &JoinHiddenWiFiPage::reConnectTlsConnect, this,
            [this, requestId](KyEapMethodTlsInfo info, KyWirelessConnectSetting connSetting) {
        QVariantMap secretMap;
        secretMap[IDENTITY_KEY] = info.identity;
        secretMap[PRIVATE_KEY] = info.clientPrivateKeyPWD;
        {
            QStringList eapList;
            eapList << QString::fromLatin1(TLS_EAP);
            secretMap[EAP_KEY] = eapList;
        }
        if (!info.domain.isEmpty()) {
            secretMap[DOMAIN_KEY] = info.domain;
        }
        secretMap[PRIVATE_KEY_FLAG] = QVariant::fromValue(static_cast<uint>(info.m_privateKeyPWDFlag));
        const QString ca = tlsPathToNmFileUrl(info.caCertPath);
        if (!ca.isEmpty()) {
            secretMap[QLatin1String("ca-cert")] = ca.toUtf8();
        }
        const QString clientCert = tlsPathToNmFileUrl(info.clientCertPath);
        if (!clientCert.isEmpty()) {
            secretMap[QLatin1String("client-cert")] = clientCert.toUtf8();
        }
        const QString pk = tlsPathToNmFileUrl(info.clientPrivateKey);
        if (!pk.isEmpty()) {
            secretMap[QLatin1String("private-key")] = pk.toUtf8();
        }
        secretAgent()->onSubmitPassword(requestId, connSetting.m_ssid, secretMap);
    });

    /* 点击页面x按钮/取消按钮时，取消 Secret 请求；成功点连接会先 emit 再 close，hiddenWiFiPageClose 仍会发但不再调 onCancelPassword */
    connect(page, &JoinHiddenWiFiPage::cancelReConnectEnterprise, this, [this, requestId, ssid, deviceName](QString devName) {
        if (deviceName ==devName && secretAgent()) {
            secretAgent()->onCancelPassword(requestId, ssid);
        }
    });

    page->exec();
}

KylinSecretAgent *MainWindow::secretAgent()
{
    if(m_agent){
        return m_agent->kylinSecretAgent();
    }
    return NULL;
}
