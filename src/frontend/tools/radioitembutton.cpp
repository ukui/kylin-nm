#include "radioitembutton.h"
#define BUTTON_SIZE 32,32

#define FRAMESPEED 60 //帧与帧之间的间隔时间(ms)
#define ALLTIME 40*1000 //等待动画持续总时间

RadioItemButton::RadioItemButton(bool isActivated, QWidget *parent) : QPushButton(parent)
{
    connect(this, &RadioItemButton::requestStartLoading, this, &RadioItemButton::onLoadingStarted);
    connect(this, &RadioItemButton::requestStopLoading, this, &RadioItemButton::onLoadingStopped);
    m_isActivated = isActivated;
    this->setAutoFillBackground(false);
    m_iconLabel = new QLabel(this);
    m_iconLabel->setScaledContents(true);
    this->setFixedSize(BUTTON_SIZE);

    this->switchTimer = new QTimer(this); //QTimer对象，控制等待动画播放

    this->raise();
    this->hide();
}

void RadioItemButton::switchAnimStep()
{
    //另外一种加载方法
    QString qpmQss = ":/res/s/conning-b/";
    qpmQss.append(QString::number(this->currentPage));
    qpmQss.append(".png");
    m_iconLabel->setPixmap(QPixmap(qpmQss));
    m_iconLabel->setProperty("useIconHighlightEffect", true);
    m_iconLabel->setProperty("iconHighlightEffectMode", true);

    this->currentPage --;

    if (this->currentPage < 1) {
        this->currentPage = 12; //循环播放
    }
    this->countCurrentTime += FRAMESPEED;
    if (this->countCurrentTime >= ALLTIME) {
        emit this->toStopLoading(); //发出信号停止主界面和托盘区的等待动画
    }
}

void RadioItemButton::startLoading()
{
    emit this->requestStartLoading();
}

void RadioItemButton::stopLoading()
{
    emit this->requestStopLoading();
}

void RadioItemButton::setPressed()
{
    //ZJP_TODO 设置颜色为点击颜色，注意区分已连接/未连接
    if (m_isActivated)
        ;
    ;
}

void RadioItemButton::setReleased()
{
    //ZJP_TODO 设置颜色为未点击颜色，注意区分已连接/未连接
    if (m_isActivated)
        ;
    ;
}

void RadioItemButton::setButtonIcon(QIcon *icon)
{
    if (!icon) {
        return;
    }
    //ZJP_TODO 绘制圆形按钮
    m_iconLabel->setPixmap(icon->pixmap(this->size()));
}

void RadioItemButton::onLoadingStarted()
{
    //ZJP_TODO 开始播放转圈动画
    this->currentPage = 12;
    this->countCurrentTime = 0;
    this->switchTimer->start(FRAMESPEED);
    this->show();
    if (this->countCurrentTime >= ALLTIME) {
        emit this->requestStopLoading(); //发出信号停止主界面和托盘区的等待动画
    }
}

void RadioItemButton::onLoadingStopped()
{
    //ZJP_TODO 停止播放转圈动画
    this->switchTimer->stop();
    this->hide();
}
