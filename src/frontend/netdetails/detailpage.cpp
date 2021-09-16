#include "detailpage.h"

extern void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed);

DetailPage::DetailPage(bool isWlan, bool isCreate, QWidget *parent)
    : mIsWlan(isWlan), isCreate(isCreate), QFrame(parent)
{
    this->setFrameShape(QFrame::Shape::StyledPanel);
    this->setMaximumWidth(960);
    initUI();
    if (isCreate) {
     connect(mSSID, &QLineEdit::textEdited, this, &DetailPage::setEnableOfSaveBtn);
    }
}

void DetailPage::setSSID(const QString &ssid) {
    this->mSSID->setText(ssid);
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
    this->forgetNetBox->setChecked(flag);
}

void DetailPage::getSsid(QString &ssid)
{
    ssid = mSSID->text();
}

bool DetailPage::checkIsChanged(const ConInfo info)
{
    if (info.isAutoConnect != forgetNetBox->isChecked()) {
        return true;
    } else {
        return false;
    }
}

void DetailPage::initUI() {
    forgetNetBox = new QCheckBox(this);
    layout = new QVBoxLayout(this);

    mDetailLayout = new QFormLayout(this);

    mSSID      = new QLineEdit(this);
    mSSID->setAlignment(Qt::AlignRight);
    if (!isCreate) {
        mSSID->setStyleSheet("background:transparent;border-width:0;border-style:outset");
        mSSID->setFocusPolicy(Qt::NoFocus);
    } else {
        mSSID->setStyleSheet("border-width:0;border-style:outset");
    }

    mProtocol  = new QLabel(this);
    QHBoxLayout *mProtocolLayout = new QHBoxLayout(this);
    mProtocolLayout->addStretch();
    mProtocolLayout->addWidget(mProtocol);

    mSecType   = new QLabel(this);
    QHBoxLayout *mSecTypeLayout = new QHBoxLayout(this);
    mSecTypeLayout->addStretch();
    mSecTypeLayout->addWidget(mSecType);

    mHz        = new QLabel(this);
    QHBoxLayout *mHzLayout = new QHBoxLayout(this);
    mHzLayout->addStretch();
    mHzLayout->addWidget(mHz);

    mChan      = new QLabel(this);
    QHBoxLayout *mChanLayout = new QHBoxLayout(this);
    mChanLayout->addStretch();
    mChanLayout->addWidget(mChan);

    mBandWidth = new QLabel(this);
    QHBoxLayout *mBandWidthLayout = new QHBoxLayout(this);
    mBandWidthLayout->addStretch();
    mBandWidthLayout->addWidget(mBandWidth);

    mIPV4      = new QLabel(this);
    QHBoxLayout *mIPV4Layout = new QHBoxLayout(this);
    mIPV4Layout->addStretch();
    mIPV4Layout->addWidget(mIPV4);

    mIPV4Dns   = new QLabel(this);
    QHBoxLayout *mIPV4DnsLayout = new QHBoxLayout(this);
    mIPV4DnsLayout->addStretch();
    mIPV4DnsLayout->addWidget(mIPV4Dns);

    mIPV6      = new QLabel(this);
    QHBoxLayout *mIPV6Layout = new QHBoxLayout(this);
    mIPV6Layout->addStretch();
    mIPV6Layout->addWidget(mIPV6);

    mMac       = new QLabel(this);
    QHBoxLayout *mMacLayout = new QHBoxLayout(this);
    mMacLayout->addStretch();
    mMacLayout->addWidget(mMac);

    autoConnect = new QLabel(this);

    autoConnect->setText(tr("Auto Connection"));
    mAutoLayout = new QHBoxLayout(this);
    QSpacerItem *horizontalSpacer;
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    mAutoLayout->addWidget(forgetNetBox);
    mAutoLayout->addWidget(autoConnect);
    mAutoLayout->addSpacerItem(horizontalSpacer);

    mDetailLayout->addRow(tr("SSID:"), mSSID);
    mDetailLayout->addRow(tr("Protocol:"), mProtocolLayout);
    if (mIsWlan) {
        mDetailLayout->addRow(tr("Security Type:"), mSecTypeLayout);
        mDetailLayout->addRow(tr("Hz:"), mHzLayout);
        mDetailLayout->addRow(tr("Chan:"), mChanLayout);
    }
    mDetailLayout->addRow(tr("BandWidth:"), mBandWidthLayout);
    mDetailLayout->addRow(tr("IPV6:"), mIPV6Layout);
    mDetailLayout->addRow(tr("IPV4:"), mIPV4Layout);
    mDetailLayout->addRow(tr("IPV4 Dns:"), mIPV4DnsLayout);
    mDetailLayout->addRow(tr("Mac:"), mMacLayout);

    layout->addLayout(mDetailLayout);
    layout->addStretch();
    layout->addLayout(mAutoLayout);

}

void DetailPage::setEnableOfSaveBtn() {
    emit setDetailPageState(!mSSID->text().isEmpty());
}
