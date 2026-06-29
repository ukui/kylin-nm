#ifndef TEST_DROWNLABEL_H
#define TEST_DROWNLABEL_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QLabel>
#include <QWidget>

class TestDrownLabel : public QObject
{
    Q_OBJECT
public:
    explicit TestDrownLabel(QObject *parent = nullptr);
    ~TestDrownLabel();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_setDropDownStatus();
    void test_mouseReleaseEvent();
    void test_memberVariables();
};

#endif // TEST_DROWNLABEL_H
