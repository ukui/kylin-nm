/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef TEST_APPSETTINGS_H
#define TEST_APPSETTINGS_H

#include <QtTest/QtTest>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTemporaryDir>
#include "../../../pub/appsettings.h"

class TestAppSettings : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 单例测试
    void test_singleton();

    // 默认有线/无线网卡测试
    void test_defaultWired();
    void test_defaultWireless();
    void test_setDefaultWired();
    void test_setDefaultWireless();

    // 防火墙配置测试
    void test_autoFirewallPermitted();
    void test_setAutoFirewallPermitted();
    void test_firewallPermissionShow();
    void test_setFirewallPermissionShow();

    // 托盘配置测试
    void test_trayShow();
    void test_setTrayShow();

    // USB隐藏列表测试
    void test_appendHiddenUsbPairs();
    void test_getHiddenUsbPairs();
    void test_clearHiddenUsbPairs();
    void test_appendHiddenUsbPairs_duplicate();
    void test_appendHiddenUsbPairs_invalidFormat();
    void test_appendHiddenUsbPairs_emptyList();

    // 边界条件测试
    void test_setEmptyDeviceName();
    void test_setSpecialCharacters();
    
    // 扩展覆盖率测试
    void test_productionMode();
    void test_initDefaultConfig_branches();
    void test_usbPairs_edgeCases();

private:
    QString m_testConfigPath;
};

#endif // TEST_APPSETTINGS_H
