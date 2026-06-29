#ifndef TEST_KYWIRELESSNETITEM_H
#define TEST_KYWIRELESSNETITEM_H

#include <QObject>
#include <QString>
#include "kywirelessnetitem.h"

class TestKyWirelessNetItem : public QObject
{
    Q_OBJECT
public:
    explicit TestKyWirelessNetItem(QObject *parent = nullptr);

private Q_SLOTS:
    void testSetKySecuType_Enterprise();
    void testSetKySecuType_WPA3Personal();
    void testSetKySecuType_WPA3WithWPA1();
    void testSetKySecuType_WPA3WithWPA2();
    void testSetKySecuType_WPA1();
    void testSetKySecuType_WPA2();
    void testSetKySecuType_WPA1WPA2();
    void testSetKySecuType_None();
    void testSetKySecuType_WEP();
    void testDefaultConstructor();
    void testGetters();
    void testGetCategory_InvalidUni();
    void testGetCategory_EmptyUni();
};

#endif // TEST_KYWIRELESSNETITEM_H
