#ifndef TEST_DLGHOTSPOTCREATE_H
#define TEST_DLGHOTSPOTCREATE_H

#include <QObject>
#include <QString>
#include <QtTest/QtTest>
#include <QSignalSpy>

class TestDlgHotspotCreate : public QObject
{
    Q_OBJECT
public:
    explicit TestDlgHotspotCreate(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testDestructor();
    void testChangeDialog();
};

#endif // TEST_DLGHOTSPOTCREATE_H
