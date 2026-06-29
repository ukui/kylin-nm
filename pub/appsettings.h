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
#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QStringList>
#include <QSettings>

// 测试模式标志（用于测试用例）
extern bool g_testMode;


class AppSettings : public QObject
{
    Q_OBJECT
public:
    // 单例模式（全局唯一，避免重复创建 QSettings）
    static AppSettings& instance();


    // ===================== 默认有线、无线 ===================
    QString defaultWired() const;
    void setDefaultWired(QString devName);

    QString defaultWireless() const;
    void setDefaultWireless(QString devName);

    // ===================== 基础配置 =====================
    bool autoFirewallPermitted() const;
    void setAutoFirewallPermitted(bool enable);

    bool firewallPermissionShow() const;
    void setFirewallPermissionShow(bool enable);

    bool trayShow() const;
    void setTrayShow(bool enable);

    // ===================== USB 隐藏列表 =====================
    void appendHiddenUsbPairs(const QStringList &pairs);
    QStringList getHiddenUsbPairs() const;
    void clearHiddenUsbPairs();

private:
    explicit AppSettings(QObject *parent = nullptr);
    ~AppSettings();

    // 禁止拷贝和赋值
    AppSettings(const AppSettings&) = delete;
    AppSettings& operator=(const AppSettings&) = delete;

    // 初始化默认配置
    void initDefaultConfig();

    QSettings *m_settings;
};
#endif // APPSETTINGS_H
