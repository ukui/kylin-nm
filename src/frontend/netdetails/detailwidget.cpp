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
//    m_valueWidget->setMaximumWidth(100000000000);
}

void DetailWidget::setKey(const QString &keyLabel)
{
    m_keyLabel->setText(keyLabel);
}
