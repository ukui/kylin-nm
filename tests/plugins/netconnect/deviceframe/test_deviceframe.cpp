#include "test_deviceframe.h"
#include "../../../../plugins/netconnect/deviceframe.h"
#include <QSignalSpy>
#include <QDebug>

TestDeviceFrame::TestDeviceFrame(QObject *parent)
    : QObject(parent)
    , m_deviceFrame(nullptr)
{
}

TestDeviceFrame::~TestDeviceFrame()
{
}

void TestDeviceFrame::initTestCase()
{
    m_deviceFrame = new DeviceFrame("eth0", nullptr);
}

void TestDeviceFrame::cleanupTestCase()
{
    if (m_deviceFrame) {
        delete m_deviceFrame;
        m_deviceFrame = nullptr;
    }
}

void TestDeviceFrame::init()
{
}

void TestDeviceFrame::cleanup()
{
}

void TestDeviceFrame::test_constructor()
{
    DeviceFrame *frame = new DeviceFrame("eth0", nullptr);
    QVERIFY(frame != nullptr);
    QVERIFY(frame->deviceLabel != nullptr);
    QVERIFY(frame->dropDownLabel != nullptr);
    QVERIFY(frame->deviceSwitch != nullptr);
    QVERIFY(frame->layout() != nullptr);
    delete frame;
}

void TestDeviceFrame::test_constructor_withParent()
{
    QWidget parentWidget;
    DeviceFrame *frame = new DeviceFrame("wlan0", &parentWidget);
    QVERIFY(frame != nullptr);
    QVERIFY(frame->parent() == &parentWidget);
    delete frame;
}

void TestDeviceFrame::test_constructor_fixedHeight()
{
    DeviceFrame frame("eth0", nullptr);
    QVERIFY(frame.height() == 58);
}

void TestDeviceFrame::test_deviceSwitchConnections()
{
    DeviceFrame frame("eth0", nullptr);
    
    QSignalSpy spy(&frame, &DeviceFrame::deviceSwitchClicked);
    QVERIFY(spy.isValid());
    
    frame.deviceSwitch->setChecked(true);
    QVERIFY(spy.count() == 0);
    
    frame.deviceSwitch->setChecked(false);
    QVERIFY(spy.count() == 0);
}

void TestDeviceFrame::test_deviceLabelExists()
{
    DeviceFrame frame("eth0", nullptr);
    QVERIFY(frame.deviceLabel != nullptr);
    QVERIFY(frame.deviceLabel->text().isEmpty());
}

void TestDeviceFrame::test_dropDownLabelExists()
{
    DeviceFrame frame("wlan0", nullptr);
    QVERIFY(frame.dropDownLabel != nullptr);
}

void TestDeviceFrame::test_deviceSwitchExists()
{
    DeviceFrame frame("eth0", nullptr);
    QVERIFY(frame.deviceSwitch != nullptr);
    QVERIFY(frame.deviceSwitch->isCheckable());
}

void TestDeviceFrame::test_layoutExists()
{
    DeviceFrame frame("eth0", nullptr);
    QHBoxLayout *layout = qobject_cast<QHBoxLayout *>(frame.layout());
    QVERIFY(layout != nullptr);
    QVERIFY(layout->count() == 4);
}

void TestDeviceFrame::test_frameShape()
{
    DeviceFrame frame("eth0", nullptr);
    QVERIFY(frame.frameShape() == QFrame::Box);
}

void TestDeviceFrame::test_setFrameStyle_data()
{
    QTest::addColumn<bool>("isChecked");

    QTest::newRow("Switch unchecked") << false;
    QTest::newRow("Switch checked") << true;
}

void TestDeviceFrame::test_setFrameStyle()
{
    QFETCH(bool, isChecked);
    
    DeviceFrame frame("eth0", nullptr);
    frame.deviceSwitch->setChecked(isChecked);
    
    QVERIFY(frame.deviceSwitch != nullptr);
    QVERIFY(frame.deviceSwitch->isChecked() == isChecked);
}
