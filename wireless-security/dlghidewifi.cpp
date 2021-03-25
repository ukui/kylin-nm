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

    MyQss objQss;

    ui->lbBoder->setStyleSheet("QLabel{border-radius:6px;background-color:rgba(19,19,20,0.95);border:1px solid rgba(255, 255, 255, 0.05);}");
    ui->lbBoder->hide();
    ui->lbLeftupTitle->setStyleSheet("QLabel{font-size:20px;}");

    ui->lineUp->setStyleSheet(objQss.lineQss);
    ui->lineDown->setStyleSheet(objQss.lineQss);

    ui->lbLeftupTitle->setText(tr("Add Hidden Wi-Fi")); //加入隐藏Wi-Fi
    ui->lbConn->setText(tr("Connection")); //连接设置:
    ui->lbNetName->setText(tr("Wi-Fi name")); //网络名称:
    ui->lbSecurity->setText(tr("Wi-Fi security")); //Wi-Fi安全性:
    ui->btnCancel->setText(tr("Cancel")); //取消
    ui->btnConnect->setText(tr("Connect")); //连接

    ui->cbxConn->clear();
    ui->cbxConn->addItem(tr("C_reate…")); //新建...
    KylinDBus mkylindbus;
    QStringList wifiList = mkylindbus.getWifiSsidList();
    foreach (QString strWifiSsid, wifiList) {
        ui->cbxConn->addItem(strWifiSsid);
    }
    ui->cbxConn->setCurrentIndex(isUsed);
    connect(ui->cbxConn,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeWindow()));

    ui->cbxSecurity->addItem(tr("None")); //无
    ui->cbxSecurity->addItem(tr("WPA and WPA2 Personal")); //WPA 及 WPA2 个人
    ui->cbxSecurity->addItem(tr("WPA and WPA2 Enterprise")); //WPA 及 WPA2 企业
    //ui->cbxSecurity->addItem(tr("WEP 40/128-bit Key (Hex or ASCII)")); //WEP 40/128 位密钥(十六进制或ASCII)
    //ui->cbxSecurity->addItem(tr("WEP 128-bit Passphrase")); //WEP 128 位密码句
    //ui->cbxSecurity->addItem("LEAP");
    //ui->cbxSecurity->addItem(tr("Dynamic WEP (802.1X)")); //动态 WEP (802.1x)
    ui->cbxSecurity->setCurrentIndex(0);
    connect(ui->cbxSecurity,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialog()));

    if (isUsed == 0) {
        ui->btnConnect->setEnabled(false);
    } else {
        ui->leNetName->setText(ui->cbxConn->currentText());
        ui->lbNetName->setEnabled(false);
        ui->leNetName->setEnabled(false);
        ui->lbSecurity->setEnabled(false);
        ui->cbxSecurity->setEnabled(false);
        ui->btnConnect->setEnabled(true);
    }

    ui->leNetName->setContextMenuPolicy(Qt::NoContextMenu); //禁止LineEdit的右键菜单

    this->setFixedSize(432,358);

    this->mw = mainWindow;

    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
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
        qDebug()<<"it's not need to change dialog";
    } else if(ui->cbxSecurity->currentIndex()==1) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->close();
        DlgHideWifiWpa *connHidWifiWpa = new DlgHideWifiWpa(0, mw);
        connHidWifiWpa->show();
        connect(connHidWifiWpa, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    } else if(ui->cbxSecurity->currentIndex()==2) {
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
            QString txt(tr("Confirm your Wi-Fi password or usable of wireless card"));
            mw->objKyDBus->showDesktopNotify(txt);
            mw->on_btnWifiList_clicked();
        });
//        DlgHideWifiEapPeap *connHidWifiEapPeap = new DlgHideWifiEapPeap(1, 0, mw);
//        connHidWifiEapPeap->show();
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
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifiEapPeap *connHidWifiEapPeap = new DlgHideWifiEapPeap(0, 0, mw);
        connHidWifiEapPeap->show();
    }
}

//同一 Wi-Fi安全类型的窗口变换
void DlgHideWifi::changeWindow(){
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
        QString tmpPath = "/tmp/kylin-nm-connshow-" + QDir::home().dirName();
        QString currStr = "nmcli connection show '" + ui->cbxConn->currentText() + "' >" + tmpPath;

        int status = system(currStr.toUtf8().data());
        if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection show' in function 'changeWindow' failed");}

        QFile file(tmpPath);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
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
                QApplication::setQuitOnLastWindowClosed(false);
                this->hide();
//                DlgHideWifiEapPeap *connHidWifiEapPeap = new DlgHideWifiEapPeap(1, ui->cbxConn->currentIndex(), mw);
//                connHidWifiEapPeap->show();
                WpaWifiDialog * wpadlg = new WpaWifiDialog(mw, mw, ui->cbxConn->currentText());
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
                    QString txt(tr("Confirm your Wi-Fi password or usable of wireless card"));
                    mw->objKyDBus->showDesktopNotify(txt);
                    mw->on_btnWifiList_clicked();
                });
            }
        } else {
            isUsed = ui->cbxConn->currentIndex();
            ui->leNetName->setText(ui->cbxConn->currentText());
            ui->lbNetName->setEnabled(false);
            ui->leNetName->setEnabled(false);
            ui->lbSecurity->setEnabled(false);
            ui->cbxSecurity->setEnabled(false);mw->on_btnWifiList_clicked();
            ui->btnConnect->setEnabled(true);
        }
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
        connect(this, SIGNAL(sigConnHiddenWifi(QString, QString)), bt, SLOT(execConnHiddenWifiWPA(QString,QString)));
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
    emit sigConnHiddenWifi(ui->leNetName->text(), QString(""));
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
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli device wifi connect' in function 'on_execSecConn' failed");}
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
