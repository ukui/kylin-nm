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

#include "oneconnform.h"
#include "ui_oneconnform.h"
#include "mainwindow.h"
#include "kylin-dbus-interface.h"
#include "kylin-network-interface.h"
#include "wireless-security/dlghidewifi.h"
#include "utils.h"
#include "wificonfigdialog.h"
#include "wpawifidialog.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusObjectPath>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QtConcurrent>

extern int currentActWifiSignalLv;

OneConnForm::OneConnForm(QWidget *parent, MainWindow *mainWindow, ConfForm *confForm, KSimpleNM *ksnm) :
    QWidget(parent),
    ui(new Ui::OneConnForm)
{
    ui->setupUi(this);

    ui->btnConnSub->setText(tr("Connect"));//"设置"
    ui->btnConn->setText(tr("Connect"));//"连接"
    ui->btnConnPWD->setText(tr("Connect"));//"连接"
    ui->btnDisConn->setText(tr("Disconnect"));//"断开连接"
    ui->btnHideConn->setText(tr("Connect"));//"连接"
    ui->btnCancel->setText(tr("Cancel"));//取消连接

    ui->lbConned->setAlignment(Qt::AlignLeft);
    ui->lePassword->setEchoMode(QLineEdit::Normal);
    ui->lePassword->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lePassword->setTextMargins(0,0,24,0);
    ui->btnConnPWD->setEnabled(false);

    leQssLow = "QLineEdit{border:none;background:transparent;font-size:14px;}";
    leQssHigh = "QLineEdit{border:none;background:transparent;font-size:14px;}";
    funcBtnQss = "QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0);color:rgba(107,142,235,0.97);font-size:14px;}"
                 "QPushButton:Hover{border:0px;border-radius:4px;background-color:rgba(255,255,255,0);color:rgba(151,175,241,0.97);font-size:14px;}"
                 "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0);color:rgba(61,107,229,0.97);font-size:14px;}";

    ui->leInfo_1->setStyleSheet(leQssLow);
    ui->leInfo_2->setStyleSheet(leQssLow);
    ui->leInfo_3->setStyleSheet(leQssLow);
    ui->leInfo_1->setEnabled(false);
    ui->leInfo_2->setEnabled(false);
    ui->leInfo_3->setEnabled(false);
    ui->btnInfo->setStyleSheet("QPushButton{border:none;background:transparent;}");
    ui->wbg->setStyleSheet("#wbg{border-radius:4px;background-color:rgba(156,156,156,0);}");
    ui->wbg_2->setStyleSheet("#wbg_2{border-radius:4px;background-color:rgba(156,156,156,0.1);}");
    ui->wbg_3->setStyleSheet("#wbg_3{border-radius:4px;background-color:rgba(156,156,156,0.1);}");
    ui->lbName->setStyleSheet("QLabel{font-size:14px;}");
    ui->lbConned->setStyleSheet("QLabel{font-size:14px;}");
    ui->lePassword->setStyleSheet("QLineEdit{border:1px solid rgba(61,107,229,1);font-size:14px;}");
    ui->checkBoxPwd->setStyleSheet("QCheckBox::indicator {width: 18px; height: 9px;}"
                                   "QCheckBox::indicator:checked {image: url(:/res/h/show-pwd.png);}"
                                   "QCheckBox::indicator:unchecked {image: url(:/res/h/hide-pwd.png);}");
    ui->btnConnSub->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->btnConn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->btnConnPWD->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
    ui->btnDisConn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->btnCancel->setStyleSheet("QPushButton{border:none;background:transparent;color:white;font-size:14px;}");
    ui->btnHideConn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->lbWaiting->setStyleSheet("QLabel{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);}");
    ui->lbWaitingIcon->setStyleSheet("QLabel{border:0px;background-color:transparent;}");

    ui->btnInfo->setCursor(QCursor(Qt::PointingHandCursor));
    ui->btnInfo->setFocusPolicy(Qt::NoFocus);
    ui->checkBoxPwd->setFocusPolicy(Qt::NoFocus);
    ui->btnConnSub->setFocusPolicy(Qt::NoFocus);
    ui->btnConn->setFocusPolicy(Qt::NoFocus);
    ui->btnConnPWD->setFocusPolicy(Qt::NoFocus);
    ui->btnDisConn->setFocusPolicy(Qt::NoFocus);
    ui->btnHideConn->setFocusPolicy(Qt::NoFocus);

    ui->wbg->show();
    ui->wbg_2->hide();
    ui->wbg_3->hide();
    ui->lbName->show();
    ui->leInfo_1->hide();
    ui->leInfo_2->hide();
    ui->leInfo_3->hide();
    ui->lePassword->hide();
    ui->checkBoxPwd->hide();
    ui->checkBoxPwd->setChecked(true);
    ui->btnConnSub->hide();
    ui->btnConn->hide();
    ui->btnDisConn->hide();
    ui->btnConnPWD->hide();
    ui->btnCancel->hide();
    ui->btnHideConn->hide();
    ui->line->show();
    ui->lbWaiting->hide();
    ui->lbWaitingIcon->hide();
    ui->btnInfo->hide();

    btnProperty = new QPushButton(ui->wbg_3);
    btnProperty->resize(60, 20);
    btnProperty->move(335, 108);
    btnProperty->setText(tr("Property"));//"属性"
    btnProperty->setStyleSheet(funcBtnQss);
    btnProperty->setFocusPolicy(Qt::NoFocus);
    btnProperty->hide();
    connect(btnProperty,SIGNAL(clicked()),this,SLOT(onBtnPropertyClicked()));

    this->mw = mainWindow;
    this->cf = confForm;
    this->ks = ksnm;

    this->isSelected = false;
    this->isActive = false;

    this->setAttribute(Qt::WA_Hover,true);
    this->installEventFilter(this);
    ui->lePassword->setAttribute(Qt::WA_Hover,true);
    ui->lePassword->installEventFilter(this);
    ui->btnInfo->setAttribute(Qt::WA_Hover,true);
    ui->btnInfo->installEventFilter(this);

//    connect(ui->lePassword, SIGNAL(returnPressed()), this, SLOT(on_btnConnPWD_clicked()));
    ui->btnConn->setShortcut(Qt::Key_Return);//将字母区回车键与连接按钮绑定在一起
    ui->btnConnSub->setShortcut(Qt::Key_Return);//点击连接按钮触发回车键

    this->waitTimer = new QTimer(this);
    connect(waitTimer, SIGNAL(timeout()), this, SLOT(waitAnimStep()));

    connect(mw, &MainWindow::reConnectWifi, this, [ = ](const QString& uuid) {
        if (isActive) {
            QThread *t = new QThread();
            BackThread *bt = new BackThread();
            bt->moveToThread(t);
            connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
            connect(t, &QThread::started, bt, [ = ]() {
                mw->is_wifi_reconnected = 1;
                bt->execReconnWIfi(uuid);
            });
            t->start();
        }
    });

    connect(mw, &MainWindow::startReconnectWifi, this, [ = ](const QString &ssid) {
        if (ssid == this->wifiName && !this->isWaiting) {
            this->startWifiWaiting(true);
        }
    });
    connect(mw, &MainWindow::stopReconnectWifi, this, [ = ](const QString &ssid) {
        if (ssid == this->wifiName && this->isWaiting) {
            this->stopWifiWaiting(true);
        }
    });

    connType = "";
    lbNameLyt = new QHBoxLayout(ui->lbName);
    lbFreq = new QLabel(ui->lbName);
    lbFreq->setAlignment(Qt::AlignCenter);
    lbFreq->setEnabled(false);
    lbFreq->setFixedHeight(14);
    lbFreq->setStyleSheet("QLabel{border: 1px solid rgba(165, 165, 165, 1);"
                          "background: transparent; color: rgba(0, 0, 0, 0.5);"
                          "border-radius: 3px; font-size: 10px;"
                          "color: rgba(165, 165, 165, 1)}");
    lbFreq->setContentsMargins(2, 0, 2, 0);
    lbFreq->hide();
    lbNameText = new QLabel(ui->lbName);
    lbNameLyt->setContentsMargins(0, 0, 0, 0);
    lbNameLyt->setSpacing(8);
    lbNameLyt->addWidget(lbNameText);
    lbNameLyt->addWidget(lbFreq);
    lbNameLyt->addStretch();
    ui->lbName->setLayout(lbNameLyt);

    connect(this, SIGNAL(requestRefreshWifiList()), mw, SLOT(onRequestRefreshWifiList()));

    m_menu = new QMenu();//右键菜单
}

OneConnForm::~OneConnForm()
{
    delete ui;
    if (m_menu) {
        delete m_menu;
        m_menu = nullptr;
    }
}

void OneConnForm::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && this->wifiBSsid != "--") {
        m_menu->clear();
        if (this->isTopItem) {
            m_menu->addAction(new QAction(tr("Disconnect"), this));
        } else {
            m_menu->addAction(new QAction(tr("Connect"), this));
        }
        if (checkIsSaved())
            m_menu->addAction(new QAction(tr("Forget"), this));
        m_menu->move(cursor().pos());
        m_menu->show();
        connect(m_menu, &QMenu::triggered, this, &OneConnForm::onMenuTriggered);
    }
    emit selectedOneWifiForm(wifiBSsid, H_WIFI_ITEM_BIG_EXTEND);
}

bool OneConnForm::checkIsSaved()
{
    QString name = this->wifiName;
    QString currStr = "nmcli -f connection.type connection show \"" + name.replace("\"","\\\"") + "\"";
    int status = system(currStr.toUtf8().data());
    if (status != 0){
        qDebug()<<"There is no configuration for wifi "<<this->wifiName;
        return false;
    } else {
        return true;
    }
}

/**
 * @brief OneConnForm::onMenuTriggered 点击右键菜单选项的槽函数
 * @param action
 */
bool OneConnForm::onMenuTriggered(QAction *action)
{
    if (action->text() == tr("Disconnect")) {
        this->on_btnDisConn_clicked();
        return true;
    } else if (action->text() == tr("Connect")) {
        this->on_btnConn_clicked();
        return true;
    } else if (action->text() == tr("Forget")) {
        QString name = this->wifiName;
        QString currStr = "nmcli connection delete \"" + name.replace("\"","\\\"") + "\"";
        int status = system(currStr.toUtf8().data());
        if (status != 0){
            qDebug()<<"Delete wifi failed. wifi="<<this->wifiName;
            return false;
        }
        return true;
    }
}

void OneConnForm::onBtnPropertyClicked()
{
    //WORKAROUND::ukui-control-center启动会出现rfkill僵尸进程
    //添加sleep后可以正常打开
    //此处sleep了10毫秒
    QProcess::startDetached(QString("bash -c \"sleep 0.01; ukui-control-center --netconnect;\""));
}

//事件过滤器
bool OneConnForm::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->btnInfo) {
        if (event->type() == QEvent::HoverEnter) {
            ui->leInfo_1->setStyleSheet(leQssHigh);
            ui->leInfo_2->setStyleSheet(leQssHigh);
            ui->leInfo_3->setStyleSheet(leQssHigh);
            return true;
        } else if(event->type() == QEvent::HoverLeave) {
            ui->leInfo_1->setStyleSheet(leQssLow);
            ui->leInfo_2->setStyleSheet(leQssLow);
            ui->leInfo_3->setStyleSheet(leQssLow);
            return true;
        }
    } else if (obj == this) {
        if(event->type() == QEvent::HoverEnter) {
            if (!this->isTopItem) {
                if (!this->isSelected) {
                    ui->btnConn->show();
                    ui->wbg->setStyleSheet("#wbg{border-radius:4px;background-color:rgba(156,156,156,0.1);}");
                    ui->wbg->show();
                }
            }
            return true;
        } else if (event->type() == QEvent::HoverLeave) {
            ui->btnConn->hide();
            ui->wbg->setStyleSheet("#wbg{border-radius:4px;background-color:rgba(156,156,156,0);}");
            ui->wbg->hide();
            return true;
        }
    }

    if (obj == ui->lePassword) {
        if (event->type() == QEvent::MouseButtonPress) {
            this->setLePassword();
        } else {
            return false;
        }
    }

    return QWidget::eventFilter(obj,event);
}

// 是否当前连接的网络，字体设置不同
void OneConnForm::setAct(bool isAct)
{
    if (isAct) {
        ui->lbConned->show();
        btnProperty->show();
    } else {
        ui->lbConned->hide();
        btnProperty->hide();
    }
    isActive = isAct;
}

void OneConnForm::setLePassword()
{
    if (ui->lePassword->text() == "Input Password..."  || ui->lePassword->text() == "输入密码...") {
        ui->lePassword->setText(tr(""));
        ui->lePassword->setEchoMode(QLineEdit::Password);
        ui->checkBoxPwd->setChecked(false);
    }
}

//点击窗口最上面的item时
void OneConnForm::setTopItem(bool isSelected)
{
    if (isSelected) {
        resize(W_ITEM, H_ITEM_BIG);
        ui->wbg_3->show();
        ui->leInfo_1->show();
        ui->leInfo_2->show();
        ui->leInfo_3->show();

        this->isSelected = true;
    } else {
        resize(W_ITEM, H_ITEM);
        ui->lePassword->setText("");
        ui->wbg_3->hide();
        ui->leInfo_1->hide();
        ui->leInfo_2->hide();
        ui->leInfo_3->hide();


        this->isSelected = false;
    }

    ui->wbg->hide();
    ui->wbg_2->hide();
    ui->lbSignal->show();
    ui->lePassword->hide();
    ui->checkBoxPwd->hide();
    ui->line->hide();
    ui->btnConn->hide();
    ui->btnConnPWD->hide();
    ui->btnHideConn->hide();
    ui->btnInfo->show();

    if (isConnected) {
        if (this->isWaiting) {
            ui->btnDisConn->hide();
        } else {
            ui->btnDisConn->show();
        }
    } else {
        ui->btnDisConn->hide();
    }

    this->isTopItem = true;
}
// 点击窗口下面的item时
void OneConnForm::setSelected(bool isSelected, bool isCurrName)
{
    if (isSelected) {
        resize(W_ITEM, H_ITEM_BIG);
        ui->line->move(X_LINE_BIG_EXTEND, Y_LINE_BIG_EXTEND);
        ui->wbg->hide();
        ui->wbg_2->hide();
        ui->wbg_3->show();
        ui->leInfo_1->show();
        ui->leInfo_2->show();
        ui->leInfo_3->show();
        ui->btnConn->hide();
        ui->btnConnSub->show();

        this->isSelected = true;
    } else {
        resize(W_ITEM, H_ITEM);
        ui->lePassword->setText("");
        ui->lePassword->setStyleSheet("QLineEdit{border:1px solid rgba(61,107,229,1);border-radius:4px;background:rgba(0,0,0,0.2);}");
        ui->lePassword->setEchoMode(QLineEdit::Normal);
        ui->checkBoxPwd->setChecked(true);

        ui->line->move(X_LINE, Y_LINE);
        ui->wbg->show();
        ui->wbg_2->hide();
        ui->wbg_3->hide();
        ui->leInfo_1->hide();
        ui->leInfo_2->hide();
        ui->leInfo_3->hide();

        if (isCurrName) {
            ui->btnConn->show();
        } else {
            ui->btnConn->hide();
        }
        ui->btnConnSub->hide();

        this->isSelected = false;
    }

    ui->lePassword->hide();
    ui->checkBoxPwd->hide();
    ui->btnConnPWD->hide();
    ui->lbSignal->show();
    ui->btnDisConn->hide();
    ui->btnHideConn->hide();
    ui->btnInfo->hide();

    this->isTopItem = false;
}
// 点击连接隐藏wifi的item时
void OneConnForm::setHideItem(bool isHideItem, bool isShowHideBtn)
{
    if (isHideItem) {
        ui->lbName->move(14, 20);
        ui->wbg->hide();
        ui->btnConn->hide();
    } else {
        ui->lbName->move(62, 8);
        ui->wbg->show();
        ui->btnConn->show();
    }

    if (isShowHideBtn) {
        ui->btnHideConn->show();
    } else{
        ui->btnHideConn->hide();
    }
}

void OneConnForm::setConnedString(bool showLable, QString str, QString str1)
{
    if (!showLable) {
        ui->lbConned->setText(str1);
        ui->lbConned->hide();
        ui->lbName->move(63, 18);
    } else {
        ui->lbConned->setText(str);
    }
}

void OneConnForm::setWifiName(QString name, QString bssid, QString uuid, QString ifname, bool isHW)
{
    QFontMetrics fontMetrics(lbNameText->font());
    QString showname = fontMetrics.elidedText(name, Qt::ElideRight, 200);
    lbNameText->setText(showname);
    wifiName = name;
    wifiBSsid = bssid;
    wifiUuid = uuid;
    wifiIfName = ifname;
    isHuaweiPC = isHW;
}

QString OneConnForm::getName()
{
    return wifiName;
}

void OneConnForm::setRate(QString rate)
{
    QString txt(tr("Rate"));//"速率"
    this->setToolTip("<span style=\"font-size:14px;border:none;background-color:#3593b5;color:white;\">&nbsp; " + txt + ": " + rate + " &nbsp;</span>");
    this->setToolTip(txt + ":" + rate);
}

void OneConnForm::setLine(bool isShow)
{
    if (isShow) {
        ui->line->show();
    } else {
        ui->line->hide();
    }
}

void OneConnForm::setSignal(QString lv, QString secu, QString category)
{
    this->m_signal = lv.toInt();
    int signal = lv.toInt();
    if (secu == "--" || secu == "") {
        hasPwd = false;
    } else {
        hasPwd = true;
    }

    QString signalStyle;
    if (isHuaweiPC) {
        signalStyle = "QLabel{border-radius:0px;background:url(:/res/hw/wifi";
    } else {
        signalStyle = "QLabel{border-radius:0px;background:url(:/res/w/wifi";
    }

    if ("1" == category) {
        signalStyle += "6";
    } else if ("2" == category) {
        signalStyle += "6+";
    }

    if (signal > 75) {
        if (hasPwd) {
            signalStyle += "-full-pwd.png);}";
        } else {
            signalStyle += "-full.png);}";
        }
        signalLv = 1;
    }
    if (signal > 55 && signal <= 75) {
        if (hasPwd) {
            signalStyle += "-high-pwd.png);}";
        } else {
            signalStyle += "-high.png);}";
        }
        signalLv = 2;
    }
    if (signal > 35 && signal <= 55) {
        if (hasPwd) {
            signalStyle += "-medium-pwd.png);}";
        } else {
            signalStyle += "-medium.png);}";
        }
        signalLv = 3;
    }
    if (signal > 15 && signal <= 35) {
        if (hasPwd) {
            signalStyle += "-low-pwd.png);}";
        } else {
            signalStyle += "-low.png);}";
        }
        signalLv = 4;
    }
    if (signal <= 15) {
        if (hasPwd) {
            signalStyle += "-none-pwd.png);}";
        } else {
            signalStyle += "-none.png);}";
        }
        signalLv = 4;
    }
    ui->lbSignal->setStyleSheet(signalStyle);
}

int OneConnForm::getSignal()
{
    return this->m_signal;
}

void OneConnForm::setWifiInfo(QString str1, QString str2, QString str3, int freq)
{
    //freq 0:含2.4G和5G， 1：只有2.4G， 2：只有5G
    lbFreq->show();
    if (freq == 1) {
        //freq ~ 2.4G
        lbFreq->setText("2.4G");
    } else if (freq == 2) {
        //freq ~ 5G
        lbFreq->setText("5G");
    } else {
        //freq ~ 5G&2.4G
        lbFreq->setText("2.4/5G");
    }
    if (str1 == "--" || str1 == ""){ str1 = tr("None"); };

    QString strSecurity = QString(tr("WiFi Security："));
    QString strSignal = QString(tr("Signal："));
    QString strMAC = QString(tr("MAC："));
    wifiSecu = str1;

    ui->leInfo_1->setText(strSecurity + str1);
    ui->leInfo_2->setText(strSignal + str2);
    ui->leInfo_3->setText(strMAC + str3);
}

void OneConnForm::slotConnWifi()
{
    this->startWifiWaiting(true);
    emit sigConnWifi(wifiName, wifiIfName);
}
void OneConnForm::slotConnWifiPWD()
{
    this->startWifiWaiting(true);
    emit sigConnWifiPWD(wifiName, ui->lePassword->text(), connType, wifiSecu);
}

//点击后断开wifi网络
void OneConnForm::on_btnDisConn_clicked()
{
    if (mw->is_stop_check_net_state == 1) {
        return;
    }

    qDebug()<<"DisConnect button about wifi net is clicked, current wifi name is "<<wifiName;
    if (mw->canReconnectWifiList.contains(wifiName)) {
        mw->canReconnectWifiList.removeOne(wifiName);
    }

    this->startWifiWaiting(false);

    mw->is_stop_check_net_state = 1;
    //mw->on_btnHotspotState();
    //kylin_network_set_con_down(wifiName.toUtf8().data());
    kylin_network_set_con_down(wifiUuid.toUtf8().data());
    disconnect(this, SIGNAL(selectedOneWifiForm(QString,int)), mw, SLOT(oneTopWifiFormSelected(QString,int)));
    emit requestHandleWifiDisconn();
}

//点击列表item扩展时会出现该按钮 用于连接网络
void OneConnForm::on_btnConnSub_clicked()
{
    if (mw->is_stop_check_net_state == 1) {
        return;
    }

    qDebug()<<"A button named on_btnConnSub about wifi net is clicked.";
    toConnectWirelessNetwork();
}

//无需密码的wifi连接
void OneConnForm::on_btnConn_clicked()
{
    if (mw->is_stop_check_net_state == 1) {
        return;
    }

    qDebug()<<"A button named btnConn about wifi net is clicked.";
    toConnectWirelessNetwork();
}

void OneConnForm::toConnectWirelessNetwork()
{
    if (wifiSecu.contains("802.1x", Qt::CaseInsensitive)) {
        //企业wifi
        WifiConfig wc;
        bool isConfiged = getWifiConfig(wc, this->wifiName);
        if (isConfiged && wc.eap != "peap") {
            mw->is_stop_check_net_state = 1;
            QThread *t = new QThread();
            BackThread *bt = new BackThread();
            bt->moveToThread(t);
            connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
            connect(t, SIGNAL(started()), this, SLOT(slotConnWifi()));
            connect(this, SIGNAL(sigConnWifi(QString, QString)), bt, SLOT(execConnWifi(QString, QString)));
            connect(bt, SIGNAL(connDone(int)), mw, SLOT(connWifiDone(int)));
            connect(bt, SIGNAL(connDone(int)), this, SLOT(slotConnWifiResult(int)));
            connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
            t->start();
            return;
        }

        WpaWifiDialog * wpadlg = new WpaWifiDialog(mw, mw, this->wifiName);
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
        return;
    }

    if (ui->lbConned->text() == "--" || ui->lbConned->text() == " ") {
        if (!isWifiConfExist(wifiName)) {
            //没有配置文件，使用有密码的wifi连接
            psk_flag = 0;
            on_btnConnPWD_clicked();
            return;
        }
    }

    //有配置文件，需要判断一下当前配置文件wifi安全性是不是wpa-eap，若是，需要把原配置文件删除，重新配置
    QProcess * process = new QProcess(this);
    process->start(QString("nmcli -f 802-11-wireless-security.key-mgmt connection show '%1'").arg(wifiName));
    connect(process, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), this, [ = ]() {
        process->deleteLater();
    });
    connect(process, &QProcess::readyReadStandardOutput, this, [ = ]() {
        QString str = process->readAllStandardOutput();
        key_mgmt = str.mid(str.lastIndexOf(" ") + 1, str.length() - str.lastIndexOf(" ") - 2);
    });
    process->waitForFinished();
    if (QString::compare(key_mgmt, "wpa-eap") == 0) {
        //原配置文件是企业wifi，删掉，请求输入新的密码
        QString cmdStr = "nmcli connection delete '" +  wifiName + "'";
        Utils::m_system(cmdStr.toUtf8().data());
        psk_flag = 0;
        slotConnWifiResult(2); //现在已无配置文件，申请输入密码
        return;
    }

    if (isWifiConfExist(wifiName)) {
        //有配置文件，获取密码存储策略
        QProcess * process = new QProcess(this);
        process->start(QString("nmcli -f 802-11-wireless-security.psk-flags connection show \"%1\"").arg(wifiName));
        connect(process, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), this, [ = ]() {
            process->deleteLater();
        });
        connect(process, &QProcess::readyReadStandardOutput, this, [ = ]() {
            //QString str = process->readAllStandardOutput();
            //psk_flag = str.mid(str.lastIndexOf(" ") - 1, 1).toInt();
            QString str = process->readAllStandardOutput();
            QString regExpPattern("[ ][0-9][ (（]");
            QRegExp regExpTest(regExpPattern);
            int pos = str.indexOf(regExpTest);
            psk_flag = str.mid(pos,2).trimmed().toInt();
        });
        process->waitForFinished();
    }

    if (key_mgmt == "wpa-psk" && this->getPskFlag() == 2) {
        //当设置为每次询问密码时执行
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

        QApplication::setQuitOnLastWindowClosed(false);
        WiFiConfigDialog *wifiConfigDialog = new WiFiConfigDialog();
        wifiConfigDialog->move(primaryGeometry.width() / 2 - wifiConfigDialog->width() / 2, primaryGeometry.height() / 2 - wifiConfigDialog->height() / 2);
        wifiConfigDialog->show();
        wifiConfigDialog->raise();

        return;
    }

    if (psk_flag != 0) { //未为所有用户存储密码
        QString homePath = getenv("HOME");
        if (QFile(QString("%1/.config/%2.psk").arg(homePath).arg(wifiName)).exists()) { //已为该用户存储密码
            mw->is_stop_check_net_state = 1;
            QThread *t = new QThread();
            BackThread *bt = new BackThread();
            bt->moveToThread(t);
            connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
            connect(t, &QThread::started, this, [ = ]() {
                this->startWifiWaiting(true);
                QString cmdStr = "nmcli connection up '" + wifiName + "' passwd-file " + homePath +"/.config/" + wifiName + ".psk";
                qDebug()<<"Trying to connect wifi. ssid="<<wifiName;
                emit this->sigConnWifiPsk(cmdStr);
            });
            connect(this, SIGNAL(sigConnWifiPsk(QString)), bt, SLOT(execConnWifiPsk(QString)));
            connect(bt, &BackThread::connDone, this, [ = ](int res) {
                this->stopWifiWaiting(true);
                mw->is_stop_check_net_state = 0;
                if (res) {
                    QFile::remove(QString("%1/.config/%2.psk").arg(homePath).arg(wifiName).toUtf8());
                }
                mw->connWifiDone(res);
            });
            connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
            t->start();
        } else { //没有为该用户存储密码
            slotConnWifiResult(2);
        }
        return;
    } else { //为所有用户存储密码
        QString homePath = getenv("HOME");
        QFile::remove(QString("%1/.config/%2.psk").arg(homePath).arg(wifiName).toUtf8()); //删除密码文件
    }

    mw->is_stop_check_net_state = 1;
    QThread *t = new QThread();
    BackThread *bt = new BackThread();
    bt->moveToThread(t);
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), this, SLOT(slotConnWifi()));
    connect(this, SIGNAL(sigConnWifi(QString, QString)), bt, SLOT(execConnWifi(QString, QString)));
    connect(bt, SIGNAL(connDone(int)), mw, SLOT(connWifiDone(int)));
    connect(bt, SIGNAL(connDone(int)), this, SLOT(slotConnWifiResult(int)));
    connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
    t->start();
}

//需要密码的wifi连接
void OneConnForm::on_btnConnPWD_clicked()
{
    qDebug()<<"A button named btnConnPWD about wifi net is clicked.";

    if (this->getPskFlag() != 0) {
//        QString cmdStr = 0;
        QString homePath = getenv("HOME");
        QFile *passwdFile = new QFile(QString("%1/.config/%2.psk").arg(homePath).arg(wifiName));
        if (passwdFile->open(QIODevice::ReadWrite)) {
            passwdFile->write(QString("802-11-wireless-security.psk:%1").arg(ui->lePassword->text()).toUtf8());
            passwdFile->close();
//            cmdStr = "nmcli connection up " + wifiName + " passwd-file " + homePath +"/.config/" + wifiName + ".psk";
        }
        mw->is_stop_check_net_state = 1;
        QThread *t = new QThread();
        BackThread *bt = new BackThread();
        bt->moveToThread(t);
        connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
        connect(t, &QThread::started, this, [ = ]() {
            this->startWifiWaiting(true);
            QString cmdStr = "nmcli connection up '" + wifiName + "' passwd-file " + homePath +"/.config/" + wifiName + ".psk";
            qDebug()<<"Trying to connect wifi. ssid="<<wifiName;
            emit this->sigConnWifiPsk(cmdStr);
        });
        connect(this, SIGNAL(sigConnWifiPsk(QString)), bt, SLOT(execConnWifiPsk(QString)));
        connect(bt, &BackThread::connDone, this, [ = ](int res) {
            this->stopWifiWaiting(true);
            mw->is_stop_check_net_state = 0;
            if (res) {
                QFile::remove(QString("%1/.config/%2.psk").arg(homePath).arg(wifiName).toUtf8());
            }
            mw->connWifiDone(res);
        });
        connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
        t->start();
    }

    if (! mw->is_stop_check_net_state) {
        mw->is_stop_check_net_state = 1;
        QThread *t = new QThread();
        BackThread *bt = new BackThread();
        bt->moveToThread(t);
        connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
        connect(t, SIGNAL(started()), this, SLOT(slotConnWifiPWD()));
        connect(this, SIGNAL(sigConnWifiPWD(QString, QString, QString, QString)),
                bt, SLOT(execConnWifiPWD(QString, QString, QString, QString)));
        connect(bt, SIGNAL(connDone(int)), mw, SLOT(connWifiDone(int)));
        connect(bt, SIGNAL(connDone(int)), this, SLOT(slotConnWifiResult(int)));
        connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
        t->start();
    }
}

//点击后弹出连接隐藏wifi网络窗口
void OneConnForm::on_btnHideConn_clicked()
{
    QApplication::setQuitOnLastWindowClosed(false);
    DlgHideWifi *connHidWifi = new DlgHideWifi(0, mw);
    connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    connHidWifi->show();
}

bool OneConnForm::isWifiConfExist(QString netName)
{
    //dbusWifiMac = ""; //这个函数之前是用来获取已经连接的wifi的MAC地址

    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QList<QDBusObjectPath> m_objNets = m_reply.value();
    foreach (QDBusObjectPath objNet, m_objNets){
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objNet.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage reply = m_interface.call("GetSettings");
        const QDBusArgument &dbusArg = reply.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg >> map;

        for(QString key : map.keys() ){
            QMap<QString,QVariant> innerMap = map.value(key);
            if (key == "connection") {
                for (QString inner_key : innerMap.keys()) {
                    if (inner_key == "id"){
                        if (netName == innerMap.value(inner_key).toString()) {
                            return true;
                        }
                    }
                }
            }
        }

    } // end foreach (QDBusObjectPath objNet, m_objNets)

    return false;
}

//设置密码隐藏或可见
void OneConnForm::on_checkBoxPwd_pressed()
{
    ui->checkBoxPwd->setChecked(true);
    ui->lePassword->setEchoMode(QLineEdit::Normal);
}

void OneConnForm::on_checkBoxPwd_released()
{
    ui->checkBoxPwd->setChecked(false);
    ui->lePassword->setEchoMode(QLineEdit::Password);
}

void OneConnForm::on_lePassword_textEdited(const QString &arg1)
{
    ui->lePassword->setStyleSheet("QLineEdit{border:2px solid rgba(28, 47, 146, 1);border-radius:4px;background:rgba(0,0,0,0.2);}");

    if (ui->lePassword->text().size() < 6) {
        ui->btnConnPWD->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                                   "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                                   "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
        ui->btnConnPWD->setEnabled(false);
        disconnect(ui->lePassword, SIGNAL(returnPressed()), this, SLOT(on_btnConnPWD_clicked()));
        if (ui->lePassword->text().size() == 0) {
            ui->lePassword->setStyleSheet("QLineEdit{border:1px solid rgba(61,107,229,1);border-radius:4px;background:rgba(0,0,0,0.2);}");
        }
    } else {
        ui->btnConnPWD->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                                      "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                                      "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
        connect(ui->lePassword, SIGNAL(returnPressed()), this, SLOT(on_btnConnPWD_clicked()));
        ui->btnConnPWD->setEnabled(true);
    }
}

void OneConnForm::on_btnInfo_clicked()
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
    QString connProp = bt->getConnProp(wifiUuid);
    QStringList propList = connProp.split("|");
    QString v4method, addr, mask, gateway, dns, v6method, v6addr;
    foreach (QString line, propList) {
        if (line.startsWith("method:")) {
            v4method = line.split(":").at(1);
        }
        if (line.startsWith("v4addr:")) {
            addr = line.split(":").at(1);
        }
        if (line.startsWith("mask:")) {
            mask = line.split(":").at(1);
        }
        if (line.startsWith("v6method:")) {
            v6method = line.split(":").at(1);
        }
        if (line.startsWith("v6addr:")) {
            v6addr = line.right(line.length() - line.indexOf(":") - 1);
        }
        if (line.startsWith("gateway:")) {
            gateway= line.split(":").at(1);
        }
        if (line.startsWith("dns:")) {
            dns = line.split(":").at(1);
        }
    }
    // qDebug()<<"v4method:"<<v4method<<" addr:"<<addr<<" mask:"<<mask<<" gateway:"<<gateway<<" dns:"<<dns;

    cf->setProp(wifiName, wifiUuid, v4method, addr, v6method, v6addr, mask, gateway, dns, this->isActive, true);
    qDebug() << Q_FUNC_INFO << __LINE__ << wifiName << wifiUuid;
    cf->move(primaryGeometry.width() / 2 - cf->width() / 2, primaryGeometry.height() / 2 - cf->height() / 2);
    cf->show();
    cf->raise();

    bt->deleteLater();
}

// Wifi连接结果，0成功 1失败 2没有配置文件
void OneConnForm::slotConnWifiResult(int connFlag)
{
    qDebug()<<"Function slotConnWifiResult receives a number: "<<connFlag;

    if (!connType.isEmpty()) {
        QString strConntype = "nmcli connection modify '" + wifiName + "' wifi-sec.psk-flags 2";
        system(strConntype.toUtf8().data());
    }
    connType = "";

    if (connFlag == 0) {
        if (mw->isHuaWeiPC) {
            //network-manager可能会连接到其他bssid对应的网络，改成我们想要连接的那个网络
            QtConcurrent::run([=]() {
                QString currConnWifiBssid;
                KylinDBus myKylinDbus;
                QStringList wifiListInfo;
                QList<QString> wifiSsidAndUuid =  myKylinDbus.getAtiveWifiBSsidUuid(wifiListInfo);
                if (wifiSsidAndUuid.size() > 1 && wifiSsidAndUuid.at(1).length() == 17) {
                    qDebug() << "想要连接的wifi的bssid是 " << wifiSsidAndUuid.at(1);
                    currConnWifiBssid = wifiSsidAndUuid.at(1);
                }

                qDebug() << "实际连接的wifi的bssid是 " << wifiBSsid;
                if (currConnWifiBssid != wifiBSsid && !currConnWifiBssid.isEmpty()) {
                    QString modityCmd = "nmcli connection modify \""+ wifiName + "\" " + "802-11-wireless.bssid " + wifiBSsid;
                    int res = system(modityCmd.toUtf8().data());
                    qDebug()<<"Modified bssid for wifi. ssid="<<wifiName<<". bssid="<<wifiBSsid<<". res="<<res;
                    QString reconnectWifiCmd = "nmcli connection up \"" + wifiName + "\"";
                    qDebug()<<"Trying to connect wifi. ssid="<<wifiName;
                    system(reconnectWifiCmd.toUtf8().data());
                } else {
                    emit requestRefreshWifiList();
                }
            });
        }

        disconnect(this, SIGNAL(selectedOneWifiForm(QString,int)), mw, SLOT(oneWifiFormSelected(QString,int)));
    }

    if (connFlag == 1  || connFlag == 4) {
        // 使用配置文件连接失败，需要删除该配置文件
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection delete '" + wifiName + "'";
        int status = system(cmd.toUtf8().data());
        if (status != 0) {
            qDebug()<<"execute 'nmcli connection delete' in function 'slotConnWifiResult' failed.";
        }
    }

    if (connFlag == 2 || connFlag == 4 || connFlag == 1) {
        mw->currSelNetName = "";
        emit selectedOneWifiForm(wifiBSsid, H_WIFI_ITEM_SMALL_EXTEND);

        resize(W_ITEM, H_ITEM_MIDDLE);
        ui->wbg->hide();
        ui->wbg_2->show();
        ui->wbg_3->hide();
        ui->leInfo_1->hide();
        ui->leInfo_2->hide();
        ui->leInfo_3->hide();
        ui->btnHideConn->hide();
        ui->btnDisConn->hide();
        ui->btnConn->hide();
        ui->btnConnSub->hide();
        ui->line->move(X_LINE_SMALL_EXTEND, Y_LINE_SMALL_EXTEND);

        ui->lePassword->show();
        ui->checkBoxPwd->show();
        ui->btnConnPWD->show();

        this->isSelected = true;
        //if (connFlag == 2) {
        //    mw->is_stop_check_net_state = 0;
        //} else {
        //    mw->is_stop_check_net_state = 0;
        //    //connType = "RequestPassword";
        //}

        //设置输入密码框被选中
        ui->lePassword->setFocus();
        ui->lePassword->setEchoMode(QLineEdit::Password);
        ui->checkBoxPwd->setChecked(false);
    }

    // 设置全局变量，当前连接Wifi的信号强度
    currentActWifiSignalLv = signalLv;

    this->stopWifiWaiting(true);

    mw->is_stop_check_net_state = 0;
}

void OneConnForm::waitAnimStep()
{
    QString qpmQss = "QLabel{background-image:url(':/res/s/conning-a/";
    qpmQss.append(QString::number(this->waitPage));
    qpmQss.append(".png');}");
    ui->lbWaitingIcon->setStyleSheet(qpmQss);

    this->waitPage ++;
    if (this->waitPage > TOTAL_PAGE) {
        this->waitPage = 1; //循环播放8张图片
    }

    this->countCurrentTime += FRAME_SPEED;
    if (this->countCurrentTime >= LIMIT_TIME) {
        QString cmd = "kill -9 $(pidof nmcli)"; //杀掉当前正在进行的有关nmcli命令的进程
        int status = system(cmd.toUtf8().data());
        if (status != 0) {
            qDebug()<<"execute 'kill -9 $(pidof nmcli)' in function 'waitAnimStep' failed";
        }

        this->stopWifiWaiting(true); //动画超出时间限制，强制停止动画

        mw->is_stop_check_net_state = 0;
    }
}

void OneConnForm::startWifiWaiting(bool isToConnect)
{
    this->isWaiting = true;
    if (isToConnect) {
        ui->btnCancel->show();
        ui->lbWaiting->setStyleSheet("QLabel{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);}");
    } else {
        ui->lbWaitingIcon->move(ui->lbWaitingIcon->x() - 24, ui->lbWaitingIcon->y());
        ui->btnDisConn->hide();
        ui->lbWaiting->setStyleSheet("QLabel{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);}");
    }
    this->countCurrentTime = 0;
    this->waitPage = 1; //总共有8张图片
    this->waitTimer->start(FRAME_SPEED);
    ui->lbWaiting->show();
    ui->lbWaitingIcon->show();

    mw->setTrayLoading(true);
}

void OneConnForm::stopWifiWaiting(bool isUpdateTrayIcon)
{
    ui->lbWaitingIcon->move(380, 20);
    ui->btnCancel->hide();
    this->isWaiting = false;
    this->waitTimer->stop();
    ui->lbWaiting->hide();
    ui->lbWaitingIcon->hide();

    if (isUpdateTrayIcon) {
        mw->setTrayLoading(false);
        mw->getActiveInfoAndSetTrayIcon();
    }
}

void OneConnForm::on_btnCancel_clicked()
{
//    QString cmd = "kill -9 $(pidof nmcli)"; //杀掉当前正在进行的有关nmcli命令的进程
//    int status = system(cmd.toUtf8().data());
//    if (status != 0) {
//        qDebug()<<"execute 'kill -9 $(pidof nmcli)' in function 'on_btnCancel_clicked' failed";
//    }

    KylinDBus myKylinDbus;
    QStringList wifiListInfo;
    QList<QString> wifiSsidAndUuid =  myKylinDbus.getAtiveWifiBSsidUuid(wifiListInfo);
    if (wifiSsidAndUuid.size() >= 1 && wifiSsidAndUuid.at(0).length() != 17) {
        QString currentConnectWifiUuid = wifiSsidAndUuid.at(0);
        kylin_network_set_con_down(currentConnectWifiUuid.toUtf8().data());
    }
    this->stopWifiWaiting(true);
}

int OneConnForm::getPskFlag()
{
    QProcess * process = new QProcess(this);
    process->start(QString("nmcli -f 802-11-wireless-security.psk-flags connection show \"%1\"").arg(wifiName));
    connect(process, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), this, [ = ]() {
        process->deleteLater();
    });
    connect(process, &QProcess::readyReadStandardOutput, this, [ = ]() {
        QString str = process->readAllStandardOutput();
        QString regExpPattern("[ ][0-9][ (（]");
        QRegExp regExpTest(regExpPattern);
        int pos = str.indexOf(regExpTest);
        psk_flag = str.mid(pos,2).trimmed().toInt();
    });
    process->waitForFinished();
    return psk_flag;
}

/**
 * @brief OneConnForm::getWifiConfig
 * @param wc
 * @return
 */
bool OneConnForm::getWifiConfig(WifiConfig &wc, QString netName)
{
    bool isConfiged = false;
    QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                      "/org/freedesktop/NetworkManager/Settings",
                                      "org.freedesktop.NetworkManager.Settings",
                                      QDBusConnection::systemBus() );
    QDBusReply<QList<QDBusObjectPath>> m_reply = m_interface.call("ListConnections");

    QList<QDBusObjectPath> m_objNets = m_reply.value();
    foreach (QDBusObjectPath objNet, m_objNets){
        QDBusInterface m_interface("org.freedesktop.NetworkManager",
                                  objNet.path(),
                                  "org.freedesktop.NetworkManager.Settings.Connection",
                                  QDBusConnection::systemBus());
        QDBusMessage reply = m_interface.call("GetSettings");
        const QDBusArgument &dbusArg = reply.arguments().at( 0 ).value<QDBusArgument>();
        QMap<QString,QMap<QString,QVariant>> map;
        dbusArg >> map;

        for(QString key : map.keys() ){
            QMap<QString,QVariant> innerMap = map.value(key);
            if (key == "connection") {
                for (QString inner_key : innerMap.keys()) {
                    if (inner_key == "id"){
                        if (netName == innerMap.value(inner_key).toString()) {
                            isConfiged = true;
                            break;
                        }
                    }
                }
            }
        }

        if (isConfiged) {
            for(QString key : map.keys()) {
                QMap<QString,QVariant> eapMap = map.value(key);
                if (key == "802-1x") {
                    wc.connectName = netName;
                    for (QString eap_key : eapMap.keys()) {
                        if (eap_key == "ca-cert"){
                            qDebug() << "OneConnForm::getWifiConfig client-cert path = " << eapMap.value(eap_key).toString();
                            wc.caCert = eapMap.value(eap_key).toString();
                        } else if (eap_key == "client-cert"){
                            qDebug() << "OneConnForm::getWifiConfig client-cert path = " << eapMap.value(eap_key).toString();
                            wc.clientCert = eapMap.value(eap_key).toString();
                        } else if (eap_key == "eap") {
                            wc.eap = eapMap.value(eap_key).toStringList().at(0);
                            qDebug() << "OneConnForm::getWifiConfig eap = " << eapMap.value(eap_key).toStringList().at(0);
                        } else if (eap_key == "identity") {
                            wc.identity = eapMap.value(eap_key).toString();
                        } else if (eap_key == "private-key") {
                            wc.privateKey = eapMap.value(eap_key).toString();
                        } else if (eap_key == "anonymous-identity") {
                            wc.anonymousIdentity = eapMap.value(eap_key).toString();
                        }
                    }
                }
            }
            return true;
        }

    } // end foreach (QDBusObjectPath objNet, m_objNets)

    return false;
}
