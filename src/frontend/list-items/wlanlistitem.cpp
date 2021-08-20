#include "wlanlistitem.h"

WlanListItem::WlanListItem(KyWirelessNetItem *data, QWidget *parent) : ListItem(parent)
{
    m_data = data;
    initWlanUI();
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

void WlanListItem::initWlanUI()
{
    //设置显示的Wlan名称
    this->setName(m_data->m_connName);
    //刷新左侧按钮图标
    refreshIcon();
}

void WlanListItem::refreshIcon()
{
//    if (m_data->m_state) //ZJP_TODO 连接中、已连接、未连接的处理，要单独写逻辑，后端接口待补全
    if (m_data->m_secuType.isEmpty() ||m_data->m_secuType == "") {
        //ZJP_TODO 无加密 注意信号格数计算方式，可能需要修改
        switch (m_data->m_signalStrength / 25 + 1) {
        case 5:
        case 4:
            break;
        case 3:
            break;
        case 2:
            break;
        case 1:
            break;
        default:
            break;
        }
    } else {
        //ZJP_TODO 有加密
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
