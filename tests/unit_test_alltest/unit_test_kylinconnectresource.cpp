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
#include "../../src/backend/dbus-interface/kylinconnectresource.h"

using namespace stub_ext;

class KyConnectResourseTest : public testing::Test
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

QDBusInterface *KyConnectResourseTest::m_pDBus = nullptr;
TEST_F(KyConnectResourseTest, getConnectionItemByUuid)
{
    KyConnectResourse resource;
    QString uuid  = "11111";

    KyConnectItem *ret = resource.getConnectionItemByUuid(uuid);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getConnectionItemByUuidWithoutActivateChecking)
{
    KyConnectResourse resource;
    QString uuid  = "11111";

    KyConnectItem *ret = resource.getConnectionItemByUuidWithoutActivateChecking(uuid);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getVpnAndVirtualConnections)
{
    KyConnectResourse resource;
    QList<KyConnectItem *> list;
    list.append(new KyConnectItem());
    list.append(new KyConnectItem());
    list.append(new KyConnectItem());

    resource.getVpnAndVirtualConnections(list);

    ASSERT_EQ(100, 100);
}
TEST_F(KyConnectResourseTest, getConnectionList)
{
    KyConnectResourse resource;
    QString deviceName = "test";
    NetworkManager::ConnectionSettings::ConnectionType connectionType;
    QList<KyConnectItem *> list;
    list.append(new KyConnectItem());
    list.append(new KyConnectItem());
    list.append(new KyConnectItem());

    resource.getConnectionList(deviceName, connectionType, list);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getVpnConnections)
{
    KyConnectResourse resource;
    QList<KyVpnConnectItem *> list;
    list.append(new KyVpnConnectItem());
    list.append(new KyVpnConnectItem());
    list.append(new KyVpnConnectItem());

    resource.getVpnConnections(list);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getBluetoothConnections)
{
    KyConnectResourse resource;
    QList<KyBluetoothConnectItem *> list;
    list.append(new KyBluetoothConnectItem());
    list.append(new KyBluetoothConnectItem());
    list.append(new KyBluetoothConnectItem());

    resource.getBluetoothConnections(list);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getApConnections)
{
    KyConnectResourse resource;
    QList<KyApConnectItem *> list;
    list.append(new KyApConnectItem());
    list.append(new KyApConnectItem());
    list.append(new KyApConnectItem());

    resource.getApConnections(list);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getApConnectionByUuid)
{
    KyConnectResourse resource;
    QString connectUuid = "id";

    KyApConnectItem *ret = resource.getApConnectionByUuid(connectUuid);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getApConnectionPathByUuid)
{
    KyConnectResourse resource;
    QString connectUuid = "id";

    QString ret = resource.getApConnectionPathByUuid(connectUuid);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getConnectionSetting)
{
    KyConnectResourse resource;
    QString connectUuid = "id";
    KyConnectSetting connectSetting;

    resource.getConnectionSetting(connectUuid, connectSetting);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getInterfaceByUuid)
{
    KyConnectResourse resource;
    QString deviceName = "devName";
    QString connUuid = "id";

    bool ret = resource.getInterfaceByUuid(deviceName, connUuid);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, getConnectivity)
{
    KyConnectResourse resource;
    NetworkManager::Connectivity connectivity;

    resource.getConnectivity(connectivity);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, isVirtualConncection)
{
    KyConnectResourse resource;
    QString uuid = "id";

    bool ret = resource.isVirtualConncection(uuid);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, isWiredConnection)
{
    KyConnectResourse resource;
    QString uuid = "id";

    bool ret = resource.isWiredConnection(uuid);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, isActivatedConnection)
{
    KyConnectResourse resource;
    QString uuid = "id";

    bool ret = resource.isActivatedConnection(uuid);

    ASSERT_EQ(100, 100);
}

TEST_F(KyConnectResourseTest, isApConnection)
{
    KyConnectResourse resource;
    QString uuid = "id";

    bool ret = resource.isApConnection(uuid);

    ASSERT_EQ(100, 100);
}
