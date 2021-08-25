#include "wlanlistitem.h"

WlanListItem::WlanListItem(KyWirelessNetResource *resource, KyWirelessNetItem *data, QWidget *parent) : ListItem(parent)
{
    m_data = data;
    m_resource = resource;
    initWlanUI();
    setExpanded(false);
    initWlanConnection();
}

WlanListItem::WlanListItem(QWidget *parent) : ListItem(parent)
{
    m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-none-symbolic", QIcon(":/res/w/wifi-none.png")));
    const QString str="未连接任何wifi";
    setExpanded(false);
    this->setName(str);
}

WlanListItem::~WlanListItem()
{
    disconnect(m_resource, &KyWirelessNetResource::secuTypeChange, this, &WlanListItem::onSecurityChanged);
}

void WlanListItem::setWlanSignal(const int &signal)
{
    m_data->m_signalStrength = signal;
    refreshIcon();
}

void WlanListItem::setWlanState(const int &state)
{
//    m_data->m_state = state; //ZJP_TODO 后端接口待补全
    refreshIcon();
}

void WlanListItem::setExpanded(const bool &expanded)
{
    m_isExpanded = expanded;
    this->setFixedHeight(expanded ? EXPANDED_HEIGHT : NORMAL_HEIGHT);
}

void WlanListItem::initWlanUI()
{
    m_hasPwd = (m_data->m_secuType.isEmpty() || m_data->m_secuType == "") ? false : true;
    //设置显示的Wlan名称
    this->setName((m_data->m_connName != "") ? m_data->m_connName : m_data->m_NetSsid);
    //刷新左侧按钮图标
    refreshIcon();
}

void WlanListItem::initWlanConnection()
{
    connect(m_resource, &KyWirelessNetResource::secuTypeChange, this, &WlanListItem::onSecurityChanged);
}

void WlanListItem::refreshIcon()
{
#define FULL_SIGNAL 5
#define EXCELLENT_SIGNAL 4
#define GOOD_SIGNAL 3
#define OK_SIGNAL 2
#define LOW_SIGNAL 1
#define STEP 25
//    if (m_data->m_state) //ZJP_TODO 连接中、已连接、未连接的处理，要单独写逻辑，后端接口待补全
    if (!m_hasPwd) {
        //ZJP_TODO 无加密 注意信号格数计算方式，可能需要修改
        switch (m_data->m_signalStrength / STEP + 1) {
        case FULL_SIGNAL:
        case EXCELLENT_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-excellent-symbolic", QIcon(":/res/w/wifi-full.png")));
            break;
        case GOOD_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-good-symbolic", QIcon(":/res/w/wifi-high.png")));
            break;
        case OK_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-ok", QIcon(":/res/w/wifi-medium.png")));
            break;
        case LOW_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-low", QIcon(":/res/w/wifi-low.png")));
            break;
        default:
            qDebug() << "Set wlan(without passwd) icon failed, signal = " << m_data->m_signalStrength << Q_FUNC_INFO << __LINE__;
            break;
        }
    } else {
        //ZJP_TODO 有加密
        switch (m_data->m_signalStrength / STEP + 1) {
        case FULL_SIGNAL:
        case EXCELLENT_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-excellent-secure-symbolic", QIcon(":/res/w/wifi-full-pwd.png")));
            break;
        case GOOD_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-good-secure-symbolic", QIcon(":/res/w/wifi-high-pwd.png")));
            break;
        case OK_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-ok-secure-symbolic", QIcon(":/res/w/wifi-medium-pwd.png")));
            break;
        case LOW_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-low-secure-symbolic", QIcon(":/res/w/wifi-low-pwd.png")));
            break;
        default:
            qDebug() << "Set wlan(with passwd) icon failed, signal = " << m_data->m_signalStrength << Q_FUNC_INFO << __LINE__;
            break;
        }
    }
}

void WlanListItem::onInfoButtonClicked()
{
    //ZJP_TODO 呼出无线详情页
    qDebug() << "On wlan info button clicked! ssid = " << m_data->m_NetSsid << "; name = " << m_data->m_connName << "." <<Q_FUNC_INFO << __LINE__;
}

void WlanListItem::onNetButtonClicked()
{
    //ZJP_TODO 点击连接/断开
    qDebug() << "On wlan clicked! ssid = " << m_data->m_NetSsid << "; name = " << m_data->m_connName << "." <<Q_FUNC_INFO << __LINE__;

}

void WlanListItem::onSecurityChanged(QString interface, QString ssid, QString securityType)
{
    if (ssid != m_data->m_NetSsid) {
        return;
    }
    qDebug() << "Security changed! ssid = " << m_data->m_NetSsid << "; security = " << m_data->m_secuType << "." <<Q_FUNC_INFO << __LINE__;
    m_data->m_secuType = securityType;
    bool newSecu = (m_data->m_secuType.isEmpty() || m_data->m_secuType == "") ? false : true;
    if (m_hasPwd^newSecu) {
        m_hasPwd = newSecu;
        refreshIcon();
    }
}
