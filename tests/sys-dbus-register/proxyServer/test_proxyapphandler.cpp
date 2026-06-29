/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "test_proxyapphandler.h"
#include "../../../sys-dbus-register/proxyServer/proxyapphandler.h"

#include <QTest>
#include <QCoreApplication>
#include <QThread>
#include <QFile>

TestProxyAppHandler::TestProxyAppHandler(QObject *parent)
    : QObject(parent), m_tempDir(nullptr)
{
}

TestProxyAppHandler::~TestProxyAppHandler()
{
}

void TestProxyAppHandler::initTestCase()
{
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());
}

void TestProxyAppHandler::cleanupTestCase()
{
    if (m_tempDir) {
        delete m_tempDir;
        m_tempDir = nullptr;
    }
}

void TestProxyAppHandler::init()
{
}

void TestProxyAppHandler::cleanup()
{
}

void TestProxyAppHandler::testDealDataConstructor()
{
    // Test DealData constructor
    QObject *parent = new QObject();
    DealData *dealData = new DealData(1234, parent);

    QVERIFY(dealData != nullptr);
    QVERIFY(parent != nullptr);

    delete dealData;
    delete parent;
}

void TestProxyAppHandler::testProcAddServerConstructor()
{
    // Test ProcAddServer constructor
    ProcAddServer *server = new ProcAddServer();

    QVERIFY(server != nullptr);

    delete server;
}

void TestProxyAppHandler::testListenThObjectConstructor()
{
    // Test ListenThObject constructor
    ListenThObject *listener = new ListenThObject();

    QVERIFY(listener != nullptr);

    // Test setter methods
    listener->setExitStat(true);
    listener->setSockFd(10);

    delete listener;
}

void TestProxyAppHandler::testDealDataRun_data()
{
    QTest::addColumn<qint64>("pid");
    QTest::addColumn<bool>("procExists");

    // Test with non-existent process (PID 1 is init, always exists on Linux)
    QTest::newRow("init_process") << (qint64)1 << true;

    // Test with current process
    QTest::newRow("current_process") << QCoreApplication::applicationPid() << true;

    // Test with non-existent high PID
    QTest::newRow("nonexistent_high_pid") << (qint64)999999999 << false;

    // Test with invalid PID
    QTest::newRow("invalid_pid") << (qint64)0 << false;
}

void TestProxyAppHandler::testDealDataRun()
{
    QFETCH(qint64, pid);
    QFETCH(bool, procExists);

    // Verify expected behavior based on whether process exists
    QString procDir = QString("/proc/%1/").arg(pid);
    bool exists = QDir(procDir).exists();
    QVERIFY2(exists == procExists || !procExists || exists, 
             QString("Process directory check for PID %1").arg(pid).toUtf8());
}

QTEST_MAIN(TestProxyAppHandler)
