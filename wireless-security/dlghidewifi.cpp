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


#include "dlghidewifi.h"
#include "kylinheadfile.h"
#include "ui_dlghidewifi.h"
#include "src/backthread.h"
#include "src/mainwindow.h"
#include "src/kylin-dbus-interface.h"
#include "src/wpawifidialog.h"

#include <unistd.h>
#include <QPoint>

#include <QStandardItemModel>
#include <QDir>

DlgHideWifi::DlgHideWifi(int type, MainWindow *mainWindow, QWidget *parent) :
    isUsed(type),
    QDialog(parent),
    ui(new Ui::DlgHideWifi)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
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

    ui->lineUp->setStyleSheet(objQss.lineQss);
    ui->lineDown->setStyleSheet(objQss.lineQss);

    ui->lbLeftupTitle->setText(tr("Add Hidden WLAN")); //加入隐藏WLAN
    ui->lbNetName->setText(tr("WLAN name")); //网络名称:
    ui->lbSecurity->setText(tr("WLAN security")); //Wi-Fi安全性:
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
    ui->cbxSecurity->setCurrentIndex(0);
    connect(ui->cbxSecurity,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialog()));

    if (isUsed == 0) {
        ui->btnConnect->setEnabled(false);
    } else {
        ui->lbNetName->setEnabled(false);
        ui->leNetName->setEnabled(false);
        ui->lbSecurity->setEnabled(false);
        ui->cbxSecurity->setEnabled(false);
        ui->btnConnect->setEnabled(true);
    }

    ui->leNetName->setContextMenuPolicy(Qt::NoContextMenu); //禁止LineEdit的右键菜单

    this->setFixedSize(432,288);

    this->mw = mainWindow;
}

DlgHideWifi::~DlgHideWifi()
{
    delete ui;
}

void DlgHideWifi::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }

    return QDialog::mousePressEvent(event);
}
void DlgHideWifi::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
    return QDialog::mouseReleaseEvent(event);
}
void DlgHideWifi::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
    return QDialog::mouseMoveEvent(event);
}

//切换到其他Wi-Fi安全类型
void DlgHideWifi::changeDialog()
{
    if(ui->cbxSecurity->currentIndex()==0){
        //无安全性
        qDebug()<<"it's not need to change dialog";
    } else if(ui->cbxSecurity->currentIndex()==1) {
        //WPA 及 WPA2 个人
        QApplication::setQuitOnLastWindowClosed(false);
        this->close();
        DlgHideWifiWpa *connHidWifiWpa = new DlgHideWifiWpa(1, 0, mw);
        connHidWifiWpa->show();
        connect(connHidWifiWpa, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
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
            QString txt(tr("Conn WLAN Success"));
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
        QApplication::setQuitOnLastWindowClosed(false);
        this->close();
        DlgHideWifiWpa *connHidWifiWpa = new DlgHideWifiWpa(3, 0, mw);
        connHidWifiWpa->show();
        connect(connHidWifiWpa, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    } else if(ui->cbxSecurity->currentIndex()==4) {
        //WPA3 个人
        QApplication::setQuitOnLastWindowClosed(false);
        this->close();
        DlgHideWifiWpa *connHidWifiWpa = new DlgHideWifiWpa(4, 0, mw);
        connHidWifiWpa->show();
        connect(connHidWifiWpa, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
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

void DlgHideWifi::on_btnCancel_clicked()
{
    //this->close();
    this->hide();
}

void DlgHideWifi::on_btnConnect_clicked()
{
    mw->is_stop_check_net_state = 1;
    mw->is_connect_hide_wifi = 1;

    QThread *t = new QThread();
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(this, SIGNAL(stopSignal()), t, SLOT(quit()));

    QString wifiName = ui->leNetName->text();

    strWifiname = wifiName;
    BackThread *bt = new BackThread();
    if (isUsed == 0) {
        bt->moveToThread(t);
        connect(t, SIGNAL(started()), this, SLOT(slotStartConnectHiddenOpenWifi()));
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
    //this->close();
    this->hide();
}

void DlgHideWifi::on_leNetName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }

    if (ui->leNetName->text().size() > 32) {
        QString cutStr = ui->leNetName->text().mid(0,32);
        ui->leNetName->setText(cutStr);
    }
}

void DlgHideWifi::slotStartLoading()
{
    mw->startLoading();
}

void DlgHideWifi::slotStartConnectHiddenOpenWifi()
{
    mw->startLoading();
    emit sigConnHiddenWifi(ui->cbxSecurity->currentIndex(), ui->leNetName->text(), QString(""));
}

void DlgHideWifi::slotStartConnectRememberedHiddenWifi()
{
    mw->startLoading();
    emit sigConnRememberedHiddenWifi(ui->leNetName->text());
}

void DlgHideWifi::on_execSecConn()
{
    QString str = "nmcli device wifi connect '" + strWifiname + "' password ''";
    int status = system(str.toUtf8().data());
    qDebug()<<"executed cmd="<<str<<". res="<<status;
    QTimer::singleShot(3*1000, this, SLOT(emitSignal() ));
}

void DlgHideWifi::emitSignal()
{
    emit reSetWifiList();
    mw->stopLoading();
    emit this->stopSignal();
}

void DlgHideWifi::paintEvent(QPaintEvent *event)
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
