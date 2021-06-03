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


#include "dlghidewifieappeap.h"
#include "ui_dlghidewifieappeap.h"
#include "kylinheadfile.h"
#include "src/mainwindow.h"
#include "src/kylin-dbus-interface.h"
#include <QStandardItemModel>
#include <QDir>

DlgHideWifiEapPeap::DlgHideWifiEapPeap(int type, int beUsed, MainWindow *mainWindow, QWidget *parent) :
    WepOrWpa(type),
    isUsed(beUsed),
    QDialog(parent),
    ui(new Ui::DlgHideWifiEapPeap)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowIcon(QIcon::fromTheme("kylin-network", QIcon(":/res/x/setup.png")) );
    //需要添加 void paintEvent(QPaintEvent *event) 函数

    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));

    MyQss objQss;

    ui->lbBoder->setStyleSheet("QLabel{border-radius:6px;background-color:rgba(19,19,20,0.95);border:1px solid rgba(255, 255, 255, 0.05);}");
    ui->lbBoder->hide();
    ui->lbLeftupTitle->setStyleSheet("QLabel{font-size:20px;}");
    ui->checkBoxPwd->setStyleSheet(objQss.checkBoxQss);
    ui->checkBoxPwdSec->setStyleSheet(objQss.checkBoxQss);

    ui->lineUp->setStyleSheet(objQss.lineQss);
    ui->lineDown->setStyleSheet(objQss.lineQss);

    ui->lbLeftupTitle->setText(tr("Add hidden Wi-Fi")); //加入隐藏Wi-Fi
    ui->lbConn->setText(tr("Connection")); //连接设置:
    ui->lbNetName->setText(tr("Network name")); //网络名称:
    ui->lbSecurity->setText(tr("Wi-Fi security")); //Wi-Fi安全性:
    ui->lbAuth->setText(tr("Authentication")); //认证:
    ui->lbAnonyId->setText(tr("Anonymous identity")); //匿名身份:
    ui->lbDomain->setText(tr("Domain")); //域名:
    ui->lbCA->setText(tr("CA certificate")); //CA 证书:
    ui->lbCaPwd->setText(tr("CA certificate password")); //CA 证书密码:
    ui->checkBoxCA->setText(tr("No CA certificate is required")); //不需要CA证书
    ui->lbPEAPver->setText(tr("PEAP version")); //PEAP版本:
    ui->lbInnerAuth->setText(tr("Inner authentication")); //内部认证:
    ui->lbUserName->setText(tr("Username")); //用户名:
    ui->lbPassword->setText(tr("Password")); //密码:
    ui->btnCancel->setText(tr("Cancel")); //取消
    ui->btnConnect->setText(tr("Connect")); //连接
    ui->btnCancel->setStyleSheet(objQss.btnOffQss);
    ui->btnConnect->setStyleSheet(objQss.btnOffQss);

    ui->cbxConn->addItem("新建...");
    int status = system("nmcli connection show>/tmp/kylin-nm-connshow");
    qDebug()<<"executed cmd=nmcli connection show>/tmp/kylin-nm-connshow. res="<<status;
    QFile file("/tmp/kylin-nm-connshow");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!";
    }
    QString txt = file.readAll();
    QStringList txtLine = txt.split("\n");
    file.close();
    foreach (QString line, txtLine) {
        if (line.indexOf("wifi") != -1) {
            QStringList subLine = line.split(" ");
            ui->cbxConn->addItem(subLine[0]);
        }
    }
    ui->cbxConn->setCurrentIndex(0);
    connect(ui->cbxConn,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeWindow()));

    ui->cbxSecurity->addItem(tr("None")); //无
    ui->cbxSecurity->addItem(tr("WPA and WPA2 Personal")); //WPA 及 WPA2 个人
    ui->cbxSecurity->addItem(tr("WPA and WPA2 Enterprise")); //WPA 及 WPA2 企业
    //ui->cbxSecurity->addItem(tr("WEP 40/128-bit Key (Hex or ASCII)")); //WEP 40/128 位密钥(十六进制或ASCII)
    //ui->cbxSecurity->addItem(tr("WEP 128-bit Passphrase")); //WEP 128 位密码句
    //ui->cbxSecurity->addItem("LEAP");
    //ui->cbxSecurity->addItem(tr("Dynamic WEP (802.1X)")); //动态 WEP (802.1x)

    if (WepOrWpa == 0) {
        ui->cbxSecurity->setCurrentIndex(5);
    } else if (WepOrWpa == 1) {
        ui->cbxSecurity->setCurrentIndex(2);
    }
    connect(ui->cbxSecurity,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialogSecu()));

    //ui->cbxAuth->addItem("TLS");
    //ui->cbxAuth->addItem("LEAP");
    //ui->cbxAuth->addItem("PWD");
    //ui->cbxAuth->addItem("FAST");
    //ui->cbxAuth->addItem(tr("Tunneled TLS"));//隧道 TLS
    ui->cbxAuth->addItem(tr("Protected EAP (PEAP)")); //受保护的 EAP
    ui->cbxAuth->setCurrentIndex(0);
    //connect(ui->cbxAuth,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialogAuth()));

    ui->cbxCA->addItem(tr("None")); //无
    ui->cbxCA->addItem(tr("Choose from file")); //从文件选择...
    ui->cbxCA->setCurrentIndex(0);

    ui->cbxPEAPver->addItem(tr("Automatic")); //自动
    ui->cbxPEAPver->addItem(tr("Version 0")); //版本 0
    ui->cbxPEAPver->addItem(tr("Version 1")); //版本 1
    ui->cbxPEAPver->setCurrentIndex(0);

    ui->cbxInnerAuth->addItem("MSCHAPv2");
    ui->cbxInnerAuth->addItem("MDS");
    ui->cbxInnerAuth->addItem("GTC");
    ui->cbxInnerAuth->setCurrentIndex(0);

    if (isUsed == 0) {
        ui->btnConnect->setEnabled(false);
    } else {
        ui->cbxConn->setCurrentIndex(isUsed);
        ui->leNetName->setText(ui->cbxConn->currentText());
        ui->leNetName->setEnabled(false);
        ui->cbxSecurity->setEnabled(false);
        ui->cbxAuth->setEnabled(false);
        ui->leAnonyId->setEnabled(false);
        ui->leDomain->setEnabled(false);
        ui->cbxCA->setEnabled(false);
        ui->leCaPwd->setEnabled(false);
        ui->checkBoxCA->setEnabled(false);
        ui->cbxPEAPver->setEnabled(false);
        ui->cbxInnerAuth->setEnabled(false);
        ui->leUserName->setText("<Hidden>");
        ui->leUserName->setEnabled(false);
        ui->lePassword->setText("<Hidden>");
        ui->lePassword->setEnabled(false);
        ui->btnConnect->setEnabled(true);
    }

    this->setFixedSize(432,700);

    this->mw = mainWindow;
}

DlgHideWifiEapPeap::~DlgHideWifiEapPeap()
{
    delete ui;
}

void DlgHideWifiEapPeap::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}
void DlgHideWifiEapPeap::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
}
void DlgHideWifiEapPeap::mouseMoveEvent(QMouseEvent *event){
    if (this->isPress) {
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
}

//切换到其他Wi-Fi安全类型
void DlgHideWifiEapPeap::changeDialogSecu()
{
    if (ui->cbxSecurity->currentIndex()==0) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifi *connHidWifi = new DlgHideWifi(0);
        connHidWifi->show();
        connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    } else if(ui->cbxSecurity->currentIndex()==1) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiWpa *connHidWifiWpa = new DlgHideWifiWpa(0);
        connHidWifiWpa->show();
        connect(connHidWifiWpa, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    } else if(ui->cbxSecurity->currentIndex()==2) {
        if (WepOrWpa == 0) {
            ui->cbxSecurity->setCurrentIndex(6);
            WepOrWpa = 1;
        }
    } else if(ui->cbxSecurity->currentIndex()==3) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiWep *connHidWifiWep = new DlgHideWifiWep(0);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==4) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiWep *connHidWifiWep = new DlgHideWifiWep(1);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==5) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiLeap *connHidWifiLeap = new DlgHideWifiLeap();
        connHidWifiLeap->show();
    } else {
        if (WepOrWpa == 1) {
            ui->cbxSecurity->setCurrentIndex(5);
            WepOrWpa = 0;
        }
    }
}

//同一 Wi-Fi安全类型的Authentication变换
void DlgHideWifiEapPeap::changeDialogAuth()
{
    if (ui->cbxAuth->currentIndex()==0) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiEapTls *connHidWifiEapTls = new DlgHideWifiEapTls(WepOrWpa, 0, mw);
        connHidWifiEapTls->show();
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
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiEapFast *connHidWifiEapFast = new DlgHideWifiEapFast(WepOrWpa);
        connHidWifiEapFast->show();
    } else if(ui->cbxAuth->currentIndex()==4) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiEapTTLS *connHidWifiEapTTls = new DlgHideWifiEapTTLS(WepOrWpa);
        connHidWifiEapTTls->show();
    } else {
        qDebug()<<"it's not need to change dialog";
    }
}

//同一 Wi-Fi安全类型的窗口变换
void DlgHideWifiEapPeap::changeWindow(){
    if (ui->cbxConn->currentIndex() == 0) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifi *connHidWifi = new DlgHideWifi(0, mw);
        connHidWifi->show();
        connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    } else if (ui->cbxConn->currentIndex() >= 1) {
        QString tmpPath = "/tmp/kylin-nm-connshow-" + QDir::home().dirName();
        QString currStr = "nmcli connection show '" + ui->cbxConn->currentText() + "' > " + tmpPath;

        int status = system(currStr.toUtf8().data());
        qDebug()<<"executed cmd="<<currStr<<". res="<<status;
        QFile file(tmpPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"Can't open the file!";
        }
        QString txt = file.readAll();
        file.close();
        if (txt.indexOf("802-11-wireless-security.key-mgmt:") != -1) {
            if (txt.indexOf("wpa-psk") != -1) {
                QApplication::setQuitOnLastWindowClosed(false);
                this->hide();
                DlgHideWifiWpa *connHidWifiWpa = new DlgHideWifiWpa(ui->cbxConn->currentIndex(), mw);
                connHidWifiWpa->show();
                connect(connHidWifiWpa, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
            }
            if (txt.indexOf("wpa-eap") != -1) {
                isUsed = ui->cbxConn->currentIndex();
                ui->leNetName->setText(ui->cbxConn->currentText());
                ui->leNetName->setEnabled(false);
                ui->cbxSecurity->setEnabled(false);
                ui->cbxAuth->setEnabled(false);
                ui->leAnonyId->setEnabled(false);
                ui->leDomain->setEnabled(false);
                ui->cbxCA->setEnabled(false);
                ui->leCaPwd->setEnabled(false);
                ui->checkBoxCA->setEnabled(false);
                ui->cbxPEAPver->setEnabled(false);
                ui->cbxInnerAuth->setEnabled(false);
                ui->leUserName->setText("<Hidden>");
                ui->leUserName->setEnabled(false);
                ui->lePassword->setText("<Hidden>");
                ui->lePassword->setEnabled(false);
                ui->btnConnect->setEnabled(true);
            }
        } else {
            QApplication::setQuitOnLastWindowClosed(false);
            this->hide();
            DlgHideWifi *connHidWifi = new DlgHideWifi(ui->cbxConn->currentIndex(), mw);
            connHidWifi->show();
            connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
        }
    }
}

//CA证书的Index变化
void DlgHideWifiEapPeap::on_cbxCA_currentIndexChanged(const QString &arg1)
{
    if (ui->cbxCA->currentIndex() == 0) {
        ui->leCaPwd->setText("");
        ui->lbCaPwd->setEnabled(false);
        ui->leCaPwd->setEnabled(false);
    } else {
        ui->leCaPwd->setText("");
        ui->lbCaPwd->setEnabled(true);
        ui->leCaPwd->setEnabled(true);
    }
}

void DlgHideWifiEapPeap::on_btnCancel_clicked()
{
    this->close();
}

void DlgHideWifiEapPeap::on_btnConnect_clicked()
{
    this->close();
}

//是否显示密码
void DlgHideWifiEapPeap::on_checkBoxPwd_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->leCaPwd->setEchoMode(QLineEdit::Password);
    } else {
        ui->leCaPwd->setEchoMode(QLineEdit::Normal);
    }
}

//是否是不需要CA证书
void DlgHideWifiEapPeap::on_checkBoxCA_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->cbxCA->setCurrentIndex(0);
        ui->leCaPwd->setText("");
        ui->lbCA->setEnabled(true);
        ui->cbxCA->setEnabled(true);
    } else {
        ui->cbxCA->setCurrentIndex(0);
        ui->leCaPwd->setText("");
        ui->lbCA->setEnabled(false);
        ui->cbxCA->setEnabled(false);
    }
}

//是否显示密码
void DlgHideWifiEapPeap::on_checkBoxPwdSec_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->lePassword->setEchoMode(QLineEdit::Password);
    } else {
        ui->lePassword->setEchoMode(QLineEdit::Normal);
    }
}

//编辑网络名称
void DlgHideWifiEapPeap::on_leNetName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else {
        if (ui->checkBoxCA->isChecked()) {
            //勾选"不需要CA证书"
            ui->btnConnect->setEnabled(true);
        } else {
            //未勾选"不需要CA证书"
            if (ui->cbxCA->currentIndex() == 0) {
                ui->btnConnect->setEnabled(true);
            } else if(ui->leCaPwd->text() == "") {
                ui->btnConnect->setEnabled(false);
            } else {
                ui->btnConnect->setEnabled(true);
            }
        }
    }
}

//编辑匿名身份
void DlgHideWifiEapPeap::on_leAnonyId_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leAnonyId->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leDomain->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else {
        if (ui->cbxCA->currentIndex() == 0) {
            ui->btnConnect->setEnabled(true);
        } else if(ui->leCaPwd->text() == "") {
            ui->btnConnect->setEnabled(false);
        } else {
            ui->btnConnect->setEnabled(true);
        }
    }
}

//编辑域名
void DlgHideWifiEapPeap::on_leDomain_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leAnonyId->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leDomain->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else {
        if (ui->cbxCA->currentIndex() == 0) {
            ui->btnConnect->setEnabled(true);
        } else if(ui->leCaPwd->text() == "") {
            ui->btnConnect->setEnabled(false);
        } else {
            ui->btnConnect->setEnabled(true);
        }
    }
}

//编辑CA证书密码
void DlgHideWifiEapPeap::on_leCaPwd_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leAnonyId->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leDomain->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leCaPwd->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

//编辑用户名
void DlgHideWifiEapPeap::on_leUserName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else {
        if (ui->checkBoxCA->isChecked()) {
            //勾选"不需要CA证书"
            ui->btnConnect->setEnabled(true);
        } else {
            //未勾选"不需要CA证书"
            if (ui->cbxCA->currentIndex() == 0) {
                ui->btnConnect->setEnabled(true);
            } else if(ui->leCaPwd->text() == "") {
                ui->btnConnect->setEnabled(false);
            } else {
                ui->btnConnect->setEnabled(true);
            }
        }
    }
}

//编辑密码
void DlgHideWifiEapPeap::on_lePassword_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->leUserName->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else if (ui->lePassword->text() == "") {
        ui->btnConnect->setEnabled(false);
    } else {
        if (ui->checkBoxCA->isChecked()) {
            //勾选"不需要CA证书"
            ui->btnConnect->setEnabled(true);
        } else {
            //未勾选"不需要CA证书"
            if (ui->cbxCA->currentIndex() == 0) {
                ui->btnConnect->setEnabled(true);
            } else if(ui->leCaPwd->text() == "") {
                ui->btnConnect->setEnabled(false);
            } else {
                ui->btnConnect->setEnabled(true);
            }
        }
    }
}

void DlgHideWifiEapPeap::paintEvent(QPaintEvent *event)
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
