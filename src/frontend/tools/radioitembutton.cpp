#include "radioitembutton.h"
#include <QPainter>
#include <QStyle>

#define BUTTON_SIZE 36,36
#define ICON_SIZE 16,16
#define BACKGROUND_COLOR QColor(0,0,0,0)
#define FOREGROUND_COLOR_NORMAL_INACTIVE QColor(230,230,230,255)
#define FOREGROUND_COLOR_PRESS_INACTIVE QColor(217,217,217,255)
#define FOREGROUND_COLOR_NORMAL_ACTIVE QColor(55,144,250,255)
#define FOREGROUND_COLOR_PRESS_ACTIVE QColor(36,109,212,255)

RadioItemButton::RadioItemButton(QWidget *parent) : QPushButton(parent)
{
    connect(this, &RadioItemButton::requestStartLoading, this, &RadioItemButton::onLoadingStarted);
    connect(this, &RadioItemButton::requestStopLoading, this, &RadioItemButton::onLoadingStopped);
    this->setAutoFillBackground(false);
    m_iconLabel = new QLabel(this);

    this->setFixedSize(BUTTON_SIZE);
    m_iconLabel->setFixedSize(BUTTON_SIZE);
    m_iconLabel->setAlignment(Qt::AlignCenter);

    setActive(false);
    //JXJ_TODO loading动画
//    switchTimer = new QTimer(this);
//    connect(switchTimer, &QTimer::timeout, this, &RadioItemButton::onLoadingStarted);
}

void RadioItemButton::startLoading()
{
    emit this->requestStartLoading();
}

void RadioItemButton::stopLoading()
{
    emit this->requestStopLoading();
}

void RadioItemButton::setButtonIcon(const QIcon &icon)
{
    if (icon.isNull()) {
        return;
    }
    m_iconLabel->setPixmap(icon.pixmap(ICON_SIZE));
}

void RadioItemButton::setActive(const bool &isActive)
{
    m_isActivated = isActive;
    m_backgroundColor = m_isActivated? FOREGROUND_COLOR_NORMAL_ACTIVE : FOREGROUND_COLOR_NORMAL_INACTIVE;
}
void RadioItemButton::onLoadingStarted()
{
    //ZJP_TODO 开始播放转圈动画
//        switchTimer->start(FRAMESPEED);
//        QString qpmQss = ":/res/s/conning-b/";
//        qpmQss.append(QString::number(this->currentPage));
//        qpmQss.append(".png");
//        m_iconLabel->setPixmap(QPixmap(qpmQss));
////        m_iconLabel->setProperty("useIconHighlightEffect", true);
////        m_iconLabel->setProperty("iconHighlightEffectMode", true);

//        this->currentPage --;

//        if (this->currentPage < 1) {
//            this->currentPage = 12; //循环播放
//        }
//        this->countCurrentTime += FRAMESPEED;
//        //达到一定的时间退出。应该是收到连接成功或者失败的信号断开
//        if (this->countCurrentTime >= ALLTIME) {
//            emit this->onLoadingStopped();
//        }
}

void RadioItemButton::onLoadingStopped()
{
    //ZJP_TODO 停止播放转圈动画
    this->switchTimer->stop();

}

void RadioItemButton::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, BACKGROUND_COLOR);
    pal.setColor(QPalette::Text, m_backgroundColor);

    QPainterPath cPath;
    cPath.addRect(0, 0, this->width(), this->height());
    cPath.addEllipse(0, 0, this->width(), this->width());

    QPainterPath innerPath;
    innerPath.addEllipse(0, 0, this->width(), this->width());

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);

    painter.setBrush(pal.color(QPalette::Base));
    painter.drawPath(cPath);

    painter.fillPath(innerPath, pal.color(QPalette::Text));
}

void RadioItemButton::mousePressEvent(QMouseEvent *event)
{
    m_backgroundColor = m_isActivated? FOREGROUND_COLOR_PRESS_ACTIVE : FOREGROUND_COLOR_PRESS_INACTIVE;
    this->repaint();
    return QPushButton::mousePressEvent(event);
}

void RadioItemButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_backgroundColor = m_isActivated? FOREGROUND_COLOR_NORMAL_ACTIVE : FOREGROUND_COLOR_NORMAL_INACTIVE;
    this->repaint();
    return QPushButton::mouseReleaseEvent(event);
}
