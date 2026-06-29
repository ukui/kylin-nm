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
#include "appsettings.h"

#include <QDir>
#include <QSet>
#include <QDebug>
#include <QRegularExpression>
//#include <QStandardPaths>  // 必须加这个头文件

// 测试模式标志（用于测试用例）
bool g_testMode = true;

// ===================== 配置宏定义（可改成你项目的） =====================
#define CONFIG_FILE_DIR        QString(QDir::homePath() + "/.config/ukui/")
#define CONFIG_FILE_PATH       CONFIG_FILE_DIR+"kylin-nm.conf"

#define CONF_GROUP_DEFAULTCARD      "DEFAULTCARD"
#define CONF_GROUP_CONF             "conf"
#define CONF_GROUP_USB_HIDDEN       "hidden_usb_vidpid"

#define KEY_AUTO_PERMIT        "autoFireWallPermitted"
#define KEY_PERMISSION_SHOW    "firewallPermissionShow"
#define KEY_TRAY_SHOW          "trayShowSwitch"
#define KEY_NET_WIRED          "wired"
#define KEY_NET_WIRELESS       "wireless"

// ===================== 单例实现 =====================
AppSettings &AppSettings::instance()
{
    static AppSettings s_instance;
    return s_instance;
}

AppSettings::AppSettings(QObject *parent)
    : QObject(parent)
{
    // 确保目录存在
    QDir dir(CONFIG_FILE_DIR);
    if (!dir.exists()) {
        dir.mkpath(CONFIG_FILE_DIR);
    }

    // 创建全局唯一 QSettings
    m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);

    initDefaultConfig();
}

AppSettings::~AppSettings()
{
    m_settings->sync();
    delete m_settings;
}

// ===================== 初始化默认值 =====================
void AppSettings::initDefaultConfig()
{
    // 默认网卡配置
    if (!m_settings->value(CONF_GROUP_DEFAULTCARD "/" KEY_NET_WIRED).isValid()) {
        setDefaultWired("");
    }

    if (!m_settings->value(CONF_GROUP_DEFAULTCARD "/" KEY_NET_WIRELESS).isValid()) {
        setDefaultWireless("");
    }

    // 防火墙配置
    if (!m_settings->value(CONF_GROUP_CONF "/" KEY_AUTO_PERMIT).isValid()) {
        setAutoFirewallPermitted(true);
    }
    if (!m_settings->value(CONF_GROUP_CONF "/" KEY_PERMISSION_SHOW).isValid()) {
        setFirewallPermissionShow(true);
    }

    // 托盘配置
    if (!m_settings->value(CONF_GROUP_CONF "/" KEY_TRAY_SHOW).isValid()) {
        setTrayShow(true);
    }

    // 默认 USB 列表
    QStringList hiddenPairs = {
        "1ff7:0200",
        "1ff7:0f10",
        "1ff7:0f0b",
        "1ff7:0f52",
        "1ff7:0f57",
        "1ff7:0f1c"
    };
    appendHiddenUsbPairs(hiddenPairs);

    m_settings->sync();
}

// ===================== Get / Set =====================
bool AppSettings::autoFirewallPermitted() const
{
    return m_settings->value(CONF_GROUP_CONF "/" KEY_AUTO_PERMIT, true).toBool();
}

void AppSettings::setAutoFirewallPermitted(bool enable)
{
    m_settings->setValue(CONF_GROUP_CONF "/" KEY_AUTO_PERMIT, enable);
    m_settings->sync();
}

bool AppSettings::firewallPermissionShow() const
{
    return m_settings->value(CONF_GROUP_CONF "/" KEY_PERMISSION_SHOW, true).toBool();
}

void AppSettings::setFirewallPermissionShow(bool enable)
{
    m_settings->setValue(CONF_GROUP_CONF "/" KEY_PERMISSION_SHOW, enable);
    m_settings->sync();
}

bool AppSettings::trayShow() const
{
    return m_settings->value(CONF_GROUP_CONF "/" KEY_TRAY_SHOW, true).toBool();
}

void AppSettings::setTrayShow(bool enable)
{
    m_settings->setValue(CONF_GROUP_CONF "/" KEY_TRAY_SHOW, enable);
    m_settings->sync();
}

// ===================== USB 隐藏列表 追加 =====================
void AppSettings::appendHiddenUsbPairs(const QStringList &pairs)
{
    m_settings->beginGroup(CONF_GROUP_USB_HIDDEN);

    QSet<QString> exist;
    const QStringList keys = m_settings->childKeys();
    for (const QString &k : keys) {
        QString v = m_settings->value(k).toString().trimmed().toLower();
        if (!v.isEmpty())
            exist.insert(v);
    }

    int maxIndex = 0;
    QRegularExpression re("^pair(\\d+)$");
    for (const QString &k : keys) {
        QRegularExpressionMatch m = re.match(k);
        if (m.hasMatch()) {
            bool ok;
            int idx = m.captured(1).toInt(&ok);
            if (ok && idx > maxIndex)
                maxIndex = idx;
        }
    }
    int next = maxIndex + 1;

    for (QString p : pairs) {
        p = p.trimmed().toLower();
        if (p.isEmpty())
            continue;

        QStringList vp = p.split(':');
        if (vp.size() != 2)
            continue;

        QString v = vp[0].trimmed();
        QString pr = vp[1].trimmed();
        if (v.isEmpty() || pr.isEmpty())
            continue;

        v = v.rightJustified(4, '0');
        pr = pr.rightJustified(4, '0');
        QString norm = v + ":" + pr;

        if (exist.contains(norm))
            continue;

        QString key;
        do {
            key = QString("pair%1").arg(next++);
        } while (m_settings->contains(key));

        m_settings->setValue(key, norm);
        exist.insert(norm);
    }

    m_settings->endGroup();
    m_settings->sync();
}

// ===================== 读取全部隐藏 USB 列表 =====================
QStringList AppSettings::getHiddenUsbPairs() const
{
    QStringList list;
    m_settings->beginGroup(CONF_GROUP_USB_HIDDEN);
    const QStringList keys = m_settings->childKeys();
    for (const QString &k : keys) {
        QString v = m_settings->value(k).toString();
        if (!v.isEmpty())
            list.append(v);
    }
    m_settings->endGroup();

    qWarning() << Q_FUNC_INFO << __LINE__  << "HiddenUsbPairsList:" << list ;
    return list;
}

// ===================== 清空 USB 列表 =====================
void AppSettings::clearHiddenUsbPairs()
{
    m_settings->remove(CONF_GROUP_USB_HIDDEN);
    m_settings->sync();
}

QString AppSettings::defaultWired() const
{
    return m_settings->value(CONF_GROUP_DEFAULTCARD "/" KEY_NET_WIRED, true).toString();
}

void AppSettings::setDefaultWired(QString devName)
{
    m_settings->setValue(CONF_GROUP_DEFAULTCARD "/" KEY_NET_WIRED, devName);
    m_settings->sync();
}

QString AppSettings::defaultWireless() const
{
    return m_settings->value(CONF_GROUP_DEFAULTCARD "/" KEY_NET_WIRELESS, true).toString();
}

void AppSettings::setDefaultWireless(QString devName)
{
    m_settings->setValue(CONF_GROUP_DEFAULTCARD "/" KEY_NET_WIRELESS, devName);
    m_settings->sync();
}
