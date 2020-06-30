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

#include <vector>

#include <QTextCodec>
#include <QByteArray>


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

    if (wiredPath.path() != "") {
        QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                             QString(wiredPath.path()),
                                             QString("org.freedesktop.NetworkManager.Device.Wired"),
                                             QString("PropertiesChanged"), this, SLOT(onLanPropertyChanged(QVariantMap) ) );
    } else {
        qDebug()<<"Can not find wired device object path when using dbus.";
    }

    if (wirelessPath.path() != "") {
        QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                             QString(wirelessPath.path()),
                                             QString("org.freedesktop.NetworkManager.Device.Wireless"),
                                             QString("PropertiesChanged"), this, SLOT(onWifiPropertyChanged(QVariantMap) ) );

        QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                             QString(wirelessPath.path()),
                                             QString("org.freedesktop.NetworkManager.Device.Wireless"),
                                             QString("AccessPointAdded"), this, SLOT(onAccessPointAdded(QDBusObjectPath) ) );
        getWirelessCardName();//获取无线网卡名称
    } else {
        qDebug()<<"Can not find wireless device object path when using dbus.";
    }

    time = new QTimer(this);
    time->setTimerType(Qt::PreciseTimer);
    QObject::connect(time, SIGNAL(timeout()), this, SLOT(slot_timeout()));

    QObject::connect(this, SIGNAL(updateWiredList(int)), mw, SLOT(onBtnNetListClicked(int)));
}


///////////////////////////////////////////////////////////////////////////////
//下方使用Dbus 进程通信方法

//获取dbus中 lan 与 WiFi 的device路径
void KylinDBus::getObjectPath()
{
    QDBusInterface m_interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.NetworkManager",
                              QDBusConnection::systemBus() );

   //先获取所有的网络设备的设备路径
   QDBusReply<QList<QDBusObjectPath>> obj_reply = m_interface.call("GetAllDevices");
   if (!obj_reply.isValid()) {
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
           qDebug()<<"execute dbus method 'Introspect' is invalid in func getObjectPath()";
       }

       if(reply.value().indexOf("org.freedesktop.NetworkManager.Device.Wired") != -1) {
           //表明有有线网设备
           wiredPath = obj_path;
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
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              wiredPath.path(),
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    //Carrier值为true，插了网线；为false，未插网线
    QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Device.Wired", "Carrier");

    try {
        if (reply.value().toString() == "true") {
            isWiredCableOn = true;
            if (n == 1){ this->mw->onPhysicalCarrierChanged(isWiredCableOn);}
        } else if (reply.value().toString() == "false") {
            isWiredCableOn = false;
            if (n == 1){ this->mw->onPhysicalCarrierChanged(isWiredCableOn);}
        } else {
            throw -1; //出现异常
        }
    } catch(...) {
        syslog(LOG_ERR, "Error occurred when get the property 'Carrier' of Wired");
        qDebug()<<"Error occurred when get the property 'Carrier' of Wired";
    }
}

//获取有线网Mac地址
void KylinDBus::getLanHwAddressState()
{
    QDBusInterface lanInterface( "org.freedesktop.NetworkManager",
                              wiredPath.path(),
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusReply<QVariant> lanReply = lanInterface.call("Get", "org.freedesktop.NetworkManager.Device.Wired", "HwAddress");
    if (!lanReply.isValid()) {
        qDebug()<<"can not get the attribute 'HwAddress' in func getLanHwAddressState()";
    } else {
        dbusLanMac = lanReply.value().toString();
    }
}

//获取有线网卡名称
void KylinDBus::getWiredCardName()
{
    if (wiredPath.path() == "") {
        dbusLanCardName = "";
    } else {
        QDBusInterface lanInterface( "org.freedesktop.NetworkManager",
                                  wiredPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> lanReply = lanInterface.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
        if (!lanReply.isValid()) {
            qDebug()<<"can not get the attribute 'Interface' in func getWiredCardName()";
            dbusLanCardName = "";
        } else {
            dbusLanCardName = lanReply.value().toString();
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
        qDebug()<<"can not get the attribute 'Interface' in func getWirelessCardName()";
    } else {
        dbusWiFiCardName = lanReply.value().toString();
    }
}

//获取有线网ip
void KylinDBus::getLanIp(QString netName)
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
        QDBusMessage result = m_interface.call("GetSettings"); //get information of this network

        const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
        //DBus type : a{sa{sv}}, a map with a key of QString, which maps to another map of QString,QVariant
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg1st >> map;

        for (QString outside_key : map.keys() ) {
            QMap<QString,QVariant> outsideMap = map.value(outside_key);
            if (outside_key == "connection") {
                for (QString search_key : outsideMap.keys()) {
                    if (search_key == "id") {
                        //const QDBusArgument &dbusArg2nd = innerMap.value(inner_key).value<QDBusArgument>();
                        if (netName == outsideMap.value(search_key).toString()) {
                            // qDebug()<<"aaaaaa"<<outsideMap.value(search_key).toString();

                            for (QString key : map.keys() ) {
                                QMap<QString,QVariant> innerMap = map.value(key);
                                //qDebug() << "Key: " << key;
                                if (key == "ipv4") {
                                    for (QString inner_key : innerMap.keys()) {
                                        if (inner_key == "address-data") {
                                            const QDBusArgument &dbusArg2nd = innerMap.value(inner_key).value<QDBusArgument>();
                                            QMap<QString,QVariant> m_map;

                                            dbusArg2nd.beginArray();
                                            while (!dbusArg2nd.atEnd()) {
                                                dbusArg2nd >> m_map;// append map to a vector here if you want to keep it
                                            }
                                            dbusArg2nd.endArray();

                                            //qDebug()<<"       " << m_map.value("address").toString();
                                            dbusLanIpv4 = m_map.value("address").toString();
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

                                            //qDebug()<<"       " << m_map.value("address").toString();
                                            dbusLanIpv6 = m_map.value("address").toString();
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
            syslog(LOG_DEBUG, "A new wired network was created.");
            qDebug()<<"A new wired network was created.";
        }
    }
}

//减少了一个网络，伴随着减少了一个网络配置文件
void KylinDBus::onConnectionRemoved(QDBusObjectPath objPath)
{
    syslog(LOG_DEBUG, "An old network was removed from configure directory.");
    qDebug()<<"An old network was removed from configure directory.";

    if (mw->is_btnNetList_clicked == 1) {
        emit this->updateWiredList(0); //send this signal to update wired network list
    }
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
    // qDebug()<<"             ";
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;
        oldPaths.append(objPath);
        // qDebug() <<"debug: *****path is: "<< objPath.path();

        QDBusInterface interface( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");
        // qDebug()<<"debug: *****connection type is: "<<reply.value().toString();
        oldPathInfo.append(reply.value().toString());
    }
    dbusArgs.endArray();
    // qDebug()<<"             ";


    //获取当前wifi是否连接
    QDBusReply<QVariant> m_result = interface.call("Get", "org.freedesktop.NetworkManager", "WirelessEnabled");
    qDebug()<<"debug: *****初始的无线网络开关状态是: "<<m_result.value().toBool();
    oldWifiSwitchState = m_result.value().toBool();
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
            dbusArg >> newPaths;
            QStringList newPathInfo;
            qDebug()<<"             ";
            foreach (QDBusObjectPath objPath, newPaths) {
                qDebug()<<"dbug: bbbbb  "<<objPath.path();

                QDBusInterface interface( "org.freedesktop.NetworkManager",
                                          objPath.path(),
                                          "org.freedesktop.DBus.Properties",
                                          QDBusConnection::systemBus() );

                QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");
                qDebug()<<"dbug: ccccc "<<reply.value().toString();
                newPathInfo.append(reply.value().toString());
            }
            qDebug()<<"             ";

            // 第二步 同上一次获取的已连接网络相比较，处理相比于上次减少的网络连接
            for (int i=0; i<oldPaths.size(); i++) {
                QDBusObjectPath old_path = oldPaths.at(i);
                if (newPaths.size() == 0) {
                    qDebug()<<"debug: 已连接网络个数由1减少到0";
                    mw->onExternalConnectionChange(oldPathInfo.at(i));
                } else {
                    for (int j=0; j<newPaths.size(); j++) {
                        QDBusObjectPath new_path = newPaths.at(j);
                        if (new_path == old_path) {
                            break; //stop if new_path also in oldPaths
                        }

                        if (j == newPaths.size()-1) {
                            if (oldPathInfo.size() == oldPaths.size()) {
                                qDebug()<<"debug: 已连接网络个数由3减少到2，或由2减少到1";
                                mw->onExternalConnectionChange(oldPathInfo.at(i));
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
                    mw->onExternalConnectionChange(newPathInfo.at(i));
                } else {
                    for (int j=0; j<oldPaths.size(); j++) {
                        QDBusObjectPath old_path = oldPaths.at(j);
                        if (new_path == old_path) {
                            break; //stop if new_path also in oldPaths
                        }

                        if (j == oldPaths.size()-1) {
                            qDebug()<<"debug: 已连接网络个数由1增加到2，或2增加到3";
                            mw->onExternalConnectionChange(newPathInfo.at(i));
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

//无线网属性变化时，执行该函数
void KylinDBus::onWifiPropertyChanged(QVariantMap qvm)
{
    //qDebug()<<"debug: *************"<<qvm.values();
    //uint ii = qvm.value("Bitrate").toUInt();
    //QString ss = QString::number(ii);
    //qDebug()<<"debug: ============="<<ss;
}

//增加了新的accesspoint时执行该函数，即检测到一个新的wifi
void KylinDBus::onAccessPointAdded(QDBusObjectPath objPath)
{
    //qDebug()<<"debug: &&&&&&&&&&&&&"<<objPath.path();
}

//利用dbus的方法连接有线网
void KylinDBus::connectWiredNet(QString netName)
{
    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QDBusObjectPath active_connection;
    active_connection.setPath("/");

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
                    if (search_key == "id") {
                        if (netName == outsideMap.value(search_key).toString()) {

                            QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                                       "/org/freedesktop/NetworkManager",
                                                       "org.freedesktop.NetworkManager",
                                                       QDBusConnection::systemBus() );
                            QDBusReply<QDBusObjectPath> connectionReply = m_interface.call("ActivateConnection",
                                                                                           QVariant::fromValue(objNet),
                                                                                           QVariant::fromValue(wiredPath),
                                                                                           QVariant::fromValue(active_connection));
                        }
                    }
                }
            }
        } // end for(QString outside_key : map.keys() )

    } //end foreach (QDBusObjectPath objNet, m_objNets)
}

//显示桌面通知
void KylinDBus::showDesktopNotify(QString message)
{
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    QList<QVariant> args;
    args<<(QCoreApplication::applicationName())
    <<((unsigned int) 0)
    <<QString("qweq")
    <<tr("kylin network applet desktop message") //显示的是什么类型的信息
    <<message //显示的具体信息
    <<QStringList()
    <<QVariantMap()
    <<(int)-1;
    iface.callWithArgumentList(QDBus::AutoDetect,"Notify",args);
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
                bool judge = getSwitchStatus(key);
                if (judge) {
                    mw->onBtnWifiClicked(1); //打开wifi开关
                } else {
                    mw->onBtnWifiClicked(2); //关闭wifi开关
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
