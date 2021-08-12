#include "new-mainwindow.h"
#include "customstyle.h"
#include "lanpage.h"
#include "wlanpage.h"
#include <KWindowEffects>
#include <QApplication>
#include <QDebug>
#include <QDBusReply>
#include <QKeyEvent>
#include <QProcess>

#define MAINWINDOW_WIDTH 420
#define MAINWINDOW_HEIGHT 456
#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    firstlyStart();
}

/**
 * @brief MainWindow::showMainwindow show主窗口，同时也作为dbus接口提供给外部组件调用
 */
void MainWindow::showMainwindow()
{
    if (!m_load_finished) {
        m_secondary_start_timer->stop();
        secondaryStart();
    }
    this->resetWindowPosition();
    this->showNormal();
    this->raise();
    this->activateWindow();
}

/**
 * @brief MainWindow::hideMainwindow 隐藏主页面时要进行的操作，后续可以添加到此函数
 */
void MainWindow::hideMainwindow()
{
    this->hide();
}

/**
 * @brief MainWindow::firstlyStart 一级启动，执行重要且不耗时的启动操作
 */
void MainWindow::firstlyStart()
{
    initWindowProperties();
    initUI();
    initWindowTheme();
    initTrayIcon();
    installEventFilter(this);
    m_secondary_start_timer = new QTimer(this);
    connect(m_secondary_start_timer, &QTimer::timeout, this, [ = ]() {
        m_secondary_start_timer->stop();
        secondaryStart();//满足条件后执行比较耗时的二级启动
    });
    m_secondary_start_timer->start(5 * 1000);
}

/**
 * @brief MainWindow::secondaryStart 二级启动，可以将较耗时的初始化操作放到此处执行
 */
void MainWindow::secondaryStart()
{
    if (m_load_finished)
        return;
    m_load_finished = true;
}

/**
 * @brief MainWindow::initWindowProperties 初始化一些窗口属性
 */
void MainWindow::initWindowProperties()
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowTitle(tr("kylin-nm"));
    this->setWindowIcon(QIcon::fromTheme("kylin-network", QIcon(":/res/x/setup.png")));
    //绘制毛玻璃特效
    this->setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
    QPainterPath path;
    auto rect = this->rect();
    path.addRoundedRect(rect, 6, 6);
    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
}

/**
 * @brief MainWindow::initUI 初始化窗口内控件
 */
void MainWindow::initUI()
{
    m_central_widget = new QTabWidget(this);
    this->setCentralWidget(m_central_widget);
    m_central_widget->tabBar()->setFixedWidth(this->width());
    LanPage * m_lanWidget = new LanPage(m_central_widget);
    WlanPage * m_wlanWidget = new WlanPage(m_central_widget);
    m_central_widget->addTab(m_lanWidget, QIcon::fromTheme("network-wired-connected-symbolic", QIcon::fromTheme("network-wired-symbolic", QIcon(":/res/l/network-online.svg"))), tr("LAN"));
    m_central_widget->addTab(m_wlanWidget, QIcon::fromTheme("network-wireless-signal-excellent-symbolic", QIcon(":/res/x/wifi-list-bg.svg")), tr("WLAN"));
}

/**
 * @brief MainWindow::initTrayIcon 初始化托盘图标和托盘右键菜单
 */
void MainWindow::initTrayIcon()
{
    m_tray_icon = new QSystemTrayIcon();
    m_tray_icon_menu = new QMenu();
    m_show_mainwindow_action = new QAction(tr("Show MainWindow"),this);
    m_show_settings_action = new QAction(tr("Settings"),this);

    m_tray_icon->setToolTip(QString(tr("kylin-nm")));
    m_show_settings_action->setIcon(QIcon::fromTheme("document-page-setup-symbolic", QIcon(":/res/x/setup.png")) );
    m_tray_icon_menu->addAction(m_show_mainwindow_action);
    m_tray_icon_menu->addAction(m_show_settings_action);
    m_tray_icon->setContextMenu(m_tray_icon_menu);
    m_tray_icon->setIcon(QIcon::fromTheme("network-wireless-signal-excellent-symbolic"));

    connect(m_tray_icon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
    connect(m_show_mainwindow_action, &QAction::triggered, this, &MainWindow::onShowMainwindowActionTriggled);
    connect(m_show_settings_action, &QAction::triggered, this, &MainWindow::onShowSettingsActionTriggled);
    m_tray_icon->show();
}

/**
 * @brief MainWindow::resetWindowPosition 重新计算窗口位置
 */
void MainWindow::resetWindowPosition()
{
#define MARGIN 4
    if (!m_position_interface) {
        m_position_interface = new QDBusInterface("org.ukui.panel",
                            "/panel/position",
                            "org.ukui.panel",
                            QDBusConnection::sessionBus());
    }
    QDBusReply<QVariantList> reply = m_position_interface->call("GetPrimaryScreenGeometry");
    //reply获取的参数共5个，分别是 主屏可用区域的起点x坐标，主屏可用区域的起点y坐标，主屏可用区域的宽度，主屏可用区域高度，任务栏位置
    if (!m_position_interface->isValid() || !reply.isValid() || reply.value().size() < 5) {
        qCritical() << QDBusConnection::sessionBus().lastError().message();
        this->setGeometry(0, 0, this->width(), this->height());
        return;
    }
    QVariantList position_list = reply.value();
    int position = position_list.at(4).toInt();
    switch(position){
    case 1:
        //任务栏位于上方
        this->setGeometry(position_list.at(0).toInt() + position_list.at(2).toInt() - this->width() - MARGIN,
                          position_list.at(1).toInt() + MARGIN,
                          this->width(), this->height());
        break;
        //任务栏位于左边
    case 2:
        this->setGeometry(position_list.at(0).toInt() + MARGIN,
                          position_list.at(1).toInt() + reply.value().at(3).toInt() - this->height() - MARGIN,
                          this->width(), this->height());
        break;
        //任务栏位于右边
    case 3:
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
        m_style_gsettings = new QGSettings(style_id);
        resetWindowTheme();
        connect(m_style_gsettings, &QGSettings::changed, this, [ = ](const QString &key) {
            if (key == COLOR_THEME) {
                qDebug() << "Received signal of theme changed, will reset theme." << Q_FUNC_INFO << __LINE__;
                resetWindowTheme();
            }
        });
    } else {
        qWarning() << "Gsettings interface \"org.ukui.style\" is not exist!" << Q_FUNC_INFO << __LINE__;
    }
}

/**
 * @brief MainWindow::resetWindowTheme 读取和设置窗口主题
 */
void MainWindow::resetWindowTheme()
{
    if (!m_style_gsettings) return;
    QString currentTheme = m_style_gsettings->get(COLOR_THEME).toString();
    auto app = static_cast<QApplication*>(QCoreApplication::instance());
    if(currentTheme == "ukui-dark" || currentTheme == "ukui-black"){
        app->setStyle(new CustomStyle("ukui-dark"));
        qDebug() << "Has set color theme to ukui-dark." << Q_FUNC_INFO << __LINE__;
        return;
    }
    app->setStyle(new CustomStyle("ukui-light"));
    qDebug() << "Has set color theme to ukui-light." << Q_FUNC_INFO << __LINE__;
    return;
}

/**
 * @brief MainWindow::showControlCenter 打开控制面板网络界面
 */
void MainWindow::showControlCenter()
{
    QProcess process;
    process.startDetached("ukui-control-center --netconnect");
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
