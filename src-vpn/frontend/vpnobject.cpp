#include "vpnobject.h"
#include <QApplication>
#include <QDebug>
#include <KWindowEffects>
#include <QDBusReply>
#include <QKeyEvent>

#include <kwindowsystem.h>
#include <kwindowsystem_export.h>
#include "windowmanager/windowmanager.h"
#include "kysdk/kysdk-system/libkysysinfo.h"
#include "ukuistylehelper/ukuistylehelper.h"

#define MAINWINDOW_WIDTH 420
#define MAINWINDOW_HEIGHT 476
const QString v10Sp1 = "V10SP1";
const QString intel = "V10SP1-edu";

VpnObject::VpnObject(QMainWindow *parent) : QMainWindow(parent)
{
    firstlyStart();
}

VpnObject::~VpnObject()
{
    if (m_vpnGsettings != nullptr) {
        delete m_vpnGsettings;
        m_vpnGsettings = nullptr;
    }
}

void VpnObject::showMainWindow()
{
    if (!m_loadFinished) {
        m_secondaryStartTimer->stop();
        secondaryStart();
    }

    resetWindowPosition();  //设置窗体位置
    setWindowProperties();
    kdk::UkuiStyleHelper::self()->removeHeader(this);  //去除窗管标题栏，传入参数为QWidget*
    this->show();
    this->raise();
    this->activateWindow();
}

void VpnObject::hideMainWindow()
{
    this->hide();
}

/**
 * @brief VpnObject::keyPressEvent  按esc键关闭主界面
 * @param event
 */
void VpnObject::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        hideMainWindow();
    }
    return QWidget::keyPressEvent(event);
}

bool VpnObject::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::ActivationChange) {
        if(QApplication::activeWindow() != this) {
            hideMainWindow();
        }
    }
    return QMainWindow::eventFilter(watched,event);
}

/**
 * @brief MainWindow::firstlyStart 一级启动，执行重要且不耗时的启动操作
 */
void VpnObject::firstlyStart()
{
    initWindowProperties();
    initUI();
    initTrayIcon();
    initDbusConnnect();
    initPlatform();
    installEventFilter(this);

    m_secondaryStartTimer = new QTimer(this);
    connect(m_secondaryStartTimer, &QTimer::timeout, this, [ = ]() {
        m_secondaryStartTimer->stop();
        secondaryStart();//满足条件后执行比较耗时的二级启动
    });
    m_secondaryStartTimer->start(5 * 1000);
}

/**
 * @brief VpnObject::secondaryStart 二级启动，可以将较耗时的初始化操作放到此处执行
 */
void VpnObject::secondaryStart()
{
    if (m_loadFinished) {
        return;
    }
    m_loadFinished = true;
}

/**
 * @brief VpnObject::initPlatform 初始化平台信息
 */
void VpnObject::initPlatform()
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

void VpnObject::initWindowProperties()
{
    this->setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
    this->setAttribute(Qt::WA_TranslucentBackground, true);  //透明
    this->setFocusPolicy(Qt::NoFocus);
    QString platform = QGuiApplication::platformName();
    if(!platform.startsWith(QLatin1String("wayland"),Qt::CaseInsensitive))
    {
        QPainterPath path;
        auto rect = this->rect();
        path.addRect(rect);
        KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));  //背景模糊
    }
}

/**
 * 设置主界面跳过任务栏和分页器的属性，隐藏再次展示有可能辉冲刷掉该属性，需要展示时重新设置
 */
void VpnObject::setWindowProperties()
{
    QString platform = QGuiApplication::platformName();
    if(!platform.startsWith(QLatin1String("wayland"),Qt::CaseInsensitive))
    {
        const KWindowInfo info(this->winId(), NET::WMState);
        if (!info.hasState(NET::SkipTaskbar) || !info.hasState(NET::SkipPager)) {
            KWindowSystem::setState(this->winId(), NET::SkipTaskbar | NET::SkipPager);
        }
    }
}

/**
 * @brief VpnObject::resetWindowPosition 重新计算窗口位置
 */
void VpnObject::resetWindowPosition()
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
    //qDebug() << " Position of ukui-panel is " << position << "; Position of mainwindow is " << this->geometry() << "." << Q_FUNC_INFO << __LINE__;
}

void VpnObject::initUI()
{
    vpnWidget = new QWidget(this);
    QPalette pal = qApp->palette();
//    pal.set
    vpnWidget->setPalette(pal);
    this->setCentralWidget(vpnWidget);
}

void VpnObject::initTrayIcon()
{
    m_vpnTrayIcon = new QSystemTrayIcon(this);
    m_vpnTrayIcon->setToolTip(QString(tr("vpn tool")));
    m_vpnTrayIcon->setIcon(QIcon::fromTheme("ukui-vpn-symbolic"));
    m_vpnTrayIcon->setVisible(true);
    initVpnIconVisible();
    connect(m_vpnTrayIcon, &QSystemTrayIcon::activated, this, &VpnObject::onTrayIconActivated);
}

void VpnObject::initVpnIconVisible()
{
    if (QGSettings::isSchemaInstalled(QByteArray(GSETTINGS_VPNICON_VISIBLE))) {
        m_vpnGsettings = new QGSettings(QByteArray(GSETTINGS_VPNICON_VISIBLE));
        if (m_vpnGsettings->keys().contains(QString(VISIBLE))) {
            m_vpnTrayIcon->setVisible(m_vpnGsettings->get(VISIBLE).toBool());
            connect(m_vpnGsettings, &QGSettings::changed, this, [=]() {
                m_vpnTrayIcon->setVisible(m_vpnGsettings->get(VISIBLE).toBool());
            });
        }
    }
}

/**
 * @brief VpnObject::onTrayIconActivated 点击托盘图标的槽函数
 */
void VpnObject::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
        case QSystemTrayIcon::Trigger:
            if(this->isActiveWindow()) {
                hideMainWindow();
            } else {
                showMainWindow();
            }
            break;

        default:
            break;
    }
}

void VpnObject::onShowMainWindow()
{
    if(QApplication::activeWindow() != this) {
        this->showMainWindow();
    }
}

void VpnObject::initDbusConnnect()
{
    //模式切换
    QDBusConnection::sessionBus().connect(QString("com.kylin.statusmanager.interface"),
                                         QString("/"),
                                         QString("com.kylin.statusmanager.interface"),
                                         QString("mode_change_signal"), this, SLOT(onTabletModeChanged(bool)));
}

void VpnObject::onTabletModeChanged(bool mode)
{
    qDebug() << "TabletMode change" << mode;
    Q_UNUSED(mode)
    //模式切换时，隐藏主界面
    hideMainWindow();
}

