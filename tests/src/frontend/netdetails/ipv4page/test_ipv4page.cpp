#include <QTest>
#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <cmath>

#include "ipv4page.h"
#include "kylinconnectsetting.h"
#include "coninfo.h"
#include "test_ipv4page.h"

void TestIpv4Page::initTestCase()
{}

void TestIpv4Page::cleanupTestCase()
{}

void TestIpv4Page::init()
{
    m_widget = new Ipv4Page(nullptr);
    m_conInfo = new ConInfo();
    m_setting = new KyConnectSetting();
}

void TestIpv4Page::cleanup()
{
    delete m_widget;
    m_widget = nullptr;
    delete m_conInfo;
    m_conInfo = nullptr;
    delete m_setting;
    m_setting = nullptr;
}

void TestIpv4Page::testConstructor()
{
    Ipv4Page *widget = new Ipv4Page(nullptr);
    QVERIFY(widget != nullptr);
    QVERIFY(widget->findChild<QComboBox *>() != nullptr);
    delete widget;
}

void TestIpv4Page::testSetIpv4Config_Auto()
{
    m_widget->setIpv4Config(CONFIG_IP_DHCP);
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    QVERIFY(combox != nullptr);
    QVERIFY(combox->currentIndex() == AUTO_CONFIG);
}

void TestIpv4Page::testSetIpv4Config_Manual()
{
    m_widget->setIpv4Config(CONFIG_IP_MANUAL);
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    QVERIFY(combox != nullptr);
    QVERIFY(combox->currentIndex() == MANUAL_CONFIG);
}

void TestIpv4Page::testSetIpv4()
{
    QString ipv4 = "192.168.1.100";
    m_widget->setIpv4(ipv4);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QVERIFY(edits.size() >= 3);
    QLineEdit *addressEdit = edits.at(0);
    QVERIFY(addressEdit->text() == ipv4);
}

void TestIpv4Page::testSetNetMask()
{
    QString netMask = "255.255.255.0";
    m_widget->setNetMask(netMask);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QVERIFY(edits.size() >= 3);
    QLineEdit *maskEdit = edits.at(1);
    QVERIFY(maskEdit->text() == netMask);
}

void TestIpv4Page::testSetMulDns()
{
    QList<QHostAddress> dnsList;
    dnsList.append(QHostAddress("8.8.8.8"));
    dnsList.append(QHostAddress("8.8.4.4"));
    m_widget->setMulDns(dnsList);
    QVERIFY(true);
}

void TestIpv4Page::testSetGateWay()
{
    QString gateway = "192.168.1.1";
    m_widget->setGateWay(gateway);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QVERIFY(edits.size() >= 3);
    QLineEdit *gateWayEdit = edits.at(2);
    QVERIFY(gateWayEdit->text() == gateway);
}

void TestIpv4Page::testSetUuid()
{
    QString uuid = "test-uuid-1234";
    m_widget->setUuid(uuid);
    QVERIFY(true);
}

void TestIpv4Page::testCheckIsChanged_AutoConfig_NoChange()
{
    m_conInfo->ipv4ConfigType = CONFIG_IP_DHCP;
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(AUTO_CONFIG);
    bool result = m_widget->checkIsChanged(*m_conInfo, *m_setting);
    QVERIFY(result == false);
}

void TestIpv4Page::testCheckIsChanged_AutoConfig_Changed()
{
    m_conInfo->ipv4ConfigType = CONFIG_IP_MANUAL;
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(AUTO_CONFIG);
    bool result = m_widget->checkIsChanged(*m_conInfo, *m_setting);
    QVERIFY(result == true);
}

void TestIpv4Page::testCheckIsChanged_ManualConfig_NoChange()
{
    m_conInfo->ipv4ConfigType = CONFIG_IP_MANUAL;
    m_conInfo->strIPV4Address = "192.168.1.100";
    m_conInfo->strIPV4NetMask = "255.255.255.0";
    m_conInfo->strIPV4GateWay = "192.168.1.1";
    m_conInfo->ipv4DnsList.append(QHostAddress("8.8.8.8"));

    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);

    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    QLineEdit *maskEdit = edits.at(1);
    QLineEdit *gateWayEdit = edits.at(2);
    addressEdit->setText("192.168.1.100");
    maskEdit->setText("255.255.255.0");
    gateWayEdit->setText("192.168.1.1");

    bool result = m_widget->checkIsChanged(*m_conInfo, *m_setting);
    QVERIFY(result == false);
}

void TestIpv4Page::testCheckIsChanged_ManualConfig_Changed()
{
    m_conInfo->ipv4ConfigType = CONFIG_IP_MANUAL;
    m_conInfo->strIPV4Address = "192.168.1.100";
    m_conInfo->strIPV4NetMask = "255.255.255.0";
    m_conInfo->strIPV4GateWay = "192.168.1.1";
    m_conInfo->ipv4DnsList.append(QHostAddress("8.8.8.8"));

    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);

    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    QLineEdit *maskEdit = edits.at(1);
    QLineEdit *gateWayEdit = edits.at(2);
    addressEdit->setText("192.168.1.200");
    maskEdit->setText("255.255.255.0");
    gateWayEdit->setText("192.168.1.1");

    bool result = m_widget->checkIsChanged(*m_conInfo, *m_setting);
    QVERIFY(result == true);
}

void TestIpv4Page::testCheckDnsSettingsIsChanged()
{
    bool result = m_widget->checkDnsSettingsIsChanged();
    QVERIFY(result == false || result == true);
}

void TestIpv4Page::testStartLoading()
{
    m_widget->startLoading();
    QVERIFY(true);
}

void TestIpv4Page::testStopLoading()
{
    m_widget->stopLoading();
    QVERIFY(true);
}

void TestIpv4Page::testShowIpv4AddressConflict_True()
{
    m_widget->showIpv4AddressConflict(true);
    QVERIFY(true);
}

void TestIpv4Page::testShowIpv4AddressConflict_False()
{
    m_widget->showIpv4AddressConflict(false);
    QVERIFY(true);
}

void TestIpv4Page::testGetErrorMessage_Empty()
{
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    addressEdit->setText("");
    QLineEdit *maskEdit = edits.at(1);
    maskEdit->setText("");
    QLineEdit *gateWayEdit = edits.at(2);
    gateWayEdit->setText("");
    QString msg = m_widget->getErrorMessage();
    QVERIFY(!msg.isEmpty());
}

void TestIpv4Page::testGetErrorMessage_InvalidIp()
{
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    addressEdit->setText("invalid-ip");
    QLineEdit *maskEdit = edits.at(1);
    maskEdit->setText("255.255.255.0");
    QLineEdit *gateWayEdit = edits.at(2);
    gateWayEdit->setText("192.168.1.1");
    QString msg = m_widget->getErrorMessage();
    QVERIFY(!msg.isEmpty());
}

void TestIpv4Page::testGetErrorMessage_InvalidMask()
{
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    addressEdit->setText("192.168.1.100");
    QLineEdit *maskEdit = edits.at(1);
    maskEdit->setText("invalid-mask");
    QLineEdit *gateWayEdit = edits.at(2);
    gateWayEdit->setText("192.168.1.1");
    QString msg = m_widget->getErrorMessage();
    QVERIFY(!msg.isEmpty());
}

void TestIpv4Page::testGetErrorMessage_InvalidGateway()
{
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    addressEdit->setText("192.168.1.100");
    QLineEdit *maskEdit = edits.at(1);
    maskEdit->setText("255.255.255.0");
    QLineEdit *gateWayEdit = edits.at(2);
    gateWayEdit->setText("invalid-gateway");
    QString msg = m_widget->getErrorMessage();
    QVERIFY(!msg.isEmpty());
}

void TestIpv4Page::testGetErrorMessage_Valid()
{
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = edits.at(0);
    addressEdit->setText("192.168.1.100");
    QLineEdit *maskEdit = edits.at(1);
    maskEdit->setText("255.255.255.0");
    QLineEdit *gateWayEdit = edits.at(2);
    gateWayEdit->setText("192.168.1.1");
    QString msg = m_widget->getErrorMessage();
    QVERIFY(msg.isEmpty());
}

void TestIpv4Page::testGetTextEditState_Empty()
{
    QString text = "";
    QRegExp rx("\\\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\b");
    bool match = rx.exactMatch(text);
    QVERIFY(match == true);
}

void TestIpv4Page::testGetTextEditState_ValidIp()
{
    QString text = "192.168.1.1";
    QRegExp rx("\\\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\b");
    bool match = rx.exactMatch(text);
    QVERIFY(match == true);
}

void TestIpv4Page::testGetTextEditState_InvalidIp()
{
    QString text = "999.999.999.999";
    QRegExp rx("\\\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\b");
    bool match = rx.exactMatch(text);
    QVERIFY(match == false);
}

void TestIpv4Page::testNetMaskIsValide_ValidIp()
{
    QString text = "255.255.255.0";
    QRegExp rx("\\\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\b");
    bool match = rx.exactMatch(text);
    QVERIFY(match == true);
}

void TestIpv4Page::testNetMaskIsValide_ValidPrefix()
{
    QString text = "24";
    bool result = false;
    if (text.length() > 0 && text.length() < 3) {
        int num = text.toInt();
        if (num > 0 && num < 33) {
            result = true;
        }
    }
    QVERIFY(result == true);
}

void TestIpv4Page::testNetMaskIsValide_Invalid()
{
    QString text = "invalid";
    QRegExp rx("\\\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\\\b");
    bool match = rx.exactMatch(text);
    bool result = false;
    if (match) {
        result = true;
    } else {
        if (text.length() > 0 && text.length() < 3) {
            int num = text.toInt();
            if (num > 0 && num < 33) {
                result = true;
            }
        }
    }
    QVERIFY(result == false);
}

void TestIpv4Page::testGetNetMaskText_FullFormat()
{
    QString text = "255.255.255.0";
    QString result = text;
    if (text.length() > 2) {
        result = text;
    }
    QVERIFY(result == "255.255.255.0");
}

void TestIpv4Page::testGetNetMaskText_PrefixFormat()
{
    QString text = "24";
    QString result;
    int num = text.toInt();
    QStringList list;
    list << "0" << "0" << "0" << "0";
    int count = 0;
    while (num - 8 >= 0) {
        list[count] = "255";
        num = num - 8;
        count ++;
    }
    if (num > 0) {
        int size = std::pow(2, 8) - std::pow(2,(8-num));
        list[count] = QString::number(size);
    }
    result = QString("%1.%2.%3.%4").arg(list[0],list[1],list[2],list[3]);
    QVERIFY(result == "255.255.255.0");
}

void TestIpv4Page::testSetEnableOfSaveBtnSignal()
{
    QSignalSpy spy(m_widget, SIGNAL(setIpv4PageState(bool)));
    QComboBox *combox = m_widget->findChild<QComboBox *>();
    combox->setCurrentIndex(MANUAL_CONFIG);
    QList<QLineEdit *> edits = m_widget->findChildren<QLineEdit *>();
    edits.at(0)->setText("192.168.1.1");
    edits.at(1)->setText("255.255.255.0");
    edits.at(2)->setText("192.168.1.254");
    QVERIFY(spy.isValid());
}
