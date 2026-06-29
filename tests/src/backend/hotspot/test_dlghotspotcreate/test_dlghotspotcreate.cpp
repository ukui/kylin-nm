#include "test_dlghotspotcreate.h"
#include "../../../../../src/backend/hotspot/dlghotspotcreate.h"
#include <QCoreApplication>
#include <QDebug>

TestDlgHotspotCreate::TestDlgHotspotCreate(QObject *parent)
    : QObject(parent)
{}

void TestDlgHotspotCreate::initTestCase()
{
}

void TestDlgHotspotCreate::cleanupTestCase()
{
}

void TestDlgHotspotCreate::init()
{
}

void TestDlgHotspotCreate::cleanup()
{
}

void TestDlgHotspotCreate::testConstructor()
{
    DlgHotspotCreate *dlg = new DlgHotspotCreate("wlan0");
    QVERIFY(dlg != nullptr);
    delete dlg;
}

void TestDlgHotspotCreate::testDestructor()
{
    DlgHotspotCreate *dlg = new DlgHotspotCreate("wlan0");
    delete dlg;
    QVERIFY(true);
}

void TestDlgHotspotCreate::testChangeDialog()
{
    DlgHotspotCreate dlg("wlan0");
    dlg.changeDialog();
    QVERIFY(true);
}
