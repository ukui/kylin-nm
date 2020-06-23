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
    connect(cmdConnWifi , SIGNAL(readyReadStandardOutput()) , this , SLOT(on_readoutput()));
    connect(cmdConnWifi , SIGNAL(readyReadStandardError()) , this , SLOT(on_readerror()));
    cmdConnWifi->start("bash");
    cmdConnWifi->waitForStarted();
}

BackThread::~BackThread()
{
    cmdConnWifi->close();
}

//get the connection state of wired and wireles network
IFace* BackThread::execGetIface()
{
    IFace *iface = new IFace();

    QString tmpPath = "/tmp/kylin-nm-iface-" + QDir::home().dirName();
    QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli -f TYPE,DEVICE,STATE device > " + tmpPath;
    Utils::m_system(cmd.toUtf8().data());

    // int status = system(cmd.toUtf8().data());
    // if (status != 0){ syslog(LOG_ERR, "execute 'nmcli device' in function 'execGetIface' failed");}

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

            if (type == "ethernet") {
                // if type is wired network
                iface->lname = iname;

                if (istateStr == "unmanaged") {
                    iface->lstate = 2; //switch of wired device is off
                }
                if (istateStr == "disconnected" || istateStr == "unavailable") {
                    iface->lstate = 1; //wired network is disconnected
                }
                if (istateStr == "connected") {
                    iface->lstate = 0; //wired network is connected
                }
            }
            if (type == "wifi") {
                // if type is wireless network
                iface->wname = iname;

                if (istateStr == "unmanaged" || istateStr == "unavailable") {
                    iface->wstate = 2; //switch of wireless device is off
                }
                if (istateStr == "disconnected") {
                    iface->wstate = 1; //wireless network is disconnected
                }
                if (istateStr == "connected") {
                    iface->wstate = 0; //wireless network is connected
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

    // int status = system("nmcli networking on");
    // if (status != 0){ syslog(LOG_ERR, "execute 'nmcli networking on' in function 'execEnNet' failed");}
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

        // int status = system("nmcli radio wifi off");
        // if (status != 0){ syslog(LOG_ERR, "execute 'nmcli radio wifi off' in function 'execDisNet' failed");}
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
    // int status1 = system("nmcli networking off");
    // if (status1 != 0){ syslog(LOG_ERR, "execute 'nmcli networking off' in function 'execDisNet' failed");}
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
    //if (execGetIface()->lstate == 2){
    //    char *chr = "nmcli networking on";
    //    Utils::m_system(chr);
    //    //int status = system("nmcli networking on");
    //    //if (status != 0){ syslog(LOG_ERR, "execute 'nmcli networking on' in function 'execEnWifi' failed");}
    //    while(1){
    //        if (execGetIface()->lstate != 2){
    //            emit launchLanDone();
    //            break;
    //        }
    //        sleep(1);
    //    }
    //}

    char *chr1 = "nmcli radio wifi on";
    Utils::m_system(chr1);
    //int status1 = system("nmcli radio wifi on");
    //if (status1 != 0){ syslog(LOG_ERR, "execute 'nmcli radio wifi on' in function 'execEnWifi' failed");}
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
    // int status = system("nmcli radio wifi off");
    // if (status != 0){ syslog(LOG_ERR, "execute 'nmcli radio wifi off' in function 'execDisWifi' failed");}
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
void BackThread::execConnLan(QString connName)
{
    disConnLanOrWifi("ethernet");

    KylinDBus objKyDbus;
    if (objKyDbus.isWiredCableOn) {
        // only if wired cable is plug in, can connect wired network
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "'";
        Utils::m_system(cmd.toUtf8().data());
        // int status = system(cmd.toUtf8().data());
        // if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection up' in function 'execConnLan' failed");}
        qDebug()<<"debug: in function execConnLan, wired net state is: "<<QString::number(execGetIface()->lstate);
        syslog(LOG_DEBUG, "In function execConnLan, wired net state is: %d", execGetIface()->lstate);
        emit connDone(0);
    } else {
        emit connDone(1);
    }

    emit btFinish();
}

//to connected wireless network need a password
void BackThread::execConnWifiPWD(QString connName, QString password)
{
    //disConnLanOrWifi("wifi");

    QString tmpPath = "/tmp/kylin-nm-btoutput-" + QDir::home().dirName();
    QString cmdStr = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli device wifi connect '" + connName + "' password '" + password + "' > " + tmpPath;
    Utils::m_system(cmdStr.toUtf8().data());
    // int status =  system(cmdStr.toUtf8().data());
    // if (status != 0){ syslog(LOG_ERR, "execute 'nmcli device wifi connect' in function 'execConnWifiPWD' failed");}

    QFile file(tmpPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        syslog(LOG_DEBUG, "Can't open the file /tmp/kylin-nm-btoutput !");
        qDebug()<<"Can't open the file /tmp/kylin-nm-btoutput !"<<endl;
    }
    QString line = file.readLine();
    file.close();

    if (line.indexOf("successfully") != -1) {
        emit connDone(0);
        qDebug()<<"debug: in function execConnWifiPWD, wireless net state is: "<<QString::number(execGetIface()->wstate);
        syslog(LOG_DEBUG, "In function execConnWifiPWD, wireless net state is: %d", execGetIface()->wstate);
    } else {
        emit connDone(1);
    }

    emit btFinish();
}

//to connected wireless network driectly do not need a password
void BackThread::execConnWifi(QString connName)
{
    //disConnLanOrWifi("wifi");

    QString cmdStr = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "'\n";
    cmdConnWifi->write(cmdStr.toUtf8().data());
}

void BackThread::on_readoutput()
{
    QString str = cmdConnWifi->readAllStandardOutput();
    cmdConnWifi->close();
    qDebug()<<"on_readoutput:  "<< str;
    if (str.indexOf("successfully") != -1) {
        emit connDone(0); //send this signal if connect net successfully
        qDebug()<<"debug: in function on_readoutput, wireless net state is: "<<QString::number(execGetIface()->wstate);
        syslog(LOG_DEBUG, "In function on_readoutput, wireless net state is: %d", execGetIface()->wstate);
    } else if(str.indexOf("unknown") != -1) {
        emit connDone(2);
    } else {
        emit connDone(1);
    }

    emit btFinish();
}
void BackThread::on_readerror()
{
    QString str = cmdConnWifi->readAllStandardError();
    cmdConnWifi->close();
    qDebug()<<"on_readerror: "<< str;
    if (str.indexOf("successfully") != -1) {
        emit connDone(0);
    } else if(str.indexOf("unknown") != -1 || str.indexOf("not exist") != -1) {
        emit connDone(2); //send this signal if the network we want to connect has not a configuration file
    } else {
        emit connDone(1); //send this signal if connect net failed
    }

    emit btFinish();
}

//get property of connected network
QString BackThread::getConnProp(QString connName)
{
    QString tmpPath = "/tmp/kylin-nm-connprop-" + QDir::home().dirName();
    QString cmd = "nmcli connection show '" + connName + "' > " + tmpPath;
    Utils::m_system(cmd.toUtf8().data());
    // int status = system(cmd.toUtf8().data());
    // if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection show' in function 'getConnProp' failed");}

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
                rtn += "addr:|mask:|";
            } else {
                QString addr = value.split("/").at(0);
                QString mask = value.trimmed().split("/").at(1);
                rtn += "addr:" + addr + "|";
                rtn += "mask:" + mask + "|";
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
    // int status = system(cmd.toUtf8().data());
    // if (status != 0){ syslog(LOG_ERR, "execute 'ethtool' in function 'execChkLanWidth' failed");}

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
        disConnLanOrWifi("wifi");
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
    pclose(p_file);
}
