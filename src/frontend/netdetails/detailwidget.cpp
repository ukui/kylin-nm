#include "detailwidget.h"
#include <QFontMetrics>
#include <QDebug>
#include <QApplication>
#include <QToolTip>

#include "coninfo.h"
#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

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
    //设置按钮背景颜色-透明
    QPalette btnPal = this->palette();
    QColor color = qApp->palette().color(QPalette::Background);
    color.setAlphaF(0);
    btnPal.setColor(QPalette::Button, color);
    this->setPalette(btnPal);

    //设置“复制成功”消息弹窗格式
    m_copiedTip = new KBallonTip();
    m_copiedTip->setTipType(Normal);
    m_copiedTip->setFixedSize(158, 58);
    m_copiedTip->setWindowFlags(Qt::FramelessWindowHint);
    m_copiedTip->setAttribute(Qt::WA_TranslucentBackground, true);
    m_copiedTip->setText(tr("Copied successfully!"));
}

CopyButton::~CopyButton()
{
    if (m_copiedTip != nullptr)
    {
        m_copiedTip = nullptr;
        delete m_copiedTip;
    }
}

QPalette CopyButton::GetTheme()
{
    //获取当前主题的颜色
    QPalette pal = qApp->palette();
    QGSettings * styleGsettings = nullptr;
    const QByteArray style_id(THEME_SCHAME);
    if (QGSettings::isSchemaInstalled(style_id)) {
       styleGsettings = new QGSettings(style_id);
       QString currentTheme = styleGsettings->get(COLOR_THEME).toString();
       if(currentTheme == "ukui-default"){
           pal = lightPalette(this);
       }
    }
    return pal;
}

void CopyButton::mousePressEvent(QMouseEvent *event)
{
    return QPushButton:: mousePressEvent(event);
}

void CopyButton::mouseReleaseEvent(QMouseEvent *event)
{
    //设置弹窗位置并显示
    QPoint m_copyBtnPosition = this->mapToGlobal(this->pos());
    m_copiedTip->move(m_copyBtnPosition.x() - 490, m_copyBtnPosition.y() + 130);
    QPalette pal = GetTheme();
    m_copiedTip->setPalette(pal);
    //    m_copiedTip->setTipTime(1500);
    m_copiedTip->showInfo();
    return QPushButton::mouseReleaseEvent(event);
}

void CopyButton::enterEvent(QEvent *event)
{
    QPalette palette = QToolTip::palette();
    palette = GetTheme();
    QToolTip::setPalette(palette);
    this->setToolTip(tr("Copy all"));
    return QPushButton::enterEvent(event);
}

void CopyButton::leaveEvent(QEvent *event)
{
    if (m_copiedTip != nullptr) {
        m_copiedTip->hide();
    }
    return QPushButton::leaveEvent(event);
}

