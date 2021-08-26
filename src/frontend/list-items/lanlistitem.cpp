#include "lanlistitem.h"
#include "backend/dbus-interface/kylinconnectitem.h"

#include <QDebug>


LanListItem::LanListItem(KyConnectItem *data, QString deviceName, QWidget *parent)
    : m_data(data), deviceName(deviceName), ListItem(parent)    //item数据传入
{
    m_wiredActivatelist = new KyActiveConnectResourse;
    m_connect_operation = new KyWiredConnectOperation;

    if (m_data != nullptr) {
        if (m_data->m_connectState == NetworkManager::ActiveConnection::State::Activated) {
            m_data = data;
            m_nameLabel->setText(m_data->m_connectName);
            setIcon(true);
            m_activated = true;
        } else {
            m_data = data;
            m_nameLabel->setText(m_data->m_connectName);
            setIcon(false);
            m_activated = false;
        }
    }
    m_itemFrame->installEventFilter(this);
}

bool LanListItem::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_itemFrame) {
        if (event->type() == QEvent::MouseButtonPress) {
            if (!m_activated) {
                //未连接,点击后连
                m_connect_operation->activateWiredConnection(m_data->m_connectUuid, deviceName);
                qDebug() << m_data->m_connectName << "点击后连接";
                m_activated = true;
            } else {
                //连接，点击后断开
                m_connect_operation->deactivateWiredConnection(m_data->m_connectName, m_data->m_connectUuid);
                qDebug() << m_data->m_connectName << "点击后断开";
                m_activated = false;
            }
        }
    }
    return ListItem::eventFilter(watched, event);
}

void LanListItem::setIcon(bool isOn)
{
    if (isOn) {
        m_netButton->setIcon(QIcon::fromTheme("network-wired-connected-symbolic"));
    } else {
        m_netButton->setIcon(QIcon::fromTheme("network-wired-disconnected-symbolic"));
    }
}
