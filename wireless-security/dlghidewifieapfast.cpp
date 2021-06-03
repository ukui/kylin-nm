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


#include "dlghidewifieapfast.h"
#include "ui_dlghidewifieapfast.h"
#include "kylinheadfile.h"
#include "src/kylin-dbus-interface.h"

DlgHideWifiEapFast::DlgHideWifiEapFast(int type, QWidget *parent) :
    WepOrWpa(type),
    QDialog(parent),
    ui(new Ui::DlgHideWifiEapFast)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //需要添加 void paintEvent(QPaintEvent *event) 函数

    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));

    this->setStyleSheet("QWidget{border-radius:6px;background-color:rgba(19,19,20,0.7);border:1px solid rgba(255, 255, 255, 0.05);}");

    MyQss objQss;

    ui->lbBoder->setStyleSheet("QLabel{border-radius:6px;background-color:rgba(19,19,20,0.95);border:1px solid rgba(255, 255, 255, 0.05);}");
    ui->lbBoder->hide();
    ui->lbLeftupTitle->setStyleSheet("QLabel{border:0px;font-size:20px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbConn->setStyleSheet(objQss.labelQss);
    ui->lbNetName->setStyleSheet(objQss.labelQss);
    ui->lbSecurity->setStyleSheet(objQss.labelQss);
    ui->lbAuth->setStyleSheet(objQss.labelQss);
    ui->lbAnonyId->setStyleSheet(objQss.labelQss);
    ui->checkBoxAutoPCA->setStyleSheet(objQss.checkBoxCAQss);
    ui->lbPCAfile->setStyleSheet(objQss.labelQss);
    ui->lbInnerAuth->setStyleSheet(objQss.labelQss);
    ui->lbUserName->setStyleSheet(objQss.labelQss);
    ui->lbPassword->setStyleSheet(objQss.labelQss);

    ui->cbxConn->setStyleSheet(objQss.cbxQss);
    ui->cbxConn->setView(new  QListView());
    ui->leNetName->setStyleSheet(objQss.leQss);
    ui->cbxSecurity->setStyleSheet(objQss.cbxQss);
    ui->cbxSecurity->setView(new  QListView());
    ui->cbxAuth->setStyleSheet(objQss.cbxQss);
    ui->cbxAuth->setView(new  QListView());
    ui->leAnonyId->setStyleSheet(objQss.leQss);
    ui->cbxAutoPCA->setStyleSheet(objQss.cbxQss);
    ui->cbxAutoPCA->setView(new  QListView());
    ui->lePCAfile->setStyleSheet(objQss.leQss);
    ui->cbxInnerAuth->setStyleSheet(objQss.cbxQss);
    ui->cbxInnerAuth->setView(new  QListView());
    ui->leUserName->setStyleSheet(objQss.leQss);
    ui->lePassword->setStyleSheet(objQss.leQss);
    ui->checkBoxPwd->setStyleSheet(objQss.checkBoxQss);

    ui->btnCancel->setStyleSheet(objQss.btnOffQss);
    ui->btnConnect->setStyleSheet(objQss.btnOffQss);
    ui->lineUp->setStyleSheet(objQss.lineQss);
    ui->lineDown->setStyleSheet(objQss.lineQss);

    ui->lbLeftupTitle->setText(tr("Add hidden Wi-Fi")); //加入隐藏Wi-Fi
    ui->lbConn->setText(tr("Connection")); //连接设置:
    ui->lbNetName->setText(tr("Network name")); //网络名称:
    ui->lbSecurity->setText(tr("Wi-Fi security")); //Wi-Fi安全性:
    ui->lbAuth->setText(tr("Authentication")); //认证:
    ui->lbAnonyId->setText(tr("Anonymous identity")); //匿名身份:
    ui->checkBoxAutoPCA->setText(tr("Allow automatic PAC pro_visioning")); //自动PAC配置:
    ui->lbPCAfile->setText(tr("PAC file"));//PAC文件:
    ui->lbInnerAuth->setText(tr("Inner authentication")); //内部认证:
    ui->lbUserName->setText(tr("Username")); //用户名:
    ui->lbPassword->setText(tr("Password")); //密码:
    ui->btnCancel->setText(tr("Cancel")); //取消
    ui->btnConnect->setText(tr("Connect")); //连接

    ui->checkBoxAutoPCA->setFocusPolicy(Qt::NoFocus);
    ui->checkBoxPwd->setFocusPolicy(Qt::NoFocus);

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
    if (WepOrWpa == 0) {
        ui->cbxSecurity->setCurrentIndex(5);
    } else if (WepOrWpa == 1) {
        ui->cbxSecurity->setCurrentIndex(6);
    }
    connect(ui->cbxSecurity,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialogSecu()));

    ui->cbxAuth->addItem("TLS");
    ui->cbxAuth->addItem("LEAP");
    ui->cbxAuth->addItem("PWD");
    ui->cbxAuth->addItem("FAST");
    ui->cbxAuth->addItem(tr("Tunneled TLS"));//隧道 TLS
    ui->cbxAuth->addItem(tr("Protected EAP (PEAP)")); //受保护的 EAP
    ui->cbxAuth->setCurrentIndex(3);
    connect(ui->cbxAuth,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialogAuth()));

    ui->checkBoxAutoPCA->setCheckState(Qt::Checked);

    ui->cbxAutoPCA->addItem(tr("Anonymous")); //匿名
    ui->cbxAutoPCA->addItem(tr("Authenticated")); //已认证
    ui->cbxAutoPCA->addItem(tr("Both")); //两者兼用
    ui->cbxAutoPCA->setCurrentIndex(0);

    ui->lePCAfile->setText(tr("None")); //(无)

    ui->cbxInnerAuth->addItem("GTC");
    ui->cbxInnerAuth->addItem("MSCHAPv2");
    ui->cbxInnerAuth->setCurrentIndex(0);

    ui->btnConnect->setEnabled(false);

    this->setFixedSize(432,673);
}

DlgHideWifiEapFast::~DlgHideWifiEapFast()
{
    delete ui;
}

void DlgHideWifiEapFast::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}
void DlgHideWifiEapFast::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
}
void DlgHideWifiEapFast::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
}

void DlgHideWifiEapFast::changeDialogSecu()
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
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiWep *connHidWifiWep = new DlgHideWifiWep(0);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==3) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiWep *connHidWifiWep = new DlgHideWifiWep(1);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==4) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiLeap *connHidWifiLeap = new DlgHideWifiLeap();
        connHidWifiLeap->show();
    } else if(ui->cbxSecurity->currentIndex()==5) {
        if (WepOrWpa == 1) {
            ui->cbxSecurity->setCurrentIndex(5);
            WepOrWpa = 0;
        }
    } else {
        if (WepOrWpa == 0){
            ui->cbxSecurity->setCurrentIndex(6);
            WepOrWpa = 1;
        }
    }
}

void DlgHideWifiEapFast::changeDialogAuth()
{
    if(ui->cbxAuth->currentIndex()==0){
        //QApplication::setQuitOnLastWindowClosed(false);
        //this->hide();
        //DlgHideWifiEapTls *connHidWifiEapTls = new DlgHideWifiEapTls(WepOrWpa);
        //connHidWifiEapTls->show();
    } else if(ui->cbxAuth->currentIndex()==1) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiEapLeap *connHidWifiEapLeap = new DlgHideWifiEapLeap(WepOrWpa);
        connHidWifiEapLeap->show();
    } else if(ui->cbxAuth->currentIndex()==2) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiEapPwd *connHidWifiEapPwd = new DlgHideWifiEapPwd(WepOrWpa);
        connHidWifiEapPwd->show();
    } else if(ui->cbxAuth->currentIndex()==3) {
        qDebug()<<"it's not need to change dialog";
    } else if(ui->cbxAuth->currentIndex()==4) {
        //QApplication::setQuitOnLastWindowClosed(false);
        //this->hide();
        //DlgHideWifiEapTTLS *connHidWifiEapTTls = new DlgHideWifiEapTTLS(WepOrWpa);
        //connHidWifiEapTTls->show();
    } else {
        //QApplication::setQuitOnLastWindowClosed(false);
        //this->hide();
        //DlgHideWifiEapPeap *connHidWifiEapPeap = new DlgHideWifiEapPeap(WepOrWpa);
        //connHidWifiEapPeap->show();
    }
}

void DlgHideWifiEapFast::on_btnCancel_clicked()
{
    this->close();
}

void DlgHideWifiEapFast::on_btnConnect_clicked()
{
    this->close();
}

void DlgHideWifiEapFast::on_checkBoxAutoPCA_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->cbxAutoPCA->setEnabled(false);
    } else {
        ui->cbxAutoPCA->setEnabled(true);
    }
}

void DlgHideWifiEapFast::on_checkBoxPwd_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->lePassword ->setEchoMode(QLineEdit::Password);
    } else {
        ui->lePassword->setEchoMode(QLineEdit::Normal);
    }
}

void DlgHideWifiEapFast::on_leNetName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if(ui->leAnonyId->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgHideWifiEapFast::on_leAnonyId_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if(ui->leAnonyId->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgHideWifiEapFast::on_leUserName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if(ui->leAnonyId->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgHideWifiEapFast::on_lePassword_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if(ui->leAnonyId->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgHideWifiEapFast::paintEvent(QPaintEvent *event)
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
