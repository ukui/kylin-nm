#include "radioitembutton.h"
#define BUTTON_SIZE 32,32

RadioItemButton::RadioItemButton(bool isActivated, QWidget *parent) : QPushButton(parent)
{
    connect(this, &RadioItemButton::requestStartLoading, this, &RadioItemButton::onLoadingStarted);
    connect(this, &RadioItemButton::requestStopLoading, this, &RadioItemButton::onLoadingStopped);
    m_isActivated = isActivated;
    this->setAutoFillBackground(false);
    m_iconLabel = new QLabel(this);
    m_iconLabel->setScaledContents(true);
    this->setFixedSize(BUTTON_SIZE);
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
    if (!icon) { return; }
    //ZJP_TODO 绘制圆形按钮
    m_iconLabel->setPixmap(icon->pixmap(this->size()));
}

void RadioItemButton::onLoadingStarted()
{
    //ZJP_TODO 开始播放转圈动画
}

void RadioItemButton::onLoadingStopped()
{
    //ZJP_TODO 停止播放转圈动画
}
