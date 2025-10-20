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
#include "../../src/backend/dbus-interface/kylinbluetoothconnectitem.h"

using namespace stub_ext;

class KyBluetoothConnectItemTest : public testing::Test
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

QDBusInterface *KyBluetoothConnectItemTest::m_pDBus = nullptr;
TEST_F(KyBluetoothConnectItemTest, allParameters)
{
    KyBluetoothConnectItem *btConnectItem = new KyBluetoothConnectItem();
    btConnectItem->m_connectName = "connectPtr->name()";
    btConnectItem->m_connectUuid = "activeConnectPtr->uuid()";
    btConnectItem->m_deviceAddress = "settingPtr->interfaceName()";
    btConnectItem->m_ipv4Address = "wirelessSetting->ssid()";
    btConnectItem->m_ipv6Address = "apConnectItem->m_connectUuid";
    btConnectItem->m_state = NetworkManager::ActiveConnection::State::Deactivated;
    ASSERT_EQ(100, 100);
    delete btConnectItem;
}

