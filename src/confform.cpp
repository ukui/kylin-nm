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

#include "confform.h"
#include "ui_confform.h"
#include "kylin-network-interface.h"
#include "backthread.h"
#include "utils.h"

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <KWindowEffects>

extern QString llname, lwname;

ConfForm::ConfForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfForm)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint); //Qt::WindowStaysOnTopHint
    this->setWindowTitle(tr("edit network"));//"网络设置"
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowIcon(QIcon::fromTheme("kylin-network", QIcon(":/res/x/setup.png")) );
    //需要添加 void paintEvent(QPaintEvent *event) 函数
    //this->setWindowIcon(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/x/setup.png")) );

    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));

    labelQss = "QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}";
    cbxQss = "QComboBox{padding-left:20px;font-size:13px;color:rgba(255,255,255,0.91);"
                        "border:1px solid rgba(255, 255, 255, 0.05);border-radius:4px;background:rgba(255,255,255,0.08);}"
             "QComboBox::drop-down{border:0px;width:30px;}"
             "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}"
             "QComboBox QAbstractItemView {margin:0px 0px 0px 0px;padding: 0px 0px;border-radius:0px;background-color:#48484C;outline:0px;}"
             "QComboBox QAbstractItemView::item{padding-left:17px;border-radius:0px;font-size:13px;color:rgba(255,255,255,0.91);height: 32px;background-color:#48484C;outline:0px;}"
             "QComboBox QAbstractItemView::item:hover{padding-left:17px;border-radius:0px;font-size:13px;color:rgba(255,255,255,0.91);background-color:#3D6BE5;outline:0px;}";
    leQss = "QLineEdit{padding-left:20px;color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}";
    btnOffQss = "QPushButton[on=false]{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.08);color:white;font-size:14px;}"
                   "QPushButton[on=false]:Hover{border:0px solid rgba(255,255,255,0.1);border-radius:4px;background-color:rgba(255,255,255,0.1);}"
                   "QPushButton[on=false]:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}"
                "QPushButton[on=true]{border:0px;border-radius:4px;background-color:rgba(244,244,244,0.12);color:white;font-size:14px;}"
                "QPushButton[on=true]:Hover{border:0px solid rgba(244,244,244,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                "QPushButton[on=true]:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}";
    btnOnQss = "QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);";
    lineQss = "background:rgba(156,156,156,0.1);";

    ui->wdHead->setStyleSheet("#wdHead{border:none}");
    ui->wgManual->setStyleSheet("#wgManual{border:none}");
    ui->wdBottom->setStyleSheet("#wdBottom{border:none}");
    ui->lbLeftupTitle->setStyleSheet("QLabel{font-size:20px;}");
    ui->leAddr->setContextMenuPolicy(Qt::NoContextMenu);
    ui->leName->setContextMenuPolicy(Qt::NoContextMenu);
    ui->leDns->setContextMenuPolicy(Qt::NoContextMenu);
    ui->leDns2->setContextMenuPolicy(Qt::NoContextMenu);
    ui->leGateway->setContextMenuPolicy(Qt::NoContextMenu);

    ui->lineUp->setStyleSheet(lineQss);
    ui->lineDown->setStyleSheet(lineQss);
    ui->lineUp->hide();
    ui->lineDown->hide();

    ui->lbName->setText(tr("LAN name: "));//"网络名称："
    ui->lbTxt1->setText(tr("Method: "));//"编辑IP设置："
    ui->lbTxt2->setText(tr("Address: "));//"IP地址："
    ui->lbTxt3->setText(tr("Netmask: "));//"子网掩码："
    ui->lbTxt4->setText(tr("Gateway: "));//"默认网关："
    ui->lbTxt5->setText(tr("DNS 1: "));//"首选DNS："
    ui->lbTxt6->setText(tr("DNS 2: "));//"备选DNS："
    ui->lbTxt_ipv6->setText(tr("Ipv6 Address: "));

    ui->lbLeftupTitle->setText(tr("Edit Conn"));//"网络设置"
    ui->cbType->addItem(tr("Auto(DHCP)"));//"自动(DHCP)"
    ui->cbType->addItem(tr("Manual"));//"手动"

    connect(ui->cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(cbTypeChanged(int)));

    ui->cbMask->addItem("255.255.255.0"); //24
    ui->cbMask->addItem("255.255.254.0"); //23
    ui->cbMask->addItem("255.255.252.0"); //22
    ui->cbMask->addItem("255.255.0.0"); //16
    ui->cbMask->addItem("255.0.0.0"); //8

    ui->btnCancel->setText(tr("Cancel"));//"取消"
    ui->btnSave->setText(tr("Save"));//"保存"
    ui->btnCreate->setText(tr("Ok"));//"确定"

    ui->btnCancel->setProperty("on",true);
    ui->btnSave->setProperty("on",false);
    ui->btnCreate->setProperty("on",false);

    ui->btnCancel->setStyleSheet(btnOffQss);
    ui->btnSave->setStyleSheet(btnOffQss);
    ui->btnCreate->setStyleSheet(btnOffQss);

    ui->btnCancel->setFocusPolicy(Qt::NoFocus);
    ui->btnSave->setFocusPolicy(Qt::NoFocus);
    ui->btnCreate->setFocusPolicy(Qt::NoFocus);

    // IP的正则格式限制
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    QRegExp ipv6_rx("^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$");
    ui->leAddr->setValidator(new QRegExpValidator(rx, this));
    ui->leGateway->setValidator(new QRegExpValidator(rx, this));
    ui->leDns->setValidator(new QRegExpValidator(rx, this));
    ui->leDns2->setValidator(new QRegExpValidator(rx, this));
    ui->leAddr_ipv6->setValidator(new QRegExpValidator(ipv6_rx, this));
    setModal(false);

    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
}

ConfForm::~ConfForm()
{
    delete ui;
}

void ConfForm::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
    return QDialog::mousePressEvent(event);
}
void ConfForm::mouseReleaseEvent(QMouseEvent *event)
{
    this->isPress = false;
}
void ConfForm::mouseMoveEvent(QMouseEvent *event)
{
    if (this->isPress) {
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
    return QDialog::mouseMoveEvent(event);
}

//网络配置参数设置界面的显示内容
void ConfForm::setProp(QString connName, QString uuidName, QString v4method, QString v4addr, QString v6method, QString v6addr, QString mask, QString gateway, QString dns, bool isActConf, bool isWiFi)
{
    this->isActConf = isActConf;
    ui->leName->setText(connName);
    lastConnName = connName;
    lastIpv4 = v4addr;
    netUuid = uuidName;

    if (isWiFi) {
        ui->leName->setEnabled(false);
        isActWifi = isWiFi;
    }

    if (v4method == "auto" || v4method == "") {
        ui->cbType->setCurrentIndex(0);
        cbTypeChanged(0);
    } else {
        ui->cbType->setCurrentIndex(1);
        cbTypeChanged(1);
    }

    if (v6method == "manual") {
        ui->leAddr_ipv6->setText(v6addr);
    }

    ui->leAddr->setText(v4addr);
    ui->leGateway->setText(gateway);

    // 配置中有多个DNS，只处理前两个
    if (dns.indexOf(",") != -1) {
        QStringList dnss = dns.split(",");
        ui->leDns->setText(dnss.at(0));
        ui->leDns2->setText(dnss.at(1));
    } else {
        ui->leDns->setText(dns);
        ui->leDns2->setText("");
    }

    if (mask == "24") {
        ui->cbMask->setCurrentIndex(0);
    } else if(mask == "23") {
        ui->cbMask->setCurrentIndex(1);
    } else if(mask == "22") {
        ui->cbMask->setCurrentIndex(2);
    } else if(mask == "16") {
        ui->cbMask->setCurrentIndex(3);
    } else if(mask == "8") {
        ui->cbMask->setCurrentIndex(4);
    } else {
        ui->cbMask->setCurrentIndex(0);
    }
}

//点击了创建新的网络的按钮
void ConfForm::on_btnCreate_clicked()
{
    KylinDBus kylindbus;
    kylindbus.getWiredCardName();
    QString mIfname;

    if (kylindbus.multiWiredIfName.size() == 0) {
        QString tip(tr("Can not create new wired network for without wired card"));
        kylindbus.showDesktopNotify(tip);
        //this->close();
        this->hide();
        return;
    } else {
        mIfname = kylindbus.multiWiredIfName.at(0);
    }

    if (ui->cbType->currentIndex() == 1 && (ui->leAddr->text() != lastIpv4)) {
        //在手动配置网络的情况下以及当前的IP参数有更改的情况下，检测IP冲突
        if (check_ip_conflict(mIfname)) {
            return;
        }
    }

    QString cmdStr = "nmcli connection add con-name '" + ui->leName->text() + "' ifname '" + mIfname + "' type ethernet";
    Utils::m_system(cmdStr.toUtf8().data());

    if (ui->cbType->currentIndex() == 1) {
        //选择手动，配置Ipv4、掩码、网关
        this->isCreateNewNet = true;
        newUuid = "--";
        this->saveNetworkConfiguration();
    } else {
        //选择自动，则配置完成并发出桌面通知
        QString txt(tr("New network already created"));
        kylindbus.showDesktopNotify(txt);
    }

    QString name = ui->leName->text();
    QStringList charToEscape;
    charToEscape << "~" << "(" << ")" << "<" << ">" <<"\\" << "*" << "|" << "&" << "#";  //一些命令行特殊字符，需要转义
    foreach (auto ch , charToEscape) {
        if (name.contains(ch)) {
            name.replace(ch, QString("%1%2").arg("\\").arg(ch));
        }
    }
    if (name.contains(" ")) { //空格会影响命令行参数的判断，需要转义
        name.replace(QRegExp("[\\s]"), "\\\ ");
    }
    if (!ui->leAddr_ipv6->text().isEmpty()) {
        QString cmdStr = "nmcli connection modify " + name + " ipv6.method manual ipv6.addresses " + ui->leAddr_ipv6->text();
        Utils::m_system(cmdStr.toUtf8().data());
    } else {
        QString cmdStr = "nmcli connection modify " + name + " ipv6.method auto";
        Utils::m_system(cmdStr.toUtf8().data());
    }

    this->hide();
}

//点击了保存更改网络设置的按钮
void ConfForm::on_btnSave_clicked()
{
    KylinDBus kylindbus;

    if (isActWifi) {
        kylindbus.getWirelessCardName();
        QString mWifiIfname = kylindbus.dbusWiFiCardName;
        this->isCreateNewNet = false;

        if (ui->cbType->currentIndex() == 1 && (ui->leAddr->text() != lastIpv4)) {
            //在手动配置网络的情况下以及当前的IP参数有更改的情况下，检测IP冲突
            if (check_ip_conflict(mWifiIfname)) {
                return;
            }
        }

        this->saveNetworkConfiguration();
    } else {
        kylindbus.getWiredCardName();
        QString mIfname;

        if (kylindbus.multiWiredIfName.size() == 0) {
            QString tip(tr("Can not save wired network for without wired card"));
            kylindbus.showDesktopNotify(tip);
            //this->close();
            this->hide();
            return;
        } else {
            mIfname = kylindbus.multiWiredIfName.at(0);
        }

        //如果网络的名称已经修改，则删掉当前网络，新建一个网络
        if (ui->leName->text() != lastConnName) {
            QString cmd = "nmcli connection delete '" + netUuid + "'";
            int status = system(cmd.toUtf8().data());
            if (status != 0) {
                syslog(LOG_ERR, "execute 'nmcli connection delete' in function 'on_btnSave_clicked' failed");
            }
            //this->hide();

            //QString cmdStr = "nmcli connection add con-name '" + ui->leName->text() + "' ifname '" + mIfname + "' type ethernet";
            //Utils::m_system(cmdStr.toUtf8().data());

            this->isCreateNewNet = true;
            newUuid = "--";

            QProcess * processAdd = new QProcess;
            QString cmdAdd = "nmcli connection add con-name '" + ui->leName->text() + "' ifname '" + mIfname + "' type ethernet";
            QStringList options;
            options << "-c" << cmdAdd;
            processAdd->start("/bin/bash",options);
            connect(processAdd, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), this, [ = ]() {
                processAdd->deleteLater();
            });
            connect(processAdd, &QProcess::channelReadyRead, this, [ = ]() {
                QString str = processAdd->readAll();
                QString regExpPattern("[A-Za-z0-9]{8}-[A-Za-z0-9]{4}-[A-Za-z0-9]{4}-[A-Za-z0-9]{4}-[A-Za-z0-9]{12}");
                QRegExp regExpTest(regExpPattern);
                int pos = str.indexOf(regExpTest);
                newUuid = str.mid(pos,36); //36是uuid的长度

                if (ui->cbType->currentIndex() == 1 && (ui->leAddr->text() != lastIpv4)) {
                    //在手动配置网络的情况下以及当前的IP参数有更改的情况下，检测IP冲突
                    if (check_ip_conflict(mIfname)) {
                        return;
                    }
                }

                this->saveNetworkConfiguration();
            });
            processAdd->waitForFinished();
        } else {
            this->isCreateNewNet = false;

            if (ui->cbType->currentIndex() == 1 && (ui->leAddr->text() != lastIpv4)) {
                //在手动配置网络的情况下以及当前的IP参数有更改的情况下，检测IP冲突
                if (check_ip_conflict(mIfname)) {
                    return;
                }
            }

            this->saveNetworkConfiguration();
        }
    }

    if (ui->cbType->currentIndex() == 1) {
        //对于已保存连接修改ipv6地址，使用UUID区分各网络配置（排除名称含空格或特殊字符的干扰）
        if (!ui->leAddr_ipv6->text().isEmpty()) {
            QString cmdStr = "nmcli connection modify " + netUuid + " ipv6.method manual ipv6.addresses " + ui->leAddr_ipv6->text();
            Utils::m_system(cmdStr.toUtf8().data());
        } else {
            QString cmdStr = "nmcli connection modify " + netUuid + " ipv6.method auto";
            Utils::m_system(cmdStr.toUtf8().data());
        }
    }
    QString txt(tr("New network settings already finished"));
    kylindbus.showDesktopNotify(txt);
}

void ConfForm::saveNetworkConfiguration()
{
    //获取对应掩码的参数
    QString mask = "";
    if (ui->cbMask->currentIndex() == 0) {
        mask = "24";
    } else if(ui->cbMask->currentIndex() == 1) {
        mask = "23";
    } else if(ui->cbMask->currentIndex() == 2) {
        mask = "22";
    } else if(ui->cbMask->currentIndex() == 3) {
        mask = "16";
    } else if(ui->cbMask->currentIndex() == 4) {
        mask = "8";
    } else {
        mask = "24";
    }

    //是选择的自动还是手动配置网络
    if (ui->cbType->currentIndex() == 0) {
        if (!this->isCreateNewNet) {
            //kylin_network_set_automethod(ui->leName->text().toUtf8().data());
            kylin_network_set_automethod(netUuid.toUtf8().data());
        }
    } else {
        QString dnss = ui->leDns->text();
        if (ui->leDns2->text() != "") {
            dnss.append(",");
            dnss.append(ui->leDns2->text());
        }
        if (this->isCreateNewNet) {
            if (newUuid != "--") {
                kylin_network_set_manualall(newUuid.toUtf8().data(), ui->leAddr->text().toUtf8().data(), mask.toUtf8().data(), ui->leGateway->text().toUtf8().data(), dnss.toUtf8().data());
            } else {
                kylin_network_set_manualall(ui->leName->text().toUtf8().data(), ui->leAddr->text().toUtf8().data(), mask.toUtf8().data(), ui->leGateway->text().toUtf8().data(), dnss.toUtf8().data());
            }
        } else {
            kylin_network_set_manualall(netUuid.toUtf8().data(), ui->leAddr->text().toUtf8().data(), mask.toUtf8().data(), ui->leGateway->text().toUtf8().data(), dnss.toUtf8().data());
        }
    }

    this->hide();
}

bool ConfForm::check_ip_conflict(QString ifname)
{
    //即将检测Ip地址冲突
    QString strIpCheck = tr("Will check the IP address conflict");
    QString bufferIpCheck = "notify-send -i network-offline " + strIpCheck;
    system(bufferIpCheck.toUtf8().data());

    FILE *fp;
    char ret[10], arp_all[1024];

    QString arp_all_cmd = "arping -c 3 -f -I " + ifname + " -D " + ui->leAddr->text();
    fp = popen(arp_all_cmd.toUtf8().data(),"r");
    if(!fp)
        return false;
    fread(arp_all, 1, sizeof(arp_all), fp);
    pclose(fp);

    if (strstr(arp_all, "Received") && strstr(arp_all, " response(s)")) {
        QString arp_result = "arping -c 1 -f -I " + ifname + " -D " + ui->leAddr->text() + " | awk '{print $2}' | sed -n '3p'";

        fp = popen(arp_result.toUtf8().data(),"r");
        if(!fp)
            return false;
        fgets(ret,sizeof(ret),fp);
        pclose(fp);

        ret[strlen(ret)-1]=0;

        if ( ret != NULL ) {
            if (!strcmp(ret,"0")) {
                //printf("正常连接");
                return  false;
            } else {
                //printf("ip地址冲突");
                QString strInfo = tr("IP address conflict, Please change IP");
                QString buffer = "notify-send -i network-offline " + strInfo;
                system(buffer.toUtf8().data());
                return  true;
            }
        }
    }
    //printf("正常连接");
    return false;
}

//点击取消按钮
void ConfForm::on_btnCancel_clicked()
{
    //this->close();
    this->hide();
}

//根据需要设置的种类(自动或手动等)显示界面内容
void ConfForm::cbTypeChanged(int index)
{
    if (isShowSaveBtn) {
        if (!isActWifi) {
            ui->leName->setEnabled(true);
        }
        ui->btnSave->show(); //显示保存按钮
        ui->btnCreate->hide(); //隐藏创建按钮
        ui->lbLeftupTitle->setText(tr("Edit Network"));
    }

    if (index == 0) {
        ui->lineUp->hide();
        ui->lineDown->hide();
        ui->wgManual->hide();
        //ui->centralWidget->resize(432, 230);
        ui->centralWidget->setFixedSize(432, 230);
        ui->wdBottom->move(1, 170);

        this->setEnableOfBtn();

        //this->resize(432, 230);
        this->setFixedSize(432, 230);
    }
    if (index == 1) {
        ui->lineUp->show();
        ui->lineDown->show();
        ui->wgManual->show();
        //ui->centralWidget->resize(432, 510);
        ui->centralWidget->setFixedSize(432, 510);
        ui->wdBottom->move(1, 440);

        this->setEnableOfBtn();

        //this->resize(432, 510);
        this->setFixedSize(432, 510);
    }
    if (index == 3) {
        setBtnEnableFalse();

        if (!isActWifi) {
            ui->leName->setEnabled(true);
        }
        ui->btnSave->hide();
        ui->btnCreate->show();
        ui->lbLeftupTitle->setText(tr("Add Wired Network"));
        isShowSaveBtn = false;

        ui->lineUp->hide();
        ui->lineDown->hide();
        ui->wgManual->hide();
        //ui->centralWidget->resize(432, 230);
        ui->centralWidget->setFixedSize(432, 230);
        ui->wdBottom->move(1, 170);
        //this->resize(432, 230);
        this->setFixedSize(432, 230);
    }
}

//编辑网络名称
void ConfForm::on_leName_textEdited(const QString &arg1)
{
    this->setEnableOfBtn();
}

//编辑网络ip
void ConfForm::on_leAddr_textEdited(const QString &arg1)
{
    this->setEnableOfBtn();
}

//编辑ipv6地址
void ConfForm::on_leAddr_ipv6_textChanged(const QString &arg1)
{
    this->setEnableOfBtn();
}

//编辑网络网关
void ConfForm::on_leGateway_textEdited(const QString &arg1)
{
    this->setEnableOfBtn();
}

//编辑网络DNS
void ConfForm::on_leDns_textEdited(const QString &arg1)
{
    this->setEnableOfBtn();
}

//编辑网络备用DNS
void ConfForm::on_leDns2_textEdited(const QString &arg1)
{
    // this->setEnableOfBtn();
}

//设置界面按钮是否可点击
void ConfForm::setEnableOfBtn()
{
    if (ui->leName->text().size() == 0 ) {
        this->setBtnEnableFalse();
        return;
    }

    if (ui->cbType->currentIndex() == 1) {
        if (!this->getTextEditState(ui->leAddr->text()) ) {
            this->setBtnEnableFalse();
            return;
        }

        if (!ui->leGateway->text().isEmpty() && !this->getTextEditState(ui->leGateway->text()) ) {
            this->setBtnEnableFalse();
            return;
        }

        if (!ui->leDns->text().isEmpty() && !this->getTextEditState(ui->leDns->text()) ) {
            this->setBtnEnableFalse();
            return;
        }

        if (!ui->leAddr_ipv6->text().isEmpty() && ! this->getIpv6EditState(ui->leAddr_ipv6->text())) {
            this->setBtnEnableFalse();
            return;
        }
    }


    ui->btnSave->setEnabled(true);
    ui->btnCreate->setEnabled(true);
    ui->btnSave->setProperty("on",true);
    ui->btnSave->style()->unpolish(ui->btnSave);
    ui->btnSave->style()->polish(ui->btnSave);

    ui->btnCreate->setProperty("on",true);
    ui->btnCreate->style()->unpolish(ui->btnCreate);
    ui->btnCreate->style()->polish(ui->btnCreate);

}

//文本的输入要符合ip的格式要求
bool ConfForm::getTextEditState(QString text)
{
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

    bool match = false;
    match = rx.exactMatch(text);

    return match;
}

bool ConfForm::getIpv6EditState(QString text)
{
    QRegExp rx("^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$");

    bool match = false;
    match = rx.exactMatch(text);

    return match;
}

//设置创建或保存按钮不可点击
void ConfForm::setBtnEnableFalse()
{
    ui->btnSave->setEnabled(false);
    ui->btnCreate->setEnabled(false);
    ui->btnSave->setProperty("on",false);
    ui->btnSave->style()->unpolish(ui->btnSave);
    ui->btnSave->style()->polish(ui->btnSave);

    ui->btnCreate->setProperty("on",false);
    ui->btnCreate->style()->unpolish(ui->btnCreate);
    ui->btnCreate->style()->polish(ui->btnCreate);
}

void ConfForm::paintEvent(QPaintEvent *event)
{
    KylinDBus mkylindbus;
    double trans = mkylindbus.getTransparentData();


    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QRect rect = this->rect();
    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    p.setBrush(opt.palette.color(QPalette::Base));
    p.setOpacity(trans);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(rect, 6, 6);
    QWidget::paintEvent(event);
}
