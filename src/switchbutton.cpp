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
    if(check == true) {
        m_bIsOn = 1;
    } else {
        m_bIsOn = 0;
    }

    m_cTimer->start(); //开始播放动画
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

