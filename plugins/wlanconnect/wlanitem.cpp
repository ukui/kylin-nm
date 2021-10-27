#include "wlanitem.h"
#define FRAME_SPEED 150
#define LIMIT_TIME 60*1000
#define TOTAL_PAGE 8

#define THEME_QT_SCHEMA  "org.ukui.style"
#define MODE_QT_KEY      "style-name"

WlanItem::WlanItem(bool bAcitve, bool isLock, QWidget *parent)
    : isAcitve(bAcitve), isLock(isLock), QPushButton(parent)
{
    this->setMinimumSize(550, 58);
    this->setProperty("useButtonPalette", true);
    setStyleSheet("QPushButton:!checked{background-color: palette(base)}");
    QHBoxLayout *mLanLyt = new QHBoxLayout(this);
    mLanLyt->setContentsMargins(16,0,16,0);
    mLanLyt->setSpacing(16);
    iconLabel = new QLabel(this);
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
    waitTimer = new QTimer(this);
    connect(waitTimer, &QTimer::timeout, this, &WlanItem::waitAnimStep);
}

WlanItem::~WlanItem()
{

}

void WlanItem::setWaitPage(int waitPage)
{
    this->waitPage = waitPage;
}

void WlanItem::setCountCurrentTime(int countCurrentTime)
{
    this->countCurrentTime = countCurrentTime;
}

void WlanItem::startLoading()
{
    waitTimer->start(FRAME_SPEED);
    loading = true;
}

void WlanItem::stopLoading(){
    waitTimer->stop();
    loading = false;
}

void WlanItem::waitAnimStep()
{
    QString qpmQss = "QLabel{background-image:url(':/img/plugins/netconnect/";
    qpmQss.append(QString::number(this->waitPage));
    QString imgPath = ":/img/plugins/netconnect/";
    imgPath.append(QString::number(this->waitPage));

    const QByteArray id(THEME_QT_SCHEMA);
    themeGsettings = new QGSettings(id, QByteArray(), this);
    if (themeGsettings->get(MODE_QT_KEY).toString() == "ukui-default") {
        qpmQss.append("-black.png');}");
        imgPath.append("-black.png");
    } else {
        qpmQss.append(".png');}");
        imgPath.append(".png");

    }
    QImage img;
    img.load(imgPath);
    statusLabel->setText("");
    statusLabel->setFixedSize(img.size());
    statusLabel->setProperty("useIconHighlightEffect", 0x10);

    statusLabel->setStyleSheet(qpmQss);
    this->waitPage ++;
    if (this->waitPage > TOTAL_PAGE) {
        this->waitPage = 1; //循环播放8张图片
    }
    this->countCurrentTime += FRAME_SPEED;
    if (this->countCurrentTime >= LIMIT_TIME) {
        this->stopLoading(); //动画超出时间限制，强制停止动画
    }
}

