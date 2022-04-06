#include "detailwidget.h"
#include <QFontMetrics>
#include <QDebug>
#include <QApplication>

#define ITEM_HEIGHT 36
#define ITEM_MARGINS 18,0,16,0

#define MAX_LABEL_WIDTH 138

FixLabel::FixLabel(QWidget *parent):
    QLabel(parent)
{

}

FixLabel::~FixLabel()
{

}

void FixLabel::paintEvent(QPaintEvent *event)
{
    QFontMetrics fontMetrics(this->font());
    int fontSize = fontMetrics.width(mStr);
    if (fontSize > this->width()) {
        this->setText(fontMetrics.elidedText(mStr, Qt::ElideRight, this->width()), false);
        this->setToolTip(mStr);
    } else {
        this->setText(mStr, false);
        this->setToolTip("");
    }
    QLabel::paintEvent(event);
}

void FixLabel::setText(const QString & text, bool saveTextFlag)
{
    if (saveTextFlag) {
        mStr = text;
    }
    QLabel::setText(text);
}

DetailWidget::DetailWidget(QWidget *valueWidget, QWidget *parent)
    : m_valueWidget(valueWidget) , QWidget(parent)
{
//    m_valueWidget = valueWidget;
    initUI();
}

DetailWidget::~DetailWidget()
{

}

void DetailWidget::initUI()
{
    this->setFixedHeight(ITEM_HEIGHT);
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(ITEM_MARGINS);

    m_keyLabel = new FixLabel(this);
    m_keyLabel->setMaximumWidth(MAX_LABEL_WIDTH);
    m_keyLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_mainLayout->addWidget(m_keyLabel);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_valueWidget);
    m_valueWidget->setMaximumWidth(100000000000);
}

void DetailWidget::setKey(const QString &keyLabel)
{
    m_keyLabel->setText(keyLabel);
}



FirstDetailWidget::FirstDetailWidget(QWidget *valueWidget,QWidget *button, QWidget *parent)
    : m_valueWidget(valueWidget) , m_copyButton (button) ,QWidget(parent)
{
    initUI();
}

FirstDetailWidget::~FirstDetailWidget()
{

}

void FirstDetailWidget::initUI()
{
    this->setFixedHeight(ITEM_HEIGHT);

    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(ITEM_MARGINS);

    m_keyLabel = new FixLabel(this);
    m_keyLabel->setMaximumWidth(MAX_LABEL_WIDTH);
    m_keyLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_copyButton->setMaximumWidth(ITEM_HEIGHT-8);
    m_copyButton->setMaximumHeight(ITEM_HEIGHT-8);

    m_mainLayout->addWidget(m_keyLabel);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_copyButton);
    m_mainLayout->addWidget(m_valueWidget);
    m_valueWidget->setMaximumWidth(1000);
}

void FirstDetailWidget::setKey(const QString &keyLabel)
{
    m_keyLabel->setText(keyLabel);
}

CopyButton::CopyButton()
{
    //按钮背景颜色-透明
    btnPal = this->palette();
    color = qApp->palette().color(QPalette::Background);
    color.setAlphaF(0);
    btnPal.setColor(QPalette::Button, color);
    this->setPalette(btnPal);

    //设置“复制成功”消息弹窗格式
    m_copiedTip = new KBallonTip();
//    m_copiedTip->setTipType(Normal);
//    m_copiedTip->setFixedSize(73 + 37, 32);
    m_copiedTip->setTipType(Nothing);
    m_copiedTip->setFixedSize(73, 32);
    m_copiedTip->setContentsMargins(6, 4, 4, 6);
    m_copiedTip->setWindowFlags(Qt::FramelessWindowHint);
}

CopyButton::~CopyButton()
{
    if (m_copiedTip != nullptr)
    {
        m_copiedTip = nullptr;
        delete m_copiedTip;
    }
}

void CopyButton::mousePressEvent(QMouseEvent *event)
{
//    //按钮背景颜色
//    color.setAlphaF(1);
//    btnPal.setColor(QPalette::Button, color);
//    this->setPalette(btnPal);

    return QPushButton:: mousePressEvent(event);
}

void CopyButton::mouseReleaseEvent(QMouseEvent *event)
{
    //设置弹窗位置并显示
    m_mousePosition = event->globalPos();
    m_copiedTip->move(m_mousePosition.x() + 0, m_mousePosition.y() + 20);  
    m_copiedTip->setText(tr("Copied successfully"));
    m_copiedTip->setTipTime(5500);
    m_copiedTip->showInfo();
    return QPushButton::mouseReleaseEvent(event);
}

void CopyButton::enterEvent(QEvent *event)
{
    this->setToolTip(tr("Copy all"));
    return QPushButton::enterEvent(event);
}

void CopyButton::leaveEvent(QEvent *event)
{
    if (m_copiedTip != nullptr) {
        m_copiedTip->close();
    }
    //按钮背景颜色
    color.setAlphaF(0);
    btnPal.setColor(QPalette::Button, color);
    this->setPalette(btnPal);
    return QPushButton::leaveEvent(event);
}


