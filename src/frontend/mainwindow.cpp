#include "mainwindow.h"
#include "customstyle.h"
#include <KWindowEffects>
#include <QApplication>
#include <QDebug>
#include <QDBusReply>
#include <QKeyEvent>
#include <QProcess>
#include <QPainter>
#include <QPainterPath>

//#include "kylinnetworkdeviceresource.h"
//#include "../backend/dbus-interface/kylinagentinterface.h"

#include "ukuistylehelper/ukuistylehelper.h"
#include "windowmanager/windowmanager.h"
#include <kwindowsystem.h>
#include <kwindowsystem_export.h>
#include "kysdk/kysdk-system/libkysysinfo.h"

#define MAINWINDOW_WIDTH 420
#define MAINWINDOW_HEIGHT 476
#define LAYOUT_MARGINS 0,0,0,0
#define LOADING_TRAYICON_TIMER_MS 60
#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

const QString v10Sp1 = "V10SP1";
const QString intel = "V10SP1-edu";

#define KEY_PRODUCT_FEATURES "PRODUCT_FEATURES"

#define LAN_PAGE_INDEX 0
#define WLAN_PAGE_INDEX 1

#define EXCELLENT_SIGNAL 80
#define GOOD_SIGNAL 55
#define OK_SIGNAL 30
#define LOW_SIGNAL 5
#define NONE_SIGNAL 0

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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    qRegisterMetaType<KyConnectState>("KyConnectState");
    qRegisterMetaType<KyConnectStatus>("KyConnectStatus");
    qRegisterMetaType<KyConnectionType>("KyConnectionType");

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
    emit this->mainWindowVisibleChanged(true);
#ifdef WITHKYSEC
    if (!kysec_is_disabled() && kysec_get_3adm_status() && (getuid() || geteuid())){
        if (nullptr != m_wlanWidget) {
            //todo m_wlanWidget->hideSetting();
        }
        if (nullptr != m_lanWidget) {
            //todo m_lanWidget->hideSetting();
        }
    } else {
        if (nullptr != m_wlanWidget) {
            //todo m_wlanWidget->showSetting();
        }
        if (nullptr != m_lanWidget) {
            //todo m_lanWidget->showSetting();
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
    registerTrayIcon();
    initUI();
    initDbusConnnect();
    initWindowTheme();
    initTrayIcon();
    initPlatform();
    installEventFilter(this);
    m_secondaryStartTimer = new QTimer(this);
    connect(m_secondaryStartTimer, &QTimer::timeout, this, [ = ]() {
        m_secondaryStartTimer->stop();
        secondaryStart();//满足条件后执行比较耗时的二级启动
    });
    m_secondaryStartTimer->start(5 * 1000);

    m_networkMode = new NetworkModeConfig(this);
    m_networkMode->initWiredNetworkMode();
    m_networkMode->initWirelessNetworkMode();
    //加载key ring
//    agent_init();
//    system("kylin-keyring --start &");
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

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(Qt::transparent);
//    auto rect = this->rect();
//    painter.drawRoundedRect(rect, 12, 12);      //窗口圆角
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
    QColor color = qApp->palette().base().color();
    color.setAlphaF(m_transparency);
    pal.setColor(QPalette::Base, color);
    m_centralWidget->setPalette(pal);

    QPalette tabPal = m_centralWidget->tabBar()->palette();
    tabPal.setColor(QPalette::Base, color);

    QColor inactiveColor = qApp->palette().window().color();
    inactiveColor.setAlphaF(0.86 *m_transparency);
    tabPal.setColor(QPalette::Window, inactiveColor);

    m_centralWidget->tabBar()->setPalette(tabPal);
}

/**
 * @brief MainWindow::initUI 初始化窗口内控件
 */
void MainWindow::initUI()
{
    m_centralWidget = new QTabWidget(this);
    this->setCentralWidget(m_centralWidget);
    m_centralWidget->tabBar()->setFixedWidth(this->width()+1);
    m_centralWidget->tabBar()->setProperty("setRadius", 12);

    loadLanPlugin();
    loadWlanPlugin();
    m_centralWidget->addTab(m_lanWidget, tr(""));
    m_centralWidget->addTab(m_wlanWidget, tr(""));

    m_lanLabel = new QLabel(tr("LAN"), m_centralWidget);
    m_lanLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_wlanLabel = new QLabel(tr("WLAN"), m_centralWidget);
    m_wlanLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_tabBarLayout = new QHBoxLayout(this);
    m_tabBarLayout->setContentsMargins(LAYOUT_MARGINS);
    m_tabBarLayout->addWidget(m_lanLabel);
    m_tabBarLayout->addWidget(m_wlanLabel);
    m_centralWidget->tabBar()->setLayout(m_tabBarLayout);
    m_centralWidget->tabBar()->setProperty("useTabbarSeparateLine", false);     // 去掉中间的分割线
    m_centralWidget->setAttribute(Qt::WA_TranslucentBackground, true);          // 背景透明 解决切换黑屏问题
    paintWithTrans();
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

/**
 * @brief MainWindow::initTrayIcon 初始化托盘图标和托盘右键菜单
 */
void MainWindow::initTrayIcon()
{
    m_thread = new QThread;
    m_manager = new KyNetworkManager();
    m_manager->moveToThread(m_thread);
    connect(m_thread, &QThread::started, m_manager, &KyNetworkManager::kylinNetworkManagerInit);
    connect(m_thread, &QThread::finished, m_manager, &KyNetworkManager::deleteLater);
    m_thread->start();

    while (!m_manager->isInitFinished()) {
        ::usleep(1000);
    }

    m_loadIcons.append(QIcon::fromTheme("kylin-network-1"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-2"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-3"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-4"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-5"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-6"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-7"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-8"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-9"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-10"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-11"));
    m_loadIcons.append(QIcon::fromTheme("kylin-network-12"));

    m_trayIconMenu = new QMenu();
    m_showMainwindowAction = new QAction(tr("Show MainWindow"),this);
    m_showSettingsAction = new QAction(tr("Settings"),this);
    m_showSettingsAction->setIcon(QIcon::fromTheme("document-page-setup-symbolic", QIcon(":/res/x/setup.png")) );
//    m_trayIconMenu->addAction(m_showMainwindowAction);
    m_trayIconMenu->addAction(m_showSettingsAction);
    m_trayIcon->setContextMenu(m_trayIconMenu);

    m_loadingTimer = new QTimer(this);
    connect(m_loadingTimer, &QTimer::timeout, this, &MainWindow::onSetTrayIconLoading);
    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, &QTimer::timeout, this, &MainWindow::updateIcon);

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
//    connect(m_showMainwindowAction, &QAction::triggered, this, &MainWindow::onShowMainwindowActionTriggled);
    connect(m_showSettingsAction, &QAction::triggered, this, &MainWindow::onShowSettingsActionTriggled);
    connect(m_manager, &KyNetworkManager::wiredStateChange, this, &MainWindow::onWiredStateChange);
    connect(m_manager, &KyNetworkManager::wirelessStateChange, this, &MainWindow::onWirelessStateChange);
    connect(m_manager, &KyNetworkManager::connectStatusChanged , [=](KyConnectStatus status){
        m_iconStatus = status;
        updateIcon();
    });

    m_manager->getConnectStatus(m_iconStatus);
    updateIcon();
    m_refreshTimer->start(10 * 1000);
    m_trayIcon->show();
}

void MainWindow::initDbusConnnect()
{
    #if 0
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
    connect(m_wlanWidget, &WlanPage::showMainWindow, this, &MainWindow::onShowMainWindow);
    connect(m_wlanWidget, &WlanPage::connectivityChanged, this, &MainWindow::onConnectivityChanged);
#endif
    //模式切换
    QDBusConnection::sessionBus().connect(QString("com.kylin.statusmanager.interfacer"),
                                         QString("/"),
                                         QString("com.kylin.statusmanager.interface"),
                                         QString("mode_change_signal"), this, SLOT(onTabletModeChanged(bool)));
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
    if (m_iconStatus ==  WLAN_CONNECTED ||  m_iconStatus == WLAN_CONNECTED_LIMITED){
        process.startDetached("ukui-control-center -m wlanconnect");
    } else {
        process.startDetached("ukui-control-center -m netconnect");
    }
}

void MainWindow::showByWaylandHelper()
{
    //去除窗管标题栏，传入参数为QWidget*
    kdk::UkuiStyleHelper::self()->removeHeader(this);
    this->show();
    resetWindowPosition();
    //设置窗体位置，传入参数为QWindow*，QRect

}

void MainWindow::loadLanPlugin()
{
    m_lanWidget = new QWidget(this);
    QPluginLoader lanPageLoader("/home/kylin/kylin-nm-plugin/build-kylin-nm-unknown-Debug/libnetconnect.so");//有线
    if (lanPageLoader.load()) {
        QObject * plugin = lanPageLoader.instance();
        if (plugin) {
            Interface * lanPluginInstance = qobject_cast<Interface *>(plugin);
            // 插件是否启用
            if (!lanPluginInstance) {
                return;
            }
            lanPluginInstance->setPluginType(COMPLEX, true);

            m_lanWidget = lanPluginInstance->pluginUi();
            lanPluginInstance->setParentWidget(m_centralWidget);

        } else {
            qDebug() <<  "Load lanPage Failed: " << lanPageLoader.errorString() << "\n";
            return;
        }
    } else {
        qDebug() <<  "Load lanPage Failed: " << lanPageLoader.errorString() << "\n";
        return;
    }
}

void MainWindow::loadWlanPlugin()
{
    m_wlanWidget = new QWidget(this);
    QPluginLoader wlanPageLoader("/home/kylin/kylin-nm-plugin/build-kylin-nm-unknown-Debug/libwlanconnect.so");//无线
    if (wlanPageLoader.load()) {
        QObject * plugin = wlanPageLoader.instance();
        if (plugin) {
            Interface * wlanPluginInstance = qobject_cast<Interface *>(plugin);
            // 插件是否启用
            if (!wlanPluginInstance) {
                return;
            }
            wlanPluginInstance->setPluginType(COMPLEX, true);

            m_wlanWidget = wlanPluginInstance->pluginUi();
            wlanPluginInstance->setParentWidget(m_centralWidget);

        } else {
            qDebug() <<  "Load wlanPage Failed: " << wlanPageLoader.errorString() << "\n";
            return;
        }
    } else {
        qDebug() <<  "Load wlanPage Failed: " << wlanPageLoader.errorString() << "\n";
        return;
    }
}

void MainWindow::startLoading()
{
    if (!m_loadingTimer->isActive()) {
        m_loadingTimer->start(60);
    }
}

void MainWindow::stopLoading()
{
    if (m_loadingTimer->isActive()) {
        m_loadingTimer->stop();
    }
}

/**
 * @brief MainWindow::onTrayIconActivated 点击托盘图标的槽函数
 */
void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::ActivationReason::Context) {
            m_trayIconMenu->popup(QCursor::pos());
    } else {
        if (this->isVisible()) {
            qDebug() << "Received signal of tray icon activated, will hide mainwindow." << Q_FUNC_INFO << __LINE__;
            hideMainwindow();
            return;
        }
        qDebug() << "Received signal of tray icon activated, will show mainwindow." << Q_FUNC_INFO << __LINE__;
        this->showMainwindow();
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
        paintWithTrans();
        emit qApp->paletteChanged(qApp->palette());
    } else {
        qDebug() << "Received signal of theme changed, key=" << key << " will do nothing." << Q_FUNC_INFO << __LINE__;
    }
}

void MainWindow::onSetTrayIconLoading()
{
    if (m_currentIconIndex > 11) {
        m_currentIconIndex = 0;
    }
    m_trayIcon->setIcon(m_loadIcons.at(m_currentIconIndex));
    m_currentIconIndex ++;
}

//更新托盘图标显示
void MainWindow::updateIcon()
{
    if (m_loadingTimer->isActive()) {
        return;
    }
    int signalStrength = 0;
    QIcon icon;

    if (m_iconStatus == LAN_CONNECTED) {
        icon = QIcon::fromTheme("network-wired-symbolic");
        m_trayIcon->setIcon(icon);
        return;
    } else if (m_iconStatus == WLAN_CONNECTED
               || m_iconStatus == WLAN_CONNECTED_LIMITED) {
        signalStrength = m_manager->getAcivateWifiSignal();
    } else if (m_iconStatus == NOT_CONNECTED) {
        icon = QIcon::fromTheme("network-wired-disconnected-symbolic");
        m_trayIcon->setIcon(icon);
        return;
    } else if (m_iconStatus == LAN_CONNECTED_LIMITED) {
        icon = QIcon::fromTheme("network-error-symbolic");
    }

    if (m_iconStatus == WLAN_CONNECTED) {
        if (signalStrength > EXCELLENT_SIGNAL){
            icon = QIcon::fromTheme(EXCELLENT_SIGNAL_ICON);
        } else if (signalStrength > GOOD_SIGNAL) {
            icon = QIcon::fromTheme(GOOD_SIGNAL_ICON);
        } else if (signalStrength > OK_SIGNAL) {
            icon = QIcon::fromTheme(OK_SIGNAL_ICON);
        } else if (signalStrength > LOW_SIGNAL) {
            icon = QIcon::fromTheme(LOW_SIGNAL_ICON);
        } else {
            icon = QIcon::fromTheme(NONE_SIGNAL_ICON);
        }
    } else if (m_iconStatus == WLAN_CONNECTED_LIMITED) {
        if (signalStrength > EXCELLENT_SIGNAL){
            icon = QIcon::fromTheme(EXCELLENT_SIGNAL_LIMIT_ICON);
        } else if (signalStrength > GOOD_SIGNAL) {
            icon = QIcon::fromTheme(GOOD_SIGNAL_LIMIT_ICON);
        } else if (signalStrength > OK_SIGNAL) {
            icon = QIcon::fromTheme(OK_SIGNAL_LIMIT_ICON);
        } else if (signalStrength > LOW_SIGNAL) {
            icon = QIcon::fromTheme(LOW_SIGNAL_LIMIT_ICON);
        } else {
            icon = QIcon::fromTheme(NONE_SIGNAL_LIMIT_ICON);
        }
    }

    m_trayIcon->setIcon(icon);
}

void MainWindow::onWiredStateChange(QString deviceName, QString uuid, KyConnectState state)
{
    Q_UNUSED(deviceName)
    Q_UNUSED(uuid)
    if (state == CONNECT_STATE_ACTIVATING
            || state == CONNECT_STATE_DEACTIVATING) {
        startLoading();
    } else {
        stopLoading();
        updateIcon();
    }
}

void MainWindow::onWirelessStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state)
{
    Q_UNUSED(deviceName)
    Q_UNUSED(ssid)
    Q_UNUSED(uuid)
    if (state == CONNECT_STATE_ACTIVATING
            || state == CONNECT_STATE_DEACTIVATING) {
        startLoading();
    } else {
        stopLoading();
        updateIcon();
    }
}

void MainWindow::onTabletModeChanged(bool mode)
{
    qDebug() << "TabletMode change" << mode;
    Q_UNUSED(mode)
    //模式切换时，隐藏主界面
    hideMainwindow();
}

void MainWindow::onShowMainWindow(int type)
{
    m_centralWidget->setCurrentIndex(type);

    if(QApplication::activeWindow() != this) {
        this->showMainwindow();
    }
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
#if 0
/**
 * @brief MainWindow::getWirelessList 获取wifi列表，供dbus调用
 * @param map
 */
void MainWindow::getWirelessList(QMap<QString, QVector<QStringList> > &map)
{
    map.clear();
//    if (nullptr != m_wlanWidget) {
//        m_wlanWidget->getWirelessList(map);
//    }
}

/**
 * @brief MainWindow::getWiredList 获取lan列表，供dbus调用
 * @param map
 */
void MainWindow::getWiredList(QMap<QString, QVector<QStringList>> &map)
{
    map.clear();
//    if (nullptr != m_lanWidget) {
//        m_lanWidget->getWiredList(map);
//    }
}

/**
 * @brief MainWindow::activeWirelessAp 开启热点，供dbus调用
 * @param apName
 * @param apPassword
 * @param apDevice
 */
void MainWindow::activeWirelessAp(const QString apName, const QString apPassword, const QString band, const QString apDevice)
{
//    m_wlanWidget->activeWirelessAp(apName, apPassword, band, apDevice);
}

/**
 * @brief MainWindow::activeWirelessAp 断开热点，供dbus调用
 * @param apName
 */
void MainWindow::deactiveWirelessAp(const QString apName, const QString uuid)
{
//    m_wlanWidget->deactiveWirelessAp(apName, uuid);
}

/**
 * @brief MainWindow::activeWirelessAp 获取热点，供dbus调用
 * @param list
 */
void MainWindow::getApInfoBySsid(QString devName, QString ssid, QStringList &list)
{
//    m_wlanWidget->getApInfoBySsid(devName, ssid, list);
}

void MainWindow::getStoredApInfo(QStringList &list)
{
//    m_wlanWidget->getStoredApInfo(list);
}

//无线开关
void MainWindow::setWirelessSwitchEnable(bool enable)
{
//    m_wlanWidget->setWirelessSwitchEnable(enable);
}

void MainWindow::setWiredDeviceEnable(const QString& devName, bool enable)
{
//    m_lanWidget->setWiredDeviceEnable(devName, enable);
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

void MainWindow::showAddOtherWlanWidget(QString devName)
{
    qDebug() << "showAddOtherWlanWidget! devName = " << devName;
        if (m_addOtherPagePtrMap.contains(devName)) {
            if (m_addOtherPagePtrMap[devName] != nullptr) {
                qDebug() << "showAddOtherWlanWidget" << devName << "already create,just raise";

                KWindowSystem::raiseWindow(m_addOtherPagePtrMap[devName]->winId());
                return;
            }
        }
        NetDetail *netDetail = new NetDetail(devName, "", "", false, true, true, this);
        connect(netDetail, &NetDetail::createPageClose, [&](QString interfaceName){
            if (m_addOtherPagePtrMap.contains(interfaceName)) {
                m_addOtherPagePtrMap[interfaceName] = nullptr;
            }
        });
        m_addOtherPagePtrMap.insert(devName, netDetail);
        netDetail->show();

}

void MainWindow::getWirelessDeviceCap(QMap<QString, int> &map)
{
//    m_wlanWidget->getWirelessDeviceCap(map);
}

//有线连接删除
void MainWindow::deleteWired(const QString &connUuid)
{
//    m_lanWidget->deleteWired(connUuid);
}

//有线连接断开
void MainWindow::activateWired(const QString& devName, const QString& connUuid)
{
//    m_lanWidget->activateWired(devName, connUuid);
}
void MainWindow::deactivateWired(const QString& devName, const QString& connUuid)
{
//    m_lanWidget->deactivateWired(devName, connUuid);
}

//无线连接断开
void MainWindow::activateWireless(const QString& devName, const QString& ssid)
{
//    m_wlanWidget->activateWirelessConnection(devName, ssid);
}

void MainWindow::deactivateWireless(const QString& devName, const QString& ssid)
{
//    m_wlanWidget->deactivateWirelessConnection(devName, ssid);
}

void MainWindow::rescan()
{
//    m_wlanWidget->requestScan();
}

void MainWindow::keyRingInit()
{
//    agent_init();
}

void MainWindow::keyRingClear()
{
//    agent_clear();
}
#endif
