/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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
#ifndef COMMON_H
#define COMMON_H

#include <QString>

namespace KYLIN_NM
{
    enum KySecuType {
        NONE = 0,
        WPA_AND_WPA2_PERSONAL,
        WPA_AND_WPA2_ENTERPRISE,
        WPA3_PERSONAL,
        WPA_AND_WPA3,
    };

    enum KyKeyMgmt {
        Unknown = -1,
        Wep, Ieee8021x,
        WpaNone,
        WpaPsk,
        WpaEap,
        SAE
    };

    enum KyNetworkDevType {
        KyWIRED = 0,
        KyWIRELESS
    };


    //需要和底层返回值对于
    enum KyNetworkCurremtActiveWiFiCategory {
        KyCategoryWiFiNone = 0,
        KyCategoryWiFi6,
        KyCategoryWiFi6Plus,
        KyCategoryWiFi7,
        KyCategoryWiFiNum
    };



    #define GSETTING_SCHEMA_UKCC                "org.ukui.control-center.plugins"
    #define GSETTING_PATH_UKCC_MOBILEHOTSPOT    "/org/ukui/control-center/plugins/mobilehotspot/"
    #define GSETTING_PATH_UKCC_NETCONNECT       "/org/ukui/control-center/plugins/netconnect/"
    #define GSETTING_PATH_UKCC_WLANCONNECT      "/org/ukui/control-center/plugins/wlanconnect/"



    const QString KWifiSymbolic     = "network-wireless-signal-excellent-symbolic";
    const QString KWifiLockSymbolic = "network-wireless-signal-excellent-secure-symbolic";
    const QString KWifiGood         = "network-wireless-signal-good-symbolic";
    const QString KWifiLockGood     = "network-wireless-signal-good-secure-symbolic";
    const QString KWifiOK           = "network-wireless-signal-ok-symbolic";
    const QString KWifiLockOK       = "network-wireless-signal-ok-secure-symbolic";
    const QString KWifiLow          = "network-wireless-signal-weak-symbolic";
    const QString KWifiLockLow      = "network-wireless-signal-weak-secure-symbolic";
    const QString KWifiNone         = "network-wireless-signal-none-symbolic";
    const QString KWifiLockNone     = "network-wireless-signal-none-secure-symbolic";

    const QString KWifi6Symbolic        = "ukui-wifi6-full-symbolic";
    const QString KWifi6PlusSymbolic    = "ukui-wifi6+-full-symbolic";
    const QString KWifi7Symbolic        = "ukui-wifi7-full-symbolic";

    const QString KWifi6LockSymbolic    = "ukui-wifi6-full-pwd-symbolic";
    const QString KWifi6PlusLockSymbolic= "ukui-wifi6+-full-pwd-symbolic";
    const QString KWifi7LockSymbolic    = "ukui-wifi7-full-pwd-symbolic";

    const QString KWifi6Good            = "ukui-wifi6-high-symbolic";
    const QString KWifi6PlusGood        = "ukui-wifi6+-high-symbolic";
    const QString KWifi7Good            = "ukui-wifi7-high-symbolic";

    const QString KWifi6LockGood        = "ukui-wifi6-high-pwd-symbolic";
    const QString KWifi6PlusLockGood    = "ukui-wifi6+-high-pwd-symbolic";
    const QString KWifi7LockGood        = "ukui-wifi7-high-pwd-symbolic";

    const QString KWifi6OK              = "ukui-wifi6-medium-symbolic";
    const QString KWifi6PlusOK          = "ukui-wifi6+-high-medium-symbolic";
    const QString KWifi7OK              = "ukui-wifi7-medium-symbolic";

    const QString KWifi6LockOK          = "ukui-wifi6-medium-pwd-symbolic";
    const QString KWifi6PlusLockOK      = "ukui-wifi6+-medium-pwd-symbolic";
    const QString KWifi7LockOK          = "ukui-wifi7-medium-pwd-symbolic";

    const QString KWifi6Low             = "ukui-wifi6-low-symbolic";
    const QString KWifi6PlusLow         = "ukui-wifi6+-low-symbolic";
    const QString KWifi7Low             = "ukui-wifi7-low-symbolic";

    const QString KWifi6LockLow         = "ukui-wifi6-low-pwd-symbolic";
    const QString KWifi6PlusLockLow     = "ukui-wifi6+-low-pwd-symbolic";
    const QString KWifi7LockLow         = "ukui-wifi7-low-pwd-symbolic";

    const QString KWifi6None            = "ukui-wifi6-none-symbolic";
    const QString KWifi6PlusNone        = "ukui-wifi6+-none-symbolic";
    const QString KWifi7None            = "ukui-wifi7-none-symbolic";

    const QString KWifi6LockNone        = "ukui-wifi6-none-pwd-symbolic";
    const QString KWifi6PlusLockNone    = "ukui-wifi6+-none-pwd-symbolic";
    const QString KWifi7LockNone        = "ukui-wifi7-none-pwd-symbolic";

}

using namespace KYLIN_NM;
#endif // COMMON_H
