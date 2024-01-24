/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "kylinwirednetresource.h"
#include "kywirelessconnectoperation.h"
#include "kylinnetworkmanagerutil.h"

#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>
#include <NetworkManagerQt/VpnConnection>
#include <NetworkManagerQt/VpnPlugin>
#include <NetworkManagerQt/VpnSetting>

static bool subLanListSort(const KyWiredItem info1, const KyWiredItem info2)
{
    QString  name1 = info1.m_connectName;
    QString  name2 = info2.m_connectName;
    bool result = true;
    if (QString::compare(name1, name2, Qt::CaseInsensitive) > 0) {
        result =  false;
    }
    return result;
}

static void lanListSort(QList<KyWiredItem> &list)
{
    qSort(list.begin(), list.end(), subLanListSort);
}

KyWiredConnectResourse::KyWiredConnectResourse(QObject *parent) : KyNetResource(parent)
{
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wiredConnectionAdd, this, &KyWiredConnectResourse::wiredConnectionAdd);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wiredConnectionUpdate, this, &KyWiredConnectResourse::wiredConnectionUpdate);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::connectionRemove, this, &KyWiredConnectResourse::connectionRemove);

    connect(m_networkResourceInstance, &KyNetworkResourceManager::wiredDeviceAdd, this, &KyWiredConnectResourse::wiredDeviceAdd);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wiredDeviceUpdate, this, &KyWiredConnectResourse::wiredDeviceUpdate);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceRemove, this, &KyWiredConnectResourse::deviceRemove);
}

//配合activeconnection 此处未单独区分已连接的wifi
void KyWiredConnectResourse::getWiredList(QMap<QString,QList<KyWiredItem>> &map)
{
    map.clear();
    kyWirdNetItemListInit();
    if (m_wiredNetworkMap.isEmpty()) {
        return;
    } else {
        QMap<QString, QList<KyWiredItem> >::iterator iter = m_wiredNetworkMap.begin();
        while (iter != m_wiredNetworkMap.end()) {
            lanListSort(m_wiredNetworkMap[iter.key()]);
            iter++;
        }
        map = m_wiredNetworkMap;
    }
}

void KyWiredConnectResourse::getWiredConnectDetail(QString interface, QString uuid, KyDetailInfo &connectSetting)
{
    getBaseInfo(interface, "", false, false, connectSetting);
    getDynamicIpInfo(uuid, connectSetting);
}

KyWiredItem KyWiredConnectResourse::getConnectionItem(NetworkManager::Connection::Ptr connectPtr)
{
    if (nullptr == connectPtr) {
        qWarning()<<"[KyConnectResourse]"<<"the connect is empty";
        return KyWiredItem();
    }

    KyWiredItem connectionItem;
    connectionItem.m_connectName = connectPtr->name();
    connectionItem.m_connectUuid = connectPtr->uuid();
    connectionItem.m_connectPath = connectPtr->path();

    return connectionItem;
}


void KyWiredConnectResourse::kyWirdNetItemListInit()
{
    m_wiredNetworkMap.clear();
    NetworkManager::Connection::List connectList = m_networkResourceInstance->getConnectList();

    if (connectList.empty()) {
        qWarning()<<"[KyConnectResourse]"<<"kyWirdNetItemListInit failed, the connect list is empty";
        return;
    }

    KyNetworkDeviceResourse deviceResource;
    QStringList devList;
    deviceResource.getNetworkDeviceList(DEVICE_TYPE_ETHERNET, devList);
    if (devList.isEmpty()) {
        qWarning()<<"[KyConnectResourse]"<<"kyWirdNetItemListInit failed, the device list is empty";
        return;
    } else {
        for (int i = 0; i < devList.size(); ++i) {
            m_wiredNetworkMap.insert(devList.at(i), QList<KyWiredItem>());
        }
    }

    NetworkManager::Connection::Ptr connectPtr = nullptr;
    for (int index = 0; index < connectList.size(); index++) {
        connectPtr = connectList.at(index);
        if (connectPtr.isNull()) {
            continue;
        }

        if (!m_networkResourceInstance->isWiredConnect(connectPtr->path())) {
            continue;
        }

        KyWiredItem connectItem = getConnectionItem(connectPtr);
        if (connectItem.m_connectUuid.isEmpty()) {
           continue;
        }

        QString connectInterface = connectPtr->settings()->interfaceName();
        if (connectInterface.isEmpty()) {
            for (int i = 0; i < m_wiredNetworkMap.keys().size(); ++i) {
                m_wiredNetworkMap[m_wiredNetworkMap.keys().at(i)] << connectItem;
            }
        } else {
            if (m_wiredNetworkMap.contains(connectInterface)) {
                m_wiredNetworkMap[connectInterface] << connectItem;
            }
        }
    }
}
