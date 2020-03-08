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

#include <unistd.h>
#include <stdlib.h>

extern QString llname, lwname;

ConfForm::ConfForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfForm)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle(tr("edit network"));//"网络设置"
    this->setAttribute(Qt::WA_TranslucentBackground);
    //需要添加 void paintEvent(QPaintEvent *event) 函数
    this->setStyleSheet("QWidget{border-radius:6px;background-color:rgba(19,19,20,0.95);border:1px solid rgba(255, 255, 255, 0.05);}");

    ui->lbName->setText(tr("Network name: "));//"网络名称："
    ui->lbTxt1->setText(tr("Method: "));//"编辑IP设置："
    ui->lbTxt2->setText(tr("Address: "));//"IP地址："
    ui->lbTxt3->setText(tr("Netmask: "));//"子网掩码："
    ui->lbTxt4->setText(tr("Gateway: "));//"默认网关："
    ui->lbTxt5->setText(tr("DNS 1: "));//"首选DNS："
    ui->lbTxt6->setText(tr("DNS 2: "));//"备选DNS："

    ui->lbLeftupTitle->setText(tr("Edit Conn"));//"网络设置"
    ui->cbType->addItem(tr("Auto(DHCP)"));//"自动(DHCP)"
    ui->cbType->addItem(tr("Manual"));//"手动"

    connect(ui->cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(cbTypeChanged(int)));

    ui->cbMask->addItem("255.255.255.0");
    ui->cbMask->addItem("255.255.254.0");
    ui->cbMask->addItem("255.255.252.0");
    ui->cbMask->addItem("255.255.0.0");
    ui->cbMask->addItem("255.0.0.0");

    ui->btnCancel->setText(tr("Cancel"));//"取消"
    ui->btnOk->setText(tr("Save"));//"保存"
    ui->btnCreate->setText(tr("Ok"));//"确定"

//    ui->centralWidget->setStyleSheet("#centralWidget{border:1px solid #297a97;background-color:#ffffff;}");
    ui->wdHead->setStyleSheet("#wdHead{border:none}");
    ui->wgManual->setStyleSheet("#wgManual{border:none}");
    ui->wdBottom->setStyleSheet("#wdBottom{border:none}");
    ui->lbLeftupTitle->setStyleSheet("QLabel{border:0px;font-size:20px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbName->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbTxt1->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbTxt2->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbTxt3->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbTxt4->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbTxt5->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbTxt6->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");

    ui->cbType->setStyleSheet("QComboBox{padding-left:20px;font-size:13px;color:rgba(255,255,255,0.91);border:1px solid rgba(255, 255, 255, 0.05);background:rgba(255,255,255,0.08);}"
                               "QComboBox::drop-down{border:0px;width:30px;}"
                               "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}"
                               "QComboBox QAbstractItemView {border:1px solid rgba(255, 255, 255, 0.05);background-color: transparent;}"
                               "QComboBox QAbstractItemView::item{font-size:13px;color:rgba(255,255,255,0.91);height: 32px;background-color: rgba(19,19,20,0.95);}"
                               "QComboBox QAbstractItemView::item:selected{font-size:13px;color:rgba(0,0,0,0.91);background-color:lightgray;}");
    ui->cbType->setView(new  QListView());
    ui->cbMask->setStyleSheet("QComboBox{padding-left:20px;font-size:13px;color:rgba(255,255,255,0.91);border:1px solid rgba(255, 255, 255, 0.05);background:rgba(255,255,255,0.08);}"
                               "QComboBox::drop-down{border:0px;width:30px;}"
                               "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}"
                               "QComboBox QAbstractItemView {border:1px solid rgba(255, 255, 255, 0.05);background-color: transparent;}"
                               "QComboBox QAbstractItemView::item{font-size:13px;color:rgba(255,255,255,0.91);height: 32px;background-color: rgba(19,19,20,0.95);}"
                               "QComboBox QAbstractItemView::item:selected{font-size:13px;color:rgba(0,0,0,0.91);background-color:lightgray;}");
    ui->cbMask->setView(new  QListView());
    ui->leName->setStyleSheet("QLineEdit{padding-left:20px;color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}");
    ui->leAddr->setStyleSheet("QLineEdit{padding-left:20px;color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}");
    ui->leGateway->setStyleSheet("QLineEdit{padding-left:20px;color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}");
    ui->leDns->setStyleSheet("QLineEdit{padding-left:20px;color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}");
    ui->leDns2->setStyleSheet("QLineEdit{padding-left:20px;color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}");

    ui->btnCancel->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
    ui->btnOk->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->btnCreate->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->lineUp->setStyleSheet("background:rgba(255,255,255,0.08);");
    ui->lineDown->setStyleSheet("background:rgba(255,255,255,0.08);");
    ui->lineUp->hide();
    ui->lineDown->hide();

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
}
void ConfForm::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
}

void ConfForm::setProp(QString connName, QString v4method, QString addr, QString mask, QString gateway, QString dns, bool isActConf){
    this->isActConf = isActConf;
    ui->leName->setText(connName);

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
    }else if(mask == "23"){
        ui->cbMask->setCurrentIndex(1);
    }else if(mask == "22"){
        ui->cbMask->setCurrentIndex(2);
    }else if(mask == "16"){
        ui->cbMask->setCurrentIndex(3);
    }else if(mask == "8"){
        ui->cbMask->setCurrentIndex(4);
    }else{
        ui->cbMask->setCurrentIndex(0);
    }
}

void ConfForm::on_btnCreate_clicked()
{
    QString cmdStr = "nmcli connection add con-name '" + ui->leName->text() + "' type ethernet";
    system(cmdStr.toUtf8().data());

    if(ui->cbType->currentIndex() == 1){
        this->on_btnOk_clicked();
    }

    this->hide();
}

void ConfForm::on_btnOk_clicked()
{
    QString mask = "";
    if(ui->cbMask->currentIndex() == 0){
        mask = "24";
    }else if(ui->cbMask->currentIndex() == 1){
        mask = "23";
    }else if(ui->cbMask->currentIndex() == 2){
        mask = "22";
    }else if(ui->cbMask->currentIndex() == 3){
        mask = "16";
    }else if(ui->cbMask->currentIndex() == 4){
        mask = "8";
    }else{
        mask = "24";
    }

    if(ui->cbType->currentIndex() == 0){
        kylin_network_set_automethod(ui->leName->text().toUtf8().data());
    }else{
        QString dnss = ui->leDns->text();
        if(ui->leDns2->text() != ""){
            dnss.append(",");
            dnss.append(ui->leDns2->text());
        }
        kylin_network_set_manualall(ui->leName->text().toUtf8().data(), ui->leAddr->text().toUtf8().data(), mask.toUtf8().data(), ui->leGateway->text().toUtf8().data(), dnss.toUtf8().data());
    }

    this->hide();

    // 如果是修改当前连接的网络，则修改设置后简略重连网络
    if(this->isActConf == true){
        //QString cmd = "/usr/share/kylin-nm/shell/connup.sh '" + ui->leName->text() + "'";
        QString cmd = "nmcli connection up '" + ui->leName->text() + "'";
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
    if (isShowSaveBtn){
        ui->btnOk->show();
        ui->btnCreate->hide();
        ui->lbLeftupTitle->setText(tr("edit network"));
    }

    if(index == 0){
        ui->lineUp->hide();
        ui->lineDown->hide();
        ui->wgManual->hide();
        ui->centralWidget->resize(432, 223);
        ui->wdBottom->move(1, 162);
        this->resize(432, 223);
    }
    if(index == 1){
        ui->lineUp->show();
        ui->lineDown->show();
        ui->wgManual->show();
        ui->centralWidget->resize(432, 487);
        ui->wdBottom->move(1, 426);
        this->resize(432, 487);
    }
    if(index == 3){
        ui->btnOk->hide();
        ui->btnCreate->show();
        ui->lbLeftupTitle->setText(tr("add wired network"));
        isShowSaveBtn = false;

        ui->lineUp->hide();
        ui->lineDown->hide();
        ui->wgManual->hide();
        ui->centralWidget->resize(432, 223);
        ui->wdBottom->move(1, 162);
        this->resize(432, 223);
    }
}

void ConfForm::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
