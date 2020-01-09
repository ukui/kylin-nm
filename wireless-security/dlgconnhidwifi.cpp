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


#include "dlgconnhidwifi.h"
#include "kylinheadfile.h"
#include "ui_dlgconnhidwifi.h"
#include "backthread.h"
#include "mainwindow.h"
#include <unistd.h>

#include <QStandardItemModel>

DlgConnHidWifi::DlgConnHidWifi(int type, MainWindow *mainWindow, QWidget *parent) :
    isUsed(type),
    QDialog(parent),
    ui(new Ui::DlgConnHidWifi)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:white;");

    ui->lbBoder->setStyleSheet("QLabel{background-color:transparent;border:1px solid #297a97;}");
    ui->lbUpback->setStyleSheet("QLabel{background-color:#f5f6f7;}");
    ui->lbBottomback->setStyleSheet("QLabel{background-color:#f5f6f7;}");
    ui->lbLeftup->setStyleSheet("QLabel{background-color:#266ab5;}");
    ui->lbLeftupIcon->setStyleSheet("QLabel{background-image:url(:/res/h/no-pwd-wifi.png);background-color:transparent;}");
    ui->lbLeftupTitle->setStyleSheet("QLabel{font-size:12px;color:#ffffff;background-color:transparent;}");
    ui->lbConn->setStyleSheet("QLabel{background-color:transparent;}");
//    ui->cbxConn->setStyleSheet("QComboBox{padding-left:20px;font-size:13px;border:1px solid #aaaaaa;}"
//                              "QComboBox::drop-down{border:0;width:30px;}"
//                              "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}");
//    ui->cbxSecurity->setStyleSheet("QComboBox{padding-left:20px;font-size:13px;border:1px solid #aaaaaa;}"
//                                   "QComboBox::drop-down{border:0;width:30px;}"
//                                   "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}");
    ui->btnCancel->setStyleSheet("QPushButton{border:1px solid #aaaaaa;background-color:#f5f5f5;}"
                                 "QPushButton:Hover{border:2px solid #629ee8;background-color:#eeeeee;}"
                                 "QPushButton:Pressed{border:1px solid #aaaaaa;background-color:#d8d8d8;}");
    ui->btnConnect->setStyleSheet("QPushButton{border:1px solid #aaaaaa;background-color:#f5f5f5;}"
                                 "QPushButton:Hover{border:2px solid #629ee8;background-color:#eeeeee;}"
                                 "QPushButton:Pressed{border:1px solid #aaaaaa;background-color:#d8d8d8;}");

    ui->lbLeftupTitle->setText(tr("Add hidden Wi-Fi")); //加入隐藏Wi-Fi
    ui->lbConn->setText(tr("Connection")); //连接设置:
    ui->lbNetName->setText(tr("Network name")); //网络名称:
    ui->lbSecurity->setText(tr("Wi-Fi security")); //Wi-Fi安全性:
    ui->btnCancel->setText(tr("Cancel")); //取消
    ui->btnConnect->setText(tr("Connect")); //连接

    ui->cbxConn->addItem(tr("C_reate…")); //新建...
    system("nmcli connection show>/tmp/kylin-nm-connshow");
    QFile file("/tmp/kylin-nm-connshow");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"Can't open the file!";
    }
    QString txt = file.readAll();
    QStringList txtLine = txt.split("\n");
    file.close();
    foreach (QString line, txtLine) {
        if(line.indexOf("wifi") != -1){
            QStringList subLine = line.split(" ");
            ui->cbxConn->addItem(subLine[0]);
        }
    }
    ui->cbxConn->setCurrentIndex(0);
    connect(ui->cbxConn,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeWindow()));

    ui->cbxSecurity->addItem(tr("None")); //无
    ui->cbxSecurity->addItem(tr("WPA & WPA2 Personal")); //WPA 及 WPA2 个人
    ui->cbxSecurity->addItem(tr("WEP 40/128-bit Key (Hex or ASCII)")); //WEP 40/128 位密钥(十六进制或ASCII)
    ui->cbxSecurity->addItem(tr("WEP 128-bit Passphrase")); //WEP 128 位密码句
    ui->cbxSecurity->addItem("LEAP");
    ui->cbxSecurity->addItem(tr("Dynamic WEP (802.1X)")); //动态 WEP (802.1x)
    ui->cbxSecurity->addItem(tr("WPA & WPA2 Enterprise")); //WPA 及 WPA2 企业
    ui->cbxSecurity->setCurrentIndex(0);
    connect(ui->cbxSecurity,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialog()));

    if (isUsed == 0){
        ui->btnConnect->setEnabled(false);
    }else{
        ui->cbxConn->setCurrentIndex(isUsed);
        ui->leNetName->setText(ui->cbxConn->currentText());
        ui->lbNetName->setEnabled(false);
        ui->leNetName->setEnabled(false);
        ui->lbSecurity->setEnabled(false);
        ui->cbxSecurity->setEnabled(false);
        ui->btnConnect->setEnabled(true);
    }

    this->setFixedSize(432,336);

    this->mw = mainWindow;
}

DlgConnHidWifi::~DlgConnHidWifi()
{
    delete ui;
}

void DlgConnHidWifi::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}
void DlgConnHidWifi::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
    this->setWindowOpacity(1);
}
void DlgConnHidWifi::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        this->setWindowOpacity(0.9);
        event->accept();
    }
}

//切换到其他Wi-Fi安全类型
void DlgConnHidWifi::changeDialog()
{
    if(ui->cbxSecurity->currentIndex()==0){
        qDebug()<<"it's not need to change dialog";
    } else if(ui->cbxSecurity->currentIndex()==1) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiWpa *connHidWifiWpa = new DlgConnHidWifiWpa(0, mw);
        connHidWifiWpa->show();
        connect(connHidWifiWpa, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    } else if(ui->cbxSecurity->currentIndex()==2) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiWep *connHidWifiWep = new DlgConnHidWifiWep(0);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==3) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiWep *connHidWifiWep = new DlgConnHidWifiWep(1);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==4) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiLeap *connHidWifiLeap = new DlgConnHidWifiLeap();
        connHidWifiLeap->show();
    } else if(ui->cbxSecurity->currentIndex()==5) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiSecTls *connHidWifiSecTls = new DlgConnHidWifiSecTls(0);
        connHidWifiSecTls->show();
    } else {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiSecTls *connHidWifiSecTls = new DlgConnHidWifiSecTls(1);
        connHidWifiSecTls->show();
    }
}

//同一 Wi-Fi安全类型的窗口变换
void DlgConnHidWifi::changeWindow(){
    if (ui->cbxConn->currentIndex() == 0){
        isUsed = ui->cbxConn->currentIndex();
        ui->cbxConn->setCurrentIndex(0);
        ui->leNetName->setText("");
        ui->lbNetName->setEnabled(true);
        ui->leNetName->setEnabled(true);
        ui->lbSecurity->setEnabled(true);
        ui->cbxSecurity->setEnabled(true);
        ui->btnConnect->setEnabled(false);
    }else if (ui->cbxConn->currentIndex() >= 1){
        QString currStr = "nmcli connection show " + ui->cbxConn->currentText() + " >/tmp/kylin-nm-connshow";
        system(currStr.toUtf8().data());
        QFile file("/tmp/kylin-nm-connshow");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"Can't open the file!";
        }
        QString txt = file.readAll();
        file.close();
        if (txt.indexOf("802-11-wireless-security.key-mgmt:") != -1){
            QApplication::setQuitOnLastWindowClosed(false);
            this->hide();
            DlgConnHidWifiWpa *connHidWifiWpa = new DlgConnHidWifiWpa(ui->cbxConn->currentIndex(), mw);
            connHidWifiWpa->show();
            connect(connHidWifiWpa, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
        }else {
            isUsed = ui->cbxConn->currentIndex();
            ui->leNetName->setText(ui->cbxConn->currentText());
            ui->lbNetName->setEnabled(false);
            ui->leNetName->setEnabled(false);
            ui->lbSecurity->setEnabled(false);
            ui->cbxSecurity->setEnabled(false);
            ui->btnConnect->setEnabled(true);
        }
    }
}

void DlgConnHidWifi::on_btnCancel_clicked()
{
    this->close();
}

void DlgConnHidWifi::on_btnConnect_clicked()
{
    QThread *t = new QThread();
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), this, SLOT(slotStartLoading()));
    connect(this, SIGNAL(stopSignal()), t, SLOT(quit()));
    t->start();

    QString wifiName = ui->leNetName->text();
    BackThread *bt = new BackThread();
    strWifiname = wifiName;
    //点击连接按钮以连接隐藏WiFi
    if (isUsed == 0){
        int x = 0;
        do{
            sleep(1);
           QString cmd = "nmcli device wifi connect " + wifiName + " password '' hidden yes >/tmp/kylin-nm-btoutput";
           system(cmd.toUtf8().data());

           QFile file("/tmp/kylin-nm-btoutput");
           if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
           {
               qDebug()<<"Can't open the file!"<<endl;
           }
           QString text = file.readAll();
           file.close();
           if(text.indexOf("Scanning not allowed") != -1){x = 1;} else { x = 0;}
        } while(x);

        connect(this, SIGNAL(execSecConn()), this,SLOT(on_execSecConn() ));
        QTimer::singleShot(5*1000, this, SLOT(on_execSecConn() ));
    } else {
        bt->execConnWifi(wifiName);
        connect(this, SIGNAL(sendMessage()), this,SLOT(emitSignal() ));
        QTimer::singleShot(4*1000, this, SLOT(emitSignal() ));
    }
    this->close();
}

void DlgConnHidWifi::on_leNetName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgConnHidWifi::slotStartLoading()
{
    mw->startLoading();
}

void DlgConnHidWifi::on_execSecConn()
{
    QString str = "nmcli device wifi connect " + strWifiname + " password ''";
    system(str.toUtf8().data());
    connect(this, SIGNAL(sendMessage()), this,SLOT(emitSignal() ));
    QTimer::singleShot(3*1000, this, SLOT(emitSignal() ));
}

void DlgConnHidWifi::emitSignal()
{
    emit reSetWifiList();
    mw->stopLoading();
    emit this->stopSignal();
}
