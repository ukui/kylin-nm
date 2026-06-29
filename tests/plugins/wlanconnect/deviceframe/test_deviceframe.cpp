#include "test_deviceframe.h"
#include "../../../../plugins/wlanconnect/deviceframe.h"
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
    m_deviceFrame = new DeviceFrame("wlan0", nullptr);
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
    DeviceFrame *frame = new DeviceFrame("wlan0", nullptr);
    QVERIFY(frame != nullptr);
    QVERIFY(frame->deviceLabel != nullptr);
    QVERIFY(frame->dropDownLabel != nullptr);
    QVERIFY(frame->layout() != nullptr);
    delete frame;
}

void TestDeviceFrame::test_constructor_withParent()
{
    QWidget parentWidget;
    DeviceFrame *frame = new DeviceFrame("wlan1", &parentWidget);
    QVERIFY(frame != nullptr);
    QVERIFY(frame->parent() == &parentWidget);
    delete frame;
}

void TestDeviceFrame::test_constructor_fixedHeight()
{
    DeviceFrame frame("wlan2", nullptr);
    QVERIFY(frame.height() == 58);
}

void TestDeviceFrame::test_deviceLabelExists()
{
    DeviceFrame frame("wlan3", nullptr);
    QVERIFY(frame.deviceLabel != nullptr);
    QVERIFY(frame.deviceLabel->text().isEmpty());
}

void TestDeviceFrame::test_dropDownLabelExists()
{
    DeviceFrame frame("wlan4", nullptr);
    QVERIFY(frame.dropDownLabel != nullptr);
}

void TestDeviceFrame::test_layoutExists()
{
    DeviceFrame frame("wlan5", nullptr);
    QHBoxLayout *layout = qobject_cast<QHBoxLayout *>(frame.layout());
    QVERIFY(layout != nullptr);
    QVERIFY(layout->count() == 3);
}

void TestDeviceFrame::test_frameShape()
{
    DeviceFrame frame("wlan6", nullptr);
    QVERIFY(frame.frameShape() == QFrame::Box);
}
