#include "test_kywirelessnetitem.h"
#include <QDebug>
#include <QtTest>

// Test KyWirelessNetItem::setKySecuType
void TestKyWirelessNetItem::testSetKySecuType_Enterprise()
{
    KyWirelessNetItem item;
    QString secuType = "802.1X";
    item.setKySecuType(secuType);
    QVERIFY(item.m_kySecuType == WPA_AND_WPA2_ENTERPRISE);
}

void TestKyWirelessNetItem::testSetKySecuType_WPA3Personal()
{
    KyWirelessNetItem item;
    QString secuType = "WPA3";
    item.setKySecuType(secuType);
    QVERIFY(item.m_kySecuType == WPA3_PERSONAL);
}

void TestKyWirelessNetItem::testSetKySecuType_WPA3WithWPA1()
{
    KyWirelessNetItem item;
    QString secuType = "WPA3 WPA1";
    item.setKySecuType(secuType);
    QVERIFY(item.m_kySecuType == WPA_AND_WPA3);
}

void TestKyWirelessNetItem::testSetKySecuType_WPA3WithWPA2()
{
    KyWirelessNetItem item;
    QString secuType = "WPA3 WPA2";
    item.setKySecuType(secuType);
    QVERIFY(item.m_kySecuType == WPA_AND_WPA3);
}

void TestKyWirelessNetItem::testSetKySecuType_WPA1()
{
    KyWirelessNetItem item;
    QString secuType = "WPA1";
    item.setKySecuType(secuType);
    QVERIFY(item.m_kySecuType == WPA_AND_WPA2_PERSONAL);
}

void TestKyWirelessNetItem::testSetKySecuType_WPA2()
{
    KyWirelessNetItem item;
    QString secuType = "WPA2";
    item.setKySecuType(secuType);
    QVERIFY(item.m_kySecuType == WPA_AND_WPA2_PERSONAL);
}

void TestKyWirelessNetItem::testSetKySecuType_WPA1WPA2()
{
    KyWirelessNetItem item;
    QString secuType = "WPA1 WPA2";
    item.setKySecuType(secuType);
    QVERIFY(item.m_kySecuType == WPA_AND_WPA2_PERSONAL);
}

void TestKyWirelessNetItem::testSetKySecuType_None()
{
    KyWirelessNetItem item;
    QString secuType = "";
    item.setKySecuType(secuType);
    QVERIFY(item.m_kySecuType == KYLIN_NM::NONE);
}

void TestKyWirelessNetItem::testSetKySecuType_WEP()
{
    KyWirelessNetItem item;
    QString secuType = "WEP";
    item.setKySecuType(secuType);
    QVERIFY(item.m_kySecuType == KYLIN_NM::NONE);
}

// Test KyWirelessNetItem default constructor
void TestKyWirelessNetItem::testDefaultConstructor()
{
    KyWirelessNetItem item;
    QVERIFY(item.m_NetSsid.isEmpty());
    QVERIFY(item.m_connectUuid.isEmpty());
    QVERIFY(item.m_bssid.isEmpty());
    QVERIFY(item.m_signalStrength == 0);
    QVERIFY(item.m_frequency == 0);
    QVERIFY(item.m_secuType.isEmpty());
    QVERIFY(item.m_kySecuType == KYLIN_NM::NONE);
    QVERIFY(item.m_uni.isEmpty());
    QVERIFY(item.m_isMix == false);
    QVERIFY(item.m_isConfigured == false);
    QVERIFY(item.m_connName.isEmpty());
    QVERIFY(item.m_connDbusPath.isEmpty());
    QVERIFY(item.m_channel == 0);
    QVERIFY(item.m_autoconnect == true);
}

// Test getter functions
void TestKyWirelessNetItem::testGetters()
{
    KyWirelessNetItem item;
    // Just verify getters work without crashing
    QString device = item.getDevice();
    QString deviceName = item.getDeviceName();
    QVERIFY(device.isEmpty());
    QVERIFY(deviceName.isEmpty());
}

// Test getCategory function with invalid uni
void TestKyWirelessNetItem::testGetCategory_InvalidUni()
{
    KyWirelessNetItem item;
    // Invalid D-Bus path should return -1
    int result = item.getCategory("/org/freedesktop/NetworkManager/InvalidPath");
    QVERIFY(result == -1);
}

// Test getCategory function with empty uni
void TestKyWirelessNetItem::testGetCategory_EmptyUni()
{
    KyWirelessNetItem item;
    // Empty D-Bus path should return -1
    int result = item.getCategory("");
    QVERIFY(result == -1);
}
