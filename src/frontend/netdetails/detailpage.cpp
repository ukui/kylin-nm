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
    m_formerSsid = ssid;
    QFontMetrics fontMetrics(this->font());
    int fontSize = fontMetrics.width(ssid);
    if (fontSize > this->width()) {
        this->mSSIDLabel->setText(fontMetrics.elidedText(ssid, Qt::ElideRight, this->width()));
        this->setToolTip(ssid);
    } else {
        this->mSSIDLabel->setText(ssid);
        this->setToolTip("");
    }
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
    m_formerIPV6 = ipv6;
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

void DetailPage::newCopiedTip()
{
    //设置“复制成功”消息弹窗格式
    m_copiedTip = new KBallonTip();
    m_copiedTip->setTipType(Normal);
    m_copiedTip->setFixedSize(158, 58);
    m_copiedTip->setWindowFlags(Qt::FramelessWindowHint);
    m_copiedTip->setAttribute(Qt::WA_TranslucentBackground, true);
    m_copiedTip->setText(tr("Copied successfully!"));
}

QPalette DetailPage::GetTheme()
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
    if (styleGsettings != nullptr) {
        delete styleGsettings;
        styleGsettings = nullptr;
    }
    return pal;
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

        m_netCopyButton = new QPushButton(this);
        m_netCopyButton->setIcon(QIcon::fromTheme("edit-copy-symbolic"));
        m_netCopyButton->setToolTip(tr("Copy all"));
        //设置按钮背景颜色-透明
        QPalette btnPal = m_netCopyButton->palette();
        QColor color = qApp->palette().color(QPalette::Background);
        color.setAlphaF(0);
        btnPal.setColor(QPalette::Button, color);
        m_netCopyButton->setPalette(btnPal);

        m_ssidWidget = new DetailWidget(qobject_cast<QWidget *>(mSSIDLabel), m_listWidget, m_netCopyButton);
        connect(m_netCopyButton, &QPushButton::clicked, this, &DetailPage::on_btnCopyNetDetail_clicked);
        newCopiedTip();

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
        m_ssidWidget = new DetailWidget(qobject_cast<QWidget *>(mSSIDEdit), m_listWidget);
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

//获取列表信息
void DetailPage::on_btnCopyNetDetail_clicked()
{
    QStringList  netDetailList;
    QString      ssidCopy = tr("SSID:");
    QString      protocolCopy = tr("Protocol:");
    QString      securityCopy = tr("Security Type:");
    QString      hzCopy= tr ("Hz:");
    QString      chanCopy= tr ("Chan:");
    QString      bandwithCopy = tr("BandWidth:");
    QString      ipv4Copy = tr("IPV4:");
    QString      ipv4dnsCopy = tr("IPV4 Dns:");
    QString      ipv6Copy = tr("IPV6:");
    QString      macCopy = tr("Mac:");
    QString      netDetailCopyText;

    ssidCopy += m_formerSsid;
    protocolCopy += this->mProtocol->text();
    netDetailList << ssidCopy << protocolCopy;

    if(mIsWlan)
    {
        securityCopy += this->mSecType->text();
        hzCopy += this->mHz->text();
        chanCopy += this->mChan->text();
        netDetailList << securityCopy << hzCopy << chanCopy;
    }

    bandwithCopy += this->mBandWidth->text();
    ipv6Copy += m_formerIPV6;
    ipv4Copy += this->mIPV4->text();
    ipv4dnsCopy += this->mIPV4Dns->text();
    macCopy += this->mMac->text();
    netDetailList << bandwithCopy << ipv4Copy << ipv4dnsCopy << ipv6Copy << macCopy;
//    qDebug() << netDetailList;

    //设置剪贴板内容
    netDetailCopyText = netDetailList.join("\n");
    QClipboard *m_clipboard = QApplication::clipboard();
    m_clipboard->setText(netDetailCopyText);

    //设置“复制成功”弹窗位置并显示
    if (m_copiedTip != nullptr) {
        delete m_copiedTip;
        newCopiedTip();
    }
    QPoint m_copyBtnPosition = m_ssidWidget->mapToGlobal(this->pos());
    double x = 0.5 * (m_ssidWidget->width() - m_copiedTip->width());
    m_copiedTip->move(m_copyBtnPosition.x() + x, m_copyBtnPosition.y() + 150);
    QPalette pal = GetTheme();
    m_copiedTip->setPalette(pal);
    m_copiedTip->showInfo();
}

