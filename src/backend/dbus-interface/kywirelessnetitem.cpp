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
#include "kywirelessnetitem.h"
#include <NetworkManagerQt/Connection>
#include "kylinutil.h"

const QString ENTERPRICE_TYPE = "802.1X";
const QString WPA1_AND_WPA2 = "WPA";
const QString WPA3 = "WPA3";

QString enumToQstring(NetworkManager::AccessPoint::Capabilities cap, NetworkManager::AccessPoint::WpaFlags wpa_flags,NetworkManager::AccessPoint::WpaFlags rsn_flags)
{
    QString out;
    if (   (cap & NM_802_11_AP_FLAGS_PRIVACY)
           && (wpa_flags == NM_802_11_AP_SEC_NONE)
           && (rsn_flags == NM_802_11_AP_SEC_NONE)) {
        out += "WEP ";
    }
    if (wpa_flags != NM_802_11_AP_SEC_NONE) {
        out += "WPA1 ";
    }
    if ((rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_PSK)
            || (rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)) {
        out += "WPA2 ";
    }
    if (rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_SAE) {
        out += "WPA3 ";
    }
    if (   (wpa_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)
           || (rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)) {
        out += "802.1X ";
    }
    return out;
}

KyWirelessNetItem::KyWirelessNetItem(NetworkManager::WirelessNetwork::Ptr net)
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

    m_bssid = "";
    m_connectUuid = "";
    m_isConfigured = false;
    m_connName = "";
    m_connDbusPath = "";
    m_secuType = "";
    m_kySecuType = NONE;
    m_device = "";
    m_channel = 0;

    init(net);
}


KyWirelessNetItem::~KyWirelessNetItem()
{
    m_networkResourceInstance = nullptr;
}


void KyWirelessNetItem::init(NetworkManager::WirelessNetwork::Ptr net)
{
   // m_NetSsid = net->ssid();

    NetworkManager::AccessPoint::Ptr accessPointPtr = net->referenceAccessPoint();
    QByteArray rawSsid = accessPointPtr->rawSsid();
    m_NetSsid = getSsidFromByteArray(rawSsid);

    m_signalStrength = net->signalStrength();
    m_frequency = net->referenceAccessPoint()->frequency();
    m_channel = NetworkManager::findChannel(m_frequency);
    NetworkManager::AccessPoint::Capabilities cap = net->referenceAccessPoint()->capabilities();
    NetworkManager::AccessPoint::WpaFlags wpaFlag = net->referenceAccessPoint()->wpaFlags();
    NetworkManager::AccessPoint::WpaFlags rsnFlag = net->referenceAccessPoint()->rsnFlags();
    m_secuType = enumToQstring(cap, wpaFlag, rsnFlag);
//    if (m_secuType.indexOf(ENTERPRICE_TYPE) >= 0) {
//            m_kySecuType = WPA_AND_WPA2_ENTERPRISE;
//        } else if (m_secuType.indexOf(WPA3) >= 0) {
//            m_kySecuType = WPA3_PERSONAL;
//        } else if ( m_secuType.indexOf(WPA1_AND_WPA2) >= 0) {
//            m_kySecuType = WPA_AND_WPA2_PERSONAL;
//    }
    setKySecuType(m_secuType);
    m_bssid = net->referenceAccessPoint()->hardwareAddress();
    m_device = net->device();
    m_uni = net->referenceAccessPoint()->uni();
    initInfoBySsid();
}

void KyWirelessNetItem::initInfoBySsid()
{
    for (auto const & conn : m_networkResourceInstance->m_connections) {
        NetworkManager::ConnectionSettings::Ptr settings = conn->settings();
        if (settings->connectionType() != NetworkManager::ConnectionSettings::Wireless) {
            continue;
        }

        NetworkManager::WirelessSetting::Ptr wifi_sett
            = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
        QString devName = m_networkResourceInstance->findDeviceUni(m_device)->interfaceName();
        QByteArray rawSsid = wifi_sett->ssid();
        QString wifiSsid = getSsidFromByteArray(rawSsid);
        if (wifiSsid == m_NetSsid
                && (settings->interfaceName().compare(devName) == 0 || settings->interfaceName().isEmpty())) {
            m_connectUuid = settings->uuid();
            m_connName    = conn->name();
            m_connDbusPath = conn->path();
            m_isConfigured = true;
            /*
            * 如果有激活的链接，则取激活的链接，没有则取最后一个，因为一个热点可以创建多个链接
            */
            if (nullptr != m_networkResourceInstance->getActiveConnect(m_connectUuid)) {
                break;
            }
        }
    }

    return;
}

int KyWirelessNetItem::getCategory(QString uni)
{

    QDBusInterface interface( "org.freedesktop.NetworkManager", uni, "org.freedesktop.DBus.Properties", QDBusConnection::systemBus() );
    if (!interface.isValid()) {
        qDebug() << Q_FUNC_INFO << "dbus is invalid";
        return -1;
    }

    QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "Category");
    if (!reply.isValid()) {
        //qDebug()<<"can not get the attribute 'Category' in func getCategory()";
        return 0;
    } else {
        return reply.value().toInt();
    }

}

void KyWirelessNetItem::setKySecuType(QString strSecuType)
{
    if (strSecuType.indexOf(ENTERPRICE_TYPE) >= 0) {
        m_kySecuType = WPA_AND_WPA2_ENTERPRISE;
    } else if (strSecuType.indexOf(WPA3) >= 0) {
        m_kySecuType = WPA3_PERSONAL;
    } else if ( strSecuType.indexOf(WPA1_AND_WPA2) >= 0) {
        m_kySecuType = WPA_AND_WPA2_PERSONAL;
    } else {
        m_kySecuType = NONE;
    }
}
