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

#include "test_kylinconnectsetting.h"
#include "../../../../../src/backend/dbus-interface/kylinconnectsetting.h"
#include <QTest>
#include <QDebug>
#include <QHostAddress>

TestKyConnectSetting::TestKyConnectSetting()
{
}

TestKyConnectSetting::~TestKyConnectSetting()
{
}

void TestKyConnectSetting::initTestCase()
{
    m_setting = new KyConnectSetting();
}

void TestKyConnectSetting::cleanupTestCase()
{
    if (m_setting) {
        delete m_setting;
        m_setting = nullptr;
    }
}

void TestKyConnectSetting::testConstructor()
{
    // Test that constructor initializes default values
    QVERIFY(m_setting != nullptr);
    QVERIFY(m_setting->m_connectName == "");
    QVERIFY(m_setting->m_ifaceName == "");
    QVERIFY(m_setting->m_ipv4ConfigIpType == CONFIG_IP_DHCP);
    QVERIFY(m_setting->m_ipv6ConfigIpType == CONFIG_IP_DHCP);
    QVERIFY(m_setting->m_ipv4Address.isEmpty());
    QVERIFY(m_setting->m_ipv4Dns.isEmpty());
    QVERIFY(m_setting->m_ipv6Address.isEmpty());
    QVERIFY(m_setting->m_ipv6Dns.isEmpty());
    QVERIFY(m_setting->m_isAutoConnect == true);
}

void TestKyConnectSetting::testSetIfaceName()
{
    QString ifaceName = "eth0";
    m_setting->setIfaceName(ifaceName);
    QVERIFY(m_setting->m_ifaceName == "eth0");

    // Test with empty string
    QString emptyIface = "";
    m_setting->setIfaceName(emptyIface);
    QVERIFY(m_setting->m_ifaceName == "");
}

void TestKyConnectSetting::testSetConnectName()
{
    QString connectName = "Wired connection 1";
    m_setting->setConnectName(connectName);
    QVERIFY(m_setting->m_connectName == "Wired connection 1");

    // Test with empty string
    QString emptyConnect = "";
    m_setting->setConnectName(emptyConnect);
    QVERIFY(m_setting->m_connectName == "");
}

void TestKyConnectSetting::testSetIpConfigType()
{
    // Test valid IPv4 DHCP configuration
    int result = m_setting->setIpConfigType(IPADDRESS_V4, CONFIG_IP_DHCP);
    QVERIFY(result == 0);
    QVERIFY(m_setting->m_ipv4ConfigIpType == CONFIG_IP_DHCP);

    // Test valid IPv4 Manual configuration
    result = m_setting->setIpConfigType(IPADDRESS_V4, CONFIG_IP_MANUAL);
    QVERIFY(result == 0);
    QVERIFY(m_setting->m_ipv4ConfigIpType == CONFIG_IP_MANUAL);

    // Test valid IPv6 DHCP configuration
    result = m_setting->setIpConfigType(IPADDRESS_V6, CONFIG_IP_DHCP);
    QVERIFY(result == 0);
    QVERIFY(m_setting->m_ipv6ConfigIpType == CONFIG_IP_DHCP);

    // Test valid IPv6 Manual configuration
    result = m_setting->setIpConfigType(IPADDRESS_V6, CONFIG_IP_MANUAL);
    QVERIFY(result == 0);
    QVERIFY(m_setting->m_ipv6ConfigIpType == CONFIG_IP_MANUAL);

    // Test invalid IP address type (not V4 or V6)
    result = m_setting->setIpConfigType(KyIpAddressType(99), CONFIG_IP_DHCP);
    QVERIFY(result == -EINVAL);

    // Test invalid config type (not DHCP or MANUAL)
    result = m_setting->setIpConfigType(IPADDRESS_V4, KyIpConfigType(99));
    QVERIFY(result == -EINVAL);
}

void TestKyConnectSetting::testIpv4DnsConstruct()
{
    QList<QHostAddress> dnsList;
    dnsList.append(QHostAddress("8.8.8.8"));
    dnsList.append(QHostAddress("114.114.114.114"));

    m_setting->ipv4DnsConstruct(dnsList);

    QVERIFY(m_setting->m_ipv4Dns.size() == 2);
    QVERIFY(m_setting->m_ipv4Dns.at(0) == QHostAddress("8.8.8.8"));
    QVERIFY(m_setting->m_ipv4Dns.at(1) == QHostAddress("114.114.114.114"));

    // Test with empty list
    QList<QHostAddress> emptyList;
    m_setting->ipv4DnsConstruct(emptyList);
    QVERIFY(m_setting->m_ipv4Dns.isEmpty());
}

void TestKyConnectSetting::testIpv4AddressConstruct()
{
    QString ip = "192.168.1.100";
    QString netmask = "255.255.255.0";
    QString gateway = "192.168.1.1";

    m_setting->ipv4AddressConstruct(ip, netmask, gateway);

    QVERIFY(m_setting->m_ipv4Address.size() == 1);
    QVERIFY(m_setting->m_ipv4Address.at(0).ip() == QHostAddress("192.168.1.100"));
    QVERIFY(m_setting->m_ipv4Address.at(0).gateway() == QHostAddress("192.168.1.1"));
    QVERIFY(m_setting->m_ipv4Address.at(0).netmask() == QHostAddress("255.255.255.0"));

    // Test replacing existing address
    QString ip2 = "10.0.0.50";
    QString netmask2 = "255.0.0.0";
    QString gateway2 = "10.0.0.1";

    m_setting->ipv4AddressConstruct(ip2, netmask2, gateway2);

    QVERIFY(m_setting->m_ipv4Address.size() == 1);  // Should replace, not append
    QVERIFY(m_setting->m_ipv4Address.at(0).ip() == QHostAddress("10.0.0.50"));
}

void TestKyConnectSetting::testIpv6DnsConstruct()
{
    QList<QHostAddress> dnsList;
    dnsList.append(QHostAddress("2001:4860:4860::8888"));
    dnsList.append(QHostAddress("2001:4860:4860::8844"));

    m_setting->ipv6DnsConstruct(dnsList);

    QVERIFY(m_setting->m_ipv6Dns.size() == 2);
    QVERIFY(m_setting->m_ipv6Dns.at(0) == QHostAddress("2001:4860:4860::8888"));
    QVERIFY(m_setting->m_ipv6Dns.at(1) == QHostAddress("2001:4860:4860::8844"));

    // Test with empty list
    QList<QHostAddress> emptyList;
    m_setting->ipv6DnsConstruct(emptyList);
    QVERIFY(m_setting->m_ipv6Dns.isEmpty());
}

void TestKyConnectSetting::testIpv6AddressConstruct()
{
    QString ip = "2001:db8::1";
    QString netmask = "64";
    QString gateway = "2001:db8::ffff";

    m_setting->ipv6AddressConstruct(ip, netmask, gateway);

    QVERIFY(m_setting->m_ipv6Address.size() == 1);
    QVERIFY(m_setting->m_ipv6Address.at(0).ip() == QHostAddress("2001:db8::1"));
    QVERIFY(m_setting->m_ipv6Address.at(0).gateway() == QHostAddress("2001:db8::ffff"));
    QVERIFY(m_setting->m_ipv6Address.at(0).prefixLength() == 64);

    // Test replacing existing address
    QString ip2 = "fe80::1";
    QString netmask2 = "128";
    QString gateway2 = "fe80::ffff";

    m_setting->ipv6AddressConstruct(ip2, netmask2, gateway2);

    QVERIFY(m_setting->m_ipv6Address.size() == 1);  // Should replace, not append
    QVERIFY(m_setting->m_ipv6Address.at(0).ip() == QHostAddress("fe80::1"));
    QVERIFY(m_setting->m_ipv6Address.at(0).prefixLength() == 128);
}

void TestKyConnectSetting::testDumpInfo()
{
    // Test that dumpInfo does not crash and runs without error
    // Just call dumpInfo and verify it does not crash
    m_setting->dumpInfo();

    // Set some values and dump again
    QString ifaceName = "eth0";
    QString connectName = "Test Connection";
    m_setting->setIfaceName(ifaceName);
    m_setting->setConnectName(connectName);

    // Just verify dumpInfo runs without crashing
    m_setting->dumpInfo();
    
    // Test completes if we reach here without crash
    QVERIFY(true);
}

void TestKyConnectSetting::testFullWorkflow()
{
    // Test a complete workflow of setting up a connection
    QString ifaceName = "eth0";
    QString connectName = "Test Connection";

    m_setting->setIfaceName(ifaceName);
    m_setting->setConnectName(connectName);

    QVERIFY(m_setting->m_ifaceName == "eth0");
    QVERIFY(m_setting->m_connectName == "Test Connection");

    // Set IPv4 to manual
    int result = m_setting->setIpConfigType(IPADDRESS_V4, CONFIG_IP_MANUAL);
    QVERIFY(result == 0);
    QVERIFY(m_setting->m_ipv4ConfigIpType == CONFIG_IP_MANUAL);

    // Set IPv6 to DHCP
    result = m_setting->setIpConfigType(IPADDRESS_V6, CONFIG_IP_DHCP);
    QVERIFY(result == 0);
    QVERIFY(m_setting->m_ipv6ConfigIpType == CONFIG_IP_DHCP);

    // Set IPv4 address
    QString ip = "192.168.1.100";
    QString netmask = "255.255.255.0";
    QString gateway = "192.168.1.1";
    m_setting->ipv4AddressConstruct(ip, netmask, gateway);

    // Set IPv4 DNS
    QList<QHostAddress> dnsList;
    dnsList.append(QHostAddress("8.8.8.8"));
    dnsList.append(QHostAddress("114.114.114.114"));
    m_setting->ipv4DnsConstruct(dnsList);

    // Set IPv6 address
    QString ip6 = "2001:db8::1";
    QString netmask6 = "64";
    QString gateway6 = "2001:db8::ffff";
    m_setting->ipv6AddressConstruct(ip6, netmask6, gateway6);

    // Set IPv6 DNS
    QList<QHostAddress> dnsList6;
    dnsList6.append(QHostAddress("2001:4860:4860::8888"));
    m_setting->ipv6DnsConstruct(dnsList6);

    // Verify all values
    QVERIFY(m_setting->m_ipv4Address.size() == 1);
    QVERIFY(m_setting->m_ipv4Dns.size() == 2);
    QVERIFY(m_setting->m_ipv6Address.size() == 1);
    QVERIFY(m_setting->m_ipv6Dns.size() == 1);
}
