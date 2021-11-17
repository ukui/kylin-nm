#include "lanlistitem.h"
#include "backend/dbus-interface/kylinconnectitem.h"

#include <QDebug>

#define LOG_FLAG "[LanListItem]"

LanListItem::LanListItem(const KyConnectItem *lanConnectItem,
                         const QString &deviceName, QWidget *parent):ListItem(parent)
{
    m_connectOperation = new KyWiredConnectOperation(this);
    m_deviceResource = new KyNetworkDeviceResourse(this);

    connectItemCopy(lanConnectItem);
    m_deviceName = deviceName;

    m_nameLabel->setText(m_lanConnectItem.m_connectName);

    m_netButton->setButtonIcon(QIcon::fromTheme("network-wired-connected-symbolic"));
    if (m_lanConnectItem.m_connectState == NetworkManager::ActiveConnection::State::Activated) {
        setIcon(true);
    } else {
        setIcon(false);
    }

    m_itemFrame->installEventFilter(this);
    connect(this->m_infoButton, &InfoButton::clicked, this, &LanListItem::onInfoButtonClicked);
    connect(m_menu, &QMenu::triggered, this, &LanListItem::onMenuTriggered);
}


LanListItem::LanListItem(QWidget *parent) : ListItem(parent)
{
    m_isActive = false;
    m_netButton->setButtonIcon(QIcon::fromTheme("network-offline-symbolic"));
    m_netButton->setDefaultPixmap();
    const QString str=tr("Not connected");
    m_nameLabel->setText(str);
    this->m_infoButton->hide();
}

LanListItem::~LanListItem()
{
    qDebug()<<"[LanPage] lan list item is deleted." << m_lanConnectItem.m_connectName;
}

void LanListItem::setIcon(bool isOn)
{
    if (isOn) {
        m_netButton->setActive(true);               //设置图标显示不同颜色
    } else {
        m_netButton->setActive(false);
    }
}

void LanListItem::connectItemCopy(const KyConnectItem *lanConnectItem)
{
    if (lanConnectItem) {
        m_lanConnectItem.m_connectName = lanConnectItem->m_connectName;
        m_lanConnectItem.m_connectPath = lanConnectItem->m_connectPath;
        m_lanConnectItem.m_connectState = lanConnectItem->m_connectState;
        m_lanConnectItem.m_connectUuid = lanConnectItem->m_connectUuid;
        m_lanConnectItem.m_ifaceName = lanConnectItem->m_ifaceName;
        m_lanConnectItem.m_itemType = lanConnectItem->m_itemType;
    } else {
        qDebug() << LOG_FLAG <<"the connect item is nullptr";
        m_lanConnectItem.m_connectName = "";
        m_lanConnectItem.m_connectPath = "";
        m_lanConnectItem.m_connectState = NetworkManager::ActiveConnection::State::Unknown;
        m_lanConnectItem.m_connectUuid = "";
        m_lanConnectItem.m_ifaceName = "";
        m_lanConnectItem.m_itemType = NetworkManager::ConnectionSettings::ConnectionType::Unknown;
    }

    return;
}

void LanListItem::onNetButtonClicked()
{
    if (m_lanConnectItem.m_connectUuid.isEmpty()) {
        qDebug() << LOG_FLAG << "connect is empty, so can not connect or disconnect.";
        return;
    }

    if (Deactivated == m_lanConnectItem.m_connectState) {
        //断开的连接，点击激活连接
        if (m_deviceResource->wiredDeviceIsCarriered(m_deviceName)) {
            m_connectOperation->activateWiredConnection(m_lanConnectItem.m_connectUuid, m_deviceName);
            qDebug() << LOG_FLAG << "it will activate connection" << m_lanConnectItem.m_connectName
                     << ". it's device is" << m_deviceName;
            m_netButton->startLoading();
        } else {
            qDebug() << LOG_FLAG << m_deviceName << "is not carried, so can not activate connection";
            this->showDesktopNotify(tr("Wired Device not carried"));
        }
    } else {
        qDebug() << LOG_FLAG <<"the connection" << m_lanConnectItem.m_connectName
                 << "is not deactived, so it can not be operation.";
    }

    return;

}

void LanListItem::onRightButtonClicked()
{
    //右键点击事件
    qDebug()<< LOG_FLAG <<"onRightButtonClicked";
    if (!m_menu) {
        return;
    }

    m_menu->clear();
    if (Activated == m_lanConnectItem.m_connectState || Activating == m_lanConnectItem.m_connectState) {
        m_menu->addAction(new QAction(tr("Disconnect"), this));
    } else if (Deactivated == m_lanConnectItem.m_connectState) {
        m_menu->addAction(new QAction(tr("Connect"), this));
    } else {
        return;
    }

    m_menu->move(cursor().pos());
    m_menu->show();
    return;
}

void LanListItem::onMenuTriggered(QAction *action)
{
    if (action->text() == tr("Connect")) {
        this->onNetButtonClicked();
    } else if (action->text() == tr("Disconnect")) {
        m_connectOperation->deactivateWiredConnection(m_lanConnectItem.m_connectName, m_lanConnectItem.m_connectUuid);
        qDebug() << LOG_FLAG << "it will disconnect connection" << m_lanConnectItem.m_connectName
                 << ". it's device is" << m_deviceName;
        m_netButton->startLoading();
    }
    return;
}


void LanListItem::onInfoButtonClicked()
{
    if (m_lanConnectItem.m_connectUuid.isEmpty()) {
        qDebug() << LOG_FLAG << "connect is empty, so can not show detail info.";
        return;
    }

    if(isDetailShow){
        qDebug()<< LOG_FLAG << "the detail page has be shown , so do not show again" << Q_FUNC_INFO << __LINE__;
        return;
    }

    qDebug()<< LOG_FLAG << "the info button of lan is clicked! uuid = "
            << m_lanConnectItem.m_connectUuid << "; name = " << m_lanConnectItem.m_connectName
            << "." <<Q_FUNC_INFO << __LINE__;

    bool isActivated = false;
    if (Activated == m_lanConnectItem.m_connectState) {
        isActivated = true;
    }

    NetDetail *netDetail = new NetDetail(m_deviceName, m_lanConnectItem.m_connectName,
                                         m_lanConnectItem.m_connectUuid, isActivated,false, false, this);
    connect(netDetail, &NetDetail::detailPageClose, this, &LanListItem::onDetailShow);
    netDetail->show();
    emit this->detailShow(true);

    return;
}

void LanListItem::updateConnectionState(ConnectState state)
{
    m_lanConnectItem.m_connectState = (NetworkManager::ActiveConnection::State)state;

    if (Deactivated == state || Activated == state) {
        m_netButton->stopLoading();
        if (state == Activated) {
            setIcon(true);
        } else {
            setIcon(false);
        }
    } else {
        m_netButton->startLoading();
    }

    return;
}

QString LanListItem::getConnectionName()
{
    return m_lanConnectItem.m_connectName;
}

void LanListItem::updateConnectionName(QString connectionName)
{
    m_lanConnectItem.m_connectName = connectionName;
    m_nameLabel->setText(m_lanConnectItem.m_connectName);
    return;
}

QString LanListItem::getConnectionPath()
{
    return m_lanConnectItem.m_connectPath;
}

void LanListItem::updateConnectionPath(QString connectionPath)
{
    m_lanConnectItem.m_connectPath = connectionPath;
}
