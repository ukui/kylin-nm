#include "detailwidget.h"
#define ITEM_HEIGHT 36
#define ITEM_MARGINS 18,0,16,0

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

    m_keyLabel = new QLabel(this);
    m_mainLayout->addWidget(m_keyLabel);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_valueWidget);
}

void DetailWidget::setKey(const QString &keyLabel)
{
    m_keyLabel->setText(keyLabel);
}
