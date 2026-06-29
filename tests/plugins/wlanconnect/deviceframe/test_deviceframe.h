#ifndef TEST_DEVICEFRAME_H
#define TEST_DEVICEFRAME_H

#include <QObject>
#include <QTest>
#include <QWidget>
#include <QHBoxLayout>

class DeviceFrame;

class TestDeviceFrame : public QObject
{
    Q_OBJECT
public:
    explicit TestDeviceFrame(QObject *parent = nullptr);
    ~TestDeviceFrame();

private:
    DeviceFrame *m_deviceFrame = nullptr;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_constructor_withParent();
    void test_constructor_fixedHeight();
    void test_deviceLabelExists();
    void test_dropDownLabelExists();
    void test_layoutExists();
    void test_frameShape();
};

#endif // TEST_DEVICEFRAME_H
