#include "lanitem.h"
#define FRAME_SPEED 150
#define LIMIT_TIME 60*1000
#define TOTAL_PAGE 8

#define THEME_QT_SCHEMA  "org.ukui.style"
#define MODE_QT_KEY      "style-name"

LanItem::LanItem(bool isAcitve, QWidget *parent)
    : QPushButton(parent),isAcitve(isAcitve)
{
    this->setMinimumSize(550, 58);
    this->setProperty("useButtonPalette", true);
    setStyleSheet("QPushButton:!checked{background-color: palette(base)}");
    QHBoxLayout *mLanLyt = new QHBoxLayout(this);
    mLanLyt->setContentsMargins(16,0,16,0);
    mLanLyt->setSpacing(16);
    iconLabel = new QLabel(this);
    iconLabel->setProperty("useIconHighlightEffect", 0x2);
    titileLabel = new FixLabel(this);
    statusLabel = new QLabel(this);
    statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//    statusLabel->setMinimumSize(36,36);
    infoLabel = new InfoButton(this);
    mLanLyt->addWidget(iconLabel);
    mLanLyt->addWidget(titileLabel,Qt::AlignLeft);
    mLanLyt->addStretch();
    mLanLyt->addWidget(statusLabel);
    mLanLyt->addWidget(infoLabel);

    loadIcons.append(QIcon::fromTheme("ukui-loading-1-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-2-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-3-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-4-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-5-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-6-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-7-symbolic"));
    waitTimer = new QTimer(this);
    connect(waitTimer, &QTimer::timeout, this, &LanItem::updateIcon);
}

LanItem::~LanItem()
{

}

void LanItem::updateIcon()
{
    if (currentIconIndex > 6) {
        currentIconIndex = 0;
    }
    statusLabel->setPixmap(loadIcons.at(currentIconIndex).pixmap(16,16));
    currentIconIndex ++;
}

void LanItem::startLoading()
{
    waitTimer->start(FRAME_SPEED);
    loading = true;
}

void LanItem::stopLoading(){
    waitTimer->stop();
    loading = false;
}

void LanItem::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(pal.color(QPalette::Base));

    QRect rect = this->rect();

    painter.drawRect(rect);
    QPushButton::paintEvent(event);
}

