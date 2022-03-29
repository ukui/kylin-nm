#include "detailpage.h"
#include <QPainter>
#include <QListWidget>

#define MAX_NAME_LENGTH 32
#define MAX_LABEL_WIDTH 250
extern void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed);

DetailPage::DetailPage(bool isWlan, bool isCreate, QWidget *parent)
    : mIsWlan(isWlan), isCreate(isCreate), QFrame(parent)
{
    this->setFrameShape(QFrame::Shape::StyledPanel);
    this->setMaximumWidth(960);
    initUI();
    if (isCreate) {
     connect(mSSIDEdit, &LineEdit::textEdited, this, &DetailPage::setEnableOfSaveBtn);
    }
}

void DetailPage::setSSID(const QString &ssid) {
    if (isCreate) {
        return;
    }
    this->mSSIDLabel->setText(fontMetrics().elidedText(ssid, Qt::ElideRight, 136, Qt::TextShowMnemonic));
}

void DetailPage::setProtocol(const QString &protocol) {
    this->mProtocol->setText(protocol);
}

void DetailPage::setSecType(const QString &secType) {
    this->mSecType->setText(secType);
}

void DetailPage::setHz(const QString &hz) {
    this->mHz->setText(hz);
}

void DetailPage::setChan(const QString &chan) {
    this->mChan->setText(chan);
}

void DetailPage::setBandWidth(const QString &brandWidth) {
    this->mBandWidth->setText(brandWidth);
}

void DetailPage::setIpv4(const QString &ipv4) {
    this->mIPV4->setText(ipv4);
}

void DetailPage::setIpv4Dns(const QString &ipv4Dns) {
    this->mIPV4Dns->setText(ipv4Dns);
}

void DetailPage::setIpv6(const QString &ipv6) {
    this->mIPV6->setText(ipv6);
}

void DetailPage::setMac(const QString &mac) {
    this->mMac->setText(mac);
}

void DetailPage::setAutoConnect(bool flag)
{
    if (!mIsWlan) {
        return;
    }
    this->forgetNetBox->setChecked(flag);
}

void DetailPage::getSsid(QString &ssid)
{
    if (isCreate) {
        ssid = mSSIDEdit->text();
    } else {
        ssid = mSSIDLabel->text();
    }
}

bool DetailPage::checkIsChanged(const ConInfo info)
{
    if (!mIsWlan) {
        return false;
    }
    if (info.isAutoConnect != forgetNetBox->isChecked()) {
        return true;
    } else {
        return false;
    }
}

void DetailPage::addDetailItem(QListWidget *listWidget, QWidget *detailWidget)
{
    QListWidgetItem *listWidgetItem = new QListWidgetItem(listWidget);
    listWidgetItem->setSizeHint(QSize(listWidget->width(),36));
    listWidgetItem->setFlags(Qt::NoItemFlags);
    listWidget->addItem(listWidgetItem);
    listWidget->setItemWidget(listWidgetItem, detailWidget);
}

void DetailPage::initUI() {
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);

    QWidget *mDetailFrame = new QFrame(this);
    mDetailLayout = new QVBoxLayout(mDetailFrame);
    mDetailLayout->setContentsMargins(0,0,0,0);

    m_listWidget = new QListWidget(mDetailFrame);
    m_listWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_listWidget->setBackgroundRole(QPalette::Base);
    m_listWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    mDetailLayout->addWidget(m_listWidget);

    if (!isCreate) {
        mSSIDLabel = new QLabel(this);
        mSSIDLabel->adjustSize();
        mSSIDLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_ssidWidget = new FirstDetailWidget(qobject_cast<QWidget *>(mSSIDLabel), m_listWidget);
//        mSSID->setStyleSheet("background:transparent;border-width:0px;border-style:none");
//        mSSID->setFocusPolicy(Qt::NoFocus);
    } else {
//        mSSID->setStyleSheet("border-width:1px;;border-style:solid;border-color:black;border-radius:2px");
        mSSIDEdit = new LineEdit(this);
        mSSIDEdit->setMaximumWidth(MAX_LABEL_WIDTH);
        mSSIDEdit->setAlignment(Qt::AlignRight);
        mSSIDEdit->setStyleSheet("border-top:0px  solid;border-bottom:1px  solid;border-left:0px  solid;border-right: 0px  solid;background:transparent");
        mSSIDEdit->setPlaceholderText(tr("Please input SSID:"));
        mSSIDEdit->setMaxLength(MAX_NAME_LENGTH);
        m_ssidWidget = new FirstDetailWidget(qobject_cast<QWidget *>(mSSIDEdit), m_listWidget);

    }
    m_ssidWidget->setKey(tr("SSID:"));

    mProtocol = new QLabel(this);
    m_protocolWidget = new DetailWidget(qobject_cast<QWidget *>(mProtocol), m_listWidget);
    m_protocolWidget->setKey(tr("Protocol:"));

    mSecType = new QLabel(this);
    m_secTypeWidget = new DetailWidget(qobject_cast<QWidget *>(mSecType));
    m_secTypeWidget->setKey(tr("Security Type:"));

    mHz = new QLabel(this);
    m_hzWidget = new DetailWidget(qobject_cast<QWidget *>(mHz));
    m_hzWidget->setKey(tr("Hz:"));

    mChan = new QLabel(this);
    m_chanelWidget = new DetailWidget(qobject_cast<QWidget *>(mChan));
    m_chanelWidget->setKey(tr("Chan:"));

    mBandWidth = new QLabel(this);
    m_bandwidthWidget = new DetailWidget(qobject_cast<QWidget *>(mBandWidth), m_listWidget);
    m_bandwidthWidget->setKey(tr("BandWidth:"));

    mIPV4 = new QLabel(this);
    m_ipv4Widget = new DetailWidget(qobject_cast<QWidget *>(mIPV4), m_listWidget);
    m_ipv4Widget->setKey(tr("IPV4:"));

    mIPV4Dns = new QLabel(this);
    m_ipv4DnsWidget = new DetailWidget(qobject_cast<QWidget *>(mIPV4Dns), m_listWidget);
    m_ipv4DnsWidget->setKey(tr("IPV4 Dns:"));

    mIPV6 = new FixLabel(this);
    mIPV6->setFixedWidth(MAX_LABEL_WIDTH);
    mIPV6->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_ipv6Widget = new DetailWidget(qobject_cast<QWidget *>(mIPV6), m_listWidget);
    m_ipv6Widget->setKey(tr("IPV6:"));

    mMac = new QLabel(this);
    m_macWidget = new DetailWidget(qobject_cast<QWidget *>(mMac), m_listWidget);
    m_macWidget->setKey(tr("Mac:"));

    if (mIsWlan) {
        autoConnect = new QLabel(this);
        forgetNetBox = new QCheckBox(this);

        autoConnect->setText(tr("Auto Connection"));
        mAutoLayout = new QHBoxLayout(this);
        QSpacerItem *horizontalSpacer;
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        mAutoLayout->addSpacing(20);
        mAutoLayout->addWidget(forgetNetBox);
        mAutoLayout->addWidget(autoConnect);
        mAutoLayout->addSpacerItem(horizontalSpacer);
    }

    this->addDetailItem(m_listWidget, m_ssidWidget);
    this->addDetailItem(m_listWidget, m_protocolWidget);
    if (mIsWlan) {
        this->addDetailItem(m_listWidget, m_secTypeWidget);
        this->addDetailItem(m_listWidget, m_hzWidget);
        this->addDetailItem(m_listWidget, m_chanelWidget);
    }
    this->addDetailItem(m_listWidget, m_bandwidthWidget);
    this->addDetailItem(m_listWidget, m_ipv6Widget);
    this->addDetailItem(m_listWidget, m_ipv4Widget);
    this->addDetailItem(m_listWidget, m_ipv4DnsWidget);
    this->addDetailItem(m_listWidget, m_macWidget);

    QPalette mpal(m_listWidget->palette());
    mpal.setColor(QPalette::Base, qApp->palette().base().color());
    mpal.setColor(QPalette::AlternateBase, qApp->palette().alternateBase().color());
    m_listWidget->setAlternatingRowColors(true);
//    m_listWidget->setAutoFillBackground(true);
    m_listWidget->setPalette(mpal);

    layout->addWidget(mDetailFrame);
    if (mIsWlan) {
        layout->addLayout(mAutoLayout);
    }
}

void DetailPage::setEnableOfSaveBtn() {
    bool saveEnable = true;
    if (isCreate) {
        saveEnable = !mSSIDEdit->text().isEmpty();
    }
    emit setDetailPageState(saveEnable);
}
