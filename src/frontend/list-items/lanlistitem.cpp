#include "lanlistitem.h"
#include "backend/dbus-interface/kylinconnectitem.h"

#include <QDebug>


LanListItem::LanListItem(KyConnectItem *data, QString deviceName, QWidget *parent)
    : m_data(data), deviceName(deviceName), ListItem(parent)    //item数据传入
{
    m_connectOperation = new KyWiredConnectOperation(this);
    m_activeConnectResource = new KyActiveConnectResourse(this);
    m_connectResource = new KyConnectResourse(this);
    m_deviceResource = new KyNetworkDeviceResourse(this);
    m_data = data;
    m_nameLabel->setText(m_data->m_connectName);

    if (m_data != nullptr) {
        if (m_data->m_connectState == NetworkManager::ActiveConnection::State::Activated) {
            setIcon(true);
            m_isActive = true;
        } else
        {
            setIcon(false);
            m_isActive = false;
        }
    }
    m_itemFrame->installEventFilter(this);
    connect(this->m_infoButton, &InfoButton::clicked, this, &LanListItem::onInfoButtonClicked);
    connect(m_activeConnectResource, &KyActiveConnectResourse::stateChangeReason, this, &LanListItem::onLanStatusChange);
}

LanListItem::LanListItem(QWidget *parent) : ListItem(parent)
{
    m_isActive = false;
    m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-none-symbolic"));
    const QString str=tr("Not connected");
    m_nameLabel->setText(str);
    this->m_infoButton->hide();
}

void LanListItem::setIcon(bool isOn)
{
    if (isOn) {
        m_netButton->setButtonIcon(QIcon::fromTheme("network-wired-connected-symbolic"));
        m_netButton->setActive(true);               //设置图标显示不同颜色
    } else {
        m_netButton->setButtonIcon(QIcon::fromTheme("network-wired-disconnected-symbolic"));
        m_netButton->setActive(false);
    }
}

void LanListItem::onNetButtonClicked()
{
    if(!m_data){
        qDebug() << "A nullItem clicked!" << Q_FUNC_INFO << __LINE__;
        return;
    }
    if (!m_isActive) {
        //未连接,点击后连
        if (m_deviceResource->wiredDeviceCarriered(deviceName)) {
            m_connectOperation->activateWiredConnection(m_data->m_connectUuid, deviceName);
            qDebug() << m_data->m_connectName << "Connect after user clicked!" << deviceName;
            m_isActive = true;
        }
        else {
            qDebug() << "[LanListItem] Wired Device not carried";
            this->showDesktopNotify(tr("Wired Device not carried"));
            m_isActive = false;
        }
    } else {
        //连接，点击后断开
        m_connectOperation->deactivateWiredConnection(m_data->m_connectName, m_data->m_connectUuid);
        qDebug() << m_data->m_connectName << "Disconnect after user clicked!" << deviceName;
        m_isActive = false;
    }
}

void LanListItem::onRightButtonClicked()
{
    //右键点击事件
}

void LanListItem::onInfoButtonClicked()
{
    if(m_data){
        qDebug()<<"Net active or not:"<<m_isActive;
        qDebug() << "On lan info button clicked! uuid = " << m_data->m_connectUuid << "; name = " << m_data->m_connectName << "." <<Q_FUNC_INFO << __LINE__;
        NetDetail *netDetail = new NetDetail(deviceName, m_data->m_connectName, m_data->m_connectUuid, m_isActive,false, false, this);
        netDetail->show();
    }
    else{
        qDebug() << "On lan info button clicked! But there is no wlan connect " ;
    }
}

void LanListItem::onLanStatusChange(QString uuid, NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason)
{
    qDebug() <<"[LanListItem] Connection State Change to:" << state << uuid;

    if (m_data->m_connectUuid == uuid) {
        if (state == NetworkManager::ActiveConnection::State::Activating
                || state == NetworkManager::ActiveConnection::State::Deactivating) {
            qDebug() << "[LanListItem]Activating!Loading!" << state;
            m_netButton->startLoading();
        } else {
            qDebug() << "[LanListItem]Stop!" << state;
            m_netButton->stopLoading();
            if (state == NetworkManager::ActiveConnection::State::Activated) {
                setIcon(true);
            } else {
                setIcon(false);
            }
        }
    }
}
