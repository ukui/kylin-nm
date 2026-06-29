#ifndef TEST_MAIN_H
#define TEST_MAIN_H

#include <QObject>
#include <QTest>
#include <QCoreApplication>

class TestMain : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testQCoreApplication();
    void testDBusConnection();
};

#endif // TEST_MAIN_H
