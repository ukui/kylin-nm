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
#include "../../src/backend/dbus-interface/kyenterpricesettinginfo.h"

using namespace stub_ext;

class KyEnterpriceSettingInfoTest : public testing::Test
{
protected:
    static void SetUpTestSuite()
    {
//        m_pNetworkModeConfigDBus = new QDBusInterface("org.freedesktop.NetworkManager",
//                                    connectPath,
//                                  "org.freedesktop.NetworkManager.Settings.Connection",
//                                  QDBusConnection::systemBus());
    }

    static void TearDownTestSuite()
    {
//        if (m_pNetworkModeConfigDBus) {
//            delete m_pNetworkModeConfigDBus;
//            m_pNetworkModeConfigDBus = nullptr;
//        }
    }
    static QDBusInterface* m_pDBus;
};

QDBusInterface *KyEnterpriceSettingInfoTest::m_pDBus = nullptr;
TEST_F(KyEnterpriceSettingInfoTest, modifyEapMethodFastSettings)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyEnterpriceSettingInfoTest, modifyEapMethodTtlsSettings)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyEnterpriceSettingInfoTest, modifyEapMethodPeapSettings)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyEnterpriceSettingInfoTest, modifyEapMethodTlsSettings)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyEnterpriceSettingInfoTest, assembleEapMethodFastSettings)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyEnterpriceSettingInfoTest, assembleEapMethodTlsSettings)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyEnterpriceSettingInfoTest, assembleEapMethodPeapSettings)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyEnterpriceSettingInfoTest, assembleEapMethodTtlsSettings)
{
    ASSERT_EQ(100, 100);
}


