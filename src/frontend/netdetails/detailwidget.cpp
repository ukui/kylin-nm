#include "detailwidget.h"
#include <QFontMetrics>
#include <QDebug>
#include <QGSettings>

#define ITEM_HEIGHT 36
#define ITEM_MARGINS 18,0,16,0

#define MAX_LABEL_WIDTH 138

FixLabel::FixLabel(QWidget *parent) :
    QLabel(parent)
{
    const QByteArray id("org.ukui.style");
    QGSettings * fontSetting = new QGSettings(id, QByteArray(), this);
    connect(fontSetting, &QGSettings::changed,[=](QString key) {
        if ("systemFont" == key || "systemFontSize" ==key) {
            changedLabelSlot();
        }
    });
}


void FixLabel::setLabelText(QString text) {

    mStr = text;
    changedLabelSlot();
}

QString FixLabel::getText(){
    return mStr;
}

void FixLabel::changedLabelSlot() {
    QFontMetrics  fontMetrics(this->font());
    int fontSize = fontMetrics.width(mStr);
    if (fontSize > this->width()) {
        setText(fontMetrics.elidedText(mStr, Qt::ElideRight, this->width()));
        setToolTip(mStr);
    } else {
        setText(mStr);
        setToolTip("");
    }
}



DetailWidget::DetailWidget(QWidget *valueWidget, QWidget *parent, QWidget *buttonWidget)
    : m_valueWidget(valueWidget) , QWidget(parent) , m_copyButton(buttonWidget)
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
    if (m_copyButton != nullptr) {
        m_copyButton->setMaximumWidth(ITEM_HEIGHT-8);
        m_copyButton->setMaximumHeight(ITEM_HEIGHT-8);
        m_mainLayout->addWidget(m_copyButton);
    }
    m_mainLayout->addWidget(m_valueWidget);
    m_valueWidget->setMaximumWidth(100000000000);
}

void DetailWidget::setKey(const QString &keyLabel)
{
    m_keyLabel->setText(keyLabel);
}



