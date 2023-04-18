#include "vpnobject.h"
#include <QApplication>
#include <QDebug>

#include "ukuistylehelper/ukuistylehelper.h"

#define MAINWINDOW_WIDTH 420
#define MAINWINDOW_HEIGHT 476

vpnObject::vpnObject(QMainWindow *parent) : QMainWindow(parent)
{
    initUI();
    initTrayIcon();
    initDbusConnnect();
    this->setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
}

vpnObject::~vpnObject()
{
    if (m_vpnGsettings != nullptr) {
        delete m_vpnGsettings;
        m_vpnGsettings = nullptr;
    }
}

void vpnObject::initUI()
{
    vpnWidget = new QWidget(this);
    QPalette pal = qApp->palette();
//    pal.set
    vpnWidget->setPalette(pal);
    this->setCentralWidget(vpnWidget);
}

void vpnObject::initTrayIcon()
{
    m_vpnTrayIcon = new QSystemTrayIcon(this);
    m_vpnTrayIcon->setToolTip(QString(tr("vpn tool")));
    m_vpnTrayIcon->setIcon(QIcon::fromTheme("ukui-vpn-symbolic"));
    m_vpnTrayIcon->setVisible(true);
    initVpnIconVisible();
    connect(m_vpnTrayIcon, &QSystemTrayIcon::activated, this, &vpnObject::onTrayIconActivated);
}

void vpnObject::initVpnIconVisible()
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
 * @brief vpnObject::onTrayIconActivated 点击托盘图标的槽函数
 */
void vpnObject::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
        case QSystemTrayIcon::Trigger:
            if(this->isActiveWindow()) {
                this->hide();
            } else {
                onShowMainWindow();
            }
            break;

        default:
            break;
    }
}

void vpnObject::onShowMainWindow()
{
    //去除窗管标题栏，传入参数为QWidget*
    kdk::UkuiStyleHelper::self()->removeHeader(this);
    this->show();
    this->raise();
    this->activateWindow();
}

void vpnObject::initDbusConnnect()
{
    //模式切换
    QDBusConnection::sessionBus().connect(QString("com.kylin.statusmanager.interface"),
                                         QString("/"),
                                         QString("com.kylin.statusmanager.interface"),
                                         QString("mode_change_signal"), this, SLOT(onTabletModeChanged(bool)));
}

void vpnObject::onTabletModeChanged(bool mode)
{
    qDebug() << "TabletMode change" << mode;
    Q_UNUSED(mode)
    //模式切换时，隐藏主界面
    this->hide();
}

