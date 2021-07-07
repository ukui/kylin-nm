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


#include "dlghidewifiwpa.h"
#include "ui_dlghidewifiwpa.h"
#include "kylinheadfile.h"
#include "src/backthread.h"
#include "src/mainwindow.h"
#include "src/wpawifidialog.h"

#include <unistd.h>
#include <stdlib.h>

#include <QDir>
#include <QtConcurrent>
#include <QFuture>

DlgHideWifiWpa::DlgHideWifiWpa(int securityType, int type, MainWindow *mainWindow, QWidget *parent) :
    secuType(securityType),
    isUsed(type),
    QDialog(parent),
    ui(new Ui::DlgHideWifiWpa)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
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

    ui->lineUp->setStyleSheet(objQss.lineQss);
    ui->lineDown->setStyleSheet(objQss.lineQss);
    ui->btnCancel->setFocusPolicy(Qt::NoFocus);
    ui->checkBoxPwd->setFocusPolicy(Qt::NoFocus);

    ui->lbLeftupTitle->setText(tr("Add Hidden WLAN")); //加入隐藏Wi-Fi
    ui->lbNetName->setText(tr("WLAN name")); //网络名称:
    ui->lbSecurity->setText(tr("WLAN security")); //Wi-Fi 安全性:
    ui->lbPassword->setText(tr("Password")); //密码:
    ui->btnCancel->setText(tr("Cancel")); //取消
    ui->btnConnect->setText(tr("Connect")); //连接
    ui->btnCancel->setStyleSheet(objQss.btnOffQss);
    ui->btnConnect->setStyleSheet(objQss.btnOffQss);

    ui->cbxSecurity->addItem(tr("None")); //无
    ui->cbxSecurity->addItem(tr("WPA and WPA2 Personal")); //WPA 及 WPA2 个人
    ui->cbxSecurity->addItem(tr("WPA and WPA2 Enterprise")); //WPA 及 WPA2 企业
    ui->cbxSecurity->addItem(tr("WPA2 and WPA3 Personal")); //WPA2 及 WPA3 个人
    ui->cbxSecurity->addItem(tr("WPA3 Personal")); //WPA3 个人
    //ui->cbxSecurity->addItem(tr("WEP 40/128-bit Key (Hex or ASCII)")); //WEP 40/128 位密钥(十六进制或ASCII)
    //ui->cbxSecurity->addItem(tr("WEP 128-bit Passphrase")); //WEP 128 位密码句
    //ui->cbxSecurity->addItem("LEAP");
    //ui->cbxSecurity->addItem(tr("Dynamic WEP (802.1X)")); //动态 WEP (802.1x)
    if (secuType == 1) {
        ui->cbxSecurity->setCurrentIndex(1);
    } else if (secuType == 3) {
        ui->cbxSecurity->setCurrentIndex(3);
    } else if (secuType == 4) {
        ui->cbxSecurity->setCurrentIndex(4);
    }
    connect(ui->cbxSecurity,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialog()));

    if (isUsed == 0) {
        ui->btnConnect->setEnabled(false);
    } else {
        ui->leNetName->setEnabled(false);
        ui->lbNetName->setEnabled(false);
        ui->lbSecurity->setEnabled(false);
        ui->cbxSecurity->setEnabled(false);
        ui->lbPassword->setEnabled(false);
        ui->lePassword->setText("<Hidden>");
        ui->lePassword->setEnabled(false);
        ui->btnConnect->setEnabled(true);
        ui->checkBoxPwd->setEnabled(false);
    }

    ui->leNetName->setContextMenuPolicy(Qt::NoContextMenu);//禁止LineEdit的右键菜单
    ui->lePassword->setContextMenuPolicy(Qt::NoContextMenu);

    this->setFixedSize(432,327);

    this->mw = mainWindow;
}

DlgHideWifiWpa::~DlgHideWifiWpa()
{
    delete ui;
}

void DlgHideWifiWpa::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
    return QDialog::mousePressEvent(event);
}
void DlgHideWifiWpa::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
    return QDialog::mouseReleaseEvent(event);
}
void DlgHideWifiWpa::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
    return QDialog::mouseMoveEvent(event);
}

//切换到其他Wi-Fi安全类型
void DlgHideWifiWpa::changeDialog()
{
    if (ui->cbxSecurity->currentIndex()==0) {
        //无安全性
        QApplication::setQuitOnLastWindowClosed(false);
        this->close();
        DlgHideWifi *connHidWifi = new DlgHideWifi(0,mw);
        connHidWifi->show();
        connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    } else if(ui->cbxSecurity->currentIndex()==1) {
        //WPA 及 WPA2 个人
        if (secuType == 1) {
            qDebug()<<"it's not need to change dialog";
        } else if (secuType == 3 || secuType == 4) {
            ui->cbxSecurity->setCurrentIndex(1);
        }
    } else if(ui->cbxSecurity->currentIndex()==2) {
        //WPA 及 WPA2 企业
        QApplication::setQuitOnLastWindowClosed(false);
        this->close();
        WpaWifiDialog * wpadlg = new WpaWifiDialog(mw, mw, "");
        QPoint pos = QCursor::pos();
        QRect primaryGeometry;
        for (QScreen *screen : qApp->screens()) {
            if (screen->geometry().contains(pos)) {
                primaryGeometry = screen->geometry();
            }
        }
        if (primaryGeometry.isEmpty()) {
            primaryGeometry = qApp->primaryScreen()->geometry();
        }
        wpadlg->move(primaryGeometry.width() / 2 - wpadlg->width() / 2, primaryGeometry.height() / 2 - wpadlg->height() / 2);
        wpadlg->show();
        connect(wpadlg, &WpaWifiDialog::conn_done, this, [ = ]() {
            QString txt(tr("Conn Wifi Success"));
            mw->objKyDBus->showDesktopNotify(txt);
            mw->on_btnWifiList_clicked();
        });
        connect(wpadlg, &WpaWifiDialog::conn_failed, this, [ = ]() {
            QString txt(tr("Confirm your WLAN password or usable of wireless card"));
            mw->objKyDBus->showDesktopNotify(txt);
            mw->on_btnWifiList_clicked();
        });
//        DlgHideWifiEapPeap *connHidWifiEapPeap = new DlgHideWifiEapPeap(1, 0, mw);
//        connHidWifiEapPeap->show();
    } else if(ui->cbxSecurity->currentIndex()==3) {
        //WPA2 及 WPA3 个人
        if (secuType == 1 || secuType == 4) {
            ui->cbxSecurity->setCurrentIndex(3);
        } else if (secuType == 3) {
            qDebug()<<"it's not need to change dialog";
        }
    } else if(ui->cbxSecurity->currentIndex()==4) {
        //WPA3 个人
        if (secuType == 1 || secuType == 3) {
            ui->cbxSecurity->setCurrentIndex(4);
        } else if (secuType == 4) {
            qDebug()<<"it's not need to change dialog";
        }
    } else if(ui->cbxSecurity->currentIndex()==5) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiWep *connHidWifiWep = new DlgHideWifiWep(0);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==6) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiWep *connHidWifiWep = new DlgHideWifiWep(1);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==7) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiLeap *connHidWifiLeap = new DlgHideWifiLeap();
        connHidWifiLeap->show();
    } else {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiEapPeap *connHidWifiEapPeap = new DlgHideWifiEapPeap(0, 0, mw);
        connHidWifiEapPeap->show();
    }
}

void DlgHideWifiWpa::on_btnCancel_clicked()
{
    //this->close();
    this->hide();
}

void DlgHideWifiWpa::on_btnConnect_clicked()
{
    mw->is_stop_check_net_state = 1;
    mw->is_connect_hide_wifi = 1;

    QThread *t = new QThread();
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(this, SIGNAL(stopSignal()), t, SLOT(quit()));

    QString wifiName = ui->leNetName->text();
    QString wifiPassword = ui->lePassword->text();

    strWifiname = wifiName;
    strWifiPassword = wifiPassword;
    BackThread *bt = new BackThread();
    if (isUsed == 0) {
        bt->moveToThread(t);
        connect(t, SIGNAL(started()), this, SLOT(slotStartConnectHiddenWifi()));
        connect(this, SIGNAL(sigConnHiddenWifi(int, QString, QString)), bt, SLOT(execConnHiddenWifiWPA(int, QString,QString)));
        connect(bt, SIGNAL(connDone(int)), mw, SLOT(connWifiDone(int)));
        connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
    } else {
        bt->moveToThread(t);
        connect(t, SIGNAL(started()), this, SLOT(slotStartConnectRememberedHiddenWifi()));
        connect(this, SIGNAL(sigConnRememberedHiddenWifi(QString)), bt, SLOT(execConnRememberedHiddenWifi(QString)));
        connect(bt, SIGNAL(connDone(int)), mw, SLOT(connWifiDone(int)));
        connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
    }
    t->start();
//    this->close();
    this->hide();
}

//切换密码明文
void DlgHideWifiWpa::on_checkBoxPwd_clicked()
{
    if (ui->lePassword->echoMode() == QLineEdit::Password) {
        ui->checkBoxPwd->setChecked(true);
        ui->lePassword->setEchoMode(QLineEdit::Normal);
    } else {
        ui->checkBoxPwd->setChecked(false);
        ui->lePassword->setEchoMode(QLineEdit::Password);
    }
}

//void DlgHideWifiWpa::on_checkBoxPwd_released()
//{
//    ui->checkBoxPwd->setChecked(false);
//    ui->lePassword->setEchoMode(QLineEdit::Password);
//}

void DlgHideWifiWpa::on_leNetName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "" || ui->lePassword->text().size() < 6){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }

    if (ui->leNetName->text().size() > 32) {
        QString cutStr = ui->leNetName->text().mid(0,32);
        ui->leNetName->setText(cutStr);
    }
}

void DlgHideWifiWpa::on_lePassword_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "" || ui->lePassword->text().size() < 6){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgHideWifiWpa::slotStartConnectHiddenWifi()
{
    mw->startLoading();
    emit sigConnHiddenWifi(ui->cbxSecurity->currentIndex(), ui->leNetName->text(), ui->lePassword->text());
}

void DlgHideWifiWpa::slotStartConnectRememberedHiddenWifi()
{
    mw->startLoading();
    emit sigConnRememberedHiddenWifi(ui->leNetName->text());
}

void DlgHideWifiWpa::on_execSecConn()
{
    QString str = "nmcli device wifi connect '" + strWifiname + "' password '" + strWifiPassword + "'";
    int status = system(str.toUtf8().data());
    qDebug()<<"executed cmd="<<str<<". res="<<status;
    qDebug() << "debug: 准备等待7秒";
    QTimer::singleShot(7*1000, this, SLOT(emitSignal() ));
}

void DlgHideWifiWpa::emitSignal()
{
    emit this->stopSignal();
    QFuture < void > future =  QtConcurrent::run([=](){
        int xx = 1;
        int nn = 0;
        do {
            BackThread *bt = new BackThread();
            IFace *iface = bt->execGetIface();

            sleep(1);
            nn += 1;
            if (nn == 8) {
                xx = 0;
            } else {
                if (iface->wstate != 2) {
                    qDebug() << "debug: 发出信号";
                    emit reSetWifiList();
                    mw->stopLoading();
                    xx = 0;
                }
            }

            delete iface;
            bt->deleteLater();
        } while(xx == 1);
    });
}

void DlgHideWifiWpa::paintEvent(QPaintEvent *event)
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
