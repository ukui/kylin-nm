#include "lanlistitem.h"

LanListItem::LanListItem(KyConnectItem *data, QWidget *parent) : ListItem(parent)
{
    m_data = data;
}

void LanListItem::initLanUI()
{
    this->setName(m_data->m_connectName);
}

void LanListItem::refreshIcon()
{
    //    if (m_data->m_connectState) //ZJP_TODO
}

void LanListItem::onInfoButtonClicked()
{
    //ZJP_TODO 呼出有线详情页
    qDebug() << "On lan info button clicked! name = " << m_data->m_connectName << "; uuid = " << m_data->m_connectUuid << "." <<Q_FUNC_INFO << __LINE__;
}

void LanListItem::onNetButtonClicked()
{
    //ZJP_TODO 点击连接/断开
    qDebug() << "On lan clicked! name = " << m_data->m_connectName << "; uuid = " << m_data->m_connectUuid << "." <<Q_FUNC_INFO << __LINE__;
}
