#include "detailwidget.h"
#include <QFontMetrics>
#include <QDebug>

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



FirstDetailWidget::FirstDetailWidget(QWidget *valueWidget, QWidget *parent)
    : m_valueWidget(valueWidget) , QWidget(parent)
{
    initUI();
}

FirstDetailWidget::~FirstDetailWidget()
{

}

void FirstDetailWidget::initUI()
{
    this->setFixedHeight(ITEM_HEIGHT);
    CopyButton *m_copyButton = new CopyButton();
//    QToolButton *m_copyButton = new QToolButton();
//    m_copyButton->setProperty("useButtonPalette",true);
//    m_copyButton->setf
    connect(m_copyButton,&QToolButton::clicked,[=](){
        emit sig_click();
    });

    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(ITEM_MARGINS);

    m_keyLabel = new FixLabel(this);
    m_keyLabel->setMaximumWidth(MAX_LABEL_WIDTH);
    m_keyLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_mainLayout->addWidget(m_keyLabel);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_copyButton);
    m_mainLayout->addWidget(m_valueWidget);
    m_valueWidget->setMaximumWidth(100000000000);
}

void FirstDetailWidget::setKey(const QString &keyLabel)
{
    m_keyLabel->setText(keyLabel);
}

CopyButton::CopyButton()
{
    this->setMaximumWidth(ITEM_HEIGHT-4);
    this->setMaximumHeight(ITEM_HEIGHT-4);
    this->setToolTip("复制全部");
    this->setIcon(QIcon::fromTheme("edit-copy-symbolic"));
//    this->setProperty("useButtonPalette",true);
//    this->setFlat(true);
}

CopyButton::~CopyButton()
{

}

void CopyButton::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"mousePressEvent";
}

void CopyButton::mouseReleaseEvent(QMouseEvent *event)
{
//    qDebug()<<"mouseReleaseEvent";
}

void CopyButton::enterEvent(QEvent *event)
{
//    qDebug()<<"enterEvent";
    return QPushButton::enterEvent(event);

}

void CopyButton::leaveEvent(QEvent *event)
{
//    qDebug()<<"leaveEvent";
}

bool CopyButton::event(QEvent *e)
{
    qDebug()<<e->type();
    return QPushButton::event(e);
}
