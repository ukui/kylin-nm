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

#include "onelancform.h"
#include "ui_onelancform.h"
#include "mainwindow.h"

#include <time.h>

OneLancForm::OneLancForm(QWidget *parent, MainWindow *mainWindow, ConfForm *confForm, KSimpleNM *ksnm) :
    QWidget(parent),
    ui(new Ui::OneLancForm)
{
    ui->setupUi(this);

    ui->btnConnSub->setText(tr("Connect"));//"连接"
    ui->btnConn->setText(tr("Connect"));//"连接"
    ui->btnDisConn->setText(tr("Disconnect"));//"断开连接"
    ui->btnCancel->setText(tr("Cancel"));//取消连接

    ui->lbConned->setAlignment(Qt::AlignLeft);

    leQssLow = "QLineEdit{border:none;background:transparent;font-size:14px;}";
    leQssHigh = "QLineEdit{border:none;background:transparent;font-size:14px;}";

    ui->leInfo_1->setStyleSheet(leQssLow);
    ui->leInfo_2->setStyleSheet(leQssLow);
    ui->leInfo_3->setStyleSheet(leQssLow);
    ui->leInfo_4->setStyleSheet(leQssLow);
    ui->btnInfo->setStyleSheet("QPushButton{border:none;background:transparent;}");
    ui->wbg->setStyleSheet("#wbg{border-radius:4px;background-color:rgba(156,156,156,0.1);}");
    ui->wbg_2->setStyleSheet("#wbg_2{border-radius:4px;background-color:rgba(156,156,156,0);}");
    ui->lbName->setStyleSheet("QLabel{font-size:14px;}");
    ui->lbConned->setStyleSheet("QLabel{font-size:14px;}");
    ui->btnConnSub->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->btnDisConn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->btnConn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->btnCancel->setStyleSheet("QPushButton{border:none;background:transparent;color:white;font-size:14px;}");
    ui->lbWaiting->setStyleSheet("QLabel{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);}");
    ui->lbWaitingIcon->setStyleSheet("QLabel{border:0px;background-color:transparent;}");

    ui->btnInfo->setCursor(QCursor(Qt::PointingHandCursor));
    ui->btnInfo->setFocusPolicy(Qt::NoFocus);
    ui->btnConnSub->setFocusPolicy(Qt::NoFocus);
    ui->btnConn->setFocusPolicy(Qt::NoFocus);
    ui->btnDisConn->setFocusPolicy(Qt::NoFocus);
    ui->btnCancel->setFocusPolicy(Qt::NoFocus);

    ui->wbg->hide();
    ui->wbg_2->show();
    ui->lbName->show();
    ui->btnConnSub->hide();
    ui->btnConn->hide();
    ui->btnDisConn->hide();
    ui->btnCancel->hide();
    ui->line->show();
    ui->lbWaiting->hide();
    ui->lbWaitingIcon->hide();

    this->mw = mainWindow;
    this->cf = confForm;
    this->ks = ksnm;

    this->isSelected = false;
    this->isActive = false;

    this->setAttribute(Qt::WA_Hover,true);
    this->installEventFilter(this);
    ui->btnInfo->setAttribute(Qt::WA_Hover,true);
    ui->btnInfo->installEventFilter(this);

    this->waitTimer = new QTimer(this);
    connect(waitTimer, SIGNAL(timeout()), this, SLOT(waitAnimStep()));

    connect(mw, SIGNAL(waitLanStop()), this, SLOT(stopWaiting()));

    srand((unsigned)time(NULL));
}

OneLancForm::~OneLancForm()
{
    delete ui;
}

void OneLancForm::mousePressEvent(QMouseEvent *)
{
    emit selectedOneLanForm(ssidName, uuidName);//避免重名情况
}

//事件过滤器
bool OneLancForm::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->btnInfo) {
        if(event->type() == QEvent::HoverEnter) {
            ui->leInfo_1->setStyleSheet(leQssHigh);
            ui->leInfo_2->setStyleSheet(leQssHigh);
            ui->leInfo_3->setStyleSheet(leQssHigh);
            ui->leInfo_4->setStyleSheet(leQssHigh);
            return true;
        } else if(event->type() == QEvent::HoverLeave) {
            ui->leInfo_1->setStyleSheet(leQssLow);
            ui->leInfo_2->setStyleSheet(leQssLow);
            ui->leInfo_3->setStyleSheet(leQssLow);
            ui->leInfo_4->setStyleSheet(leQssLow);
            return true;
        }
    } else if (obj == this) {
        if (event->type() == QEvent::HoverEnter) {
            if (!this->isTopItem) {
                if (!this->isSelected) {
                    ui->btnConn->show();
                    ui->wbg_2->setStyleSheet("#wbg_2{border-radius:4px;background-color:rgba(156,156,156,0.1);}");
                    ui->wbg_2->show();
                }
            }
            return true;
        } else if(event->type() == QEvent::HoverLeave) {
            ui->btnConn->hide();
            ui->wbg_2->setStyleSheet("#wbg_2{border-radius:4px;background-color:rgba(156,156,156,0);}");
            ui->wbg_2->hide();
            return true;
        }
    }

    return QWidget::eventFilter(obj,event);
}

// 是否当前连接的网络，字体设置不同
void OneLancForm::setAct(bool isAct)
{
    if (isAct) {
        ui->lbConned->show();
        ui->btnConnSub->hide();
    } else {
        ui->lbConned->hide();
        ui->btnConnSub->hide();
    }
    isActive = isAct;
}

// 是否选中
void OneLancForm::setSelected(bool isSelected, bool isCurrName)
{
    if (isSelected) {
        resize(W_ITEM, H_ITEM_EXTEND);
        ui->wbg->show();
        ui->wbg_2->hide();
        ui->line->move(X_LINE_EXTEND, Y_LINE_EXTEND);
        ui->btnConn->hide();
        ui->btnConnSub->show();

        this->isSelected = true;
    } else {
        resize(W_ITEM, H_ITEM);
        ui->wbg->hide();
        ui->wbg_2->show();
        ui->line->move(X_LINE, Y_LINE);
        if(isCurrName){
            ui->btnConn->show();
        }else{
            ui->btnConn->hide();
        }
        ui->btnConnSub->hide();

        this->isSelected = false;
    }

    ui->btnDisConn->hide();

    this->isTopItem = false;
}

//设置顶部这个item的显示
void OneLancForm::setTopItem(bool isSelected)
{
    if (isSelected) {
        resize(W_ITEM, H_ITEM_EXTEND);
        ui->wbg->show();
        ui->btnConnSub->hide();
        this->isSelected = true;
        ui->line->move(X_LINE_EXTEND, Y_LINE_EXTEND);
    } else {
        resize(W_ITEM, H_ITEM);
        ui->wbg->hide();
        ui->btnConnSub->hide();
        this->isSelected = false;
        ui->line->move(X_LINE, Y_LINE);
    }
    if (isConnected) {
        ui->btnDisConn->show();
    } else {
        ui->btnDisConn->hide();
    }

    ui->btnConn->hide();
    ui->wbg_2->hide();

    this->isTopItem = true;
}

//设置网络名称
void OneLancForm::setName(QString ssid, QString transSsid, QString uuid, QString interface)
{
    ui->lbName->setText(ssid);
    ssidName = ssid;
    m_sConnectType = transSsid;
    uuidName = uuid;
    ifName = interface;
}

//根据有线网络连接与否，设置显示'已连接'文字的控件的可见与否
void OneLancForm::setConnedString(bool showLable, QString str, QString ifname)
{
    if (!showLable) {
        ui->lbConned->hide();
        ui->lbName->move(63, 18);
    } else {
        ui->lbConned->setText(str+ifname);
    }
}

//设置item被扩展后出现的网络信息
void OneLancForm::setLanInfo(QString str1, QString str2, QString str3, QString str4)
{
    if (str2 == "" || str2 == "auto") {
        str2 = tr("No Configuration");
    }

    if (str1 == "" || str1 == "auto") {
        str1 = tr("No Configuration");
        str2 = tr("No Configuration");
    }

    if (str4 == "--" || str4 == "") {
        str1 = tr("No Configuration");
        str4 = tr("No IfName");
    }

    QString strIPv4 = QString(tr("IPv4："));
    QString strIPv6 = QString(tr("IPv6："));
    QString strBW = QString(tr("BandWidth："));
    QString strMAC = QString(tr("MAC："));

    ui->leInfo_1->setText(strIPv4 + str1);
    ui->leInfo_2->setText(strIPv6 + str2);
    ui->leInfo_3->setText(strBW + str3);
    ui->leInfo_4->setText(strMAC + str4);
}

//根据网络是否连接，设置网络状态图标
void OneLancForm::setIcon(bool isOn)
{
    if (isOn) {
        ui->lbIcon->setStyleSheet("QLabel{border-radius:0px;background:url(:/res/l/network-online.png);}");
    } else {
        ui->lbIcon->setStyleSheet("QLabel{border-radius:0px;background:url(:/res/l/network-offline.png);}");
    }
}

//设置item下方横线的可见与否
void OneLancForm::setLine(bool isShow)
{
    if (isShow) {
        ui->line->show();
    } else {
        ui->line->hide();
    }
}

void OneLancForm::slotConnLan()
{
    //mw->startLoading();
    this->startWaiting(true);
    //emit sigConnLan(ui->lbName->text(), ifName);
    emit sigConnLan(uuidName, ifName, m_sConnectType);
}

//点击网络断开按钮，执行该函数
void OneLancForm::on_btnDisConn_clicked()
{
    syslog(LOG_DEBUG, "DisConnect button about lan net is clicked, current wired net name is %s .", ui->lbName->text().toUtf8().data());
    qDebug()<<"DisConnect button about lan net is clicked, current wired net name is "<<ui->lbName->text();

    this->startWaiting(false);
    mw->is_stop_check_net_state = 1;

    //使用有线网ssid断开网络
    //kylin_network_set_con_down(ssidName.toUtf8().data());
    //使用有线网uuid断开网络
    kylin_network_set_con_down(uuidName.toUtf8().data());
    //使用dbus接口断开网络
    //toDisConnWiredNetwork(uuidName);

    disconnect(this, SIGNAL(selectedOneLanForm(QString, QString)), mw, SLOT(oneTopLanFormSelected(QString, QString)));

    emit disconnActiveLan();
}

void OneLancForm::toDisConnWiredNetwork(QString netUuid)
{
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    //获取已经连接了那些网络，及这些网络对应的网络类型(ethernet or wifi)
    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath activeConnPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> activeConnPath;

        QDBusInterface interfacePro( "org.freedesktop.NetworkManager",
                                  activeConnPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> replyUuid = interfacePro.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Uuid");
        if (replyUuid.value().toString() == netUuid) {
            //断开当前连接网络
            QDBusInterface disConnIf("org.freedesktop.NetworkManager",
                                       "/org/freedesktop/NetworkManager",
                                       "org.freedesktop.NetworkManager",
                                       QDBusConnection::systemBus() );

            QDBusReply<QDBusObjectPath> disReply = disConnIf.call("DeactivateConnection", QVariant::fromValue(activeConnPath));
        }
    }
    dbusArgs.endArray();
}


//点击了连接网络按钮，执行该函数
void OneLancForm::on_btnConn_clicked()
{
    syslog(LOG_DEBUG, "A button named btnConn about lan net is clicked.");
    qDebug()<<"A button named btnConn about lan net is clicked.";
    toConnectWiredNetwork();
}

//点击了item被扩展中的连接网络按钮，执行该函数
void OneLancForm::on_btnConnSub_clicked()
{
    syslog(LOG_DEBUG, "A button named btnConnSub about lan net is clicked.");
    qDebug()<<"A button named btnConnSub about lan net is clicked.";
    toConnectWiredNetwork();
}

void OneLancForm::toConnectWiredNetwork()
{
    mw->is_stop_check_net_state = 1;
    QThread *t = new QThread();
    BackThread *bt = new BackThread();
    bt->moveToThread(t);
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), this, SLOT(slotConnLan()));
    connect(this, SIGNAL(sigConnLan(QString, QString, QString)), bt, SLOT(execConnLan(QString, QString, QString)));
    connect(bt, SIGNAL(connDone(int)), mw, SLOT(connLanDone(int)));
    connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
    t->start();
}

//点击列表中item扩展后显示信息的位置时，执行该函数，用于显示网络配置界面
void OneLancForm::on_btnInfo_clicked()
{
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

    BackThread *bt = new BackThread();
    QString connProp = bt->getConnProp(uuidName);
    QStringList propList = connProp.split("|");
    QString v4method, addr, mask, gateway, dns;
    foreach (QString line, propList) {
        if (line.startsWith("method:")) {
            v4method = line.split(":").at(1);
        }
        if (line.startsWith("addr:")) {
            addr = line.split(":").at(1);
        }
        if (line.startsWith("mask:")) {
            mask = line.split(":").at(1);
        }
        if (line.startsWith("gateway:")) {
            gateway= line.split(":").at(1);
        }
        if (line.startsWith("dns:")) {
            dns = line.split(":").at(1);
        }
    }
    // qDebug()<<v4method<<addr<<mask<<gateway<<dns;

    connect(cf, SIGNAL(requestRefreshLanList(int)), mw, SLOT(onBtnNetListClicked(int)));
    cf->setProp(ui->lbName->text(), uuidName, v4method, addr, mask, gateway, dns, this->isActive, false);
    cf->move(primaryGeometry.width() / 2 - cf->width() / 2, primaryGeometry.height() / 2 - cf->height() / 2);
    cf->exec();
    cf->raise();
    cf->activateWindow();

    bt->deleteLater();
}

void OneLancForm::waitAnimStep()
{
    QString qpmQss = "QLabel{background-image:url(':/res/s/conning-a/";
    qpmQss.append(QString::number(this->waitPage));
    qpmQss.append(".png');}");
    ui->lbWaitingIcon->setStyleSheet(qpmQss);

    this->waitPage --;
    if (this->waitPage < 1) {
        this->waitPage = TOTAL_PAGE; //循环播放8张图片
    }

    this->countCurrentTime += FRAME_SPEED;
    if (this->countCurrentTime >= LIMIT_TIME) {
        QString cmd = "kill -9 $(pidof nmcli)"; //杀掉当前正在进行的有关nmcli命令的进程
        int status = system(cmd.toUtf8().data());
        if (status != 0) {
            qDebug()<<"execute 'kill -9 $(pidof nmcli)' in function 'waitAnimStep' failed";
            syslog(LOG_ERR, "execute 'kill -9 $(pidof nmcli)' in function 'waitAnimStep' failed");
        }

        this->stopWaiting(); //动画超出时间限制，强制停止动画

        mw->is_stop_check_net_state = 0;
    }
}

void OneLancForm::startWaiting(bool isToConnect)
{
    if (isToConnect) {
        ui->btnCancel->show();
        ui->lbWaiting->setStyleSheet("QLabel{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);}");
    } else {
        ui->lbWaitingIcon->move(ui->lbWaitingIcon->x() - 24, ui->lbWaitingIcon->y());
        ui->btnDisConn->hide();
        ui->lbWaiting->setStyleSheet("QLabel{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);}");
    }
    this->countCurrentTime = 0;
    this->waitPage = TOTAL_PAGE; //总共有8张图片
    this->waitTimer->start(FRAME_SPEED);
    ui->lbWaiting->show();
    ui->lbWaitingIcon->show();

    mw->setTrayLoading(true);
}

void OneLancForm::stopWaiting()
{
    ui->lbWaitingIcon->move(380, 20);
    ui->btnCancel->hide();

    this->waitTimer->stop();
    ui->lbWaiting->hide();
    ui->lbWaitingIcon->hide();

    mw->setTrayLoading(false);
    mw->getActiveInfo();
}

void OneLancForm::on_btnCancel_clicked()
{
    QString cmd = "kill -9 $(pidof nmcli)"; //杀掉当前正在进行的有关nmcli命令的进程
    int status = system(cmd.toUtf8().data());
    if (status != 0) {
        qDebug()<<"execute 'kill -9 $(pidof nmcli)' in function 'on_btnCancel_clicked' failed";
        syslog(LOG_ERR, "execute 'kill -9 $(pidof nmcli)' in function 'on_btnCancel_clicked' failed");
    }

    this->stopWaiting();
    mw->is_stop_check_net_state = 0;
}
