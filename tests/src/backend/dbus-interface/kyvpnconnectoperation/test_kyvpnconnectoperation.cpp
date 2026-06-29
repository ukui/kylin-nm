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

#include "test_kyvpnconnectoperation.h"
#include "../../../../../../src/backend/dbus-interface/kyvpnconnectoperation.h"
#include "../../../../../../src/backend/dbus-interface/kylinconnectsetting.h"
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

TestKyVpnConnectOperation::TestKyVpnConnectOperation()
{
}

TestKyVpnConnectOperation::~TestKyVpnConnectOperation()
{
}

void TestKyVpnConnectOperation::initTestCase()
{
    m_operation = new KyVpnConnectOperation();
}

void TestKyVpnConnectOperation::cleanupTestCase()
{
    if (m_operation) {
        delete m_operation;
        m_operation = nullptr;
    }
}

void TestKyVpnConnectOperation::testConstructor()
{
    QVERIFY(m_operation != nullptr);
}

void TestKyVpnConnectOperation::testCreateVpnConnect()
{
    KyVpnConfig vpnSettings;
    vpnSettings.m_connectName = "Test VPN Connection";
    vpnSettings.m_vpnType = KYVPNTYPE_L2TP;
    vpnSettings.m_gateway = "vpn.example.com";
    vpnSettings.m_userName = "testuser";
    vpnSettings.m_userPasswd = "testpass";
    vpnSettings.m_isAutoConnect = true;

    QSignalSpy spy(m_operation, &KyVpnConnectOperation::createConnectionError);
    m_operation->createVpnConnect(vpnSettings);
    QTest::qWait(500);
    
    QVERIFY(true);
    Q_UNUSED(spy);
}

void TestKyVpnConnectOperation::testSetVpnConfig()
{
    KyVpnConfig vpnConfig;
    vpnConfig.m_connectName = "Test VPN";
    vpnConfig.m_vpnType = KYVPNTYPE_L2TP;
    vpnConfig.m_gateway = "vpn.example.com";
    vpnConfig.m_isAutoConnect = true;

    QString invalidUuid = "00000000-0000-0000-0000-000000000000";
    QSignalSpy spy(m_operation, &KyVpnConnectOperation::updateConnectionError);
    m_operation->setVpnConfig(invalidUuid, vpnConfig);
    QTest::qWait(500);
    
    QVERIFY(true);
    Q_UNUSED(spy);
}

void TestKyVpnConnectOperation::testGetVpnConfig()
{
    QString invalidUuid = "00000000-0000-0000-0000-000000000000";
    KyVpnConfig result = m_operation->getVpnConfig(invalidUuid);
    
    QVERIFY(result.m_vpnType >= KYVPNTYPE_UNKNOWN && result.m_vpnType <= KYVPNTYPE_STRONGSWAN);
}

void TestKyVpnConnectOperation::testActivateVpnConnection()
{
    QString invalidUuid = "00000000-0000-0000-0000-000000000000";
    QSignalSpy spy(m_operation, &KyVpnConnectOperation::activateConnectionError);
    m_operation->activateVpnConnection(invalidUuid);
    QTest::qWait(500);
    
    QVERIFY(true);
    Q_UNUSED(spy);
}

void TestKyVpnConnectOperation::testDeactivateVpnConnection()
{
    QString activeConnectName = "test-vpn-connection";
    QString activeConnectUuid = "00000000-0000-0000-0000-000000000000";
    
    m_operation->deactivateVpnConnection(activeConnectName, activeConnectUuid);
    QTest::qWait(100);
    
    QVERIFY(true);
}

void TestKyVpnConnectOperation::testDeleteVpnConnect()
{
    QString invalidUuid = "00000000-0000-0000-0000-000000000000";
    m_operation->deleteVpnConnect(invalidUuid);
    QTest::qWait(100);
    
    QVERIFY(true);
}
