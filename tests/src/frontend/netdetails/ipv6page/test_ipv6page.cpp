#include <QTest>
#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>

#include "ipv6page.h"
#include "kylinconnectsetting.h"
#include "coninfo.h"
#include "test_ipv6page.h"

void TestIpv6Page::initTestCase()
{
}

void TestIpv6Page::cleanupTestCase()
{
}

void TestIpv6Page::init()
{
    m_widget = new Ipv6Page(nullptr);
    m_conInfo = new ConInfo();
    m_setting = new KyConnectSetting();
}

void TestIpv6Page::cleanup()
{
    delete m_widget;
    m_widget = nullptr;
    delete m_conInfo;
    m_conInfo = nullptr;
    delete m_setting;
    m_setting = nullptr;
}

void TestIpv6Page::testConstructor()
{
    Ipv6Page *widget = new Ipv6Page(nullptr);
    QVERIFY(widget != nullptr);
    QVERIFY(widget->findChild<QComboBox *>() != nullptr);
    delete widget;
}

void TestIpv6Page::testSetIpv6Config_Auto()
{
    m_widget->setIpv6Config(CONFIG_IP_DHCP);
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    QVERIFY(combox != nullptr);
    QVERIFY(combox->currentIndex() == AUTO_CONFIG);
}

void TestIpv6Page::testSetIpv6Config_Manual()
{
    m_widget->setIpv6Config(CONFIG_IP_MANUAL);
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    QVERIFY(combox != nullptr);
    QVERIFY(combox->currentIndex() == MANUAL_CONFIG);
}

void TestIpv6Page::testSetIpv6()
{
    QString ipv6 = "2001:db8::1";
    m_widget->setIpv6(ipv6);
    QLineEdit *addressEdit = m_widget->findChild<QLineEdit *>();
    QVERIFY(addressEdit != nullptr);
    QVERIFY(addressEdit->text() == ipv6);
}

void TestIpv6Page::testSetIpv6Perfix()
{
    int prefix = 64;
    m_widget->setIpv6Perfix(prefix);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QVERIFY(edits.size() >= 2);
    QLineEdit *lengthEdit = edits.at(1);
    QVERIFY(lengthEdit->text() == QString::number(prefix));
}

void TestIpv6Page::testSetMulDns()
{
    QList<QHostAddress> dnsList;
    dnsList.append(QHostAddress("2001:4860:4860::8888"));
    dnsList.append(QHostAddress("2001:4860:4860::8844"));
    m_widget->setMulDns(dnsList);
    QVERIFY(true);
}

void TestIpv6Page::testSetGateWay()
{
    QString gateway = "2001:db8::ffff";
    m_widget->setGateWay(gateway);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QVERIFY(edits.size() >= 3);
    QLineEdit *gateWayEdit = edits.at(2);
    QVERIFY(gateWayEdit->text() == gateway);
}

void TestIpv6Page::testCheckIsChanged_AutoConfig_NoChange()
{
    m_conInfo->ipv6ConfigType = CONFIG_IP_DHCP;
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(AUTO_CONFIG);
    bool result = m_widget->checkIsChanged(*m_conInfo, *m_setting);
    QVERIFY(result == false);
}

void TestIpv6Page::testCheckIsChanged_AutoConfig_Changed()
{
    m_conInfo->ipv6ConfigType = CONFIG_IP_MANUAL;
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(AUTO_CONFIG);
    bool result = m_widget->checkIsChanged(*m_conInfo, *m_setting);
    QVERIFY(result == true);
}

void TestIpv6Page::testCheckIsChanged_ManualConfig_NoChange()
{
    m_conInfo->ipv6ConfigType = CONFIG_IP_MANUAL;
    m_conInfo->strIPV6Address = "2001:db8::1";
    m_conInfo->iIPV6Prefix = 64;
    m_conInfo->strIPV6GateWay = "2001:db8::ffff";
    m_conInfo->ipv6DnsList.append(QHostAddress("2001:4860:4860::8888"));

    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);

    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    QLineEdit *lengthEdit = edits.at(1);
    QLineEdit *gateWayEdit = edits.at(2);
    addressEdit->setText("2001:db8::1");
    lengthEdit->setText("64");
    gateWayEdit->setText("2001:db8::ffff");

    bool result = m_widget->checkIsChanged(*m_conInfo, *m_setting);
    QVERIFY(result == false);
}

void TestIpv6Page::testCheckIsChanged_ManualConfig_Changed()
{
    m_conInfo->ipv6ConfigType = CONFIG_IP_MANUAL;
    m_conInfo->strIPV6Address = "2001:db8::1";
    m_conInfo->iIPV6Prefix = 64;
    m_conInfo->strIPV6GateWay = "2001:db8::ffff";
    m_conInfo->ipv6DnsList.append(QHostAddress("2001:4860:4860::8888"));

    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);

    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    QLineEdit *lengthEdit = edits.at(1);
    QLineEdit *gateWayEdit = edits.at(2);
    addressEdit->setText("2001:db8::2");
    lengthEdit->setText("64");
    gateWayEdit->setText("2001:db8::ffff");

    bool result = m_widget->checkIsChanged(*m_conInfo, *m_setting);
    QVERIFY(result == true);
}

void TestIpv6Page::testGetPerfixLength_Empty()
{
    QString text = "";
    int length = m_widget->getPerfixLength(text);
    QVERIFY(length == 0);
}

void TestIpv6Page::testGetPerfixLength_Valid()
{
    QString text = "2001:db8::";
    int length = m_widget->getPerfixLength(text);
    QVERIFY(length >= 0);
}

void TestIpv6Page::testStartLoading()
{
    m_widget->startLoading();
    QVERIFY(true);
}

void TestIpv6Page::testStopLoading()
{
    m_widget->stopLoading();
    QVERIFY(true);
}

void TestIpv6Page::testShowIpv6AddressConflict_True()
{
    m_widget->showIpv6AddressConflict(true);
    QVERIFY(true);
}

void TestIpv6Page::testShowIpv6AddressConflict_False()
{
    m_widget->showIpv6AddressConflict(false);
    QVERIFY(true);
}

void TestIpv6Page::testGetErrorMessage_Empty()
{
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);

    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    addressEdit->setText("");

    QString msg = m_widget->getErrorMessage();
    QVERIFY(!msg.isEmpty());
}

void TestIpv6Page::testGetErrorMessage_Invalid()
{
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);

    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    addressEdit->setText("invalid-ip");

    QString msg = m_widget->getErrorMessage();
    QVERIFY(!msg.isEmpty());
}

void TestIpv6Page::testGetErrorMessage_Valid()
{
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);

    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    QLineEdit *lengthEdit = edits.at(1);
    addressEdit->setText("2001:db8::1");
    lengthEdit->setText("64");

    QString msg = m_widget->getErrorMessage();
    QVERIFY(msg.isEmpty());
}
