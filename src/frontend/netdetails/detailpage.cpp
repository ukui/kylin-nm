#include "detailpage.h"
#include <QPainter>
#include <QListWidget>

#define MAX_NAME_LENGTH 32
#define MAX_LABEL_WIDTH 250

extern void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed);

DetailPage::DetailPage(bool isWlan, bool isCreate, QWidget *parent)
    : m_IsWlan(isWlan), m_IsCreate(isCreate), QFrame(parent)
{
    this->setFrameShape(QFrame::Shape::StyledPanel);
    this->setMaximumWidth(960);
    initUI();
    if (isCreate) {
     connect(m_SSIDEdit, &LineEdit::textEdited, this, &DetailPage::setEnableOfSaveBtn);
    }
}

DetailPage::~DetailPage()
{
    if (m_copiedTip != nullptr) {
        delete m_copiedTip;
        m_copiedTip = nullptr;
    }
}

void DetailPage::setSSID(const QString &ssid) {
    if (m_IsCreate) {
        return;
    }
    m_formerSSID = ssid;
    QFontMetrics fontMetrics(this->font());
    int fontSize = fontMetrics.width(ssid);
    if (fontSize > this->width()) {
        this->m_SSIDLabel->setText(fontMetrics.elidedText(ssid, Qt::ElideRight, this->width()));
        this->setToolTip(ssid);
    } else {
        this->m_SSIDLabel->setText(ssid);
        this->setToolTip("");
    }
}

void DetailPage::setProtocol(const QString &protocol) {
    this->m_Protocol->setText(protocol);
}

void DetailPage::setSecType(const QString &secType) {
    this->m_SecType->setText(secType);
}

void DetailPage::setHz(const QString &hz) {
    this->m_Hz->setText(hz);
}

void DetailPage::setChan(const QString &chan) {
    this->m_Chan->setText(chan);
}

void DetailPage::setBandWidth(const QString &brandWidth) {
    this->m_BandWidth->setText(brandWidth);
}

void DetailPage::setIpv4(const QString &ipv4) {
    this->m_IPV4->setText(ipv4);
}

void DetailPage::setIpv4Dns(const QString &ipv4Dns) {
    this->m_IPV4Dns->setText(ipv4Dns);
}

void DetailPage::setIpv6(const QString &ipv6) {
    m_formerIPV6 = ipv6;
    this->m_IPV6->setText(ipv6);
}

void DetailPage::setMac(const QString &mac) {
    this->m_Mac->setText(mac);
}

void DetailPage::setAutoConnect(bool flag)
{
    if (!m_IsWlan) {
        return;
    }
    this->m_forgetNetBox->setChecked(flag);
}

void DetailPage::getSsid(QString &ssid)
{
    if (m_IsCreate) {
        ssid = m_SSIDEdit->text();
    } else {
        ssid = m_SSIDLabel->text();
    }
}

bool DetailPage::checkIsChanged(const ConInfo info)
{
    if (!m_IsWlan) {
        return false;
    }
    if (info.isAutoConnect != m_forgetNetBox->isChecked()) {
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
    m_copiedTip->setContentsMargins(16, 0, 16, 0);
    m_copiedTip->setFixedHeight(48);
    m_copiedTip->setWindowFlags(Qt::FramelessWindowHint);
    m_copiedTip->setAttribute(Qt::WA_TranslucentBackground, true);
    m_copiedTip->setText(tr("Copied successfully!"));
}

QPalette DetailPage::getTheme()
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
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);

    QWidget *mDetailFrame = new QFrame(this);
    m_DetailLayout = new QVBoxLayout(mDetailFrame);
    m_DetailLayout->setContentsMargins(0,0,0,0);

    m_listWidget = new QListWidget(mDetailFrame);
    m_listWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_listWidget->setBackgroundRole(QPalette::Base);
    m_listWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_DetailLayout->addWidget(m_listWidget);

    if (!m_IsCreate) {
        m_SSIDLabel = new QLabel(this);
        m_SSIDLabel->adjustSize();
        m_SSIDLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        m_netCopyButton = new QPushButton(this);
        m_netCopyButton->setIcon(QIcon::fromTheme("edit-copy-symbolic"));
        m_netCopyButton->setToolTip(tr("Copy all"));
        //设置按钮背景颜色-透明
        QPalette btnPal = m_netCopyButton->palette();
        QColor color = qApp->palette().color(QPalette::Background);
        color.setAlphaF(0);
        btnPal.setColor(QPalette::Button, color);
        m_netCopyButton->setPalette(btnPal);

        m_ssidWidget = new DetailWidget(qobject_cast<QWidget *>(m_SSIDLabel), m_listWidget, m_netCopyButton);
        connect(m_netCopyButton, &QPushButton::clicked, this, &DetailPage::on_btnCopyNetDetail_clicked);
        newCopiedTip();

//        mSSID->setStyleSheet("background:transparent;border-width:0px;border-style:none");
//        mSSID->setFocusPolicy(Qt::NoFocus);
    } else {
//        mSSID->setStyleSheet("border-width:1px;;border-style:solid;border-color:black;border-radius:2px");
        m_SSIDEdit = new LineEdit(this);
        m_SSIDEdit->setMaximumWidth(MAX_LABEL_WIDTH);
        m_SSIDEdit->setAlignment(Qt::AlignRight);
        m_SSIDEdit->setStyleSheet("border-top:0px  solid;border-bottom:1px  solid;border-left:0px  solid;border-right: 0px  solid;background:transparent");
        m_SSIDEdit->setPlaceholderText(tr("Please input SSID:"));
        m_SSIDEdit->setMaxLength(MAX_NAME_LENGTH);
        m_ssidWidget = new DetailWidget(qobject_cast<QWidget *>(m_SSIDEdit), m_listWidget);
    }


    m_ssidWidget->setKey(tr("SSID:"));

    m_Protocol = new QLabel(this);
    m_protocolWidget = new DetailWidget(qobject_cast<QWidget *>(m_Protocol), m_listWidget);
    m_protocolWidget->setKey(tr("Protocol:"));

    m_SecType = new QLabel(this);
    m_secTypeWidget = new DetailWidget(qobject_cast<QWidget *>(m_SecType));
    m_secTypeWidget->setKey(tr("Security Type:"));

    m_Hz = new QLabel(this);
    m_hzWidget = new DetailWidget(qobject_cast<QWidget *>(m_Hz));
    m_hzWidget->setKey(tr("Hz:"));

    m_Chan = new QLabel(this);
    m_chanelWidget = new DetailWidget(qobject_cast<QWidget *>(m_Chan));
    m_chanelWidget->setKey(tr("Chan:"));

    m_BandWidth = new QLabel(this);
    m_bandwidthWidget = new DetailWidget(qobject_cast<QWidget *>(m_BandWidth), m_listWidget);
    m_bandwidthWidget->setKey(tr("BandWidth:"));

    m_IPV4 = new QLabel(this);
    m_ipv4Widget = new DetailWidget(qobject_cast<QWidget *>(m_IPV4), m_listWidget);
    m_ipv4Widget->setKey(tr("IPV4:"));

    m_IPV4Dns = new QLabel(this);
    m_ipv4DnsWidget = new DetailWidget(qobject_cast<QWidget *>(m_IPV4Dns), m_listWidget);
    m_ipv4DnsWidget->setKey(tr("IPV4 Dns:"));

    m_IPV6 = new FixLabel(this);
    m_IPV6->setFixedWidth(MAX_LABEL_WIDTH);
    m_IPV6->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_ipv6Widget = new DetailWidget(qobject_cast<QWidget *>(m_IPV6), m_listWidget);
    m_ipv6Widget->setKey(tr("IPV6:"));

    m_Mac = new QLabel(this);
    m_macWidget = new DetailWidget(qobject_cast<QWidget *>(m_Mac), m_listWidget);
    m_macWidget->setKey(tr("Mac:"));

    if (m_IsWlan) {
        m_autoConnect = new QLabel(this);
        m_forgetNetBox = new QCheckBox(this);

        m_autoConnect->setText(tr("Auto Connection"));
        m_AutoLayout = new QHBoxLayout(this);
        QSpacerItem *horizontalSpacer;
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        m_AutoLayout->addSpacing(20);
        m_AutoLayout->addWidget(m_forgetNetBox);
        m_AutoLayout->addWidget(m_autoConnect);
        m_AutoLayout->addSpacerItem(horizontalSpacer);
    }

    this->addDetailItem(m_listWidget, m_ssidWidget);
    this->addDetailItem(m_listWidget, m_protocolWidget);
    if (m_IsWlan) {
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

    m_layout->addWidget(mDetailFrame);
    if (m_IsWlan) {
        m_layout->addLayout(m_AutoLayout);
    }
}

void DetailPage::setEnableOfSaveBtn() {
    bool saveEnable = true;
    if (m_IsCreate) {
        saveEnable = !m_SSIDEdit->text().isEmpty();
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

    ssidCopy.append(m_formerSSID);
    protocolCopy.append(this->m_Protocol->text());
    netDetailList.append(ssidCopy);
    netDetailList.append(protocolCopy);

    if(m_IsWlan)
    {
        securityCopy.append(this->m_SecType->text());
        hzCopy.append(this->m_Hz->text());
        chanCopy.append(this->m_Chan->text());
        netDetailList.append(securityCopy);
        netDetailList.append(hzCopy);
        netDetailList.append(chanCopy);
    }

    bandwithCopy += this->m_BandWidth->text();
    ipv6Copy += m_formerIPV6;
    ipv4Copy += this->m_IPV4->text();
    ipv4dnsCopy += this->m_IPV4Dns->text();
    macCopy += this->m_Mac->text();
    netDetailList.append(bandwithCopy);
    netDetailList.append(ipv4Copy);
    netDetailList.append(ipv4dnsCopy);
    netDetailList.append(ipv6Copy);
    netDetailList.append(macCopy);
//    qDebug() << netDetailList;

    //设置剪贴板内容
    netDetailCopyText = netDetailList.join("\n");
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(netDetailCopyText);

    //设置“复制成功”弹窗位置并显示
    if (m_copiedTip != nullptr) {
        delete m_copiedTip;
        newCopiedTip();
    }
    QPoint position = m_ssidWidget->mapToGlobal(this->pos());
    double x = (this->width() - 0.5 * m_copiedTip->width());
    m_copiedTip->move(position.x() + x, position.y() + this->height() * 0.3);
    QPalette pal = getTheme();
    m_copiedTip->setPalette(pal);
    m_copiedTip->showInfo();
}

