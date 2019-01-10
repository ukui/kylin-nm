#include "loadingdiv.h"

LoadingDiv::LoadingDiv(QWidget *parent) : QWidget(parent)
{
    this->resize(316, 467);

    QLabel *aaa = new QLabel(this);
    aaa->resize(316, 467);
    aaa->move(0, 0);
    aaa->setStyleSheet("QLabel{background-color:rgba(0,0,0,0.5);}");

    this->loadingGif = new QLabel(this);
    this->loadingGif->resize(96, 96);
    this->loadingGif->move(this->width() / 2 - 96 / 2, this->height() / 2 - 96 / 2);
    this->loadingGif->show();


    this->switchTimer = new QTimer(this);
    connect(switchTimer, SIGNAL(timeout()), this, SLOT(switchAnimStep()));

    this->raise();
    this->hide();
}

void LoadingDiv::switchAnimStep(){
    QString qpmQss = "QLabel{background-image:url(':/res/s/conning-b/";
    qpmQss.append(QString::number(this->currentPage));
    qpmQss.append(".png');}");
    loadingGif->setStyleSheet(qpmQss);

    this->currentPage --;

    if(this->currentPage < 1){
        this->currentPage = 12;
    }
}

void LoadingDiv::startLoading(){
    this->currentPage = 12;
    this->switchTimer->start(60);
    this->show();
}

void LoadingDiv::stopLoading(){
    this->switchTimer->stop();
    this->hide();
}
