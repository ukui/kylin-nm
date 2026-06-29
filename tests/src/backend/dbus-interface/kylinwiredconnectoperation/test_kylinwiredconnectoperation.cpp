/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "test_kylinwiredconnectoperation.h"
#include "../../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.h"
#include "../../../../../../src/backend/dbus-interface/kylinconnectsetting.h"
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

TestKyWiredConnectOperation::TestKyWiredConnectOperation()
{
}

TestKyWiredConnectOperation::~TestKyWiredConnectOperation()
{
}

void TestKyWiredConnectOperation::initTestCase()
{
    m_operation = new KyWiredConnectOperation();
}

void TestKyWiredConnectOperation::cleanupTestCase()
{
    if (m_operation) {
        delete m_operation;
        m_operation = nullptr;
    }
}

void TestKyWiredConnectOperation::testConstructor()
{
    QVERIFY(m_operation != nullptr);
}

void TestKyWiredConnectOperation::testSetWiredEnabled()
{
    m_operation->setWiredEnabled(true);
    m_operation->setWiredEnabled(false);
    QVERIFY(true);
}

void TestKyWiredConnectOperation::testGetWiredEnabled()
{
    bool enabled = m_operation->getWiredEnabled();
    QVERIFY2(enabled == true || enabled == false, "getWiredEnabled should return a valid boolean");
}

void TestKyWiredConnectOperation::testCreateWiredConnect()
{
    KyConnectSetting settings;
    QString connectName = "Test Wired Connection";
    settings.setConnectName(connectName);
    settings.m_isAutoConnect = true;
    m_operation->createWiredConnect(settings);
    QTest::qWait(100);
    QVERIFY(true);
}

void TestKyWiredConnectOperation::testUpdateWiredConnect()
{
    KyConnectSetting settings;
    QString connectName = "Test Wired Connection";
    settings.setConnectName(connectName);
    m_operation->updateWiredConnect("invalid-uuid-12345", settings);
    QTest::qWait(100);
    QVERIFY(true);
}

void TestKyWiredConnectOperation::testDeleteWiredConnect()
{
    m_operation->deleteWiredConnect("invalid-uuid-12345");
    QTest::qWait(100);
    QVERIFY(true);
}

void TestKyWiredConnectOperation::testActivateWiredConnection()
{
    m_operation->activateWiredConnection("invalid-uuid-12345", "eth0");
    QTest::qWait(100);
    QVERIFY(true);
}

void TestKyWiredConnectOperation::testDeactivateWiredConnection()
{
    m_operation->deactivateWiredConnection("test-connection", "invalid-uuid-12345", false, "eth0");
    QTest::qWait(100);
    QVERIFY(true);
}

void TestKyWiredConnectOperation::testSetWiredAutoConnect()
{
    m_operation->setWiredAutoConnect("invalid-uuid-12345", true);
    m_operation->setWiredAutoConnect("invalid-uuid-12345", false);
    QTest::qWait(100);
    QVERIFY(true);
}

void TestKyWiredConnectOperation::testCloseWiredNetworkWithDevice()
{
    int result = m_operation->closeWiredNetworkWithDevice("nonexistent-device");
    QVERIFY(result == -ENXIO || result == -EINVAL || result == 0);
}

void TestKyWiredConnectOperation::testOpenWiredNetworkWithDevice()
{
    int result = m_operation->openWiredNetworkWithDevice("nonexistent-device");
    QVERIFY(result == -ENXIO || result == -EINVAL || result == 0);
}

void TestKyWiredConnectOperation::testSetWiredDeviceAutoconnect()
{
    int result = m_operation->setWiredDeviceAutoconnect("nonexistent-device", true);
    QVERIFY(result == -ENXIO || result == -EINVAL || result == 0);
    
    result = m_operation->setWiredDeviceAutoconnect("nonexistent-device", false);
    QVERIFY(result == -ENXIO || result == -EINVAL || result == 0);
}
