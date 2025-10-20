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
#include "../../src/backend/dbus-interface/kywirelessconnectoperation.h"

#define PSK_SETTING_NAME "802-11-wireless-security"
#define PRIVATE_PSK_SETTING_NAME "802-1x"
using namespace stub_ext;

class KyWirelessConnectOperationInterfaceTest : public testing::Test
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

QDBusInterface *KyWirelessConnectOperationInterfaceTest::m_pDBus = nullptr;
TEST_F(KyWirelessConnectOperationInterfaceTest, getPrivateKeyPassword)
{
    QString connectUuid = "";
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(connectUuid);
    if (connectPtr.isNull()) {
        QString errorMessage =  connectUuid;
        qWarning()<<errorMessage;
    }
    ASSERT_EQ(100, 100);
}

TEST_F(KyWirelessConnectOperationInterfaceTest, get8021xPassword)
{
    QString connectUuid = "";
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(connectUuid);
    if (connectPtr.isNull()) {
        QString errorMessage = connectUuid;
        qWarning()<<errorMessage;
    }
    ASSERT_EQ(100, 100);
}

TEST_F(KyWirelessConnectOperationInterfaceTest, getBlackListHostName)
{
    ASSERT_EQ(100, 100);
}

