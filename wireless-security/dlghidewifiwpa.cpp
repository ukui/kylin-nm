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

DlgHideWifiWpa::DlgHideWifiWpa(int type, MainWindow *mainWindow, QWidget *parent) :
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

    MyQss objQss;

    ui->lbBoder->setStyleSheet("QLabel{border-radius:6px;background-color:rgba(19,19,20,0.95);border:1px solid rgba(255, 255, 255, 0.05);}");
    ui->lbBoder->hide();
    ui->lbLeftupTitle->setStyleSheet("QLabel{font-size:20px;}");

    ui->checkBoxPwd->setStyleSheet(objQss.checkBoxQss);

    ui->lineUp->setStyleSheet(objQss.lineQss);
    ui->lineDown->setStyleSheet(objQss.lineQss);
    ui->btnCancel->setFocusPolicy(Qt::NoFocus);
    ui->checkBoxPwd->setFocusPolicy(Qt::NoFocus);


    ui->lbLeftupTitle->setText(tr("Add Hidden Wi-Fi")); //加入隐藏Wi-Fi
    ui->lbConn->setText(tr("Connection")); //连接设置:
    ui->lbNetName->setText(tr("Wi-Fi name")); //网络名称:
    ui->lbSecurity->setText(tr("Wi-Fi security")); //Wi-Fi 安全性:
    ui->lbPassword->setText(tr("Password")); //密码:
    ui->btnCancel->setText(tr("Cancel")); //取消
    ui->btnConnect->setText(tr("Connect")); //连接

    ui->cbxConn->addItem(tr("C_reate…")); //新建...
    KylinDBus mkylindbus;
    QStringList wifiList = mkylindbus.getWifiSsidList();
    foreach (QString strWifiSsid, wifiList) {
        ui->cbxConn->addItem(strWifiSsid);
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
    ui->cbxSecurity->setCurrentIndex(1);
    connect(ui->cbxSecurity,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialog()));

    if (isUsed == 0) {
        ui->btnConnect->setEnabled(false);
    } else {
        ui->cbxConn->setCurrentIndex(isUsed);
        ui->leNetName->setText(ui->cbxConn->currentText());
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

    this->setFixedSize(432,397);

    this->mw = mainWindow;

    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
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
        QApplication::setQuitOnLastWindowClosed(false);
        this->close();
        DlgHideWifi *connHidWifi = new DlgHideWifi(0,mw);
        connHidWifi->show();
        connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    } else if(ui->cbxSecurity->currentIndex()==1) {
        qDebug()<<"it's not need to change dialog";
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
void DlgHideWifiWpa::changeWindow()
{
    if (ui->cbxConn->currentIndex() == 0){
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgHideWifi *connHidWifi = new DlgHideWifi(0, mw);
        connHidWifi->show();
        connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    }else if (ui->cbxConn->currentIndex() >= 1){
        QString tmpPath = "/tmp/kylin-nm-connshow-" + QDir::home().dirName();
        QString currStr = "nmcli connection show '" + ui->cbxConn->currentText() + "' > " + tmpPath;

        int status = system(currStr.toUtf8().data());
        if(status != 0){
            syslog(LOG_ERR, "execute 'nmcli connection show' in function 'changeWindow' failed");
        }
        QFile file(tmpPath);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"Can't open the file!";
        }
        QString txt = file.readAll();
        file.close();
        if (txt.indexOf("802-11-wireless-security.key-mgmt:") != -1){
            if (txt.indexOf("wpa-psk") != -1) {
                isUsed = ui->cbxConn->currentIndex();
                ui->lbNetName->setEnabled(false);
                ui->leNetName->setText(ui->cbxConn->currentText());
                ui->leNetName->setEnabled(false);
                ui->lbSecurity->setEnabled(false);
                ui->cbxSecurity->setEnabled(false);
                ui->lePassword->setText("<Hidden>");
                ui->lbPassword->setEnabled(false);
                ui->lePassword->setEnabled(false);
                ui->btnConnect->setEnabled(true);
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
        }else {
            QApplication::setQuitOnLastWindowClosed(false);
            this->hide();
            DlgHideWifi *connHidWifi = new DlgHideWifi(ui->cbxConn->currentIndex(), mw);
            connHidWifi->show();
            connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
        }
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
    this->close();
}

//void DlgHideWifiWpa::finishedProcess(int state) {
//    if (! state) syslog(LOG_ERR, "Scan wifi list failed in functin on_btnConnect_clicked() in dlghidewifiwpa.cpp 359.");
//    wlist = shellOutput.split("\n");
//    bool is_hidden  = true;
//    foreach (QString wifi, wlist) {
//        if (wifi.trimmed() == ui->leNetName->text()) {
//            is_hidden = false;
//        }
//    }
//    if (! is_hidden) {
//        BackThread *bt = new BackThread();
//        bt->execConnWifi(ui->leNetName->text());
//        QTimer::singleShot(4*1000, this, SLOT(emitSignal() ));
//    } else {
//        //已保存的wifi没有在wifi列表找到（隐藏wifi保存后也会出现在wifi列表），则当前区域无法连接此wifi
//        syslog(LOG_DEBUG, "Choosen wifi can not be sacnned in finishedProcess() in dlghidewifiwpa.cpp 377.");
//        QString txt(tr("Selected Wifi has not been scanned."));
//        mw->objKyDBus->showDesktopNotify(txt);
//        emit this->stopSignal();
//        emit reSetWifiList();
//    }
//    shellProcess->deleteLater();
//}

void DlgHideWifiWpa::on_checkBoxPwd_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->lePassword ->setEchoMode(QLineEdit::Password);
    } else {
        ui->lePassword->setEchoMode(QLineEdit::Normal);
    }
}

void DlgHideWifiWpa::on_leNetName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "" || ui->lePassword->text() == ""){
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
    if (ui->leNetName->text() == "" || ui->lePassword->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgHideWifiWpa::slotStartConnectHiddenWifi()
{
    mw->startLoading();
    emit sigConnHiddenWifi(ui->leNetName->text(), ui->lePassword->text());
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
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli device wifi connect' in function 'on_execSecConn' failed");}
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
