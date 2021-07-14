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

#include "backthread.h"
#include "utils.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <QFile>
#include <QRegExp>
#include <QDir>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusArgument>

BackThread::BackThread(QObject *parent) : QObject(parent)
{
    cmdProcessWifi = new QProcess(this);
    connect(cmdProcessWifi , SIGNAL(readyReadStandardOutput()) , this , SLOT(onReadOutputWifi()));
    connect(cmdProcessWifi , SIGNAL(readyReadStandardError()) , this , SLOT(onReadErrorWifi()));

    cmdProcessLan = new QProcess(this);
    connect(cmdProcessLan , SIGNAL(readyReadStandardOutput()) , this , SLOT(onReadOutputLan()));
    connect(cmdProcessLan , SIGNAL(readyReadStandardError()) , this , SLOT(onReadErrorLan()));
}

BackThread::~BackThread()
{
    cmdProcessWifi->close();
    cmdProcessLan->close();
}

//get the connection state of wired and wireles network
IFace* BackThread::execGetIface()
{
    IFace *iface = new IFace();

    QString tmpPath = "/tmp/kylin-nm-iface-" + QDir::home().dirName();
    QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli -f TYPE,DEVICE,STATE device > " + tmpPath;
    Utils::m_system(cmd.toUtf8().data());

    QFile file(tmpPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file ~/.config/kylin-nm-iface!";
    }
    QString txt = file.readAll();
    QStringList txtList = txt.split("\n");
    file.close();

    iface->lstate = DEVICE_UNMANAGED;
    iface->wstate = DEVICE_UNMANAGED;

    for (int i = 1; i < txtList.size(); i ++) {
        QString line = txtList.at(i);
        if (line != "") {
            int index1 = line.indexOf(" ");
            QString type = line.left(index1);
            QString lastStr = line.mid(index1).trimmed();
            int index2 = lastStr.indexOf(" ");
            QString iname = lastStr.left(index2);
            QString istateStr = lastStr.mid(index2).trimmed();

            if (type == "ethernet") {
                iface->lcards.append(iname);
            } else if (type == "wifi") {
                iface->wcards.append(iname);
            }

            //只要存在一个有线设备已连接，就不再扫描其他有线设备状态，避免有线被误断开
            if (type == "ethernet" && iface->lstate != DEVICE_CONNECTED) {
                // if type is wired network
                iface->lname = iname;

                if (istateStr == "unmanaged") {
                    iface->lstate = DEVICE_UNMANAGED; //switch of wired device is off
                } else if (istateStr == "unavailable") {
                    iface->lstate = DEVICE_UNAVALIABLE;
                } else if (istateStr == "disconnected") {
                    iface->lstate = DEVICE_DISCONNECTED; //wired network is disconnected
                } else if (istateStr == "connected" || istateStr == "connecting (getting IP configuration)") {
                    iface->lstate = DEVICE_CONNECTED; //wired network is connected
                } else {
                    //连接中，正在配置
                    iface->lstate = DEVICE_CONNECTING;
                }
            }
            if (type == "wifi" && iface->wname.isEmpty()) { //仅统计第一个无线网卡，后续无线网卡状态必然等于或差与第一个获取到的无线网卡
                // if type is wireless network
                iface->wname = iname;

                if (istateStr == "unmanaged" || istateStr == "unavailable") {
                    iface->wstate = 2; //switch of wireless device is off
                } else if (istateStr == "disconnected") {
                    iface->wstate = 1; //wireless network is disconnected
                } else if (istateStr == "connected") {
                    iface->wstate = 0; //wireless network is connected
                } else {
                    //连接中，正在配置
                    iface->wstate = 3;
                }
            }
        }
    }

    return iface;
}

void BackThread::getInitStatus() {
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );
    //　获取当前wifi是否打开
    QDBusReply<QVariant> m_result = interface.call("Get", "org.freedesktop.NetworkManager", "WirelessEnabled");

    if (m_result.isValid()) {
        bool status = m_result.value().toBool();
        emit wifiStatus(status);
        emit getWifiStatusComplete();
    } else {
        qDebug()<<"org.freedesktop.NetworkManager get invalid"<<endl;
        emit wifiStatus(false);
        emit getWifiStatusComplete();
    }
}

void BackThread::saveSwitchButtonState(const QString &key, const QVariant &value)
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->setValue(key, value);
    m_settings->sync();
    delete m_settings;
    return;
}

QVariant BackThread::getSwitchState(const QString &key)
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    QVariant value = m_settings->value(key);
    delete m_settings;
    if (!value.isValid())
        return QVariant();
    return value;
}

//turn on the switch of network
void BackThread::execEnNet()
{
    char *chr = "nmcli networking on";
    Utils::m_system(chr);

    while (1) {
        if (execGetIface()->lstate != DEVICE_UNMANAGED) {
            sleep(3);
            emit enNetDone();
            emit btFinish();
            break;
        }
        sleep(1);
    }
}

//turn off the switch of wireless network first, then turn off the switch of network
void BackThread::execDisNet()
{
    if (execGetIface()->wstate != 2) {
        char *chr = "nmcli radio wifi off";
        Utils::m_system(chr);

        while (1) {
            if (execGetIface()->wstate == 2) {
                emit disWifiDone();
                emit btFinish();
                break;
            }
            sleep(1);
        }
    }

    char *chr1 = "nmcli networking off";
    Utils::m_system(chr1);

    while (1) {
        if (execGetIface()->lstate == DEVICE_UNMANAGED) {
            emit disNetDone();
            emit btFinish();
            break;
        }
        sleep(1);
    }
}

//turn on the switch of wireless network
void BackThread::execEnWifi()
{
    char *chr1 = "nmcli radio wifi on";
    Utils::m_system(chr1);
    emit btFinish();
    KylinDBus objBackThreadDBus;

    while (1) {
        if (execGetIface()->wstate != 2) {
            while (1) {
                if (objBackThreadDBus.getAccessPointsNumber() > 0) {
                    // objBackThreadDBus.getAccessPointsNumber()>0 standard can get wireless accesspoints now
                    emit enWifiDone();
                    emit btFinish();
                    break;
                }
                sleep(2);
            }
            break;
        }
        sleep(1);
    }
}

void BackThread::rfKillexecEnWifi()
{
    char *chr1 = "nmcli radio wifi on";
    Utils::m_system(chr1);

    if (execGetIface()->wstate != 2) {
        emit enWifiDoneByRfkill();
        emit btFinishByRfkill();
    } else {
        usleep(500*1000);
        emit enWifiDoneByRfkill();
        emit btFinishByRfkill();
    }

}

//turn off the switch of wireless network
void BackThread::execDisWifi()
{
    char *chr = "nmcli radio wifi off";
    Utils::m_system(chr);
    while (1) {
        if (execGetIface()->wstate == 2) {
            emit disWifiDone();
            emit btFinish();
            break;
        }
        sleep(1);
    }
}

void BackThread::rfkillExecDisWifi()
{
    char *chr = "nmcli radio wifi off";
    Utils::m_system(chr);
    while (1) {
        if (execGetIface()->wstate == 2) {
            emit disWifiDoneByRfkill();
            emit btFinishByRfkill();
            break;
        }
        sleep(1);
    }
}

//to connect wired network
void BackThread::execConnLan(QString connName, QString ifname, QString connectType)
{
    currConnLanUuid = connName;
    currConnLanType = connectType;
    QString mycmd; //连接命令
    KylinDBus objBackThreadDBus;

    bool isWiredCableAlready = objBackThreadDBus.getWiredCableStateByIfname(ifname);

    if (connectType == "bluetooth" || connectType == "vpn"|| ifname == "" || ifname == "--") {
        isWiredCableAlready = true; //对于蓝牙类型的网络不需要接入网线就可以连接
        mycmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "'";
    } else {
        mycmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "' ifname '" + ifname + "'";
    }
    qDebug()<<"Trying to connect lan. uuid="<<connName<<". cmd="<<mycmd;

    if (isWiredCableAlready) {
        QStringList options;
        options << "-c" << mycmd;
        cmdProcessLan->start("/bin/bash",options);
        cmdProcessLan->waitForStarted();
        cmdProcessLan->waitForFinished();
    } else {
        qDebug()<<"connect wired network failed for without wired cable plug in.";
        emit connDone(1);
    }

    emit btFinish();
}

void BackThread::onReadOutputLan()
{
    QByteArray cmdout = cmdProcessLan->readAllStandardOutput();
    QString strResult = QString::fromLocal8Bit(cmdout);
    qDebug()<<"on_readoutput_lan:  "<< strResult;
    dellConnectLanResult(strResult);
}
void BackThread::onReadErrorLan()
{
    QByteArray cmdout = cmdProcessLan->readAllStandardError();
    QString strResult = QString::fromLocal8Bit(cmdout);
    qDebug()<<"on_readerror_lan:  "<< strResult;
    dellConnectLanResult(strResult);
}

void BackThread::dellConnectLanResult(QString info)
{
    if (info.indexOf("successfully") != -1) {
        qDebug()<<"debug: in function execConnLan, wired net state is: "<<QString::number(execGetIface()->lstate);

        if (currConnLanType == "bluetooth") {
            emit connDone(2);
        } else {
            emit connDone(0);
        }
    } else {
        QString cmd = "nmcli connection down '" + currConnLanUuid + "'";
        Utils::m_system(cmd.toUtf8().data());
        if (info.indexOf("IP configuration could not be reserved") != -1) {
            emit connDone(4);
        } else if(info.indexOf("MACs") != -1 || info.indexOf("Mac") != -1 || info.indexOf("MAC") != -1) {
            emit connDone(5);
        } else if(info.indexOf("Killed") != -1 || info.indexOf("killed") != -1) {
            emit connDone(6);
        } else if(info.indexOf("The Bluetooth connection failed") != -1) {
            emit connDone(7);
        } else if(info.indexOf("Carrier/link changed") != -1) {
            emit connDone(8);
        } else {
            if (currConnLanType == "vpn"){
                emit connDone(10);
            }
            else{
                emit connDone(9);
            }
        }
    }
}

//to connected wireless network need a password
void BackThread::execConnWifiPWD(QString connName, QString password, QString connType, QString security, QString ifname)
{
    //disConnLanOrWifi("wifi");
    if (!connType.isEmpty()) {
        QString strConntype = "nmcli connection modify '" + connName + "' wifi-sec.psk-flags 0";
        Utils::m_system(strConntype.toUtf8().data());
    }

    QString tmpPath = "/tmp/kylin-nm-btoutput-" + QDir::home().dirName();
//    if (security.contains("WPA3")) {
//        QString create_cmd = QString("nmcli connection add con-name \"1\" type wifi 802-11-wireless-security.key-mgmt sae ssid \"%2\" 802-11-wireless-security.psk %3").arg(connName).arg(connName).arg(password);
//        Utils::m_system(create_cmd.toUtf8().data());
//        QString connect_cmdStr = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "' ifname " + ifname +" > " + tmpPath;
//        Utils::m_system(connect_cmdStr.toUtf8().data());
//    } else {
//        QString cmdStr = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli device wifi connect '" + connName + "' password '" + password + "' > " + tmpPath;
//        Utils::m_system(cmdStr.toUtf8().data());
//    }
    if (security.contains("WPA3")) {
        QString create_cmd;
        if (security.contains("WPA2")) {
            qDebug() << "连接wpa2 和 wpa3混合模式的wifi";
            create_cmd = QString("nmcli connection add con-name \"%1\" "
                                 "type wifi 802-11-wireless-security.psk-flags %2 "
                                 " 802-11-wireless-security.key-mgmt wpa-psk ssid \"%3\" "
                                 "802-11-wireless-security.psk %4")
                                 .arg(connName).arg(PSKFLAG).arg(connName).arg(password);
        } else {
            qDebug() << "连接 wpa3个人模式的wifi";
            create_cmd = QString("nmcli connection add con-name \"%1\""
                                 " type wifi 802-11-wireless-security.psk-flags %2 "
                                 "802-11-wireless-security.key-mgmt sae ssid \"%3\" "
                                 "802-11-wireless-security.psk %4")
                                 .arg(connName).arg(PSKFLAG).arg(connName).arg(password);
        }
        Utils::m_system(create_cmd.toUtf8().data());
        qDebug()<<"create connection by shell cmd:"<<create_cmd;
        QString homepath = getenv("HOME");
        QString psk_file = homepath + "/.config/" + connName + ".psk";
        if (QFile::exists(psk_file)) {
            QString connect_cmdStr =
                    "export LANG='en_US.UTF-8';export LANGUAGE='en_US';"
                    "nmcli connection up '" + connName
                    + "' passwd-file '" + psk_file + "'"
                    + " ifname " + ifname +" > " + tmpPath;
            Utils::m_system(connect_cmdStr.toUtf8().data());
        } else {
            QString connect_cmdStr =
                    "export LANG='en_US.UTF-8';export LANGUAGE='en_US';"
                    "nmcli connection up '" + connName
                    + "' ifname " + ifname +" > " + tmpPath;
            Utils::m_system(connect_cmdStr.toUtf8().data());
        }
    } else {
        qDebug() << "连接普通安全性类型的wifi";
        QString cmdStr = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli device wifi connect '" + connName + "' password '" + password + "' > " + tmpPath;
        Utils::m_system(cmdStr.toUtf8().data());
    }

    QFile file(tmpPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file /tmp/kylin-nm-btoutput !"<<endl;
    }
    QString line = file.readLine();
    file.close();
    qDebug()<<"connect_wifi_result: "<< line;

    if (line.indexOf("successfully") != -1) {
        emit connDone(0);
        qDebug()<<"debug: in function execConnWifiPWD, wireless net state is: "<<QString::number(execGetIface()->wstate);
    } else if(line.indexOf("Secrets were required") != -1){
        //emit connDone(4);//发出信号4是之前添加每次连接输入密码的功能时需要的
        emit connDone(1);
    } else {
        emit connDone(1);
    }

    emit btFinish();
}

void BackThread::execConnHiddenWifiWPA(int secuType, QString wifiName, QString wifiPassword)
{
    //this->getTheWifiCardName();

    QProcess shellProcess;
    shellProcess.start("nmcli -f ssid device wifi");
    shellProcess.waitForFinished(3000); // 等待最多3s
    bool is_hidden  = true;
    if (shellProcess.exitCode() == 0) {
        QString shellOutput = shellProcess.readAllStandardOutput();
        QStringList wlist = shellOutput.split("\n");
        foreach (QString wifi, wlist) {
            if (wifi.trimmed() == wifiName) {
                is_hidden = false;
            }
        }
    }

    int x(1), n(0);
    do {
        n += 1;
        if (n >= 4) {
            qDebug()<<"connection attempt of hidden wifi"<<wifiName<<" failed for 3 times, no more attempt";
            x = 0;
            emit connDone(1);
            emit btFinish();
            return ;
        }

        QString tmpPath = "/tmp/kylin-nm-btoutput-" + QDir::home().dirName();

        if (secuType == 3 || secuType == 4) {
            qDebug() << "第一步 删除连接失败的配置文件";
            QString delete_cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection delete '" + wifiName + "'";
            Utils::m_system(delete_cmd.toUtf8().data());

            qDebug() << "第二步 创建隐藏wifi对应的配置文件";
            QString create_cmd;
            if (secuType == 3) {
                //wpa2 and wpa3 personal
                create_cmd = QString("nmcli connection add con-name \"%1\" "
                            "type wifi 802-11-wireless-security.psk-flags %2 "
                            "802-11-wireless-security.key-mgmt wpa-psk ssid \"%3\" "
                            "802-11-wireless-security.psk %4")
                            .arg(wifiName).arg(PSKFLAG).arg(wifiName).arg(wifiPassword);
            }
            if (secuType == 4) {
                //wpa3 personal
                create_cmd = QString("nmcli connection add con-name \"%1\""
                            " type wifi 802-11-wireless-security.psk-flags %2 "
                            "802-11-wireless-security.key-mgmt sae ssid \"%3\" "
                            "802-11-wireless-security.psk %4")
                            .arg(wifiName).arg(PSKFLAG).arg(wifiName).arg(wifiPassword);
            }
            Utils::m_system(create_cmd.toUtf8().data());

            qDebug() << "第三步 再执行连接隐藏wifi";
            QString homepath = getenv("HOME");
            QString psk_file = homepath + "/.config/" + wifiName + ".psk";
            if (QFile::exists(psk_file)) {
                QString connect_cmdStr =
                        "export LANG='en_US.UTF-8';export LANGUAGE='en_US';"
                        "nmcli connection up '" + wifiName
                        + "' passwd-file '" + psk_file + "' > " + tmpPath;
                Utils::m_system(connect_cmdStr.toUtf8().data());
            } else {
                QString connect_cmdStr =
                        "export LANG='en_US.UTF-8';export LANGUAGE='en_US';"
                        "nmcli connection up '" + wifiName + "' > " + tmpPath;
                Utils::m_system(connect_cmdStr.toUtf8().data());
            }
        } else {
            QString cmd;
            if (is_hidden) {
                cmd = "nmcli device wifi connect '" + wifiName
                        + "' password '" + wifiPassword + "' hidden yes > " + tmpPath + " 2>&1";
            } else {
                cmd = "nmcli device wifi connect '" + wifiName
                        + "' password '" + wifiPassword + "' > " + tmpPath + " 2>&1";
            }

            int status = Utils::m_system(cmd.toUtf8().data());
            qDebug() << Q_FUNC_INFO << cmd << tmpPath << "res=" << status;
        }

        QFile file(tmpPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"Can't open the file!";
        }
        QString text = file.readAll();
        qDebug() << text;
        file.close();
        if (text.indexOf("Scanning not allowed") != -1 || text.isEmpty() || text.indexOf("No network with SSID") != -1) {
            x = 1;
            sleep(10);//nm扫描冷却为10s
        } else if (text.indexOf("Secrets were required, but not provided.") != -1) {
            emit connDone(7);
            QString delete_cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection delete '" + wifiName + "'";
            Utils::m_system(delete_cmd.toUtf8().data());
            x = 0;
        } else if (text.indexOf("Error: 802-11-wireless-security.psk: ????.") != -1) {
            emit connDone(8);
            x = 0;
        } else if (text.indexOf("Passwords or encryption keys are required") != -1) {
            emit connDone(9);
            QString delete_cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection delete '" + wifiName + "'";
            Utils::m_system(delete_cmd.toUtf8().data());
            x = 0;
        } else {
            emit connDone(6);
            x = 0;
        }
    } while (x);

    emit btFinish();
}

void BackThread::execConnRememberedHiddenWifi(QString wifiName)
{
    QProcess shellProcess;
    shellProcess.start("nmcli -f ssid device wifi");
    shellProcess.waitForFinished(3000); // 等待最多3s
    if (shellProcess.exitCode() == 0) {
        QString shellOutput = shellProcess.readAllStandardOutput();
        QStringList wlist = shellOutput.split("\n");
        bool is_hidden  = true;
        foreach (QString wifi, wlist) {
            if (wifi.trimmed() == wifiName) {
                is_hidden = false;
            }
        }
        if (! is_hidden) {
            QString cmd = "nmcli connection up '" + wifiName + "'";
            qDebug()<<"Trying to connect wifi. ssid="<<wifiName;
            int res = Utils::m_system(cmd.toUtf8().data());
            if (res == 0) {
                emit connDone(6);
            } else {
                emit connDone(res);
            }
        } else {
            //已保存的wifi没有在wifi列表找到（隐藏wifi保存后也会出现在wifi列表），则当前区域无法连接此wifi
            qDebug() << "Choosen wifi can not be sacnned, wifiName=" << wifiName;
            emit connDone(5);
        }
    }
    emit btFinish();
}
void BackThread::execConnWifiPsk(QString cmd)
{
    int res = Utils::m_system(cmd.toUtf8().data());
    emit connDone(res);
}

void BackThread::getTheWifiCardName()
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
           qDebug()<<Q_FUNC_INFO <<" Result of executing dbus method 'Introspect' is invalid";
       }

       if (reply.value().indexOf("org.freedesktop.NetworkManager.Device.Wireless") != -1) {
           //表明有wifi设备
           wifiCardPaths.append(obj_path);
       }
   }

   if (wifiCardPaths.size() == 0) {
       wifiIfnameInUse = "";
       return;
   }

   QDBusInterface lanInterface( "org.freedesktop.NetworkManager",
                             wifiCardPaths.at(0).path(),
                             "org.freedesktop.DBus.Properties",
                             QDBusConnection::systemBus() );

   QDBusReply<QVariant> lanReply = lanInterface.call("Get", "org.freedesktop.NetworkManager.Device", "Interface");
   if (!lanReply.isValid()) {
       qDebug()<<"can not get the attribute 'Interface' in func getWirelessCardName()";
   } else {
       wifiIfnameInUse = lanReply.value().toString();
   }
}

//to connected wireless network driectly do not need a password
void BackThread::execConnWifi(QString connName, QString connIfName)
{
    qDebug() << "Will to connect wifi " << connName << " with wifi card named " << connIfName;

    QString cmdStr;
    KylinDBus objBackThreadDBus;
    QString wifiUuid = objBackThreadDBus.checkHasWifiConfigFile(connName);
    if (!wifiUuid.isEmpty()) {
        //有配置文件
        cmdStr = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + wifiUuid + "' ifname '" + connIfName + "'\n";
    } else {
        //没有配置文件
        cmdStr = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "' ifname '" + connIfName + "'\n";
    }
    qDebug() << "Trying to connect wifi. cmd=" << cmdStr;

    QStringList options;
    options << "-c" << cmdStr;
    cmdProcessWifi->start("/bin/bash",options);
    cmdProcessWifi->waitForStarted();
    cmdProcessWifi->waitForFinished();
}

void BackThread::execReconnWIfi(QString uuid)
{
    QString cmd = "nmcli connection down " + uuid;
    Utils::m_system(cmd.toUtf8().data());
    cmd = "nmcli connection up " + uuid;
    Utils::m_system(cmd.toUtf8().data());
    qDebug()<<"Trying to connect wifi. uuid="<<uuid;
}

void BackThread::onReadOutputWifi()
{
    QString str = cmdProcessWifi->readAllStandardOutput();
    qDebug()<<"on_readoutput_wifi:  "<< str;
    dellConnectWifiResult(str);
}
void BackThread::onReadErrorWifi()
{
    QString str = cmdProcessWifi->readAllStandardError();
    qDebug()<<"on_readerror_wifi: "<< str;
    dellConnectWifiResult(str);
}

void BackThread::dellConnectWifiResult(QString info)
{
    if (info.indexOf("successfully") != -1) {
        emit connDone(0);
    } else if(info.indexOf("unknown") != -1 || info.indexOf("not exist") != -1) {
        //qDebug() << "send this signal if the network we want to connect has not a configuration file";
        emit connDone(2);
    } else if (info.indexOf("The connection was not a Wi-Fi connection..") != -1) {
        emit connDone(2);
    } else if(info.indexOf("not given") != -1 || info.indexOf("Secrets were required") != -1) {
        //nothing to do
    } else if(info.indexOf("Passwords or encryption keys are required") != -1){
        //qDebug() << "password for '802-11-wireless-security.psk' not given in 'passwd-file'";
        emit connDone(4);
    } else if(info.indexOf("No network with SSID") != -1){
        //nothing to do
    } else {
        //qDebug() << "send this signal if connect net failed";
        emit connDone(1);
    }

    emit btFinish();
}


//get property of connected network
QString BackThread::getConnProp(QString connName)
{
    QString tmpPath = "/tmp/kylin-nm-connprop-" + QDir::home().dirName();
    QString cmd = "nmcli connection show '" + connName + "' > " + tmpPath;
    Utils::m_system(cmd.toUtf8().data());

    QFile file(tmpPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can't open the file /tmp/kylin-nm-connprop!";
    }

    QString txt = file.readAll();
    QStringList txtLine = txt.split("\n");
    file.close();

    QString rtn = "";
    foreach (QString line, txtLine) {
        if (line.startsWith("ipv4.method:")) {
            QString v4method = line.mid(12).trimmed();
            rtn += "method:" + v4method + "|";
        }

        if (line.startsWith("ipv4.addresses:")) {
            QString value = line.mid(15).trimmed();
            if (value == "--" || value == "") {
                rtn += "v4addr:|mask:|";
            } else {
                QString addr = value.split("/").at(0);
                QString mask = value.trimmed().split("/").at(1);
                rtn += "v4addr:" + addr + "|";
                rtn += "mask:" + mask + "|";
            }
        }

        if (line.startsWith("ipv6.method:")) {
            QString value = line.mid(12).trimmed();
            if (value == "auto") {
                rtn += "v6method:auto|v6addr:|";
            } else {
                rtn += "v6method:manual|";
            }
        }

        if (line.startsWith("ipv6.addresses:")) {
            QString value = line.mid(15).trimmed();
            if (value == "--" || value == "") {
                rtn += "v6addr:|";
            } else {
                QString addr = value.split("/").at(0);
                rtn += "v6addr:" + addr + "|";
            }
        }


        if (line.startsWith("ipv4.gateway:")) {
            QString value = line.mid(13).trimmed();
            if (value == "--" || value == "") {
                rtn += "gateway:|";
            } else {
                rtn += "gateway:" + value + "|";
            }
        }

        if (line.startsWith("ipv4.dns:")) {
            QString value = line.mid(9).trimmed();
            if (value == "--" || value == "") {
                rtn += "dns:|";
            } else {
                rtn += "dns:" + value + "|";
            }
        }

        if (line.startsWith("connection.type:")) {
            QString value = line.mid(16).trimmed();
            if (value == "--" || value == "") {
                rtn += "type:|";
            } else {
                rtn += "type:" + value + "|";
            }
        }
    }

    return rtn.left(rtn.length() - 1);
}

//get band width of wired network
QString BackThread::execChkLanWidth(QString ethName)
{
    QString tmpPath = "/tmp/kylin-nm-bandwidth-" + QDir::home().dirName();
    QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';ethtool '" + ethName + "' | grep Speed > " + tmpPath;
    int res = Utils::m_system(cmd.toUtf8().data());
    qDebug() << "executed cmd=" << cmd << ".res="<<res;

    QFile file(tmpPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can't open the file /tmp/kylin-nm-bandwidth!";
    }
    QString line = file.readLine();
    file.close();

    QStringList params = line.split(":");
    if (params.size() < 2) {
        return "";
    }

    QString rtn = params.at(1);
    return rtn.trimmed();
}

//disconnected spare ethernet or wifi
void BackThread::disConnSparedNetSlot(QString type)
{
    if (type == "wifi") {
        //disConnLanOrWifi("wifi");
    } else if(type == "ethernet") {
        sleep(1);
        disConnLanOrWifi("ethernet");
    }
}

//disconnected ethernet or wifi according to network type
void BackThread::disConnLanOrWifi(QString type)
{
    QString strSlist;
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];

    FILE * p_file = NULL;

    p_file = popen("nmcli connection show -active", "r");
    if (!p_file) {
        qDebug() << "Error occurred when popen cmd 'nmcli connection show";
    }

    while (fgets(buf, BUF_SIZE, p_file) != NULL) {
        QString line(buf);

        if (line.indexOf("802-11-wireless") != -1) {
            if (type == "wifi") {
                type = "802-11-wireless";
            }
        }

        if (line.indexOf("802-3-ethernet") != -1) {
            if (type == "ethernet") {
                type = "802-3-ethernet";
            }
        }

        if (line.indexOf(type) != -1) {
            QStringList subLine = line.split(" ");
            if (subLine[1].size() == 1) {
                strSlist =  subLine[0]+ " " + subLine[1];
            } else {
                strSlist =  subLine[0];
            }
            kylin_network_set_con_down(strSlist.toUtf8().data());
        }
    }
    emit btFinish();
    pclose(p_file);
}
