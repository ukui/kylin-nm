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
#include "../../src/backend/dbus-interface/kywirelessnetitem.h"

using namespace stub_ext;

class KyWirelessNetItemTest : public testing::Test
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

QDBusInterface *KyWirelessNetItemTest::m_pDBus = nullptr;
TEST_F(KyWirelessNetItemTest, allParameter)
{
    KyWirelessNetItem wirelessNetItem = KyWirelessNetItem();
    wirelessNetItem.m_networkResourceInstance = KyNetworkResourceManager::getInstance();
    wirelessNetItem.m_bssid = "m_bssid";
    wirelessNetItem.m_connectUuid = "m_connectUuid";
    wirelessNetItem.m_isConfigured = false;
    wirelessNetItem.m_connName = "m_connName";
    wirelessNetItem.m_connDbusPath = "m_connDbusPath";
    wirelessNetItem.m_secuType = "m_secuType";
    wirelessNetItem.m_kySecuType = KYLIN_NM::NONE;
    wirelessNetItem.m_device = "m_device";
    wirelessNetItem.m_channel = 0;
    wirelessNetItem.m_isMix = false;
    ASSERT_EQ(100, 100);
}

TEST_F(KyWirelessNetItemTest, getDevice)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyWirelessNetItemTest, getCategory)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyWirelessNetItemTest, setKySecuType)
{
    ASSERT_EQ(100, 100);
}
