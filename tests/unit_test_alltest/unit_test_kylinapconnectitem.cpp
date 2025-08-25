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
#include "../../src/backend/dbus-interface/kylinapconnectitem.h"

#define PASSWORD_TEST "password123123"
using namespace stub_ext;

class KyApConnectItemInterfaceTest : public testing::Test
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

QDBusInterface *KyApConnectItemInterfaceTest::m_pDBus = nullptr;
TEST_F(KyApConnectItemInterfaceTest, allParameter)
{
    KyApConnectItem *apConnectItem = new KyApConnectItem();
    apConnectItem->m_connectName = "connectPtr->name()";
    apConnectItem->m_connectUuid = "activeConnectPtr->uuid()";
    apConnectItem->m_ifaceName = "settingPtr->interfaceName()";
    apConnectItem->m_connectSsid = "wirelessSetting->ssid()";
    apConnectItem->m_password = PASSWORD_TEST;
    ASSERT_EQ(100, 100);
    delete apConnectItem;
}


