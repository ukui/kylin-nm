#include "test_dbus_interface.h"

TestComKylinNetworkInterface::TestComKylinNetworkInterface()
    : QObject(nullptr)
    , m_service("com.kylin.network")
    , m_path("/com/kylin/network")
    , m_connection(QDBusConnection::sessionBus())
    , m_interface(nullptr)
{
}

TestComKylinNetworkInterface::~TestComKylinNetworkInterface()
{
}

void TestComKylinNetworkInterface::SetUp()
{
    m_service = "com.kylin.network";
    m_path = "/com/kylin/network";
    m_connection = QDBusConnection::sessionBus();
    m_interface = nullptr;
}

void TestComKylinNetworkInterface::TearDown()
{
    if (m_interface) {
        delete m_interface;
        m_interface = nullptr;
    }
}

TEST_F(TestComKylinNetworkInterface, testConstructorWithValidParams)
{
    m_interface = new ComKylinNetworkInterface(m_service, m_path, m_connection, nullptr);
    ASSERT_NE(m_interface, nullptr);
    EXPECT_STREQ(m_interface->staticInterfaceName(), "com.kylin.network");
}

TEST_F(TestComKylinNetworkInterface, testDestructor)
{
    m_interface = new ComKylinNetworkInterface(m_service, m_path, m_connection, nullptr);
    ASSERT_NE(m_interface, nullptr);
    delete m_interface;
    m_interface = nullptr;
    SUCCEED() << "Destructor completed without crash";
}

TEST_F(TestComKylinNetworkInterface, testStaticInterfaceName)
{
    const char* interfaceName = ComKylinNetworkInterface::staticInterfaceName();
    EXPECT_STREQ(interfaceName, "com.kylin.network");
}

TEST_F(TestComKylinNetworkInterface, testInterfaceNameMethod)
{
    m_interface = new ComKylinNetworkInterface(m_service, m_path, m_connection, nullptr);
    ASSERT_NE(m_interface, nullptr);
    QString interfaceName = m_interface->interface();
    EXPECT_EQ(interfaceName, QLatin1String("com.kylin.network"));
}
