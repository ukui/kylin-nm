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
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <QFile>
#include <QRegExp>
#include <QDir>

BackThread::BackThread(QObject *parent) : QObject(parent)
{
    cmdConnWifi = new QProcess(this);
    connect(cmdConnWifi , SIGNAL(readyReadStandardOutput()) , this , SLOT(onReadOutputWifi()));
    connect(cmdConnWifi , SIGNAL(readyReadStandardError()) , this , SLOT(onReadErrorWifi()));
    cmdConnWifi->start("/bin/bash");
    cmdConnWifi->waitForStarted();

    process = new QProcess(this);
    connect(process , SIGNAL(readyReadStandardOutput()) , this , SLOT(onReadOutputLan()));
    connect(process , SIGNAL(readyReadStandardError()) , this , SLOT(onReadErrorLan()));
}

BackThread::~BackThread()
{
    cmdConnWifi->close();
    process->close();
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
        // print information if can not open file ~/.config/kylin-nm-iface
        syslog(LOG_ERR, "Can't open the file ~/.config/kylin-nm-iface!");
        qDebug()<<"Can't open the file ~/.config/kylin-nm-iface!";
    }
    QString txt = file.readAll();
    QStringList txtList = txt.split("\n");
    file.close();

    iface->lstate = 2;
    iface->wstate = 2;

    for (int i = 1; i < txtList.size(); i ++) {
        QString line = txtList.at(i);
        if (line != "") {
            int index1 = line.indexOf(" ");
            QString type = line.left(index1);
            QString lastStr = line.mid(index1).trimmed();
            int index2 = lastStr.indexOf(" ");
            QString iname = lastStr.left(index2);
            QString istateStr = lastStr.mid(index2).trimmed();

            //只要存在一个有线设备已连接，就不再扫描其他有线设备状态，避免有线被误断开
            if (type == "ethernet" && iface->lstate != 0) {
                // if type is wired network
                iface->lname = iname;

                if (istateStr == "unmanaged") {
                    iface->lstate = 2; //switch of wired device is off
                } else if (istateStr == "disconnected" || istateStr == "unavailable") {
                    iface->lstate = 1; //wired network is disconnected
                } else if (istateStr == "connected" || istateStr == "connecting (getting IP configuration)") {
                    iface->lstate = 0; //wired network is connected
                } else {
                    //连接中，正在配置
                    iface->lstate = 3;
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

//turn on the switch of network
void BackThread::execEnNet()
{
    char *chr = "nmcli networking on";
    Utils::m_system(chr);

    while (1) {
        if (execGetIface()->lstate != 2) {
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
        if (execGetIface()->lstate == 2) {
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

    while (1) {
        if (execGetIface()->wstate != 2) {
            KylinDBus objKyDbus;
            while (1) {
                if (objKyDbus.getAccessPointsNumber() > 0) {
                    // objKyDbus.getAccessPointsNumber()>0 standard can get wireless accesspoints now
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

//to connect wired network
void BackThread::execConnLan(QString connName, QString ifname, QString connectType)
{
    currConnLanUuid = connName;
    currConnLanType = connectType;
    QString mycmd; //连接命令
    KylinDBus objKyDbus;

    //先断开当前网卡对应的已连接有线网
//    QString uuid = objKyDbus.getConnLanNameByIfname(ifname);
//    if (uuid != "--") {
//        kylin_network_set_con_down(uuid.toUtf8().data());
//    }

    bool wiredCableState = objKyDbus.getWiredCableStateByIfname(ifname);

    if (connectType == "bluetooth") {
        wiredCableState = true; //对于蓝牙类型的网络不需要接入网线就可以连接
        mycmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "'";
    } else {
        mycmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "' ifname '" + ifname + "'";
    }

    if (wiredCableState) {
        //if(objKyDbus.toConnectWiredNet(connName, ifname)) { //此处connName是有线网Uuid
        //    emit connDone(2);
        //} else {
        //    emit connDone(8);
        //}
        QStringList options;
        options << "-c" << mycmd;
        process->start("/bin/bash",options);
        process->waitForStarted();
        process->waitForFinished();
    } else {
        qDebug()<<"connect wired network failed for without wired cable plug in.";
        syslog(LOG_DEBUG, "connect wired network failed for without wired cable plug in.");
        emit connDone(1);
    }

    emit btFinish();
}

void BackThread::onReadOutputLan()
{
    QByteArray cmdout = process->readAllStandardOutput();
    QString strResult = QString::fromLocal8Bit(cmdout);
    qDebug()<<"on_readoutput_lan:  "<< strResult;
    dellConnectLanResult(strResult);
}
void BackThread::onReadErrorLan()
{
    QByteArray cmdout = process->readAllStandardError();
    QString strResult = QString::fromLocal8Bit(cmdout);
    qDebug()<<"on_readerror_lan:  "<< strResult;
    dellConnectLanResult(strResult);
}

void BackThread::dellConnectLanResult(QString info)
{
    if (info.indexOf("successfully") != -1) {
        qDebug()<<"debug: in function execConnLan, wired net state is: "<<QString::number(execGetIface()->lstate);
        syslog(LOG_DEBUG, "In function execConnLan, wired net state is: %d", execGetIface()->lstate);

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
            emit connDone(9);
        }
    }
}

//to connected wireless network need a password
void BackThread::execConnWifiPWD(QString connName, QString password, QString connType)
{
    //disConnLanOrWifi("wifi");

    if (!connType.isEmpty()) {
        QString strConntype = "nmcli connection modify '" + connName + "' wifi-sec.psk-flags 0";
        Utils::m_system(strConntype.toUtf8().data());
    }

    QString tmpPath = "/tmp/kylin-nm-btoutput-" + QDir::home().dirName();
    QString cmdStr = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli device wifi connect '" + connName + "' password '" + password + "' > " + tmpPath;
    Utils::m_system(cmdStr.toUtf8().data());

    QFile file(tmpPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        syslog(LOG_DEBUG, "Can't open the file /tmp/kylin-nm-btoutput !");
        qDebug()<<"Can't open the file /tmp/kylin-nm-btoutput !"<<endl;
    }
    QString line = file.readLine();
    file.close();
    qDebug()<<"connect_wifi_result: "<< line;

    if (line.indexOf("successfully") != -1) {
        emit connDone(0);
        qDebug()<<"debug: in function execConnWifiPWD, wireless net state is: "<<QString::number(execGetIface()->wstate);
        syslog(LOG_DEBUG, "In function execConnWifiPWD, wireless net state is: %d", execGetIface()->wstate);
    } else if(line.indexOf("Secrets were required") != -1){
        //emit connDone(4);//发出信号4是之前添加每次连接输入密码的功能时需要的
        emit connDone(1);
    } else {
        emit connDone(1);
    }

    emit btFinish();
}

void BackThread::execConnHiddenWifiWPA(QString wifiName, QString wifiPassword)
{
    int x(1), n(0);
    do {
        n += 1;
        if (n >= 3) {
            syslog(LOG_ERR, "connection attempt of hidden wifi %s failed for 3 times, no more attempt", wifiName);
            x = 0;
            emit connDone(1);
        }

        QString tmpPath = "/tmp/kylin-nm-btoutput-" + QDir::home().dirName();
        QString cmd = "nmcli device wifi connect '" + wifiName + "' password '" + wifiPassword + "' hidden yes > " + tmpPath + " 2>&1";

//                qDebug() << Q_FUNC_INFO << cmd << tmpPath;
        int status = Utils::m_system(cmd.toUtf8().data());
        if (status != 0) {
            syslog(LOG_ERR, "execute 'nmcli device wifi connect' in function 'on_btnConnect_clicked' failed");
        }

        QFile file(tmpPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"Can't open the file!"<<endl;
        }
        QString text = file.readAll();
        file.close();
        if(text.indexOf("Scanning not allowed") != -1
           || text.isEmpty()
           || text.indexOf("No network with SSID") != -1){
            x = 1;
            sleep(15);//nm扫描冷却为10s
        } else {
            emit connDone(0);
            x = 0;
        }
//                qDebug() << Q_FUNC_INFO << x << text;
    } while (x == 1);

    emit btFinish();
}

void BackThread::execConnRememberedHiddenWifi(QString wifiName)
{
    QProcess shellProcess;
    shellProcess.start("nmcli -f ssid device wifi");
    shellProcess.waitForFinished(3000); // 等待最多3s
    if (shellProcess.exitCode() == 0)
    {
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
            int res = Utils::m_system(cmd.toUtf8().data());
            emit connDone(res);
        } else {
            //已保存的wifi没有在wifi列表找到（隐藏wifi保存后也会出现在wifi列表），则当前区域无法连接此wifi
            syslog(LOG_DEBUG, "Choosen wifi can not be sacnned in finishedProcess() in dlghidewifiwpa.cpp 377.");
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

//to connected wireless network driectly do not need a password
void BackThread::execConnWifi(QString connName)
{
    //disConnLanOrWifi("wifi");

    QString cmdStr = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "'\n";
    cmdConnWifi->write(cmdStr.toUtf8().data());
}

void BackThread::execReconnWIfi(QString uuid)
{
    QString cmd = "nmcli connection down " + uuid;
    Utils::m_system(cmd.toUtf8().data());
   cmd = "nmcli connection up " + uuid;
    Utils::m_system(cmd.toUtf8().data());
}

void BackThread::onReadOutputWifi()
{
    QString str = cmdConnWifi->readAllStandardOutput();
    qDebug()<<"on_readoutput_wifi:  "<< str;
    syslog(LOG_DEBUG, "on_readoutput_wifi : %s", str.toUtf8().data());
    dellConnectWifiResult(str);
}
void BackThread::onReadErrorWifi()
{
    QString str = cmdConnWifi->readAllStandardError();
    qDebug()<<"on_readerror_wifi: "<< str;
    syslog(LOG_DEBUG, "on_readerror_wifi : %s", str.toUtf8().data());
    dellConnectWifiResult(str);
}

void BackThread::dellConnectWifiResult(QString info)
{
    if (info.indexOf("successfully") != -1) {
        emit connDone(0);
    } else if(info.indexOf("unknown") != -1 || info.indexOf("not exist") != -1) {
        //qDebug() << "send this signal if the network we want to connect has not a configuration file";
        emit connDone(2);
    } else if(info.indexOf("not given") != -1 || info.indexOf("Secrets were required") != -1) {
        //no need to handle this situation
    } else if(info.indexOf("Passwords or encryption keys are required") != -1){
        //qDebug() << "password for '802-11-wireless-security.psk' not given in 'passwd-file'";
        emit connDone(4);
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
        syslog(LOG_ERR, "Can't open the file /tmp/kylin-nm-connprop!");
        qDebug()<<"Can't open the file /tmp/kylin-nm-connprop!"<<endl;
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
    }

    return rtn.left(rtn.length() - 1);
}

//get band width of wired network
QString BackThread::execChkLanWidth(QString ethName)
{
    QString tmpPath = "/tmp/kylin-nm-bandwidth-" + QDir::home().dirName();
    QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';ethtool '" + ethName + "' | grep Speed > " + tmpPath;
    Utils::m_system(cmd.toUtf8().data());

    QFile file(tmpPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        syslog(LOG_ERR, "Can't open the file /tmp/kylin-nm-bandwidth!");
        qDebug()<<"Can't open the file /tmp/kylin-nm-bandwidth!"<<endl;
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
    sleep(1);
    if (type == "wifi") {
        //disConnLanOrWifi("wifi");
    } else if(type == "ethernet") {
        disConnLanOrWifi("ethernet");
    }

    emit disFinish();
    emit ttFinish();
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
        syslog(LOG_ERR, "Error occurred when popen cmd 'nmcli connection show'");
        qDebug()<<"Error occurred when popen cmd 'nmcli connection show";
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
