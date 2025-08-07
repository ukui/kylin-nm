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
    PEEK_LIMIT();
    if (enable != getWiredMainSwitch())
    {
        m_kylinNmSettings->setValue(KYLIN_NM_WIRED_MAIN_SWITCH, enable);
        Q_EMIT sysWiredMainSwitchChanged(enable);
    }
}

bool KynmSystemDbus::getWiredMainSwitch()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;

    PEEK_LIMIT_RET(false);
    return m_kylinNmSettings->value(KYLIN_NM_WIRED_MAIN_SWITCH).toBool();
}
void KynmSystemDbus::setWiredDeviceSwitch(QString devName, bool enable)
{
    qDebug() << Q_FUNC_INFO << __LINE__<< devName << enable;

    PEEK_LIMIT();
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
    PEEK_LIMIT_RET(false);
    QString conf_name = devName+"-switch";
    if (m_kylinNmSettings->contains(conf_name))
        return m_kylinNmSettings->value(conf_name).toBool();
    else
        return true;//111
}

