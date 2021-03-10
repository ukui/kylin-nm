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

#include "kylin-dbus-interface.h"
#include "mainwindow.h"
#include "utils.h"

#include <vector>

#include <QTextCodec>
#include <QByteArray>


namespace {

void quitThread(QThread *thread)
{
    Q_ASSERT(thread);
    if (thread) {
        thread->quit();
        if (!thread->wait(2000)) {
            thread->terminate();
            thread->wait();
        }
    }
}

} // namespace

KylinDBus::KylinDBus(MainWindow *mainWindow, QObject *parent) :QObject(parent)
{
    this->mw = mainWindow;

    getObjectPath(); //获取dbus中 lan 与 WiFi 的device路径
    getPhysicalCarrierState(0); //初始化获取网线插入状态
    getLanHwAddressState(); //获取有线网Mac地址
    getWiredCardName(); //获取有线网卡名称
    //initTaskbarGsetting(); //初始化taskbar的GSetting方法
    getWifiSwitchState(); //初始化wifi开关GSetting通信方法
    initTransparentState(); //初始化窗口透明度的GSetting方法
    getLanIpChanged();

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager"),
                                         QString("org.freedesktop.NetworkManager"),
                                         QString("PropertiesChanged"), this, SLOT(onPropertiesChanged(QVariantMap) ) );

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager"),
                                         QString("org.freedesktop.NetworkManager"),
                                         QString("DeviceAdded"), mw, SLOT(onNetworkDeviceAdded(QDBusObjectPath) ) );

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager"),
                                         QString("org.freedesktop.NetworkManager"),
                                         QString("DeviceRemoved"), mw, SLOT(onNetworkDeviceRemoved(QDBusObjectPath) ) );

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager/Settings"),
                                         QString("org.freedesktop.NetworkManager.Settings"),
                                         QString("NewConnection"), this, SLOT(onNewConnection(QDBusObjectPath) ) );

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager/Settings"),
                                         QString("org.freedesktop.NetworkManager.Settings"),
                                         QString("ConnectionRemoved"), this, SLOT(onConnectionRemoved(QDBusObjectPath) ) );

    if (multiWiredPaths.size() > 0) {
        foreach (QDBusObjectPath mWiredPath, multiWiredPaths) {
            QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                                 QString(mWiredPath.path()),
                                                 QString("org.freedesktop.NetworkManager.Device.Wired"),
                                                 QString("PropertiesChanged"), this, SLOT(onLanPropertyChanged(QVariantMap) ) );
        }
    } else {
        syslog(LOG_DEBUG, "Can not find wired device object path when using dbus.");
        qDebug()<<"Can not find wired device object path when using dbus.";
    }

    if (wirelessPath.path() != "") {
        getWirelessCardName();//获取无线网卡名称
    } else {
        syslog(LOG_DEBUG, "Can not find wireless device object path when using dbus.");
        qDebug()<<"Can not find wireless device object path when using dbus.";
    }

    time = new QTimer(this);
    time->setTimerType(Qt::PreciseTimer);
    QObject::connect(time, SIGNAL(timeout()), this, SLOT(slot_timeout()));

    if (mw) {
        QObject::connect(this, SIGNAL(updateWiredList(int)), mw, SLOT(onBtnNetListClicked(int)));
        QObject::connect(this, SIGNAL(newConnAdded(int)), mw, SLOT(onNewConnAdded(int)));
        QObject::connect(this, SIGNAL(updateWirelessList()), mw, SLOT(on_wifi_changed()));
    }

    mUtils = new Utils();
    mUtilsThread = new QThread(this);
    mUtils->moveToThread(mUtilsThread);
    connect(mUtilsThread, &QThread::finished, mUtils, &Utils::deleteLater);
    connect(this, SIGNAL(requestSendDesktopNotify(QString)), mUtils, SLOT(onRequestSendDesktopNotify(QString)), Qt::QueuedConnection);
    QTimer::singleShot(1, this, [=] {
        mUtilsThread->start();
    });
}

KylinDBus::~KylinDBus()
{
    quitThread(mUtilsThread);
}


///////////////////////////////////////////////////////////////////////////////
//下方使用Dbus 进程通信方法

//获取dbus中 lan 与 WiFi 的device路径
void KylinDBus::getObjectPath()
{
    foreach (QDBusObjectPath mPath, multiWiredPaths) {
        QDBusConnection::systemBus().disconnect(QString("org.freedesktop.NetworkManager"),
                                             QString(mPath.path()),
                                             QString("org.freedesktop.NetworkManager.Device.Wired"),
                                             QString("PropertiesChanged"), this, SLOT(onLanPropertyChanged(QVariantMap) ) );
        multiWiredPaths.removeOne(mPath);
    }

    QDBusInterface m_interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.NetworkManager",
                              QDBusConnection::systemBus() );

   //先获取所有的网络设备的设备路径
   QDBusReply<QList<QDBusObjectPath>> obj_reply = m_interface.call("GetAllDevices");
   if (!obj_reply.isValid()) {
       syslog(LOG_DEBUG, "execute dbus method 'GetAllDevices' is invalid in func getObjectPath()");
       qDebug()<<"execute dbus method 'GetAllDevices' is invalid in func getObjectPath()";
   }

   QList<QDBusObjectPath> obj_paths = obj_reply.value();

   //再判断有无有线设备和无线设备的路径
   foreach (QDBusObjectPath obj_path, obj_paths) {
       QDBusInterface interface( "org.freedesktop.NetworkManager",
                                 obj_path.path(),
                                 "org.freedesktop.DBus.Introspectable",
                                 QDBusConnection::systemBus() );

       QDBusReply<QString> reply = interface.call("Introspect");
       if (!reply.isValid()) {
           syslog(LOG_DEBUG, "execute dbus method 'Introspect' is invalid in func getObjectPath()");
           qDebug()<<"execute dbus method 'Introspect' is invalid in func getObjectPath()";
       }

       if(reply.value().indexOf("org.freedesktop.NetworkManager.Device.Wired") != -1) {
           //表明有有线网设备
           multiWiredPaths.append(obj_path);
       } else if (reply.value().indexOf("org.freedesktop.NetworkManager.Device.Wireless") != -1) {
           //表明有wifi设备
           wirelessPath = obj_path;
           isWirelessCardOn = true;
       }
   }
}

//获取是否连接有线网网线
void KylinDBus::getPhysicalCarrierState(int n)
{
    foreach (QString mStr, multiWiredCableState) {
        multiWiredCableState.removeOne(mStr);
    }

    foreach (QDBusObjectPath localPath, multiWiredPaths) {
        QDBusInterface interface( "org.freedesktop.NetworkManager",
                                  localPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        //Carrier值为true，插了网线；为false，未插网线
        QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Device.Wired", "Carrier");

        try {
            if (reply.value().toString() == "true") {
                multiWiredCableState.append("true");
            } else if (reply.value().toString() == "false") {
                multiWiredCableState.append("false");
            } else {
                throw -1; //出现异常
            }
        } catch(...) {
            syslog(LOG_ERR, "Error occurred when get the property 'Carrier' of Wired");
            qDebug()<<"Error occurred when get the property 'Carrier' of Wired";
        }
    }

    isWiredCableOn = false;
    foreach (QString state, multiWiredCableState) {
        if (state == "true") {
            isWiredCableOn = true;
        }
    }

    if (n == 1) { this->mw->onPhysicalCarrierChanged(isWiredCableOn);}
}

//获取有线网Mac地址
void KylinDBus::getLanHwAddressState()
{
    foreach (QString mStr, multiWiredMac) {
        multiWiredMac.removeOne(mStr);
    }

    foreach (QDBusObjectPath mPath, multiWiredPaths) {
        QDBusInterface lanInterface( "org.freedesktop.NetworkManager",
                                  mPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> lanReply = lanInterface.call("Get", "org.freedesktop.NetworkManager.Device.Wired", "HwAddress");
        if (!lanReply.isValid()) {
            syslog(LOG_DEBUG, "can not get the attribute 'HwAddress' in func getLanHwAddressState()");
            qDebug()<<"can not get the attribute 'HwAddress' in func getLanHwAddressState()";
        } else {
            QString dbusLanMac = lanReply.value().toString();
            multiWiredMac.append(dbusLanMac);
        }
    }

    if (multiWiredMac.size() > 0) {
        dbusMacDefault = multiWiredMac.at(0);
    } else {
        dbusMacDefault = "--";
    }
}

//获取有线网卡名称
void KylinDBus::getWiredCardName()
{
    foreach (QString mStr, multiWiredIfName) {
        multiWiredIfName.removeOne(mStr);
    }

    foreach (QDBusObjectPath mPath, multiWiredPaths) {
        QDBusInterface lanInterface( "org.freedesktop.NetworkManager",
                                     mPath.path(),
                                     "org.freedesktop.DBus.Properties",
                                     QDBusConnection::systemBus() );

        QDBusReply<QVariant> lanReply = lanInterface.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
        if (!lanReply.isValid()) {
            syslog(LOG_DEBUG, "can not get the attribute 'Interface' in func getWiredCardName()");
            qDebug()<<"can not get the attribute 'Interface' in func getWiredCardName()";
        } else {
            QString dbusLanCardName = lanReply.value().toString();
            multiWiredIfName.append(dbusLanCardName);
        }
    }
}

//获取无线网卡名称
void KylinDBus::getWirelessCardName()
{
    QDBusInterface lanInterface( "org.freedesktop.NetworkManager",
                              wirelessPath.path(),
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusReply<QVariant> lanReply = lanInterface.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
    if (!lanReply.isValid()) {
        syslog(LOG_DEBUG, "can not get the attribute 'Interface' in func getWirelessCardName()");
        qDebug()<<"can not get the attribute 'Interface' in func getWirelessCardName()";
    } else {
        dbusWiFiCardName = lanReply.value().toString();
    }
}

//获取普通的有线网ip、DNS、ifname
void KylinDBus::getLanIpDNS(QString uuidName, bool isActNet)
{
    dbusIfName = "--";
    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QList<QDBusObjectPath> m_objNets = m_reply.value();
    foreach (QDBusObjectPath objNet, m_objNets) {
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objNet.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage result = m_interface.call("GetSettings"); //get information of this network

        const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg1st >> map;

        for (QString outside_key : map.keys() ) {
            QMap<QString,QVariant> outsideMap = map.value(outside_key);
            if (outside_key == "connection") {
                for (QString search_key : outsideMap.keys()) {
                    if (search_key == "uuid") {
                        if (uuidName == outsideMap.value(search_key).toString()) {
                            for (QString search_key : outsideMap.keys()) {
                                if (search_key == "interface-name") {
                                    dbusIfName = outsideMap.value("interface-name").toString();
                                }
                            }

                            for (QString key : map.keys() ) {
                                QMap<QString,QVariant> innerMap = map.value(key);
                                if (key == "ipv4") {
                                    for (QString inner_key : innerMap.keys()) {
                                        //获取ipv4
                                        if (inner_key == "address-data") {
                                            const QDBusArgument &dbusArg2nd = innerMap.value(inner_key).value<QDBusArgument>();
                                            QMap<QString,QVariant> m_map;

                                            dbusArg2nd.beginArray();
                                            while (!dbusArg2nd.atEnd()) {
                                                dbusArg2nd >> m_map;// append map to a vector here if you want to keep it
                                            }
                                            dbusArg2nd.endArray();

                                            dbusLanIpv4 = m_map.value("address").toString();
                                        }

                                        if (isActNet && inner_key == "dns") {
                                            const QDBusArgument &dbusArg2nd = innerMap.value(inner_key).value<QDBusArgument>();
                                            int strDns = 0;
                                            dbusArg2nd.beginArray();
                                            while (!dbusArg2nd.atEnd()) {
                                                dbusArg2nd >> strDns;// append map to a vector here if you want to keep it
                                                if (strDns == 0) {
                                                    //dbusArg2nd为空时，strDns没有从dbusArg2nd获取到结果
                                                    dbusArg2nd.endArray();
                                                }
                                            }
                                            dbusArg2nd.endArray();
                                            dbusActLanDNS = strDns;
                                        }
                                    }
                                }

                                if (key == "ipv6") {
                                    for (QString inner_key : innerMap.keys()) {
                                        if (inner_key == "address-data"){
                                            const QDBusArgument &dbusArg2nd = innerMap.value(inner_key).value<QDBusArgument>();
                                            QMap<QString,QVariant> m_map;

                                            dbusArg2nd.beginArray();
                                            while (!dbusArg2nd.atEnd()) {
                                                dbusArg2nd >> m_map;// append map to a vector here if you want to keep it
                                            }
                                            dbusArg2nd.endArray();

                                            dbusLanIpv6 = m_map.value("address").toString();
                                        } else if (inner_key == "method") {
                                            dbusLanIpv6Method = innerMap.value(inner_key).toString();
                                        }
                                    }
                                }
                            } //end for(QString key : map.keys() )

                        }
                    }
                } //end for (QString search_key : outsideMap.keys())
            }
        } // end for(QString outside_key : map.keys() )

    } //end foreach (QDBusObjectPath objNet, m_objNets)
}

void KylinDBus::getWifiIp(QString uuid)
{
    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QList<QDBusObjectPath> m_objNets = m_reply.value();
    foreach (QDBusObjectPath objNet, m_objNets) {
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objNet.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage result = m_interface.call("GetSettings");

        const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg1st >> map;

        for (QString outside_key : map.keys() ) {
            QMap<QString,QVariant> outsideMap = map.value(outside_key);
            if (outside_key == "connection") {
                for (QString search_key : outsideMap.keys()) {
                    if (search_key == "uuid") {
                        if (uuid == outsideMap.value(search_key).toString()) {
                            for (QString key : map.keys() ) {
                                QMap<QString,QVariant> innerMap = map.value(key);
                                if (key == "ipv4") {
                                    for (QString inner_key : innerMap.keys()) {
                                        if (inner_key == "address-data") {
                                            const QDBusArgument &dbusArg2nd = innerMap.value(inner_key).value<QDBusArgument>();
                                            QMap<QString,QVariant> m_map;
                                            dbusArg2nd.beginArray();
                                            while (!dbusArg2nd.atEnd()) {
                                                dbusArg2nd >> m_map;
                                            }
                                            dbusArg2nd.endArray();
                                            dbusWifiIpv4 = m_map.value("address").toString();
                                        } else if (inner_key == "method") {
                                            dbusWifiIpv4Method = innerMap.value(inner_key).toString();
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        continue;
                    }
                }
            }
        }
    }
}

//获取有线网络的MAC
QString KylinDBus::getLanMAC(QString ifname)
{
    QString macAddress = "--";

    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "AllDevices");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath; //保存设备的路径
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;

        QDBusInterface interfacePro( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> reply = interfacePro.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
        if (ifname == reply.value().toString()) {
            //先获取已连接有线网络对应的设备路径
            QDBusInterface interfaceMac( "org.freedesktop.NetworkManager",
                                          objPath.path(),
                                          "org.freedesktop.DBus.Properties",
                                          QDBusConnection::systemBus() );

            QDBusReply<QVariant> reply = interfaceMac.call("Get", "org.freedesktop.NetworkManager.Device.Wired", "HwAddress");
            macAddress = reply.value().toString();
        }
    }
    dbusArgs.endArray();

    return macAddress;
}

//获取已经连接的有线网ip
void KylinDBus::getConnectNetIp(QString netUuid)
{
    dbusWifiIpv4 = "";
    dbusActiveWifiIpv4 = "";
    dbusWifiIpv4Method = "";
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    //获取已经连接了那些网络，及这些网络对应的网络类型(ethernet or wifi)
    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;

        QDBusInterface interfacePro( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> replyType = interfacePro.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");
        QDBusReply<QVariant> replyUuid = interfacePro.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Uuid");
        if (replyType.value().toString() == "ethernet" || replyType.value().toString() == "802-3-ethernet") { //有线网
            if (replyUuid.value().toString() == netUuid) {
                //ipv4的路径信息和ip信息
                QDBusInterface interfaceIp4( "org.freedesktop.NetworkManager",
                                          objPath.path(),
                                          "org.freedesktop.DBus.Properties",
                                          QDBusConnection::systemBus() );
                QDBusMessage replyIp4 = interfaceIp4.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Ip4Config");
                QList<QVariant> outArgsIp4 = replyIp4.arguments();
                QVariant firstIp4 = outArgsIp4.at(0);
                QDBusVariant dbvFirstIp4 = firstIp4.value<QDBusVariant>();
                QVariant vFirstIp4 = dbvFirstIp4.variant();
                QDBusObjectPath dbusPathIp4 = vFirstIp4.value<QDBusObjectPath>();

                QDBusInterface interfaceIpv4( "org.freedesktop.NetworkManager",
                                              dbusPathIp4.path(),
                                              "org.freedesktop.DBus.Properties",
                                              QDBusConnection::systemBus() );
                QDBusMessage replyIpv4 = interfaceIpv4.call("Get", "org.freedesktop.NetworkManager.IP4Config", "AddressData");

                QList<QVariant> outArgsIpv4 = replyIpv4.arguments();
                QVariant firstIpv4 = outArgsIpv4.at(0);
                QDBusVariant dbvFirstIpv4 = firstIpv4.value<QDBusVariant>();
                QVariant vFirstIpv4 = dbvFirstIpv4.variant();

                const QDBusArgument &dbusArgIpv4 = vFirstIpv4.value<QDBusArgument>();
                QList<QVariantMap> mDatasIpv4;
                dbusArgIpv4 >> mDatasIpv4;

                foreach (QVariantMap mDataIpv4, mDatasIpv4) {
                    dbusActiveLanIpv4 = mDataIpv4.value("address").toString();
                }

                //ipv6的路径信息和ip信息
                QDBusInterface interfaceIp6( "org.freedesktop.NetworkManager",
                                          objPath.path(),
                                          "org.freedesktop.DBus.Properties",
                                          QDBusConnection::systemBus() );
                QDBusMessage replyIp6 = interfaceIp6.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Ip6Config");
                QList<QVariant> outArgsIp6 = replyIp6.arguments();
                QVariant firstIp6 = outArgsIp6.at(0);
                QDBusVariant dbvFirstIp6 = firstIp6.value<QDBusVariant>();
                QVariant vFirstIp6 = dbvFirstIp6.variant();
                QDBusObjectPath dbusPathIp6 = vFirstIp6.value<QDBusObjectPath>();

                QDBusInterface interfaceIpv6( "org.freedesktop.NetworkManager",
                                              dbusPathIp6.path(),
                                              "org.freedesktop.DBus.Properties",
                                              QDBusConnection::systemBus() );
                QDBusMessage replyIpv6 = interfaceIpv6.call("Get", "org.freedesktop.NetworkManager.IP6Config", "AddressData");

                QList<QVariant> outArgsIpv6 = replyIpv6.arguments();
                QVariant firstIpv6 = outArgsIpv6.at(0);
                QDBusVariant dbvFirstIpv6 = firstIpv6.value<QDBusVariant>();
                QVariant vFirstIpv6 = dbvFirstIpv6.variant();

                const QDBusArgument &dbusArgIpv6 = vFirstIpv6.value<QDBusArgument>();
                QList<QVariantMap> mDatasIpv6;
                dbusArgIpv6 >> mDatasIpv6;

                foreach (QVariantMap mDataIpv6, mDatasIpv6) {
                    dbusActiveLanIpv6 = mDataIpv6.value("address").toString();
                }
            }

        } else { //无线网
            if (replyUuid.value().toString() == netUuid) {
                //ipv4的路径信息和ip信息
                QDBusInterface interfaceIp4( "org.freedesktop.NetworkManager",
                                          objPath.path(),
                                          "org.freedesktop.DBus.Properties",
                                          QDBusConnection::systemBus() );
                QDBusMessage replyIp4 = interfaceIp4.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Ip4Config");
                QList<QVariant> outArgsIp4 = replyIp4.arguments();
                QVariant firstIp4 = outArgsIp4.at(0);
                QDBusVariant dbvFirstIp4 = firstIp4.value<QDBusVariant>();
                QVariant vFirstIp4 = dbvFirstIp4.variant();
                QDBusObjectPath dbusPathIp4 = vFirstIp4.value<QDBusObjectPath>();

                QDBusInterface interfaceIpv4( "org.freedesktop.NetworkManager",
                                              dbusPathIp4.path(),
                                              "org.freedesktop.DBus.Properties",
                                              QDBusConnection::systemBus() );
                QDBusMessage replyIpv4 = interfaceIpv4.call("Get", "org.freedesktop.NetworkManager.IP4Config", "AddressData");

                QList<QVariant> outArgsIpv4 = replyIpv4.arguments();
                QVariant firstIpv4 = outArgsIpv4.at(0);
                QDBusVariant dbvFirstIpv4 = firstIpv4.value<QDBusVariant>();
                QVariant vFirstIpv4 = dbvFirstIpv4.variant();

                const QDBusArgument &dbusArgIpv4 = vFirstIpv4.value<QDBusArgument>();
                QList<QVariantMap> mDatasIpv4;
                dbusArgIpv4 >> mDatasIpv4;

                foreach (QVariantMap mDataIpv4, mDatasIpv4) {
                    dbusActiveWifiIpv4 = mDataIpv4.value("address").toString();
                }
            }
        }
    }
    dbusArgs.endArray();
}

 //监听有线网络ip变化
void KylinDBus::getLanIpChanged()
{
    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QList<QDBusObjectPath> m_objNets = m_reply.value();
    foreach (QDBusObjectPath objNet, m_objNets) {
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objNet.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage result = m_interface.call("GetSettings");

        const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg1st >> map;

        for(QString key : map.keys() ) {
            if (key == "802-3-ethernet") {
                oldSettingPaths.append(objNet);
                QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                                     objNet.path(),
                                                     QString("org.freedesktop.NetworkManager.Settings.Connection"),
                                                     QString("Updated"), this, SLOT(onLanIpPropertiesChanged() ) );
            } else if (key == "802-11-wireless") {
                QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                                     objNet.path(),
                                                     QString("org.freedesktop.NetworkManager.Settings.Connection"),
                                                     QString("Updated"), this, SLOT(onWifiIpPropertiesChanged() ) );
            }
        }
    }
}

//获取wifi的mac地址
void KylinDBus::getWifiMac(QString netName)
{
    dbusWifiMac = "";

    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              wirelessPath.path(),
                              "org.freedesktop.NetworkManager.Device.Wireless",
                              QDBusConnection::systemBus() );

    QDBusReply<QList<QDBusObjectPath>> reply = interface.call("GetAllAccessPoints"); //get accesspoint for each wifi
    QList<QDBusObjectPath> objPaths = reply.value();

    foreach (QDBusObjectPath objPath, objPaths) {
        QDBusInterface ssid_interface( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> ssid_replys = ssid_interface.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "Ssid");
        QString str_name = ssid_replys.value().toString();

        if (str_name == netName) {
            //if the ssid of the accesspoint is just the netName, we can get this hardware address
            QDBusInterface path_interface( "org.freedesktop.NetworkManager",
                                      objPath.path(),
                                      "org.freedesktop.DBus.Properties",
                                      QDBusConnection::systemBus() );

            QDBusReply<QVariant> path_reply = path_interface.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "HwAddress");
            dbusWifiMac = path_reply.value().toString();
        }
    }
}

//获取当前能获取到的wifi的数量， 每个accesspoint 代表一个wifi
int KylinDBus::getAccessPointsNumber()
{
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              wirelessPath.path(),
                              "org.freedesktop.NetworkManager.Device.Wireless",
                              QDBusConnection::systemBus() );

    QDBusReply<QList<QDBusObjectPath>> reply = interface.call("GetAllAccessPoints");
    QList<QDBusObjectPath> objPaths = reply.value();

    return objPaths.size();
}

//返回当前已创建有线的个数
int KylinDBus::getWiredNetworkNumber()
{
    QDBusInterface interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> reply = interface.call("ListConnections");
    QList<QDBusObjectPath> objNetPaths = reply.value();

    int wiredNetworkNumber = 0;
    foreach (QDBusObjectPath objNetPath, objNetPaths) {
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objNetPath.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage result = m_interface.call("GetSettings");

        const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg1st >> map;

        for(QString key : map.keys() ) {
            if (key == "802-3-ethernet") {
                wiredNetworkNumber += 1;
                qDebug()<<"A new wired network was created.";
            }
        }
    }

    return wiredNetworkNumber;
}

QStringList KylinDBus::getWifiSsidList()
{
    QStringList wifiSsidList;

    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QDBusObjectPath specific_connection;
    specific_connection.setPath("/");

    QList<QDBusObjectPath> m_objSettingPaths = m_reply.value();
    foreach (QDBusObjectPath objSettingPath, m_objSettingPaths) {
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objSettingPath.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage result = m_interface.call("GetSettings");

        const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg1st >> map;

        if (map.value("802-11-wireless").value("mode").toString() == "infrastructure") {
            wifiSsidList.append(map.value("connection").value("id").toString());
        }
    }

    return wifiSsidList;
}

void KylinDBus::toCreateNewLan()
{
    int i = 1;
    if (multiWiredIfName.size() != 0) {
        foreach (QString strIfName, multiWiredIfName) {
            QString cmdStr = "nmcli connection add con-name '有线连接 " + QString::number(i,10) + "' ifname '" + strIfName + "' type ethernet";
            Utils::m_system(cmdStr.toUtf8().data());
            i += 1;
        }
    }
}

//根据网卡接口的名称获取接口对应网线是否接入
bool KylinDBus::getWiredCableStateByIfname(QString ifname)
{
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "AllDevices");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;
        QDBusInterface interfaceInterface( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> replyInterface = interfaceInterface.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
        if (replyInterface.value().toString() == ifname) {
            QDBusInterface interfaceCarrier( "org.freedesktop.NetworkManager",
                                      objPath.path(),
                                      "org.freedesktop.DBus.Properties",
                                      QDBusConnection::systemBus() );

            QDBusReply<QVariant> replyCarrier = interfaceCarrier.call("Get", "org.freedesktop.NetworkManager.Device.Wired", "Carrier");

            if (replyCarrier.value().toBool()) {
               return true;
            }
        }
    }
    dbusArgs.endArray();

    return false;
}

//根据网卡接口的名称获取uuid
QString KylinDBus::getConnLanNameByIfname(QString ifname)
{
    QString uuidName = "--";
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;

        QDBusInterface interfaceDevice( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusMessage replyDevices = interfaceDevice.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Devices");
        QList<QVariant> outArgsDevices = replyDevices.arguments();
        QVariant firstDevices = outArgsDevices.at(0);
        QDBusVariant dbvFirstDevices = firstDevices.value<QDBusVariant>();
        QVariant vFirstDevices = dbvFirstDevices.variant();
        QDBusArgument dbusArgsDevices = vFirstDevices.value<QDBusArgument>();

        QDBusObjectPath objPathDevices;
        dbusArgsDevices.beginArray();
        while (!dbusArgsDevices.atEnd()) {
            dbusArgsDevices >> objPathDevices;
            QDBusInterface interfaceInterface( "org.freedesktop.NetworkManager",
                                      objPathDevices.path(),
                                      "org.freedesktop.DBus.Properties",
                                      QDBusConnection::systemBus() );

            QDBusReply<QVariant> replyInterface = interfaceInterface.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
            if (replyInterface.value().toString() == ifname) {
                QDBusInterface interfaceUuid( "org.freedesktop.NetworkManager",
                                          objPath.path(),
                                          "org.freedesktop.DBus.Properties",
                                          QDBusConnection::systemBus() );

                QDBusReply<QVariant> replyDevices = interfaceUuid.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Uuid");
                uuidName = replyDevices.value().toString();
            }
        }
        dbusArgsDevices.endArray();
    }
    dbusArgs.endArray();

    return uuidName;
}

//新增了一个网络，伴随着多了一个网络配置文件
void KylinDBus::onNewConnection(QDBusObjectPath objPath)
{
    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                              objPath.path(),
                              "org.freedesktop.NetworkManager.Settings.Connection",
                              QDBusConnection::systemBus());
    QDBusMessage result = m_interface.call("GetSettings");

    const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
    QMap<QString,QMap<QString,QVariant>> map;
    dbusArg1st >> map;

    for(QString key : map.keys() ) {
        if (key == "802-3-ethernet") {
            emit this->updateWiredList(0); //send this signal to update wired network list
            emit this->newConnAdded(0);
            syslog(LOG_DEBUG, "A new wired network was created.");
            qDebug()<<"A new wired network was created.";
            break;
        } else if (key == "802-11-wireless") {
            syslog(LOG_DEBUG, "A new wireless network(wifi) was created.");
            qDebug()<<"A new wireless network was created.";
            emit this->newConnAdded(1);
            break;
        }
    }

    onWiredSettingNumChanged();
}

//减少了一个网络，伴随着减少了一个网络配置文件
void KylinDBus::onConnectionRemoved(QDBusObjectPath objPath)
{
    syslog(LOG_DEBUG, "An old network was removed from configure directory.");
    qDebug()<<"An old network was removed from configure directory.";

    if (mw->is_btnLanList_clicked == 1) {
        emit this->updateWiredList(0); //send this signal to update wired network list
    }

    onWiredSettingNumChanged();
}

//有线网的个数，也即有线网配置的个数变化
void KylinDBus::onWiredSettingNumChanged()
{
    //先取消之前建立的信号槽连接
    foreach (QDBusObjectPath objSettingPath, oldSettingPaths) {
        QDBusConnection::systemBus().disconnect(QString("org.freedesktop.NetworkManager"),
                                             objSettingPath.path(),
                                             QString("org.freedesktop.NetworkManager.Settings.Connection"),
                                             QString("Updated"), this, SLOT(onLanIpPropertiesChanged() ) );
    }

    //再建立新的信号槽连接
    getLanIpChanged();
}

//应用启动时，获取已经连接的网络信息，包括该网络的路径和类型
void KylinDBus::initConnectionInfo()
{
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    //获取已经连接了那些网络，及这些网络对应的网络类型(ethernet or wifi)
    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;
        oldPaths.append(objPath);

        QDBusInterface interface( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");
        oldPathInfo.append(reply.value().toString());
    }
    dbusArgs.endArray();

    //获取当前wifi的开关状态
    QDBusReply<QVariant> m_result = interface.call("Get", "org.freedesktop.NetworkManager", "WirelessEnabled");
    qDebug()<<"debug: *****初始的无线网络开关状态是: "<<m_result.value().toBool();
    oldWifiSwitchState = m_result.value().toBool();
}

//获取已经连接有线网络的ssid和uuid和state
QList<QString> KylinDBus::getAtiveLanSsidUuidState()
{
    QList<QString> strSsidUuidState;

    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );
    //获取已经连接了那些网络，及这些网络对应的网络类型(ethernet or wifi)
    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;

        QDBusInterface interfaceType( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );
        QDBusReply<QVariant> reply = interfaceType.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");

        if (reply.value().toString() == "ethernet" || reply.value().toString() == "802-3-ethernet" || reply.value().toString() == "bluetooth") {
            QDBusInterface interfaceInfo( "org.freedesktop.NetworkManager",
                                      objPath.path(),
                                      "org.freedesktop.DBus.Properties",
                                      QDBusConnection::systemBus() );
            QDBusReply<QVariant> replyId = interfaceInfo.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Id");
            QDBusReply<QVariant> replyUuid = interfaceInfo.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Uuid");
            QDBusReply<QVariant> replyState = interfaceInfo.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "State");
            strSsidUuidState.append(replyId.value().toString());
            strSsidUuidState.append(replyUuid.value().toString());
            if (replyState.value().toUInt() == 1) {
                strSsidUuidState.append("connecting");
            }
            if (replyState.value().toUInt() == 2) {
                strSsidUuidState.append("connected");
            }
        }
    }
    dbusArgs.endArray();

    return strSsidUuidState;
}

//获取已连接wifi的uuid
QString KylinDBus::getActiveWifiUuid()
{
    QString ssid;

    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );
    //获取已经连接了那些网络，及这些网络对应的网络类型(ethernet or wifi)
    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;

        QDBusInterface interfaceType( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );
        QDBusReply<QVariant> reply = interfaceType.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");

        if (reply.value().toString() == "wifi" || reply.value().toString() == "802-11-wireless") {
            QDBusInterface interfaceInfo( "org.freedesktop.NetworkManager",
                                      objPath.path(),
                                      "org.freedesktop.DBus.Properties",
                                      QDBusConnection::systemBus() );
            QDBusReply<QVariant> replyUuid = interfaceInfo.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Uuid");
            ssid = replyUuid.value().toString();
        }
    }
    dbusArgs.endArray();

    return ssid;
}

//检查wifi连接状态
int KylinDBus::checkWifiConnectivity()
{
    int wifiState = 3;

    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;

        QDBusInterface interfaceType( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );
        QDBusReply<QVariant> replyType = interfaceType.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");

        if (replyType.value().toString() == "wifi" || replyType.value().toString() == "802-11-wireless") {
            QDBusReply<QVariant> replyState = interfaceType.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "State");
            wifiState = replyState.value().toUInt(); //正在连接的状态是1，连接上的状态是2
        }
    }
    dbusArgs.endArray();

    return wifiState;
}

bool KylinDBus::checkNetworkConnectivity()
{
    bool hasNetworkConnecting = false;

    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;

        QDBusInterface interfaceType( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );
        QDBusReply<QVariant> replyType = interfaceType.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");

        if (replyType.value().toString() == "802-3-ethernet" || replyType.value().toString() == "802-11-wireless") {
            QDBusReply<QVariant> replyState = interfaceType.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "State");
            if (replyState.value().toUInt() == 1) {
                hasNetworkConnecting = true;
            }
        }
    }
    dbusArgs.endArray();

    return hasNetworkConnecting;
}

//获取已经连接无线网络的ssid和uuid
QList<QString> KylinDBus::getAtiveWifiBSsidUuid(QStringList wifilist)
{
    QList<QString> strBSsidUuid;

    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );
    //获取已经连接了那些网络，及这些网络对应的网络类型(ethernet or wifi)
    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;

        QDBusInterface interfaceType( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );
        QDBusReply<QVariant> reply = interfaceType.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");

        if (reply.value().toString() == "wifi" || reply.value().toString() == "802-11-wireless") {
            QDBusInterface interfaceInfo( "org.freedesktop.NetworkManager",
                                      objPath.path(),
                                      "org.freedesktop.DBus.Properties",
                                      QDBusConnection::systemBus() );

            //获取已经连接wifi的uuid
            QDBusReply<QVariant> replyUuid = interfaceInfo.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Uuid");
            //qDebug() << "wifi uuid : "<< replyUuid.value().toString();
            strBSsidUuid.append(replyUuid.value().toString());


            //再获取bssid
            QDBusMessage resultConnection = interfaceInfo.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Connection");

            QList<QVariant> outArgsConnection = resultConnection.arguments();
            QVariant firstConnection = outArgsConnection.at(0);
            QDBusVariant dbvFirstConnection = firstConnection.value<QDBusVariant>();
            QVariant vFirstConnection = dbvFirstConnection.variant();
            QDBusObjectPath dbusArgsConnection = vFirstConnection.value<QDBusObjectPath>();

            QDBusInterface interfaceSet("org.freedesktop.NetworkManager",
                                      dbusArgsConnection.path(),
                                      "org.freedesktop.NetworkManager.Settings.Connection",
                                      QDBusConnection::systemBus());
            QDBusMessage resultSet = interfaceSet.call("GetSettings");

            const QDBusArgument &dbusArg1stSet = resultSet.arguments().at( 0 ).value<QDBusArgument>();
            QMap<QString,QMap<QString,QVariant>> mapSet;
            dbusArg1stSet >> mapSet;

            for (QString setKey : mapSet.keys() ) {
                QMap<QString,QVariant> subSetMap = mapSet.value(setKey);
                if (setKey == "802-11-wireless") {
                    for (QString searchKey : subSetMap.keys()) {
                        if (searchKey == "seen-bssids") {
                            //qDebug() << "wifi bssid : "<<subSetMap.value(searchKey).toStringList();
                            QStringList strBssidList = subSetMap.value(searchKey).toStringList();
                            foreach (QString strBssid, strBssidList) {
                                strBSsidUuid.append(strBssid);
                            }
                        }
                    }
                }
            }
        }
    }
    dbusArgs.endArray();

//    //获取已经连接wifi的bssid
//    if (wifilist.size() != 0) {
//        // 填充可用网络列表
//        QString headLine = wifilist.at(0);
//        int indexSignal,indexSecu, indexFreq, indexBSsid, indexName;
//        headLine = headLine.trimmed();

//        bool isChineseExist = headLine.contains(QRegExp("[\\x4e00-\\x9fa5]+"));
//        if (isChineseExist) {
//            indexSignal = headLine.indexOf("SIGNAL");
//            indexSecu = headLine.indexOf("安全性");
//            indexFreq = headLine.indexOf("频率") + 4;
//            indexBSsid = headLine.indexOf("BSSID") + 6;
//            indexName = indexBSsid + 19;
//        } else {
//            indexSignal = headLine.indexOf("SIGNAL");
//            indexSecu = headLine.indexOf("SECURITY");
//            indexFreq = headLine.indexOf("FREQ");
//            indexBSsid = headLine.indexOf("BSSID");
//            indexName = indexBSsid + 19;
//        }

//        for (int i = 1, j = 0; i < wifilist.size(); i ++) {
//            QString line = wifilist.at(i);
//            QString winuse = line.mid(0, indexSignal).trimmed();
//            QString wbssid = line.mid(indexBSsid, 17).trimmed();
//            if (winuse == "*") {
//                strBSsidUuid.append(wbssid);
//            }
//        }
//    }

    return strBSsidUuid;
}


//使用dbus的方法获取wifi的信息，形成包含每个wifi信息的列表
void KylinDBus::toGetWifiList()
{
    QStringList slist;

    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusMessage resultAllDevices = interface.call("Get", "org.freedesktop.NetworkManager", "AllDevices");
    QList<QVariant> outArgsAllDevices = resultAllDevices.arguments();
    QVariant firstAllDevices = outArgsAllDevices.at(0);
    QDBusVariant dbvFirstAllDevices = firstAllDevices.value<QDBusVariant>();
    QVariant vFirstAllDevices = dbvFirstAllDevices.variant();
    QDBusArgument dbusArgsAllDevices = vFirstAllDevices.value<QDBusArgument>();

    QDBusObjectPath objPathDevice;
    dbusArgsAllDevices.beginArray();
    while (!dbusArgsAllDevices.atEnd()) {
        dbusArgsAllDevices >> objPathDevice;

        QDBusInterface interfacePro( "org.freedesktop.NetworkManager",
                                  objPathDevice.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusMessage resultAccessPoints = interfacePro.call("Get", "org.freedesktop.NetworkManager.Device.Wireless", "AccessPoints");
        QList<QVariant> outArgsAccessPoints = resultAccessPoints.arguments();
        QVariant firstAccessPoints = outArgsAccessPoints.at(0);
        QDBusVariant dbvFirstAccessPoints = firstAccessPoints.value<QDBusVariant>();
        QVariant vFirstAccessPoints = dbvFirstAccessPoints.variant();
        QDBusArgument dbusArgsAccessPoints = vFirstAccessPoints.value<QDBusArgument>();

        QDBusObjectPath objPathAccessPoint;
        dbusArgsAccessPoints.beginArray();
        while (!dbusArgsAccessPoints.atEnd()) {
            dbusArgsAccessPoints >> objPathAccessPoint;

            QDBusInterface interfaceAP( "org.freedesktop.NetworkManager",
                                      objPathAccessPoint.path(),
                                      "org.freedesktop.DBus.Properties",
                                      QDBusConnection::systemBus() );

            QDBusReply<QVariant> replyFlags = interfaceAP.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "Flags"); //in-use
            QDBusReply<QVariant> replyStrength = interfaceAP.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "Strength"); //signal
            QDBusReply<QVariant> replyWpaFlags = interfaceAP.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "WpaFlags"); //security
            QDBusReply<QVariant> replyFrequency = interfaceAP.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "Frequency"); //freq
            QDBusReply<QVariant> replyHwAddress = interfaceAP.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "HwAddress"); //bssid
            QDBusReply<QVariant> replySsid = interfaceAP.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "Ssid"); //ssid

            QString strFlags = replyFlags.value().toString();

            QString strStrength;
            QByteArray sesultStrength = replyStrength.value().toByteArray();
            int addr = sesultStrength[0] & 0x000000FF;
            addr |= ((sesultStrength[1] << 8) & 0x0000FF00);
            addr |= ((sesultStrength[2] << 16) & 0x00FF0000);
            addr |= ((sesultStrength[3] << 24) & 0xFF000000);
            strStrength = QString::number(addr);

            QString strWpaFlags = replyWpaFlags.value().toString();

            QString strFrequency = replyFrequency.value().toString();

            QString strHwAddress = replyHwAddress.value().toString();

            QString strSsid = replySsid.value().toString();

            slist.append(strFlags);
            slist.append(strStrength);
            slist.append(strWpaFlags);
            slist.append(strFrequency);
            slist.append(strHwAddress);
            slist.append(strSsid);
        }
        dbusArgsAccessPoints.endArray();

    }
    dbusArgsAllDevices.endArray();

    emit toGetWifiListFinished(slist);
}

//网络连接变化时，如有新增或减少的网络，发信号通知更新主界面
void KylinDBus::onPropertiesChanged(QVariantMap qvm)
{
    for(QString keyStr : qvm.keys()) {
        //有关已连接网络变化的信号
        if (keyStr == "ActiveConnections") {
            // 第一步 获取当前已连接网络的对象路径和对应的网络类型(ethernet or wifi)
            const QDBusArgument &dbusArg = qvm.value(keyStr).value<QDBusArgument>();
            QList<QDBusObjectPath> newPaths;
            newPaths.clear();
            dbusArg >> newPaths;
            QStringList newPathInfo;
            qDebug()<<"             ";
            foreach (QDBusObjectPath objPath, newPaths) {
                QDBusInterface interface( "org.freedesktop.NetworkManager",
                                          objPath.path(),
                                          "org.freedesktop.DBus.Properties",
                                          QDBusConnection::systemBus() );

                QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");
                newPathInfo.append(reply.value().toString());
            }
            // 第二步 同上一次获取的已连接网络相比较，处理相比于上次减少的网络连接
            for (int i=0; i<oldPaths.size(); i++) {
                QDBusObjectPath old_path = oldPaths.at(i);
                if (newPaths.size() == 0) {
                    if (oldPathInfo.size() == oldPaths.size()) {
                        qDebug()<<"debug: 已连接网络个数由1减少到0";
                        mw->onExternalConnectionChange(oldPathInfo.at(i), false);
                    }
                } else {
                    for (int j=0; j<newPaths.size(); j++) {
                        QDBusObjectPath new_path = newPaths.at(j);
                        if (new_path == old_path) {
                            break; //stop if new_path also in oldPaths
                        }

                        if (j == newPaths.size()-1) {
                            if (oldPathInfo.size() == oldPaths.size()) {
                                qDebug()<<"debug: 已连接网络个数由3减少到2，或由2减少到1";
                                mw->onExternalConnectionChange(oldPathInfo.at(i), false);
                            }
                        }
                    }
                }
            }

            // 第三步 同上一次获取的已连接网络相比较，处理相比于上次增加的网络连接
            for (int i=0; i<newPaths.size(); i++) {
                QDBusObjectPath new_path = newPaths.at(i);
                if (oldPaths.size() == 0) {
                    qDebug()<<"debug: 已连接网络个数由0增加到1";
                    mw->onExternalConnectionChange(newPathInfo.at(i), true);
                } else {
                    for (int j=0; j<oldPaths.size(); j++) {
                        QDBusObjectPath old_path = oldPaths.at(j);
                        if (new_path == old_path) {
                            break; //stop if new_path also in oldPaths
                        }

                        if (j == oldPaths.size()-1) {
                            qDebug()<<"debug: 已连接网络个数由1增加到2，或2增加到3";
                            mw->onExternalConnectionChange(newPathInfo.at(i), true);
                        }
                    }
                }
            }

            bool isChangeOldPathInfo = true;
            for (int k=0; k<newPathInfo.size(); k++) {
                if (newPathInfo.at(k) == "") {
                    isChangeOldPathInfo = false;
                }
            }
            if (isChangeOldPathInfo) {
                oldPathInfo = newPathInfo;
            }
            oldPaths = newPaths;
        }

        //收到wifi开关打开或关闭的信号后，进行处理
        if (keyStr == "WirelessEnabled") {
            bool newWifiSwitchState = qvm.value("WirelessEnabled").toBool();
            if (oldWifiSwitchState == false && newWifiSwitchState == true) {
                qDebug()<<"debug: wifi开关已经打开";
                mw->onExternalWifiSwitchChange(true);
            }
            if (oldWifiSwitchState == true && newWifiSwitchState == false) {
                qDebug()<<"debug: wifi开关已经关闭";
                mw->onExternalWifiSwitchChange(false);
            }

            oldWifiSwitchState = newWifiSwitchState; //更新状态用于下一次
        }
    }
}

//有线网属性变化时，执行该函数。由于可能在短时间收到几条相同属性变化信息，所以在短时间内，执行一次
void KylinDBus::onLanPropertyChanged(QVariantMap qvm)
{
    if (!isRunningFunction) {
        syslog(LOG_DEBUG, "kylin-nm receive a signal 'Device.Wired PropertiesChanged' about interface.");
        qDebug()<<"kylin-nm receive a signal 'Device.Wired PropertiesChanged' about interface.";
        isRunningFunction = true;  //function onLanPropertyChanged is running
        time->start(3000);

        QString str = qvm.value("Carrier").toString();
        if (str == "false" || str == "true") {
            getPhysicalCarrierState(1);
        }
    } else { a = 0; }
}
void KylinDBus::slot_timeout()
{
    isRunningFunction = false;
    time->stop();
}

//有线网的Ip属性变化时的响应函数
void KylinDBus::onLanIpPropertiesChanged()
{
    emit this->updateWiredList(0);
}

//无线网的Ip属性变化时的响应函数
void KylinDBus::onWifiIpPropertiesChanged() {
    emit this->updateWirelessList();
}
//利用dbus的方法对已经连接的有线网进行再次连接
void KylinDBus::reConnectWiredNet(QString netUuid)
{
    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QDBusObjectPath specific_connection;
    specific_connection.setPath("/");

    QList<QDBusObjectPath> m_objSettingPaths = m_reply.value();
    foreach (QDBusObjectPath objSettingPath, m_objSettingPaths) {
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objSettingPath.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage result = m_interface.call("GetSettings");

        const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg1st >> map;

        for (QString outside_key : map.keys() ) {
            QMap<QString,QVariant> outsideMap = map.value(outside_key);
            if (outside_key == "connection") {
                for (QString search_key : outsideMap.keys()) {
                    if (search_key == "uuid") {
                        if (netUuid == outsideMap.value(search_key).toString()) {

                            QDBusInterface conn_interface("org.freedesktop.NetworkManager",
                                                       "/org/freedesktop/NetworkManager",
                                                       "org.freedesktop.NetworkManager",
                                                       QDBusConnection::systemBus() );

                            QDBusInterface interface( "org.freedesktop.NetworkManager",
                                                      "/org/freedesktop/NetworkManager",
                                                      "org.freedesktop.DBus.Properties",
                                                      QDBusConnection::systemBus() );

                            //获取已经连接了那些网络，及这些网络对应的网络类型(ethernet or wifi)
                            QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
                            QList<QVariant> outArgs = result.arguments();
                            QVariant first = outArgs.at(0);
                            QDBusVariant dbvFirst = first.value<QDBusVariant>();
                            QVariant vFirst = dbvFirst.variant();
                            QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

                            QDBusObjectPath connWiredDivicePath; //已经连接的有线网对应的device路径
                            QDBusObjectPath objPath;
                            dbusArgs.beginArray();
                            while (!dbusArgs.atEnd()) {
                                dbusArgs >> objPath;

                                QDBusInterface interfacePro( "org.freedesktop.NetworkManager",
                                                          objPath.path(),
                                                          "org.freedesktop.DBus.Properties",
                                                          QDBusConnection::systemBus() );

                                QDBusReply<QVariant> reply = interfacePro.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");
                                if (reply.value().toString() == "ethernet" || reply.value().toString() == "802-3-ethernet") {
                                    //先获取已连接有线网络对应的设备路径
                                    QDBusInterface interfaceConn( "org.freedesktop.NetworkManager",
                                                              objPath.path(),
                                                              "org.freedesktop.DBus.Properties",
                                                              QDBusConnection::systemBus() );
                                    QDBusMessage replyConn = interfaceConn.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Devices");
                                    QList<QVariant> outArgsIp4 = replyConn.arguments();
                                    QVariant firstIp4 = outArgsIp4.at(0);
                                    QDBusVariant dbvFirstIp4 = firstIp4.value<QDBusVariant>();
                                    QVariant vFirstIp4 = dbvFirstIp4.variant();
                                    const QDBusArgument &dbusArgIpv4 = vFirstIp4.value<QDBusArgument>();
                                    QList<QDBusObjectPath> mDatasIpv4;
                                    dbusArgIpv4 >> mDatasIpv4;
                                    connWiredDivicePath = mDatasIpv4.at(0);
                                    //qDebug() << "xxxxxxxxxxxxxxxxxxxxxxxxxx" << mDatasIpv4.at(0).path();
                                }
                            }
                            dbusArgs.endArray();

                            /*
                             * 获取到所需的信息后，接下来进行连接
                             * conn_interface.call("ActivateConnection" 用到的参数
                             * 1. QVariant::fromValue(objSettingPath)  有线网络对应的Setting Path
                             * 2. QVariant::fromValue(connWiredDivicePath) 有线网络对应的Devices Path
                             * 3. QVariant::fromValue(specific_connection) 其他选项路径，可设置为"/"
                             */
                            QDBusReply<QDBusObjectPath> connectionReply = conn_interface.call("ActivateConnection",
                                                                                           QVariant::fromValue(objSettingPath),
                                                                                           QVariant::fromValue(connWiredDivicePath),
                                                                                           QVariant::fromValue(specific_connection));
                        }
                    }
                }
            }
        } // end for(QString outside_key : map.keys() )

    } //end foreach (QDBusObjectPath objSettingPath, m_objSettingPaths)
}

//利用dbus的方法连接有线网
bool KylinDBus::toConnectWiredNet(QString netUuid, QString netIfName)
{
    bool isConnectUp = false; //是否连接上

    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QDBusObjectPath specific_connection;
    specific_connection.setPath("/");

    QList<QDBusObjectPath> m_objSettingPaths = m_reply.value();
    foreach (QDBusObjectPath objSettingPath, m_objSettingPaths) {
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objSettingPath.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage result = m_interface.call("GetSettings");

        const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg1st >> map;

        for (QString outside_key : map.keys() ) {
            QMap<QString,QVariant> outsideMap = map.value(outside_key);
            if (outside_key == "connection") {
                for (QString search_key : outsideMap.keys()) {
                    if (search_key == "uuid") {
                        if (netUuid == outsideMap.value(search_key).toString()) {
                            QDBusInterface connInterface("org.freedesktop.NetworkManager",
                                                       "/org/freedesktop/NetworkManager",
                                                       "org.freedesktop.NetworkManager",
                                                       QDBusConnection::systemBus() );

                            //获取对应的设备路径
                            QDBusObjectPath connWiredDivicePath;
                            connWiredDivicePath.setPath("/");
                            QDBusInterface interfaceDevice( "org.freedesktop.NetworkManager",
                                                      "/org/freedesktop/NetworkManager",
                                                      "org.freedesktop.DBus.Properties",
                                                      QDBusConnection::systemBus() );

                            QDBusMessage resultDevice = interfaceDevice.call("Get", "org.freedesktop.NetworkManager", "AllDevices");
                            QList<QVariant> outArgsDevice = resultDevice.arguments();
                            QVariant firstDevice = outArgsDevice.at(0);
                            QDBusVariant dbvFirstDevice = firstDevice.value<QDBusVariant>();
                            QVariant vFirstDevice = dbvFirstDevice.variant();
                            QDBusArgument dbusArgsDevice = vFirstDevice.value<QDBusArgument>();

                            QDBusObjectPath objPathDevice;
                            dbusArgsDevice.beginArray();
                            while (!dbusArgsDevice.atEnd()) {
                                dbusArgsDevice >> objPathDevice;
                                QDBusInterface interfaceInterfaceDevice( "org.freedesktop.NetworkManager",
                                                          objPathDevice.path(),
                                                          "org.freedesktop.DBus.Properties",
                                                          QDBusConnection::systemBus() );

                                QDBusReply<QVariant> replyInterfaceDevice = interfaceInterfaceDevice.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
                                if (replyInterfaceDevice.value().toString() == netIfName) {
                                    connWiredDivicePath = objPathDevice;
                                }
                            }
                            dbusArgsDevice.endArray();

                            //下面执行连接网络
                            QDBusReply<QDBusObjectPath> connectionReply = connInterface.call("ActivateConnection",
                                                                                           QVariant::fromValue(objSettingPath),
                                                                                           QVariant::fromValue(connWiredDivicePath),
                                                                                           QVariant::fromValue(specific_connection));
                            if (!connectionReply.value().path().isEmpty()) {
                                isConnectUp = true;
                            }
                        }
                    }
                }
            }
        } // end for(QString outside_key : map.keys() )

    } //end foreach (QDBusObjectPath objSettingPath, m_objSettingPaths)

    return isConnectUp;
}

//显示桌面通知
void KylinDBus::showDesktopNotify(QString message)
{
    emit requestSendDesktopNotify(message);
}

//获取任务栏位置，上下左右
int KylinDBus::getTaskBarPos(QString str)
{
    QDBusInterface interface( "com.ukui.panel.desktop",
                              "/",
                              "com.ukui.panel.desktop",
                              QDBusConnection::sessionBus() );

    QDBusReply<int> reply = interface.call("GetPanelPosition", str);
    if (reply.isValid()) {
        return reply;
    } else {
        return 0; //如果没有成功获取，返回一个默认值
    }
}

//获取任务栏高度
int KylinDBus::getTaskBarHeight(QString str)
{
    QDBusInterface interface( "com.ukui.panel.desktop",
                              "/",
                              "com.ukui.panel.desktop",
                              QDBusConnection::sessionBus() );

    QDBusReply<int> reply = interface.call("GetPanelSize", str);
    if (reply.isValid()) {
        return reply;
    } else {
        return 46; //如果没有成功获取，返回一个默认值
    }
}

///////////////////////////////////////////////////////////////////////////////
//下方使用Gsetting 进程通信方法

//创建获取任务栏信息的GSetting对象
void KylinDBus::initTaskbarGsetting()
{
    if (QGSettings::isSchemaInstalled("org.ukui.panel.settings")) {
        m_tastbar_gsettings = new QGSettings("org.ukui.panel.settings");
    }
}

//使用GSetting方法获取任务栏高度
int KylinDBus::getTaskbarHeight()
{
    if (!m_tastbar_gsettings) {
        return 46;
    }

    QStringList keys = m_tastbar_gsettings->keys();
    if (keys.contains("panelsize")) {
        int hh = m_tastbar_gsettings->get("panelsize").toInt();
        return hh;
    } else {
        return 46;
    }
}

//使用GSetting方法获取任务栏位置，上下左右
int KylinDBus::getTaskbarPos()
{
    if (!m_tastbar_gsettings) {
        return 0;
    }

    QStringList keys = m_tastbar_gsettings->keys();
    if (keys.contains("panelposition")) {
        int pp = m_tastbar_gsettings->get("panelposition").toInt();
        return pp;
    } else {
        return 0;
    }
}

//创建监听wifi打开或关闭信息的GSetting对象
void KylinDBus::getWifiSwitchState()
{
    if (QGSettings::isSchemaInstalled("org.ukui.control-center.wifi.switch")) {

        m_gsettings = new QGSettings("org.ukui.control-center.wifi.switch");

        // 监听key的value是否发生了变化
        connect(m_gsettings, &QGSettings::changed, this, [=] (const QString &key) {

            if (key == "switch") {
                if (isWirelessCardOn) {
                    bool judge = getSwitchStatus(key);
                    if (judge) {
                        mw->onBtnWifiClicked(2); //打开wifi开关
                        qDebug()<<"receive a signal to turn on wifi switch from control-center";
                        syslog(LOG_DEBUG, "receive a signal to turn on wifi switch from control-center");
                    } else {
                        mw->onBtnWifiClicked(3); //关闭wifi开关
                        qDebug()<<"receive a signal to turn off wifi switch from control-center";
                        syslog(LOG_DEBUG, "receive a signal to turn off wifi switch from control-center");
                    }
                }
            }
        });
    }
}

//收到控制面板发来的消息后，获取wifi开关信息
bool KylinDBus::getSwitchStatus(QString key) {
    if (!m_gsettings) {
        return true;
    }
    const QStringList list = m_gsettings->keys();
    if (!list.contains(key)) {
        return true;
    }
    bool res = m_gsettings->get(key).toBool();
    return res;
}

//通知控制面板wifi开关的信息
void KylinDBus::setWifiSwitchState(bool signal)
{
    if (!m_gsettings) {
        return ;
    }

    const QStringList list = m_gsettings->keys();

    if (!list.contains("switchor")) {
        return ;
    }
    m_gsettings->set("switchor",signal);
}

//通知控制面板无线网卡的状态信息
void KylinDBus::setWifiCardState(bool signal)
{
    if (!m_gsettings) {
        return ;
    }

    const QStringList list = m_gsettings->keys();

    if (!list.contains("wificard")) {
        return ;
    }
    m_gsettings->set("wificard",signal);
}

//创建获取窗口透明度信息的GSetting的对象
void KylinDBus::initTransparentState()
{
    if (QGSettings::isSchemaInstalled("org.ukui.control-center.personalise")) {
        m_transparency_gsettings = new QGSettings("org.ukui.control-center.personalise");
    }
}

//使用GSetting获取当前窗口应该使用的透明度
double KylinDBus::getTransparentData()
{
    if (!m_transparency_gsettings) {
        return 0.7;
    }

    QStringList keys = m_transparency_gsettings->keys();
    if (keys.contains("transparency")) {
        double tp = m_transparency_gsettings->get("transparency").toDouble();
        return tp;
    } else {
        return 0.7;
    }
}
