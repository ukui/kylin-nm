/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 *  This program is free software: you can redistribute it and/or modify
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "kynmsystemdbus.h"
#include "kylinipv4arping.h"
#include "kylinipv6arping.h"
#include "authority-base.h"

#define LOG_FLAG "[KynmSystemDbus]"

#define KYLIN_NM_WIRED_MAIN_SWITCH "wired_main_switch"
#define EXTRA_DNS_CONF_FILE "/etc/nm_enhance/"
#define NM_ENHANCE_DNS "-nm_enhance_dns.conf"
#define STR_ATTEMPTS "attempts"
#define STR_TIMEOUT "timeout"
#define STR_TYPE "type"

KynmSystemDbus::KynmSystemDbus(QObject *parent) : QObject(parent)
{
    QString switchSettingFile = "/etc/kylin-nm/netSwitch.conf";
    m_kylinNmSettings = new QSettings(switchSettingFile, QSettings::IniFormat);
    QStringList groups = m_kylinNmSettings->childGroups();
    if (groups.indexOf("conf") == -1)
    {
        qDebug() << Q_FUNC_INFO << __LINE__;
        m_kylinNmSettings->beginGroup("conf");
        init_conf();
    }
    else
    {
        m_kylinNmSettings->beginGroup("conf");
        init_conf();
    }
    qDBusRegisterMetaType<QMap<QString, QString>>();
}

KynmSystemDbus::~KynmSystemDbus()
{

}


void KynmSystemDbus::init_conf()
{
    if (!m_kylinNmSettings->contains(KYLIN_NM_WIRED_MAIN_SWITCH))
        m_kylinNmSettings->setValue(KYLIN_NM_WIRED_MAIN_SWITCH, true);
}

bool KynmSystemDbus::checkIpv4IsConflict(const QString devName, const QString ipv4Address, QStringList macList)
{
#if 0
    //这里会让Dbus异常退出，先注释
    PEEK_LIMIT_RET(false);
#endif
    bool isConflict = false;
    KyIpv4Arping* ipv4Arping = new KyIpv4Arping(devName, ipv4Address);
    if (ipv4Arping->ipv4ConflictCheck() >= 0) {
        isConflict =  ipv4Arping->ipv4IsConflict();
        if (isConflict && !macList.isEmpty()) {
            QString macAddress = ipv4Arping->getMacAddress();
            for (const auto &mac : macList) {
                if (macAddress == mac) {
                    qDebug() << LOG_FLAG << "IPv4 conflict mac" << mac;
                    isConflict = false;
                    break;
                }
            }
        }
    } else {
        qWarning() << LOG_FLAG << "checkIpv4Conflict internal error";
    }

    delete ipv4Arping;
    ipv4Arping = nullptr;

    qDebug() << "checkIpv4IsConflict : devName = " <<  devName << "ip = " << ipv4Address << "result = " << isConflict;
    return isConflict;
}

bool KynmSystemDbus::checkIpv6IsConflict(const QString devName, const QString ipv6Address)
{
    PEEK_LIMIT_RET(false);
    bool isConflict = false;
    KyIpv6Arping* ipv6rping = new KyIpv6Arping(devName, ipv6Address);
    if (ipv6rping->ipv6ConflictCheck() >= 0) {
        isConflict =  ipv6rping->ipv6IsConflict();
    } else {
        qWarning() << LOG_FLAG << "checkIpv6Conflict internal error";
    }

    delete ipv6rping;
    ipv6rping = nullptr;

    return isConflict;
}

void KynmSystemDbus::setWiredMainSwitch(bool enable)
{
    qDebug() << Q_FUNC_INFO << __LINE__ << enable;

    message().service();
#if 0
    //这里会让Dbus异常退出，先注释
    PEEK_LIMIT();
#endif
    if (enable != getWiredMainSwitch())
    {
        m_kylinNmSettings->setValue(KYLIN_NM_WIRED_MAIN_SWITCH, enable);
        Q_EMIT sysWiredMainSwitchChanged(enable);
    }
}

bool KynmSystemDbus::getWiredMainSwitch()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

#if 0
    //这里会让Dbus异常退出，先注释
    PEEK_LIMIT_RET(false);
#endif
    return m_kylinNmSettings->value(KYLIN_NM_WIRED_MAIN_SWITCH).toBool();
}
void KynmSystemDbus::setWiredDeviceSwitch(QString devName, bool enable)
{
    qDebug() << Q_FUNC_INFO << __LINE__<< devName << enable;
#if 0
    //这里会让Dbus异常退出，先注释
    PEEK_LIMIT();
#endif

    QString conf_name = devName+"-switch";
    if (m_kylinNmSettings->contains(conf_name) && enable != getWiredDeviceSwitch(conf_name))
    {
        m_kylinNmSettings->setValue(conf_name, enable);
        Q_EMIT sysWiredDevSwitchChanged(conf_name, enable);
    }
}
bool KynmSystemDbus::getWiredDeviceSwitch(QString devName)
{
    qDebug() << Q_FUNC_INFO << __LINE__<< devName;
#if 0
    //这里会让Dbus异常退出，先注释
    PEEK_LIMIT_RET(false);
#endif

    QString conf_name = devName+"-switch";
    if (m_kylinNmSettings->contains(conf_name))
        return m_kylinNmSettings->value(conf_name).toBool();
    else
        return true;//111
}

bool KynmSystemDbus::setOptionsEnhance(const QString& name, const QString& timeout, const QString& attempts, const QString& type)
{
    QString filePath = EXTRA_DNS_CONF_FILE + name + NM_ENHANCE_DNS;
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite))
    {
        return false;
    }

    QSettings settings(filePath, QSettings::IniFormat);
    settings.setValue("/options/timeout", timeout);
    settings.setValue("/options/attempts", attempts);
    settings.setValue("/options/type", type);
    settings.sync();

    file.close();
    return true;
}

QVariantMap KynmSystemDbus::getExtraDnsEnhance(const QString& name)
{
    QString filePath = EXTRA_DNS_CONF_FILE + name + NM_ENHANCE_DNS;
    QSettings settings(filePath, QSettings::IniFormat);

    QVariantMap map;
    map[STR_ATTEMPTS] = settings.value("/options/attempts").toString();
    map[STR_TIMEOUT] = settings.value("/options/timeout").toString();
    map[STR_TYPE] = settings.value("/options/type").toString();

    return map;
}

/*
 * 【较为通用的写配置文件接口】:
 *
 *  filePath：文件路径
 *  feature：节点名
 *  key:键
 *  value:值
 */
bool KynmSystemDbus::writeNmConfig(const QString& filePath, const QString& feature, const QString &key, const QString& value)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite)){
        return false;
    }

    QSettings settings(filePath, QSettings::IniFormat);
    QString jointKey = "/" + feature + "/" + key;
    settings.setValue(jointKey, value);

    settings.sync();
    file.close();
    return true;
}

/*
 * 【较为通用的读配置文件接口】:
 *
 *  filePath：文件路径
 *  feature：节点名
 */
QMap<QString, QString> KynmSystemDbus::getNmConfig(const QString& filePath, const QString& feature)
{
    QSettings settings(filePath, QSettings::IniFormat);
    QMap<QString, QString> configMap;

    settings.beginGroup(feature);

    QStringList keys = settings.childKeys();
    for (const QString &key : keys) {
        QString value = settings.value(key).toString();
        configMap.insert(key, value);
    }
    settings.endGroup();
    
    return configMap;
}


void KynmSystemDbus::setDeviceSwitch(const QString& devName, bool checked)
{
    if (checked && !devName.isEmpty())
    {
        Q_EMIT sysDeviceSwitchChanged(devName);
    }
}