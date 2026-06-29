/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#ifndef TEST_KYLINCONNECTSETTING_H
#define TEST_KYLINCONNECTSETTING_H

#include <QObject>
#include <QString>

class KyConnectSetting;

class TestKyConnectSetting : public QObject
{
    Q_OBJECT
public:
    explicit TestKyConnectSetting();
    ~TestKyConnectSetting();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testConstructor();
    void testSetIfaceName();
    void testSetConnectName();
    void testSetIpConfigType();
    void testIpv4DnsConstruct();
    void testIpv4AddressConstruct();
    void testIpv6DnsConstruct();
    void testIpv6AddressConstruct();
    void testDumpInfo();
    void testFullWorkflow();

private:
    KyConnectSetting* m_setting;
};

#endif // TEST_KYLINCONNECTSETTING_H
