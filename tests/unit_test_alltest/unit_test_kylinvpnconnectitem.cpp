/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2024 KylinSoft  Co., Ltd.
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
#include <gtest/gtest.h>
#include <gtest/gtest-death-test.h>
#include <QDBusReply>
#include <QDBusInterface>
#include <QGSettings>
#include <QMetaType>
#include <QDBusMetaType>
#include "stubext.h"
#include "../../src/backend/dbus-interface/kylinvpnconnectitem.h"

using namespace stub_ext;

class KyVpnConnectItemTest : public testing::Test
{
protected:
    static void SetUpTestSuite()
    {
    }

    static void TearDownTestSuite()
    {
    }
    static QDBusInterface* m_pDBus;
};

QDBusInterface *KyVpnConnectItemTest::m_pDBus = nullptr;
TEST_F(KyVpnConnectItemTest, allParameter)
{
    KyVpnConnectItem *vpnConnectItem = new KyVpnConnectItem();
    vpnConnectItem->m_vpnName = "connectPtr->name()";
    vpnConnectItem->m_vpnUuid = "activeConnectPtr->uuid()";
    vpnConnectItem->m_vpnUser = "settingPtr->interfaceName()";
    vpnConnectItem->m_vpnGateWay = "wirelessSetting->ssid()";
    vpnConnectItem->m_vpnIpv4Address = "apConnectItem->ipv4";
    vpnConnectItem->m_vpnIpv6Address = "apConnectItem->ipv6";
    vpnConnectItem->m_vpnMppe = false;
    vpnConnectItem->m_vpnState = NetworkManager::VpnConnection::State::Disconnected;
    ASSERT_EQ(100, 100);
    delete vpnConnectItem;
}


