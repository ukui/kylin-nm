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

#ifndef TEST_PROXYAPPHANDLER_H
#define TEST_PROXYAPPHANDLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QDir>
#include <QTemporaryDir>
#include <QSignalSpy>

class TestProxyAppHandler : public QObject
{
    Q_OBJECT

public:
    explicit TestProxyAppHandler(QObject *parent = nullptr);
    ~TestProxyAppHandler();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test DealData constructor
    void testDealDataConstructor();

    // Test ProcAddServer constructor
    void testProcAddServerConstructor();

    // Test ListenThObject constructor
    void testListenThObjectConstructor();

    // Test DealData::run method
    void testDealDataRun_data();
    void testDealDataRun();

private:
    QTemporaryDir *m_tempDir;
};

#endif // TEST_PROXYAPPHANDLER_H
