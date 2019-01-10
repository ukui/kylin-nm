#include "onelancform.h"
#include "ui_onelancform.h"
#include "mainwindow.h"

OneLancForm::OneLancForm(QWidget *parent, MainWindow *mainWindow, ConfForm *confForm, KSimpleNM *ksnm) :
    QWidget(parent),
    ui(new Ui::OneLancForm)
{
    ui->setupUi(this);

    ui->btnConf->setText("设置");
    ui->btnConn->setText("连接");
    ui->btnDisConn->setText("断开连接");

    ui->wbg->hide();
    ui->btnConf->hide();
    ui->btnConn->hide();
    ui->btnDisConn->hide();

    ui->wbg->setStyleSheet("#wbg{background-color:#3593b5;}");
    ui->lbName->setStyleSheet("QLabel{font-size:13px;color:#ffffff;}");
    ui->lbBandWidth->setStyleSheet("QLabel{font-size:12px;color:#aaaaaa;}");
    ui->lbConned->setStyleSheet("QLabel{font-size:13px;color:#ffffff;}");
    ui->btnConf->setStyleSheet("QPushButton{border:0px;background-color:rgba(0,0,0,0.4);color:white;font-size:12px;}"
                               "QPushButton:Hover{border:1px solid rgba(255,255,255,0.2);background-color:rgba(0,0,0,0.2);}"
                               "QPushButton:Pressed{background-color:rgba(0,0,0,0.6);}");
    ui->btnConn->setStyleSheet("QPushButton{border:0px;background-color:rgba(0,0,0,0.4);color:white;font-size:12px;}"
                               "QPushButton:Hover{border:1px solid rgba(255,255,255,0.2);background-color:rgba(0,0,0,0.2);}"
                               "QPushButton:Pressed{background-color:rgba(0,0,0,0.6);}");
    ui->btnDisConn->setStyleSheet("QPushButton{border:0px;background-color:rgba(0,0,0,0.4);color:white;font-size:12px;}"
                               "QPushButton:Hover{border:1px solid rgba(255,255,255,0.2);background-color:rgba(0,0,0,0.2);}"
                               "QPushButton:Pressed{background-color:rgba(0,0,0,0.6);}");

    this->mw = mainWindow;
    this->cf = confForm;
    this->ks = ksnm;

    this->isSelected = false;
    this->isActive = false;
}

OneLancForm::~OneLancForm()
{
    delete ui;
}

void OneLancForm::mouseReleaseEvent(QMouseEvent *){
    emit selectedOneLanForm(lanName);
}

// 是否当前连接的网络，字体设置不同
void OneLancForm::setAct(bool isAct){
    if(isAct){
        ui->lbName->setStyleSheet("QLabel{font-size:14px;color:#ffffff;}");
        ui->lbConned->show();
        ui->btnConf->show();
    }else{
        ui->lbName->setStyleSheet("QLabel{font-size:12px;color:#ffffff;}");
        ui->lbConned->hide();
        ui->btnConf->hide();
    }
    isActive = isAct;
}

// 是否选中
void OneLancForm::setSelected(bool isSelected){
    if(isSelected){
        resize(314, 111);
        ui->wbg->show();
        ui->btnConf->show();
        if(isActive){
            ui->btnDisConn->show();
        }else{
            ui->btnConn->show();
        }

        this->isSelected = true;

    }else{
        resize(314, 60);
        ui->wbg->hide();
        ui->btnConf->hide();
        ui->btnConn->hide();
        ui->btnDisConn->hide();
        ui->lbName->show();

        this->isSelected = false;
    }
}

void OneLancForm::setName(QString name){
    ui->lbName->setText(name);
    lanName = name;
}

void OneLancForm::setConnedString(QString str){
    ui->lbConned->setText(str);
}

void OneLancForm::setIcon(bool isOn){
    if(isOn){
        ui->lbIcon->setStyleSheet("QLabel{background:url(:/res/s/network-line.png);}");
    }else{
        ui->lbIcon->setStyleSheet("QLabel{background:url(:/res/s/network-offline.png);}");
    }
}

void OneLancForm::setBandWidth(QString bandWidth){
    ui->lbBandWidth->setText(bandWidth);

    if(bandWidth == "--"){
        ui->lbPoint->setStyleSheet("QLabel{background:url(:/res/s/pbad.png);}");
    }else{
        if(bandWidth != ""){
            QString rateStr = bandWidth.mid(0, bandWidth.indexOf("Mb"));
            int rateNum = rateStr.toInt();
            if(rateNum >= 1000){
                ui->lbPoint->setStyleSheet("QLabel{background:url(:/res/s/pgood.png);}");
            }
            if(rateNum >= 100 && rateNum < 1000){
                ui->lbPoint->setStyleSheet("QLabel{background:url(:/res/s/pnormal.png);}");
            }
            if(rateNum < 100){
                ui->lbPoint->setStyleSheet("QLabel{background:url(:/res/s/pbad.png);}");
            }
        }
    }
}

void OneLancForm::setShowPoint(bool flag){
    if(flag){
        ui->lbPoint->show();
    }else{
        ui->lbPoint->hide();
    }
}

void OneLancForm::on_btnConf_clicked()
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
    QString connProp = bt->getConnProp(ui->lbName->text());
    QStringList propList = connProp.split("|");
    QString v4method, addr, mask, gateway, dns;
    foreach (QString line, propList) {
        if(line.startsWith("method:")){
            v4method = line.split(":").at(1);
        }
        if(line.startsWith("addr:")){
            addr = line.split(":").at(1);
        }
        if(line.startsWith("mask:")){
            mask = line.split(":").at(1);
        }
        if(line.startsWith("gateway:")){
            gateway= line.split(":").at(1);
        }
        if(line.startsWith("dns:")){
            dns = line.split(":").at(1);
        }
    }
//    qDebug()<<v4method<<addr<<mask<<gateway<<dns;

    cf->setProp(ui->lbName->text(), v4method, addr, mask, gateway, dns, this->isActive);

    cf->move(primaryGeometry.width() / 2 - cf->width() / 2, primaryGeometry.height() / 2 - cf->height() / 2);
    cf->show();
    cf->raise();
}

void OneLancForm::slotConnLan(){
    mw->startLoading();
    emit sigConnLan(ui->lbName->text());
}

void OneLancForm::on_btnDisConn_clicked()
{
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
