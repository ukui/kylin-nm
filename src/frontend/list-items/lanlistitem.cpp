#include "lanlistitem.h"
#include "backend/dbus-interface/kylinconnectitem.h"

#include <QDebug>


LanListItem::LanListItem(KyConnectItem *data, QString deviceName, QWidget *parent)
    : m_data(data), deviceName(deviceName), ListItem(parent)    //item数据传入
{
    m_connectOperation = new KyWiredConnectOperation;
    m_data = data;
    m_nameLabel->setText(m_data->m_connectName);

    if (m_data != nullptr) {
        if (m_data->m_connectState == NetworkManager::ActiveConnection::State::Activated) {
            setIcon(true);
            m_isActive = true;
        } else {
            setIcon(false);
            m_isActive = false;
        }
    }
    m_netButton->setActive(m_isActive);
    m_itemFrame->installEventFilter(this);
    connect(this->m_infoButton, &InfoButton::clicked, this, &LanListItem::onInfoButtonClicked);
}

void LanListItem::onNetButtonClicked()
{
    if (!m_isActive) {
        //未连接,点击后连
        m_connectOperation->activateWiredConnection(m_data->m_connectUuid, deviceName);
        qDebug() << m_data->m_connectName << "Connect after user clicked!";
        m_isActive = true;
    } else {
        //连接，点击后断开
        m_connectOperation->deactivateWiredConnection(m_data->m_connectName, m_data->m_connectUuid);
        qDebug() << m_data->m_connectName << "Disconnect after user clicked!";
        m_isActive = false;
    }
}

void LanListItem::setIcon(bool isOn)
{
    if (isOn) {
        m_netButton->setButtonIcon(QIcon::fromTheme("network-wired-connected-symbolic"));
    } else {
        m_netButton->setButtonIcon(QIcon::fromTheme("network-wired-disconnected-symbolic"));
    }
}

void LanListItem::onInfoButtonClicked()
{
    if(m_data){
        qDebug()<<"是否激活："<<m_isActive;
        qDebug() << "On lan info button clicked! uuid = " << m_data->m_connectUuid << "; name = " << m_data->m_connectName << "." <<Q_FUNC_INFO << __LINE__;
        NetDetail *netDetail = new NetDetail(m_data->m_connectName, m_data->m_connectUuid, m_isActive,false, false);
        netDetail->show();
    }
    else{
        qDebug() << "On wlan info button clicked! But there is no wlan connect " ;
    }
}
