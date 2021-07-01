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


#include "ui_dlghidewifiwep.h"
#include "kylinheadfile.h"
#include "src/kylin-dbus-interface.h"

DlgHideWifiWep::DlgHideWifiWep(int type, QWidget *parent) :
    WepPwdOrCode(type),
    QDialog(parent),
    ui(new Ui::DlgHideWifiWep)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //需要添加 void paintEvent(QPaintEvent *event) 函数

    this->setStyleSheet("QWidget{border-radius:6px;background-color:rgba(19,19,20,0.7);border:1px solid rgba(255, 255, 255, 0.05);}");

    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));

    MyQss objQss;

    ui->lbBoder->setStyleSheet("QLabel{border-radius:6px;background-color:rgba(19,19,20,0.95);border:1px solid rgba(255, 255, 255, 0.05);}");
    ui->lbBoder->hide();
    ui->lbLeftupTitle->setStyleSheet("QLabel{border:0px;font-size:20px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbConn->setStyleSheet(objQss.labelQss);
    ui->lbNetName->setStyleSheet(objQss.labelQss);
    ui->lbSecurity->setStyleSheet(objQss.labelQss);
    ui->lbKey->setStyleSheet(objQss.labelQss);
    ui->lbWEPindex->setStyleSheet(objQss.labelQss);
    ui->lbAuth->setStyleSheet(objQss.labelQss);

    ui->cbxConn->setStyleSheet(objQss.cbxQss);
    ui->cbxConn->setView(new  QListView());
    ui->leNetName->setStyleSheet(objQss.leQss);
    ui->leKey->setStyleSheet(objQss.leQss);
    ui->checkBoxPwd->setStyleSheet(objQss.checkBoxQss);
    ui->cbxSecurity->setStyleSheet(objQss.cbxQss);
    ui->cbxSecurity->setView(new  QListView());
    ui->cbxWEPindex->setStyleSheet(objQss.cbxQss);
    ui->cbxWEPindex->setView(new  QListView());
    ui->cbxAuth->setStyleSheet(objQss.cbxQss);
    ui->cbxAuth->setView(new  QListView());

    ui->btnCancel->setStyleSheet(objQss.btnOffQss);
    ui->btnConnect->setStyleSheet(objQss.btnOffQss);
    ui->lineUp->setStyleSheet(objQss.lineQss);
    ui->lineDown->setStyleSheet(objQss.lineQss);

    ui->lbLeftupTitle->setText(tr("Add hidden WLAN")); //加入隐藏Wi-Fi
    ui->lbConn->setText(tr("Connection")); //连接设置:
    ui->lbNetName->setText(tr("Network name")); //网络名称:
    ui->lbSecurity->setText(tr("WLAN security")); //Wi-Fi 安全性:
    ui->lbKey->setText(tr("Key")); //密钥:
    ui->lbWEPindex->setText(tr("WEP index")); //WEP 检索:
    ui->lbAuth->setText(tr("Authentication")); //认证:
    ui->btnCancel->setText(tr("Cancel")); //取消
    ui->btnConnect->setText(tr("Connect")); //连接

    ui->cbxConn->addItem(tr("C_reate…")); //新建...
    int status = system("nmcli connection show>/tmp/kylin-nm-connshow");
    qDebug()<<"executed cmd=nmcli connection show>/tmp/kylin-nm-connshow. res="<<status;
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

    ui->cbxSecurity->addItem(tr("None")); //无
    ui->cbxSecurity->addItem(tr("WPA and WPA2 Personal")); //WPA 及 WPA2 个人
    ui->cbxSecurity->addItem(tr("WEP 40/128-bit Key (Hex or ASCII)")); //WEP 40/128 位密钥(十六进制或ASCII)
    ui->cbxSecurity->addItem(tr("WEP 128-bit Passphrase")); //WEP 128 位密码句
    ui->cbxSecurity->addItem("LEAP");
    ui->cbxSecurity->addItem(tr("Dynamic WEP (802.1X)")); //动态 WEP (802.1x)
    ui->cbxSecurity->addItem(tr("WPA and WPA2 Enterprise")); //WPA 及 WPA2 企业
    if (WepPwdOrCode == 0) {
        ui->cbxSecurity->setCurrentIndex(2);
    } else if (WepPwdOrCode == 1) {
        ui->cbxSecurity->setCurrentIndex(3);
    }
    connect(ui->cbxSecurity,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialog()));

    ui->cbxWEPindex->addItem(tr("1(default)")); //1(默认)
    ui->cbxWEPindex->addItem("2");
    ui->cbxWEPindex->addItem("3");
    ui->cbxWEPindex->addItem("4");
    ui->cbxWEPindex->setCurrentIndex(0);

    ui->cbxAuth->addItem(tr("Open System")); //开放式系统
    ui->cbxAuth->addItem(tr("Shared Key")); //共享密钥
    ui->cbxAuth->setCurrentIndex(0);

    ui->btnConnect->setEnabled(false);

    this->setFixedSize(432,493);
}

DlgHideWifiWep::~DlgHideWifiWep()
{
    delete ui;
}

void DlgHideWifiWep::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}
void DlgHideWifiWep::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
}
void DlgHideWifiWep::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
}

//切换到其他Wi-Fi安全类型
void DlgHideWifiWep::changeDialog()
{
    if(ui->cbxSecurity->currentIndex()==0){
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifi *connHidWifi = new DlgHideWifi(0);
        connHidWifi->show();
    } else if(ui->cbxSecurity->currentIndex()==1) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiWpa *connHidWifiWpa = new DlgHideWifiWpa(0);
        connHidWifiWpa->show();
    } else if(ui->cbxSecurity->currentIndex()==2) {
        if (WepPwdOrCode == 1) {
            ui->cbxSecurity->setCurrentIndex(2);
            WepPwdOrCode = 0;
        }
    } else if(ui->cbxSecurity->currentIndex()==3) {
        if (WepPwdOrCode == 0) {
            ui->cbxSecurity->setCurrentIndex(3);
            WepPwdOrCode = 1;
        }
    } else if(ui->cbxSecurity->currentIndex()==4) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiLeap *connHidWifiLeap = new DlgHideWifiLeap();
        connHidWifiLeap->show();
    } else if(ui->cbxSecurity->currentIndex()==5) {
        //QApplication::setQuitOnLastWindowClosed(false);
        //this->hide();
        //DlgHideWifiEapTls *connHidWifiEapTls = new DlgHideWifiEapTls(0);
        //connHidWifiEapTls->show();
    } else {
        //QApplication::setQuitOnLastWindowClosed(false);
        //this->hide();
        //DlgHideWifiEapTls *connHidWifiEapTls = new DlgHideWifiEapTls(1);
        //connHidWifiEapTls->show();
    }
}

void DlgHideWifiWep::on_btnCancel_clicked()
{
    this->close();
}

void DlgHideWifiWep::on_btnConnect_clicked()
{
    this->close();
}

void DlgHideWifiWep::on_checkBoxPwd_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->leKey->setEchoMode(QLineEdit::Password);
    } else {
        ui->leKey->setEchoMode(QLineEdit::Normal);
    }
}

void DlgHideWifiWep::on_leKey_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "" || ui->leKey->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgHideWifiWep::on_leNetName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "" || ui->leKey->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgHideWifiWep::paintEvent(QPaintEvent *event)
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
