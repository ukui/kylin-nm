#include "test_kylinagent.h"

void TestKylinAgent::SetUp()
{
    m_agent = nullptr;
}

void TestKylinAgent::TearDown()
{
    if (m_agent) {
        delete m_agent;
        m_agent = nullptr;
    }
}

TEST_F(TestKylinAgent, testConstructor)
{
    m_agent = new KylinAgent();
    ASSERT_NE(m_agent, nullptr);
    EXPECT_NE(m_agent, nullptr);
}

TEST_F(TestKylinAgent, testConstructorWithParent)
{
    m_agent = new KylinAgent(nullptr);
    ASSERT_NE(m_agent, nullptr);
}

TEST_F(TestKylinAgent, testKylinSecretAgentGetter)
{
    static int argc = 0;
    static char* argv[] = {(char*)""};
    QCoreApplication app(argc, argv);
    m_agent = new KylinAgent();
    KylinSecretAgent *secretAgent = m_agent->kylinSecretAgent();
    ASSERT_NE(secretAgent, nullptr);
}

TEST_F(TestKylinAgent, testStopKylinAgent)
{
    static int argc = 0;
    static char* argv[] = {(char*)""};
    QCoreApplication app(argc, argv);
    m_agent = new KylinAgent();
    m_agent->stopKylinAgent();
    KylinSecretAgent *secretAgent = m_agent->kylinSecretAgent();
    EXPECT_EQ(secretAgent, nullptr);
}

TEST_F(TestKylinAgent, testDestructor)
{
    {
        KylinAgent *agent = new KylinAgent();
        delete agent;
        agent = nullptr;
    }
    SUCCEED() << "Destructor completed without crash";
}
