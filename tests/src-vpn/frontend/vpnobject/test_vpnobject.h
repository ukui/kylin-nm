#ifndef TEST_VPNOBJECT_H
#define TEST_VPNOBJECT_H

#include <QObject>
#include <QTest>
#include <QVector>
#include <QStringList>
#include <QMainWindow>
#include "vpnobject.h"

class TestVpnObject : public QObject
{
    Q_OBJECT
public:
    explicit TestVpnObject(QObject *parent = nullptr);

private Q_SLOTS:
    void testConstructor();
    void testDestructor();
    void testGetVirtualList();
    void testDeleteVpn();
    void testActivateVpn();
    void testDeactivateVpn();
    void testShowDetailPage();
    void testShowVpnAddWidget();
    void testOnShowMainWindow();

private:
    vpnObject *m_vpnObject;
};

#endif // TEST_VPNOBJECT_H
