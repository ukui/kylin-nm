/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "vpnlistitem.h"
#include "backend/dbus-interface/kylinconnectitem.h"

#include <QDebug>

#define LOG_FLAG "[VpnListItem]"

VpnListItem::VpnListItem(const KyConnectItem *lanConnectItem, QWidget *parent):ListItem(parent)
{
    m_infoButton->setVisible(false);
    m_connectOperation = new KyWiredConnectOperation(this);
    m_deviceResource = new KyNetworkDeviceResourse(this);

    connectItemCopy(lanConnectItem);

    m_nameLabel->setLabelText(m_vpnConnectItem.m_connectName);
    m_netButton->setButtonIcon(QIcon::fromTheme("ukui-vpn-symbolic"));

    qDebug() << "VpnListItem init:" << m_vpnConnectItem.m_connectName << m_vpnConnectItem.m_connectState << m_vpnConnectItem.m_ifaceName;

    if (Deactivated == m_vpnConnectItem.m_connectState || Activated == m_vpnConnectItem.m_connectState) {
        m_netButton->stopLoading();
        if (m_vpnConnectItem.m_connectState == Activated) {
            setIcon(true);
        } else {
            setIcon(false);
        }
    } else {
        m_netButton->startLoading();
    }

    m_itemFrame->installEventFilter(this);
    connect(this->m_infoButton, &InfoButton::clicked, this, &VpnListItem::onInfoButtonClicked);
    connect(m_menu, &QMenu::triggered, this, &VpnListItem::onMenuTriggered);
}


VpnListItem::VpnListItem(QWidget *parent) : ListItem(parent)
{
    m_isActive = false;
    m_netButton->setButtonIcon(QIcon::fromTheme("ukui-vpn-symbolic"));
    setIcon(false);
    const QString str=tr("Not connected");
    m_nameLabel->setLabelText(str);
    this->m_infoButton->hide();
}

VpnListItem::~VpnListItem()
{
    qDebug()<<"[LanPage] lan list item is deleted." << m_vpnConnectItem.m_connectName;
}

void VpnListItem::setIcon(bool isOn)
{
    if (isOn) {
        m_netButton->setActive(true);               //设置图标显示不同颜色
    } else {
        m_netButton->setActive(false);
    }
}

void VpnListItem::connectItemCopy(const KyConnectItem *lanConnectItem)
{
    if (lanConnectItem) {
        m_vpnConnectItem.m_connectName = lanConnectItem->m_connectName;
        m_vpnConnectItem.m_connectPath = lanConnectItem->m_connectPath;
        m_vpnConnectItem.m_connectState = lanConnectItem->m_connectState;
        m_vpnConnectItem.m_connectUuid = lanConnectItem->m_connectUuid;
        m_vpnConnectItem.m_ifaceName = lanConnectItem->m_ifaceName;
        m_vpnConnectItem.m_itemType = lanConnectItem->m_itemType;
    } else {
        qDebug() << LOG_FLAG <<"the connect item is nullptr";
        m_vpnConnectItem.m_connectName = "";
        m_vpnConnectItem.m_connectPath = "";
        m_vpnConnectItem.m_connectState = NetworkManager::ActiveConnection::State::Unknown;
        m_vpnConnectItem.m_connectUuid = "";
        m_vpnConnectItem.m_ifaceName = "";
        m_vpnConnectItem.m_itemType = NetworkManager::ConnectionSettings::ConnectionType::Unknown;
    }

    return;
}

//void VpnListItem::changeState(QString uuid,
//                              NetworkManager::ActiveConnection::State state,
//                              NetworkManager::ActiveConnection::Reason reason)
//{

//}

void VpnListItem::onNetButtonClicked()
{
    if (m_vpnConnectItem.m_connectUuid.isEmpty()) {
        qDebug()<<"--cxc--"<<Q_FUNC_INFO<<__LINE__;
        qDebug() << LOG_FLAG << "connect is empty, so can not connect or disconnect.";
        return;
    }

    if (Deactivated == m_vpnConnectItem.m_connectState) {
        //断开的连接，点击激活连接
        m_connectOperation->activateVpnConnection(m_vpnConnectItem.m_connectUuid);
        qDebug() << LOG_FLAG << "it will activate connection" << m_vpnConnectItem.m_connectName;
        m_netButton->startLoading();
    } else {
        qDebug() << LOG_FLAG <<"the connection" << m_vpnConnectItem.m_connectName
                 << "is not deactived, so it can not be operation.";
    }

    return;

}

void VpnListItem::onRightButtonClicked()
{
    //右键点击事件
    qDebug()<< LOG_FLAG <<"onRightButtonClicked";
    if (!m_menu) {
        return;
    }

    m_menu->clear();
    if (Activated == m_vpnConnectItem.m_connectState || Activating == m_vpnConnectItem.m_connectState) {
        m_menu->addAction(new QAction(tr("Disconnect"), this));
    } else if (Deactivated == m_vpnConnectItem.m_connectState) {
        m_menu->addAction(new QAction(tr("Connect"), this));
    } else {
        return;
    }

    m_menu->move(cursor().pos());
    m_menu->show();
    return;
}

void VpnListItem::onMenuTriggered(QAction *action)
{
    if (action->text() == tr("Connect")) {
        this->onNetButtonClicked();
    } else if (action->text() == tr("Disconnect")) {
        m_connectOperation->deactivateWiredConnection(m_vpnConnectItem.m_connectName, m_vpnConnectItem.m_connectUuid);
        qDebug() << LOG_FLAG << "it will disconnect connection" << m_vpnConnectItem.m_connectName;
        m_netButton->startLoading();
    }
    return;
}


void VpnListItem::onInfoButtonClicked()
{
//    if (m_vpnConnectItem.m_connectUuid.isEmpty()) {
//        qDebug() << LOG_FLAG << "connect is empty, so can not show detail info.";
//        return;
//    }

//    if(netDetail != nullptr){
//        netDetail->activateWindow();
//        return;
//    }

//    qDebug()<< LOG_FLAG << "the info button of lan is clicked! uuid = "
//            << m_vpnConnectItem.m_connectUuid << "; name = " << m_vpnConnectItem.m_connectName
//            << "." <<Q_FUNC_INFO << __LINE__;

//    bool isActivated = false;
//    if (Activated == m_vpnConnectItem.m_connectState) {
//        isActivated = true;
//    }

//    netDetail = new NetDetail(m_deviceName, m_vpnConnectItem.m_connectName,
//                                         m_vpnConnectItem.m_connectUuid, isActivated,false, false);

//    connect(netDetail, &NetDetail::destroyed, [&](){
//        if (netDetail != nullptr) {
//            netDetail = nullptr;
//        }
//    });

//    netDetail->show();
//    Q_EMIT this->detailShow(true);

    return;
}

void VpnListItem::updateConnectionState(ConnectState state)
{
    m_vpnConnectItem.m_connectState = (NetworkManager::ActiveConnection::State)state;

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

QString VpnListItem::getConnectionName()
{
    return m_vpnConnectItem.m_connectName;
}

void VpnListItem::updateConnectionName(QString connectionName)
{
    m_vpnConnectItem.m_connectName = connectionName;
    m_nameLabel->setLabelText(m_vpnConnectItem.m_connectName);
    return;
}

QString VpnListItem::getConnectionPath()
{
    return m_vpnConnectItem.m_connectPath;
}

void VpnListItem::updateConnectionPath(QString connectionPath)
{
    m_vpnConnectItem.m_connectPath = connectionPath;
}
