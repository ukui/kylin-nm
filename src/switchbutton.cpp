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
#include "switchbutton.h"

SwitchButton::SwitchButton(QWidget *parent) : QWidget(parent)
{

    setFixedSize(50,24);
    m_fWidth = (float)width();
    m_fHeight = (float)height();
    m_cTimer = new QTimer(this);
    m_cTimer->setInterval(5);

    if(m_bIsOn == 1) {
        m_fCurrentValue = m_fWidth - 16 - 4;
    }
    else {
        m_fCurrentValue = 4;
    }
    connect(m_cTimer,SIGNAL(timeout()),this,SLOT(startAnimation()));


}

void SwitchButton::setSwitchStatus(bool check) {
    if (!m_enabled)
        return;
    if(check == true) {
        m_bIsOn = 1;
    } else {
        m_bIsOn = 0;
    }

    m_cTimer->start(); //开始播放动画
}

bool SwitchButton::getSwitchStatus()
{
    if (m_bIsOn == 1)
        return true;
    return false;
}

void SwitchButton::setEnabled(bool enabled)
{
    m_enabled = enabled;
}


/* 播放按钮开启关闭动画 */
void SwitchButton::startAnimation() { //滑动按钮动作播放
    int pos = 4;
    int size = m_fWidth - 16;
    if(m_bIsOn) {
        m_fCurrentValue ++;                     //往右滑动
        if(m_fCurrentValue >= size - pos) {      //到达边界停下来
            m_fCurrentValue = size - pos;
            m_cTimer->stop();
        }

    } else {
        m_fCurrentValue --;
        if(m_fCurrentValue <= pos) {             //到达最小值，停止继续前进
            m_fCurrentValue = pos;
            m_cTimer->stop();
        }
    }
    update();
}

/* 按钮按下处理 */
void SwitchButton::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    if (!m_enabled)
        return QWidget::mousePressEvent(event);
    m_bIsOn = !m_bIsOn;

    Q_EMIT clicked(m_bIsOn);

    return QWidget::mousePressEvent(event);
}

/* 绘制滑动按钮主体 */
void SwitchButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿效果
    painter.setPen(Qt::NoPen);
    QColor colorActive(61,107,229);
    QColor colorInactive(190,190,190);
    colorInactive.setAlphaF(0.12);
    if(m_bIsOn) {
        painter.save();
        painter.setBrush(colorActive);
        QRectF active_rect = QRectF(0,0,m_fWidth,m_fHeight);
        painter.drawRoundedRect(active_rect, 0.5 * m_fHeight, 0.5 * m_fHeight); //画开启状态
    } else {
        painter.save();
        painter.setBrush(colorInactive);
        QRectF inactive_rect = QRectF(0 ,0,m_fWidth,m_fHeight);
        painter.drawRoundedRect(inactive_rect, 0.5 * m_fHeight, 0.5 * m_fHeight); //画关闭状态
    }
    painter.restore();
    painter.save();
    painter.setBrush(Qt::white);
    painter.drawEllipse(m_fCurrentValue,4, 16, 16);
    painter.restore();
}

