#include "netbutton.h"

NetButton::NetButton(bool isActivated, QWidget *parent) : QPushButton(parent)
{
    connect(this, &NetButton::requestStartLoading, this, &NetButton::onLoadingStarted);
    connect(this, &NetButton::requestStopLoading, this, &NetButton::onLoadingStopped);
    m_isActivated = isActivated;
    this->setAutoFillBackground(false);
    m_iconLabel = new QLabel(this);
    m_iconLabel->setScaledContents(true);
}

void NetButton::startLoading()
{
    emit this->requestStartLoading();
}

void NetButton::stopLoading()
{
    emit this->requestStopLoading();
}

void NetButton::setPressed()
{
    //ZJP_TODO 设置颜色为点击颜色，注意区分已连接/未连接
    if (m_isActivated)
        ;
    ;
}

void NetButton::setReleased()
{
    //ZJP_TODO 设置颜色为未点击颜色，注意区分已连接/未连接
    if (m_isActivated)
        ;
    ;
}

void NetButton::setButtonIcon(QIcon *icon)
{
    if (!icon) { return; }
    //ZJP_TODO 绘制圆形按钮
    m_iconLabel->setPixmap(icon->pixmap(this->size()));
}

void NetButton::onLoadingStarted()
{
    //ZJP_TODO 开始播放转圈动画
}

void NetButton::onLoadingStopped()
{
    //ZJP_TODO 停止播放转圈动画
}
