#include "radioitembutton.h"
#include <QPainter>
#include <QStyle>
#define ICON_SIZE 16,16
#define BUTTON_SIZE 36,36

RadioItemButton::RadioItemButton(bool isActivated, QWidget *parent) : QPushButton(parent)
{
    connect(this, &RadioItemButton::requestStartLoading, this, &RadioItemButton::onLoadingStarted);
    connect(this, &RadioItemButton::requestStopLoading, this, &RadioItemButton::onLoadingStopped);
    m_isActivated = isActivated;
    this->setAutoFillBackground(false);
    m_iconLabel = new QLabel(this);

    this->setFixedSize(BUTTON_SIZE);
    m_iconLabel->setFixedSize(BUTTON_SIZE);
    m_iconLabel->setAlignment(Qt::AlignCenter);

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

void RadioItemButton::setButtonIcon(const QIcon &icon)
{
    if (icon.isNull()) {
        return;
    }
    //ZJP_TODO 绘制圆形按钮
    m_iconLabel->setPixmap(icon.pixmap(ICON_SIZE));
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
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Text, QColor(230,230,230,255));

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

//    QRect iconRect;
//    iconRect = this->rect();
//    iconRect.adjust(8,8,-8,-8);
//    QPixmap pixmap = this->icon().pixmap(32,32);
//    painter.drawPixmap(iconRect, pixmap);

//    return QPushButton::paintEvent(event);
}
