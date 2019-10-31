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

#include "confform.h"
#include "ui_confform.h"
#include "kylin-network-interface.h"
#include "backthread.h"

extern QString llname, lwname;

ConfForm::ConfForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfForm)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setWindowTitle(tr("edit network"));//"网络设置"

    ui->lbName->hide();
    ui->lbTxt1->setText(tr("Method: "));//"编辑IP设置："
    ui->lbTxt2->setText(tr("Address: "));//"IP地址："
    ui->lbTxt3->setText(tr("Netmask: "));//"子网掩码："
    ui->lbTxt4->setText(tr("Gateway: "));//"默认网关："
    ui->lbTxt5->setText(tr("DNS 1: "));//"首选DNS："
    ui->lbTxt6->setText(tr("DNS 2: "));//"备选DNS："

    ui->lbTitle->setText(tr("Edit Conn"));//"网络设置"
    ui->cbType->addItem(tr("Auto(DHCP)"));//"自动(DHCP)"
    ui->cbType->addItem(tr("Manual"));//"手动"

    connect(ui->cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(cbTypeChanged(int)));

    ui->cbMask->addItem("255.255.255.0");
    ui->cbMask->addItem("255.255.0.0");
    ui->cbMask->addItem("255.0.0.0");

    ui->btnCancel->setText(tr("Cancel"));//"取消"
    ui->btnOk->setText(tr("Save"));//"确定"

    ui->centralWidget->setStyleSheet("#centralWidget{border:1px solid #297a97;background-color:#ffffff;}");
    ui->lbIcon->setStyleSheet("QLabel{background-image:url(:/res/s/setting.png)}");
    ui->lbTitle->setStyleSheet("QLabel{font-size:12px;color:#ffffff;}");
    ui->lbTxt1->setStyleSheet("QLabel{font-weight:100;font-size:13px;color:#000000;}");
    ui->lbTxt2->setStyleSheet("QLabel{font-weight:100;font-size:13px;color:#000000;}");
    ui->lbTxt3->setStyleSheet("QLabel{font-weight:100;font-size:13px;color:#000000;}");
    ui->lbTxt4->setStyleSheet("QLabel{font-weight:100;font-size:13px;color:#000000;}");
    ui->lbTxt5->setStyleSheet("QLabel{font-weight:100;font-size:13px;color:#000000;}");
    ui->lbTxt6->setStyleSheet("QLabel{font-weight:100;font-size:13px;color:#000000;}");
    ui->wdLeftup->setStyleSheet("#wdLeftup{background-color:#266ab5;}");
    ui->wdHead->setStyleSheet("#wdHead{background-color:#f5f6f7}");
    ui->wdBottom->setStyleSheet("#wdBottom{background-color:#f5f6f7}");
    ui->lbTraingle->setStyleSheet("QLabel{background-image:url(:/res/g/triangle.png)}");
    ui->cbType->setStyleSheet("QComboBox{padding-left:20px;font-size:13px;border:1px solid #aaaaaa;}"
                              "QComboBox::drop-down{border:0;width:30px;}"
                              "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}");
    ui->cbMask->setStyleSheet("QComboBox{padding-left:20px;font-size:13px;border:1px solid #aaaaaa;}"
                              "QComboBox::drop-down{border:0;width:30px;}"
                              "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}");
    ui->leAddr->setStyleSheet("#leAddr{padding-left:20px;font-size:13px;border:1px solid #aaaaaa;}");
    ui->leGateway->setStyleSheet("#leGateway{padding-left:20px;font-size:13px;border:1px solid #aaaaaa;}");
    ui->leDns->setStyleSheet("#leDns{padding-left:20px;font-size:13px;border:1px solid #aaaaaa;}");
    ui->leDns2->setStyleSheet("#leDns2{padding-left:20px;font-size:13px;border:1px solid #aaaaaa;}");
    ui->btnCancel->setStyleSheet("QPushButton{border:1px solid #aaaaaa;background-color:#f5f5f5;}"
                                 "QPushButton:Hover{border:2px solid #629ee8;background-color:#eeeeee;}"
                                 "QPushButton:Pressed{border:1px solid #aaaaaa;background-color:#d8d8d8;}");
    ui->btnOk->setStyleSheet("QPushButton{border:1px solid #aaaaaa;background-color:#f5f5f5;}"
                                 "QPushButton:Hover{border:2px solid #629ee8;background-color:#eeeeee;}"
                                 "QPushButton:Pressed{border:1px solid #aaaaaa;background-color:#d8d8d8;}");

    // IP的正则格式限制
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    ui->leAddr->setValidator(new QRegExpValidator(rx, this));
    ui->leGateway->setValidator(new QRegExpValidator(rx, this));
    ui->leDns->setValidator(new QRegExpValidator(rx, this));
    ui->leDns2->setValidator(new QRegExpValidator(rx, this));
}

ConfForm::~ConfForm()
{
    delete ui;
}

void ConfForm::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}
void ConfForm::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
    this->setWindowOpacity(1);
}
void ConfForm::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        this->setWindowOpacity(0.9);
        event->accept();
    }
}

void ConfForm::setProp(QString connName, QString v4method, QString addr, QString mask, QString gateway, QString dns, bool isActConf){
    this->isActConf = isActConf;
    ui->lbName->setText(connName);

    if(v4method == "auto" || v4method == ""){
        ui->cbType->setCurrentIndex(0);
        cbTypeChanged(0);
    }else{
        ui->cbType->setCurrentIndex(1);
        cbTypeChanged(1);
    }

    ui->leAddr->setText(addr);
    ui->leGateway->setText(gateway);

    // 配置中有多个DNS，只处理前两个
    if(dns.indexOf(",") != -1){
        QStringList dnss = dns.split(",");
        ui->leDns->setText(dnss.at(0));
        ui->leDns2->setText(dnss.at(1));
    }else{
        ui->leDns->setText(dns);
        ui->leDns2->setText("");
    }

    if(mask == "24"){
        ui->cbMask->setCurrentIndex(0);
    }else if(mask == "16"){
        ui->cbMask->setCurrentIndex(1);
    }else if(mask == "8"){
        ui->cbMask->setCurrentIndex(2);
    }else{
        ui->cbMask->setCurrentIndex(0);
    }
}

void ConfForm::on_btnOk_clicked()
{
    QString mask = "";
    if(ui->cbMask->currentIndex() == 0){
        mask = "24";
    }else if(ui->cbMask->currentIndex() == 1){
        mask = "16";
    }else if(ui->cbMask->currentIndex() == 2){
        mask = "8";
    }else{
        mask = "24";
    }

    if(ui->cbType->currentIndex() == 0){
        kylin_network_set_automethod(ui->lbName->text().toUtf8().data());
    }else{
        QString dnss = ui->leDns->text();
        if(ui->leDns2->text() != ""){
            dnss.append(",");
            dnss.append(ui->leDns2->text());
        }
        kylin_network_set_manualall(ui->lbName->text().toUtf8().data(), ui->leAddr->text().toUtf8().data(), mask.toUtf8().data(), ui->leGateway->text().toUtf8().data(), dnss.toUtf8().data());
    }

    this->hide();

    // 如果是修改当前连接的网络，则修改设置后简略重连网络
    if(this->isActConf == true){
        QString cmd = "/usr/share/kylin-nm/shell/connup.sh '" + ui->lbName->text() + "'";
        system(cmd.toUtf8().data());

        QString txt(tr("New settings already effective"));
        cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';notify-send '" + txt + "' -t 3800";
        system(cmd.toUtf8().data());
    }
}

void ConfForm::on_btnCancel_clicked()
{
    this->hide();
}

void ConfForm::cbTypeChanged(int index){
    if(index == 0){
        ui->wgManual->hide();
        ui->centralWidget->resize(432, 178);
        ui->wdBottom->move(1, 117);
        this->resize(432, 178);
    }
    if(index == 1){
        ui->wgManual->show();
        ui->centralWidget->resize(432, 442);
        ui->wdBottom->move(1, 381);
        this->resize(432, 442);
    }
}
