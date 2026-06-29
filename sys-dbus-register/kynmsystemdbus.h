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

#ifndef KYNMSYSTEMDBUS_H
#define KYNMSYSTEMDBUS_H

#include <QObject>
#include <QSettings>

#include <QDBusContext>
#include <QDBusMessage>

#include <QDBusMetaType>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/Device>

class KynmSystemDbus : public QObject,protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylin.network.interface")

public:
    explicit KynmSystemDbus(QObject *parent = nullptr);
    ~KynmSystemDbus();

public Q_SLOTS:
    Q_SCRIPTABLE bool checkIpv4IsConflict(const QString devName, const QString ipv4Address, QStringList macList);
    Q_SCRIPTABLE bool checkIpv6IsConflict(const QString devName, const QString ipv6Address);

    Q_SCRIPTABLE void setWiredMainSwitch(bool enable);
    Q_SCRIPTABLE bool getWiredMainSwitch();
    Q_SCRIPTABLE void setWiredDeviceSwitch(QString devName, bool enable);
    Q_SCRIPTABLE bool getWiredDeviceSwitch(QString devName);
    Q_SCRIPTABLE bool setOptionsEnhance(const QString&, const QString&, const QString&, const QString&);
    Q_SCRIPTABLE QVariantMap getExtraDnsEnhance(const QString&); 
    Q_SCRIPTABLE bool writeNmConfig(const QString& filePath, const QString& feature, const QString &key, const QString& value);
    Q_SCRIPTABLE QVariantMap getNmConfig(const QString& filePath, const QString& feature);

    //开关网卡会被调用
    Q_SCRIPTABLE void setDeviceSwitch(const QString& devName, bool checked);

Q_SIGNALS:
    Q_SCRIPTABLE void sysWiredMainSwitchChanged(bool enbale);
    Q_SCRIPTABLE void sysWiredDevSwitchChanged(QString devName, bool enbale);

    //网卡打开触发的信号，调用槽函数尝试重连
    Q_SCRIPTABLE void sysDeviceSwitchChanged(const QString &devName);
private:
    QSettings *m_kylinNmSettings;

    void initConf();

    // 关闭有线开关保存后断开有线设备
    void disconnectWiredDevices();
};

#endif // KYNMSYSTEMDBUS_H
