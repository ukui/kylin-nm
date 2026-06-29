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

#ifndef TEST_KYVPNCONNECTOPERATION_H
#define TEST_KYVPNCONNECTOPERATION_H

#include <QObject>
#include <QString>
#include "../../../../../../src/backend/dbus-interface/kyvpnconnectoperation.h"

class TestKyVpnConnectOperation : public QObject
{
    Q_OBJECT
public:
    TestKyVpnConnectOperation();
    ~TestKyVpnConnectOperation();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testConstructor();
    void testCreateVpnConnect();
    void testSetVpnConfig();
    void testGetVpnConfig();
    void testActivateVpnConnection();
    void testDeactivateVpnConnection();
    void testDeleteVpnConnect();

private:
    KyVpnConnectOperation *m_operation;
};

#endif // TEST_KYVPNCONNECTOPERATION_H
