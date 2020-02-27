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

#include "onelancform.h"
#include "ui_onelancform.h"
#include "mainwindow.h"

#include <time.h>

OneLancForm::OneLancForm(QWidget *parent, MainWindow *mainWindow, ConfForm *confForm, KSimpleNM *ksnm) :
    QWidget(parent),
    ui(new Ui::OneLancForm)
{
    ui->setupUi(this);

    ui->btnConnSub->setText(tr("Connect"));//"设置"
    ui->btnConn->setText(tr("Connect"));//"连接"
    ui->btnDisConn->setText(tr("Disconnect"));//"断开连接"

    ui->lbConned->setAlignment(Qt::AlignLeft);
    ui->lbLoadUp->setAlignment(Qt::AlignLeft);
    ui->lbLoadDown->setAlignment(Qt::AlignLeft);

    ui->lbInfo->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.57);}");
    ui->wbg->setStyleSheet("#wbg{border-radius:4px;background-color:rgba(255,255,255,0.1);}");
    ui->wbg_2->setStyleSheet("#wbg_2{border-radius:4px;background-color:rgba(255,255,255,0);}");
                             //"#wbg_2:Hover{border-radius:4px;background-color:rgba(255,255,255,0.1);}");
    ui->lbName->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.91);}");
    ui->lbConned->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.57);}");
    ui->lbLoadUp->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.57);}");
    ui->lbLoadDown->setStyleSheet("QLabel{font-size:14px;color:rgba(255,255,255,0.57);}");
    ui->btnConnSub->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->btnConn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->btnDisConn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
    ui->lbLoadDownImg->setStyleSheet("QLabel{background-image:url(:/res/x/load-down.png);}");
    ui->lbLoadUpImg->setStyleSheet("QLabel{background-image:url(:/res/x/load-up.png);}");
    ui->lbWaiting->setStyleSheet("QLabel{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);}");
    ui->lbWaitingIcon->setStyleSheet("QLabel{border:0px;background-color:transparent;}");

    ui->btnConnSub->setFocusPolicy(Qt::NoFocus);
    ui->btnConn->setFocusPolicy(Qt::NoFocus);
    ui->btnDisConn->setFocusPolicy(Qt::NoFocus);

    ui->wbg->hide();
    ui->wbg_2->show();
    ui->lbName->show();
    ui->btnConnSub->hide();
    ui->btnConn->hide();
    ui->btnDisConn->hide();
    ui->line->show();
    ui->lbLoadDownImg->hide();
    ui->lbLoadUpImg->hide();
    ui->lbWaiting->hide();
    ui->lbWaitingIcon->hide();

    this->mw = mainWindow;
    this->cf = confForm;
    this->ks = ksnm;

    this->isSelected = false;
    this->isActive = false;

    this->setAttribute(Qt::WA_Hover,true);
    this->installEventFilter(this);

    this->waitTimer = new QTimer(this);
    connect(waitTimer, SIGNAL(timeout()), this, SLOT(waitAnimStep()));

    connect(mw, SIGNAL(waitLanStop()), this, SLOT(stopWaiting()));

    srand((unsigned)time(NULL));
}

OneLancForm::~OneLancForm()
{
    delete ui;
}

void OneLancForm::mousePressEvent(QMouseEvent *){
    emit selectedOneLanForm(lanName);
}

//事件过滤器
bool OneLancForm::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this){
        if(event->type() == QEvent::HoverEnter) {
            if (this->isTopItem){
                qDebug()<<"hover enter the top item of the window";
            }else{
                if (this->isSelected){
                    qDebug()<<"will not show btnConn and wbg_2";
                } else {
                    ui->btnConn->show();
                    ui->wbg_2->setStyleSheet("#wbg_2{border-radius:4px;background-color:rgba(255,255,255,0.1);}");
                    ui->wbg_2->show();
                }
            }
            return true;
        } else if(event->type() == QEvent::HoverLeave){
            ui->btnConn->hide();
            ui->wbg_2->setStyleSheet("#wbg_2{border-radius:4px;background-color:rgba(255,255,255,0);}");
            ui->wbg_2->hide();
            return true;
        }
    }

    return QWidget::eventFilter(obj,event);
}

// 是否当前连接的网络，字体设置不同
void OneLancForm::setAct(bool isAct){
    if(isAct){
        ui->lbName->setStyleSheet("QLabel{font-size:14px;color:#ffffff;}");
        ui->lbConned->show();
        ui->btnConnSub->hide();
    }else{
        ui->lbName->setStyleSheet("QLabel{font-size:14px;color:#ffffff;}");
        ui->lbConned->hide();
        ui->btnConnSub->hide();
    }
    isActive = isAct;
}

// 是否选中
void OneLancForm::setSelected(bool isSelected, bool isCurrName)
{
    if(isSelected){
        resize(422, 168);
        ui->wbg->show();
        ui->wbg_2->hide();
        ui->line->move(0, 166);
        ui->btnConn->hide();
        ui->btnConnSub->show();

        this->isSelected = true;
    }else{
        resize(422, 60);
        ui->wbg->hide();
        ui->wbg_2->show();
        ui->line->move(0, 58);
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

void OneLancForm::setTopItem(bool isSelected)
{
    if(isSelected){
        resize(422, 168);
        ui->wbg->show();
        ui->btnConnSub->hide();
        this->isSelected = true;
    }else{
        resize(422, 60);
        ui->wbg->hide();
        ui->btnConnSub->hide();
        this->isSelected = false;
    }
    if (isConnected){
//        ui->lbLoadUp->setText("0Kb/s");
//        ui->lbLoadDown->setText("0Kb/s");
        ui->lbLoadUp->hide();
        ui->lbLoadDown->hide();
        ui->btnDisConn->show();
    } else {
//        ui->lbLoadUp->setText("0Kb/s");
//        ui->lbLoadDown->setText("0Kb/s");
        ui->lbLoadUp->hide();
        ui->lbLoadDown->hide();
        ui->btnDisConn->hide();
    }

    ui->btnConn->hide();
    ui->wbg_2->hide();
    ui->line->hide();
    ui->lbLoadUpImg->show();
    ui->lbLoadDownImg->show();

    this->isTopItem = true;
}

void OneLancForm::setName(QString name){
    ui->lbName->setText(name);
    lanName = name;
}

void OneLancForm::setConnedString(bool showLable, QString str){
    if (!showLable){
        ui->lbConned->hide();
        ui->lbName->move(63, 18);
    }else{
        ui->lbConned->setText(str);
    }
}

void OneLancForm::setLanInfo(QString str1, QString str2, QString str3, QString str4)
{
    if (str1 == "" || str1 == "auto"){
        str1 = tr("no configuration");
    }

    if (str2 == "" || str2 == "auto"){
        str2 = tr("no configuration");
    }

    QString str = "IPv4地址：" + str1 + "\nIPv6地址：" + str2 + "\n带宽：" + str3 + " \n物理地址(MAC)：" + str4;
    ui->lbInfo->setText(str);
}

void OneLancForm::setIcon(bool isOn){
    if(isOn){
        ui->lbIcon->setStyleSheet("QLabel{background:url(:/res/l/network-online.png);}");
    }else{
        ui->lbIcon->setStyleSheet("QLabel{background:url(:/res/l/network-offline.png);}");
    }
}

void OneLancForm::setLine(bool isShow)
{
    if(isShow){
        ui->line->show();
    }else{
        ui->line->hide();
    }
}

void OneLancForm::slotConnLan(){
    //mw->startLoading();
    this->startWaiting(true);
    emit sigConnLan(ui->lbName->text());
}

void OneLancForm::on_btnDisConn_clicked()
{
    this->startWaiting(false);

    kylin_network_set_con_down(ui->lbName->text().toUtf8().data());

    disconnect(this, SIGNAL(selectedOneLanForm(QString)), mw, SLOT(oneLanFormSelected(QString)));

    emit disconnActiveLan();
}

void OneLancForm::on_btnConn_clicked()
{
    QThread *t = new QThread();
    BackThread *bt = new BackThread();
    bt->moveToThread(t);
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), this, SLOT(slotConnLan()));
    connect(this, SIGNAL(sigConnLan(QString)), bt, SLOT(execConnLan(QString)));
    connect(bt, SIGNAL(connDone(int)), mw, SLOT(connLanDone(int)));
    connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
    t->start();
}

void OneLancForm::on_btnConnSub_clicked()
{
    QThread *t = new QThread();
    BackThread *bt = new BackThread();
    bt->moveToThread(t);
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), this, SLOT(slotConnLan()));
    connect(this, SIGNAL(sigConnLan(QString)), bt, SLOT(execConnLan(QString)));
    connect(bt, SIGNAL(connDone(int)), mw, SLOT(connLanDone(int)));
    connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
    t->start();
}

void OneLancForm::waitAnimStep(){
    QString qpmQss = "QLabel{background-image:url(':/res/s/conning-a/";
    qpmQss.append(QString::number(this->waitPage));
    qpmQss.append(".png');}");
    ui->lbWaitingIcon->setStyleSheet(qpmQss);

    this->waitPage --;

    if(this->waitPage < 1){
        this->waitPage = 8;
    }
}

void OneLancForm::startWaiting(bool isConn){
    if (isConn){
        ui->lbWaiting->setStyleSheet("QLabel{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);}");
    }else{
        ui->btnDisConn->hide();
        ui->lbWaiting->setStyleSheet("QLabel{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);}");
    }
    this->waitPage = 8;
    this->waitTimer->start(150);
    ui->lbWaiting->show();
    ui->lbWaitingIcon->show();

    mw->setTrayLoading(true);
}

void OneLancForm::stopWaiting(){
    this->waitTimer->stop();
    ui->lbWaiting->hide();
    ui->lbWaitingIcon->hide();

    mw->setTrayLoading(false);
}
