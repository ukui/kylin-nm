#ifndef TEST_VPNDBUSADAPTOR_H
#define TEST_VPNDBUSADAPTOR_H

#include <QObject>
#include <QTest>
#include <QVector>
#include <QStringList>
#include "vpndbusadaptor.h"
#include "vpnobject.h"

class TestVpnDbusAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit TestVpnDbusAdaptor(QObject *parent = nullptr);

private Q_SLOTS:
    void testConstructor();
    void testGetVirtualList();
    void testDeleteVpn();
    void testActivateVpn();
    void testDeactivateVpn();
    void testShowKylinVpn();
    void testShowVpnAddWidget();
    void testShowDetailPage();

private:
    vpnObject *m_parent;
    VpnDbusAdaptor *m_adaptor;
};

#endif // TEST_VPNDBUSADAPTOR_H
