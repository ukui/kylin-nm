#ifndef TEST_WLANLISTITEM_H
#define TEST_WLANLISTITEM_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include "wlanlistitem.h"

// Forward declaration of the getIcon function from wlanlistitem.cpp
QString getIcon(bool isEncrypted, int signalStrength, int category);

class TestWlanListItem : public QObject
{
    Q_OBJECT
public:
    explicit TestWlanListItem(QObject *parent = nullptr);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testConstructorWithAllParams();
    void testConstructorWithDeviceAndWifi6Plus();
    void testConstructorDefault();
    void testGetSsid();
    void testGetUuid();
    void testGetPath();
    void testSetSignalStrength();
    void testGetSignalStrength();
    void testIsConfigured();
    void testSetWlanState();
    void testSetExpanded();
    void testSetExpandedWithNullPointers();
    void testGetConnectionState();
    void testUpdateConnectStateActivated();
    void testUpdateConnectStateDeactivated();
    void testUpdateConnectStateActivating();
    void testUpdateWirelessNetSecurity();
    void testUpdateWirelessNetItem();
    void testForgetPwd();
    void testGetAutoConnect();
    void testGetIconFunction();

private:
    WlanListItem *m_wlanListItem = nullptr;
};

#endif // TEST_WLANLISTITEM_H
