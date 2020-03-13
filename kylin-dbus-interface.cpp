/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
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
#include <QTextCodec>
#include <vector>
#include <QByteArray>


KylinDBus::KylinDBus(MainWindow *mainWindow, QObject *parent) :QObject(parent)
{
    this->mw = mainWindow;

    getObjectPath(); //获取dbus中 lan 与 WiFi 的device路径
    getPhysicalCarrierState(0); //初始化获取网线插入状态
    getLanHwAddressState(); //获取有线网Mac地址
    getWiredCardName();

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager"),
                                         QString("org.freedesktop.NetworkManager"),
                                         QString("DeviceAdded"), mw, SLOT(onWirelessDeviceAdded(QDBusObjectPath) ) );

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager"),
                                         QString("org.freedesktop.NetworkManager"),
                                         QString("DeviceRemoved"), mw, SLOT(onWirelessDeviceRemoved(QDBusObjectPath) ) );

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager/Settings"),
                                         QString("org.freedesktop.NetworkManager.Settings"),
                                         QString("NewConnection"), this, SLOT(onNewConnection(QDBusObjectPath) ) );

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager/Settings"),
                                         QString("org.freedesktop.NetworkManager.Settings"),
                                         QString("ConnectionRemoved"), this, SLOT(onConnectionRemoved(QDBusObjectPath) ) );

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString(wiredPath.path()),
                                         QString("org.freedesktop.NetworkManager.Device.Wired"),
                                         QString("PropertiesChanged"), this, SLOT(onLanPropertyChanged(QVariantMap) ) );

    if (wirelessPath.path() != ""){
        QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                             QString(wirelessPath.path()),
                                             QString("org.freedesktop.NetworkManager.Device.Wireless"),
                                             QString("PropertiesChanged"), this, SLOT(onWifiPropertyChanged(QVariantMap) ) );

        QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                             QString(wirelessPath.path()),
                                             QString("org.freedesktop.NetworkManager.Device.Wireless"),
                                             QString("AccessPointAdded"), this, SLOT(onAccessPointAdded(QDBusObjectPath) ) );
        getWirelessCardName();//获取无线网卡名称
    }

    time = new QTimer(this);
    time->setTimerType(Qt::PreciseTimer);
    QObject::connect(time, SIGNAL(timeout()), this, SLOT(slot_timeout()));

    QObject::connect(this, SIGNAL(updateWiredList(int)), mw, SLOT(onBtnNetListClicked(int)));
}

void KylinDBus::getObjectPath()
{
    QDBusInterface m_interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.NetworkManager",
                              QDBusConnection::systemBus() );

   QDBusReply<QList<QDBusObjectPath>> obj_reply = m_interface.call("GetAllDevices");
   QList<QDBusObjectPath> obj_paths = obj_reply.value();

   foreach (QDBusObjectPath obj_path, obj_paths){
       QDBusInterface interface( "org.freedesktop.NetworkManager",
                                 obj_path.path(),
                                 "org.freedesktop.DBus.Introspectable",
                                 QDBusConnection::systemBus() );

       QDBusReply<QString> reply = interface.call("Introspect");
       if(reply.value().indexOf("org.freedesktop.NetworkManager.Device.Wired") != -1){
           wiredPath = obj_path;
       } else if (reply.value().indexOf("org.freedesktop.NetworkManager.Device.Wireless") != -1){
           wirelessPath = obj_path;
           isWirelessCardOn = true;
       }
   }
}

void KylinDBus::getPhysicalCarrierState(int n)
{
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              wiredPath.path(),
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Device.Wired", "Carrier");

    if (reply.value().toString() == "true"){
        isWiredCableOn = true;
    } else if (reply.value().toString() == "false"){
        isWiredCableOn = false;
    } else {
        syslog(LOG_ERR, "Error occurred when get the property 'Carrier' of Wired");
        qDebug()<<"Error occurred when get the property 'Carrier' of Wired";
    }

    if (n == 1){ this->mw->onPhysicalCarrierChanged(isWiredCableOn);}
}

void KylinDBus::getLanHwAddressState()
{
    QDBusInterface lanInterface( "org.freedesktop.NetworkManager",
                              wiredPath.path(),
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusReply<QVariant> lanReply = lanInterface.call("Get", "org.freedesktop.NetworkManager.Device.Wired", "HwAddress");
    dbusLanMac = lanReply.value().toString();
}

void KylinDBus::getWiredCardName()
{
    QDBusInterface lanInterface( "org.freedesktop.NetworkManager",
                              wiredPath.path(),
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusReply<QVariant> lanReply = lanInterface.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
    dbusLanCardName = lanReply.value().toString();
}

void KylinDBus::getWirelessCardName()
{
    QDBusInterface lanInterface( "org.freedesktop.NetworkManager",
                              wirelessPath.path(),
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusReply<QVariant> lanReply = lanInterface.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
    dbusWiFiCardName = lanReply.value().toString();
}

void KylinDBus::getLanIp(QString netName)
{
    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QList<QDBusObjectPath> m_objNets = m_reply.value();
    foreach (QDBusObjectPath objNet, m_objNets){
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objNet.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage result = m_interface.call("GetSettings");

        const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
        //DBus type : a{sa{sv}}, a map with a key of QString, which maps to another map of QString,QVariant
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg1st >> map;

        for (int i =0;i<2;i++){
            if(map.values().at(1).values().at(i).toString() == netName){
                for(QString key : map.keys() ){
                    QMap<QString,QVariant> innerMap = map.value(key);
                    //qDebug() << "Key: " << key;
                    if (key == "ipv4") {
                        for (QString inner_key : innerMap.keys()){
                            if (inner_key == "address-data"){
                                const QDBusArgument &dbusArg2nd = innerMap.value(inner_key).value<QDBusArgument>();
                                QMap<QString,QVariant> m_map;

                                dbusArg2nd.beginArray();
                                while (!dbusArg2nd.atEnd()){
                                    dbusArg2nd >> m_map;// append map to a vector here if you want to keep it
                                }
                                dbusArg2nd.endArray();

                                //qDebug()<<"       " << m_map.value("address").toString();
                                dbusLanIpv4 = m_map.value("address").toString();
                            }
                        }
                    }
                }

                dbusLanIpv6 = map.values().at(3).values().at(5).toString();
                break;
            }
        }
    }
}

void KylinDBus::getWifiMac(QString netName)
{
    dbusWifiMac = "";

//    //将wifi名转为utf-8 十进制形式
//    std::vector<int> vec;
//    int len_encoded;
//    int len_netName = netName.size();
//    for (int i=0;i<len_netName;i++){
//        QTextCodec *utf8 = QTextCodec::codecForName("utf-8");
//        QByteArray encoded = utf8->fromUnicode(netName.at(i)).toHex();
//        len_encoded = encoded.size();
//        qDebug()<<"debug: 11"<<QString::number(len_encoded);

//        if (len_encoded == 2){
//            QString str;
//            str.append(encoded.at(0));
//            str.append(encoded.at(1));
//            bool ok;
//            QString hex = str;
//            int dec = hex.toInt(&ok, 16);
//            vec.push_back(dec);
//        } else if (len_encoded == 6){
//            for (int j=0;j<3;j++){
//                QString str = "";
//                str.append(encoded.at(j*2));
//                str.append(encoded.at(j*2 + 1));
//                bool ok;
//                QString hex = str;
//                int dec = hex.toInt(&ok, 16);
//                vec.push_back(dec);
//            }

//        }
//        len_encoded = 0;
//    }

    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              wirelessPath.path(),
                              "org.freedesktop.NetworkManager.Device.Wireless",
                              QDBusConnection::systemBus() );

    QDBusReply<QList<QDBusObjectPath>> reply = interface.call("GetAllAccessPoints");
    QList<QDBusObjectPath> objPaths = reply.value();

    foreach (QDBusObjectPath objPath, objPaths){
        QDBusInterface ssid_interface( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> ssid_replys = ssid_interface.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "Ssid");
        QString str_name = ssid_replys.value().toString();

        if (str_name == netName){
            QDBusInterface path_interface( "org.freedesktop.NetworkManager",
                                      objPath.path(),
                                      "org.freedesktop.DBus.Properties",
                                      QDBusConnection::systemBus() );

            QDBusReply<QVariant> path_reply = path_interface.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "HwAddress");
            dbusWifiMac = path_reply.value().toString();
        }
    }
}

void KylinDBus::getActWifiMac(QString netName)
{
    dbusWifiMac = "";

    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QList<QDBusObjectPath> m_objNets = m_reply.value();
    foreach (QDBusObjectPath objNet, m_objNets){
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objNet.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage reply = m_interface.call("GetSettings");
        const QDBusArgument &dbusArg = reply.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg >> map;

        for(QString key : map.keys() ){
            QMap<QString,QVariant> innerMap = map.value(key);
            if (key == "connection"){
                for (QString inner_key : innerMap.keys()){
                    if (inner_key == "id"){
                        if (netName == innerMap.value(inner_key).toString()){

                            for (QString subkey : map.keys()){
                                QMap<QString,QVariant> subMap = map.value(subkey);
                                if (subkey == "802-11-wireless"){
                                    dbusWifiMac = subMap.value("seen-bssids").toString();
                                }
                            } //end for (QString subkey : map.keys())

                        }
                    }
                }
            }
        }

    } // end foreach (QDBusObjectPath objNet, m_objNets)
}

int KylinDBus::getAccessPointsNumber()
{
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              wirelessPath.path(),
                              "org.freedesktop.NetworkManager.Device.Wireless",
                              QDBusConnection::systemBus() );

    QDBusReply<QList<QDBusObjectPath>> reply = interface.call("GetAllAccessPoints");
    QList<QDBusObjectPath> objPaths = reply.value();

//    foreach (QDBusObjectPath objPath, objPaths){
//        qDebug()<<"debug: *****path is: "<<objPath.path(); //列出每一个objectPath
//    }

    return objPaths.size();
}

int KylinDBus::getLanConnState()
{
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager/Devices/2",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Device", "ActiveConnection");

    return 0;
}

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

    for(QString key : map.keys() ){
        if (key == "802-3-ethernet") {
            emit this->updateWiredList(0);
            syslog(LOG_DEBUG, "A new wired network was created.");
            qDebug()<<"A new wired network was created.";
        }
    }
}

void KylinDBus::onConnectionRemoved(QDBusObjectPath objPath)
{
    syslog(LOG_DEBUG, "An old network was removed from configure directory.");
    qDebug()<<"An old network was removed from configure directory.";

    emit this->updateWiredList(0);
}

void KylinDBus::onLanPropertyChanged(QVariantMap qvm)
{
    if (!isRunningFunction) {
        syslog(LOG_DEBUG, "kylin-nm receive a signal 'Device.Wired propertiesChanged' about interface.");
        qDebug()<<"kylin-nm receive a signal 'Device.Wired propertiesChanged' about interface.";
        isRunningFunction = true;
        time->start(3000);

        QString str = qvm.value("Carrier").toString();
        if (str == "false" || str == "true"){
            getPhysicalCarrierState(1);
        }
    } else { a = 0; }
}
void KylinDBus::slot_timeout()
{
    isRunningFunction = false;
    time->stop();
}

void KylinDBus::onWifiPropertyChanged(QVariantMap qvm)
{
    //qDebug()<<"debug: *************"<<qvm.values();
    //uint ii = qvm.value("Bitrate").toUInt();
    //QString ss = QString::number(ii);
    //qDebug()<<"debug: ============="<<ss;
}

void KylinDBus::onAccessPointAdded(QDBusObjectPath objPath)
{
    //qDebug()<<"debug: &&&&&&&&&&&&&"<<objPath.path();
}

int KylinDBus::getTaskbarPos(QString str)
{
    QDBusInterface interface( "com.ukui.panel.desktop",
                              "/",
                              "com.ukui.panel.desktop",
                              QDBusConnection::sessionBus() );

    QDBusReply<int> reply = interface.call("GetPanelPosition", str);
    return reply;
}

int KylinDBus::getTaskbarHeight(QString str)
{
    QDBusInterface interface( "com.ukui.panel.desktop",
                              "/",
                              "com.ukui.panel.desktop",
                              QDBusConnection::sessionBus() );

    QDBusReply<int> reply = interface.call("GetPanelSize", str);
    return reply;
}
