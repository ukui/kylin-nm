#include "vpnobject.h"
#include <QApplication>

#include "ukuistylehelper/ukuistylehelper.h"

vpnObject::vpnObject(QObject *parent) : QObject(parent)
{
    initUI();
    initTrayIcon();
    initDbusConnnect();
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
    m_vpnPage = new VpnPage(nullptr);
    m_vpnPage->update();

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
            if(m_vpnPage->isActiveWindow()) {
                m_vpnPage->hide();
            } else {
                onShowMainWindow();
            }
            break;

        default:
            break;
    }
}

void vpnObject::getVirtualList(QVector<QStringList> &vector)
{
    vector.clear();
    if (nullptr != m_vpnPage) {
        m_vpnPage->getVirtualList(vector);
    }
}

//Vpn连接删除
void vpnObject::deleteVpn(const QString &connUuid)
{
    m_vpnPage->deleteVpn(connUuid);
}

//Vpn连接断开
void vpnObject::activateVpn(const QString& connUuid)
{
    m_vpnPage->activateVpn(connUuid);
}
void vpnObject::deactivateVpn(const QString& connUuid)
{
    m_vpnPage->deactivateVpn(connUuid);
}

void vpnObject::onShowMainWindow()
{
    kdk::UkuiStyleHelper::self()->removeHeader(m_vpnPage);
    m_vpnPage->showUI();;
}

void vpnObject::initDbusConnnect()
{
    connect(m_vpnPage, &VpnPage::activateFailed, this, &vpnObject::activateFailed);
    connect(m_vpnPage, &VpnPage::deactivateFailed, this, &vpnObject::deactivateFailed);

    connect(m_vpnPage, &VpnPage::vpnAdd, this, &vpnObject::vpnAdd);
    connect(m_vpnPage, &VpnPage::vpnRemove, this, &vpnObject::vpnRemove);
    connect(m_vpnPage, &VpnPage::vpnUpdate, this, &vpnObject::vpnUpdate);
    connect(m_vpnPage, &VpnPage::vpnActiveConnectionStateChanged, this, &vpnObject::vpnActiveConnectionStateChanged);
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
    m_vpnPage->hide();
}
