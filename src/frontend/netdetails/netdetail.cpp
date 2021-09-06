#include "netdetail.h"

#define  WINDOW_WIDTH  540
#define  WINDOW_HEIGHT 574
#define  BUTTON_SIZE 30
#define  ICON_SIZE 22,22
#define  TITLE_LAYOUT_MARGINS 9,9,0,0
#define  LAYOUT_MARGINS 24,0,24,0
#define  BOTTOM_LAYOUT_SPACING 16
#define  PAGE_LAYOUT_SPACING 1
#define  DETAIL_PAGE_NUM 0
#define  IPV4_PAGE_NUM 1
#define  IPV6_PAGE_NUM 2
#define  SECURITY_PAGE_NUM 3
#define  CREATE_NET_PAGE_NUM 4
#define  PAGE_MIN_HEIGHT 40

extern void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed);

NetDetail::NetDetail(QString name, QString uuid, bool isActive, bool isWlan, bool isCreateNet, QWidget *parent)
    :m_name(name), m_uuid(uuid), isActive(isActive), isWlan(isWlan), isCreateNet(isCreateNet), QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    centerToScreen();

    m_netDeviceResource = new KyNetworkDeviceResourse(this);
    initWifiDevice();
    initLanDevice();
    initUI();
    loadPage();
    initComponent();
    getConInfo(mInfo);
    pagePadding(name,isWlan);
}

NetDetail::~NetDetail()
{

}
void NetDetail::centerToScreen()
{
    QDesktopWidget* m = QApplication::desktop();
    QRect desk_rect = m->screenGeometry(m->screenNumber(QCursor::pos()));
    int desk_x = desk_rect.width();
    int desk_y = desk_rect.height();
    int x = this->width();
    int y = this->height();
    this->move(desk_x / 2 - x / 2 + desk_rect.left(), desk_y / 2 - y / 2 + desk_rect.top());
}

void NetDetail::initUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(9,9,14,24);

    detailPage = new DetailPage(isWlan,this);
    ipv4Page = new Ipv4Page(isWlan,this);
    ipv6Page = new Ipv6Page(isWlan,this);
    securityWidget = new SecurityPage(this);
    createNetPage = new CreatNetPage(this);
//    addLanWidget = new AddLanWidget;

    titleWidget = new QWidget(this);
    centerWidget = new QWidget(this);
    bottomWidget = new QWidget(this);

    stackWidget = new QStackedWidget(centerWidget);
    stackWidget->addWidget(detailPage);
    stackWidget->addWidget(ipv4Page);
    stackWidget->addWidget(ipv6Page);
    stackWidget->addWidget(securityWidget);
    stackWidget->addWidget(createNetPage);

    mainLayout->addWidget(titleWidget);
    mainLayout->addWidget(centerWidget);
    mainLayout->addWidget(bottomWidget);

    titleWidget->setMinimumHeight(PAGE_MIN_HEIGHT);
    bottomWidget->setMinimumHeight(PAGE_MIN_HEIGHT);

    QHBoxLayout *titleLayout = new QHBoxLayout(titleWidget);
    titleLayout->setContentsMargins(TITLE_LAYOUT_MARGINS);
    pageFrame = new QFrame(this);

    QHBoxLayout *pageLayout = new QHBoxLayout(pageFrame);
    pageLayout->setSpacing(PAGE_LAYOUT_SPACING);

    detailBtn = new QPushButton(this);
    detailBtn->setText(tr("Detail"));

    ipv4Btn = new QPushButton(this);
    ipv4Btn->setText(tr("Ipv4"));

    ipv6Btn = new QPushButton(this);
    ipv6Btn->setText(tr("Ipv6"));

    securityBtn = new QPushButton(this);
    securityBtn->setText(tr("Security"));

    pageLayout->addStretch();
    pageLayout->addWidget(detailBtn);
    pageLayout->addWidget(ipv4Btn);
    pageLayout->addWidget(ipv6Btn);
    pageLayout->addWidget(securityBtn);
    pageLayout->addStretch();

    closeBtn  = new QPushButton(this);
    closeBtn->setFixedSize(BUTTON_SIZE,BUTTON_SIZE);
    closeBtn->setToolTip(tr("Close"));
    closeBtn->setProperty("isWindowButton", 0x02);
    closeBtn->setProperty("useIconHighlightEffect", 0x08);
    closeBtn->setFlat(true);
    closeBtn->setIcon(QIcon::fromTheme("window-close-symbolic"));

    confimBtn = new QPushButton(this);
    confimBtn->setText(tr("Confirm"));

    cancelBtn = new QPushButton(this);
    cancelBtn->setText(tr("Cancel"));

    forgetBtn = new QPushButton(this);
    forgetBtn->setText(tr("Forget this network"));

    titleLabel = new QLabel(this);

    iconLabel = new QLabel(this);
    iconLabel->setFixedSize(ICON_SIZE);
    titleLayout->addWidget(iconLabel);
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(closeBtn);

    QVBoxLayout *centerlayout = new QVBoxLayout(centerWidget);
    centerlayout->setContentsMargins(LAYOUT_MARGINS);
    centerlayout->addWidget(pageFrame);
    centerlayout->addWidget(stackWidget);

    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(LAYOUT_MARGINS);
    bottomLayout->setSpacing(BOTTOM_LAYOUT_SPACING);
    bottomLayout->addWidget(forgetBtn);
    bottomLayout->addStretch();
    bottomLayout->addWidget(cancelBtn);
    bottomLayout->addWidget(confimBtn);

}

void NetDetail::loadPage()
{
    //判断是否创建网络
    if (isCreateNet) {
        pageFrame->hide();
        stackWidget->setCurrentIndex(CREATE_NET_PAGE_NUM);
        titleLabel->setText(tr("Add Connect"));
    } else {
        stackWidget->setCurrentIndex(DETAIL_PAGE_NUM);
        titleLabel->setText(m_name);
    }
    if (!isWlan) {
        securityBtn->hide();
    } else {
        securityBtn->show();
    }
}

void NetDetail::initComponent()
{
    connect(closeBtn, &QPushButton::clicked, this, [=] {
       close();
    });
    connect(cancelBtn, &QPushButton::clicked, this, [=] {
        close();
    });
    connect(detailBtn, &QPushButton::clicked, this, [=] {
        stackWidget->setCurrentIndex(DETAIL_PAGE_NUM);
    });
    connect(ipv4Btn, &QPushButton::clicked, this, [=] {
        stackWidget->setCurrentIndex(IPV4_PAGE_NUM);
    });
    connect(ipv6Btn, &QPushButton::clicked, this, [=] {
        stackWidget->setCurrentIndex(IPV6_PAGE_NUM);
    });
    connect(securityBtn, &QPushButton::clicked, this, [=] {
        stackWidget->setCurrentIndex(SECURITY_PAGE_NUM);
    });
    connect(confimBtn, SIGNAL(clicked()), this, SLOT(on_btnConfirm_clicked()));
}

void NetDetail::pagePadding(QString netName, bool isWlan)
{
    foreach (ConInfo netInfo, mInfo) {
        //网络详情页填充
        if (isWlan) {
            if (!netInfo.strConName.compare(netName, Qt::CaseInsensitive)) {
                detailPage->setSSID(netName);
                detailPage->setProtocol(netInfo.strConType);
                detailPage->setSecType(netInfo.strSecType);
                detailPage->setHz(netInfo.strHz);
                detailPage->setChan(netInfo.strChan);
                detailPage->setIpv4(netInfo.strIPV4Address);
                detailPage->setIpv4Dns(netInfo.strIPV4FirDns);
                detailPage->setIpv6(netInfo.strIPV6Address);
                detailPage->setMac(netInfo.strMac);
                detailPage->setBandWidth(netInfo.strBandWidth);
            }
        } else {
            if (!netInfo.strConName.compare(netName, Qt::CaseInsensitive)) {
                detailPage->setSSID(netName);
                detailPage->setProtocol(netInfo.strConType);
                detailPage->setIpv4(netInfo.strIPV4Address);
                detailPage->setIpv4Dns(netInfo.strIPV4FirDns);
                detailPage->setIpv6(netInfo.strIPV6Address);
                detailPage->setMac(netInfo.strMac);
                detailPage->setBandWidth(netInfo.strBandWidth);
            }
        }
        //ipv4页面填充
        if (!netInfo.strConName.compare(netName, Qt::CaseInsensitive)) {
            if (netInfo.strIPV4ConfigType.toInt() == AUTO_CONFIG) {
                ipv4Page->setIpv4Config(netInfo.strIPV4ConfigType);
                ipv4Page->setIpv4(netInfo.strIPV4Address);
                ipv4Page->setIpv4FirDns(netInfo.strIPV4FirDns);
                ipv4Page->setIpv4SecDns(netInfo.strIPV4SecDns);
                ipv4Page->setGateWay(netInfo.strIPV4GateWay);
            } else {
                ipv4Page->setIpv4Config(netInfo.strIPV4ConfigType);
            }
        }
        //ipv6页面填充
        if (!netInfo.strConName.compare(netName, Qt::CaseInsensitive)) {
            if (netInfo.strIPV4ConfigType.toInt() == AUTO_CONFIG) {
                ipv6Page->setIpv6Config(netInfo.strIPV6ConfigType);
                ipv6Page->setIpv6(netInfo.strIPV4Address);
                ipv6Page->setIpv6FirDns(netInfo.strIPV6FirDns);
                ipv6Page->setIpv6SecDns(netInfo.strIPV4SecDns);
                ipv6Page->setGateWay(netInfo.strIPV4GateWay);
            } else {
                ipv6Page->setIpv6Config(netInfo.strIPV6ConfigType);
            }
        }
    }
}

void NetDetail::initLanDevice()
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    QString key("wired");
    m_deviceName = m_settings->value(key, "").toString();
    if (m_deviceName.isEmpty()) {
        qDebug() << "initDevice but  defalut wired card is null";
        QStringList list;
        list.empty();
        m_netDeviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, list);
        if (!list.isEmpty()) {
            m_deviceName = list.at(0);
            m_settings->setValue(key, m_deviceName);
        }
    }
    qDebug() << "[LanPage] initDevice defaultDevice = " << m_deviceName;
    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;
}

void NetDetail::initWifiDevice()
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    QString key("wireless");
    QString deviceName = m_settings->value(key, "").toString();
    m_netDeviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, m_devList);
    if (deviceName.isEmpty()) {
        qDebug() << "initDevice but  defalut wireless card is null";
        if (!m_devList.isEmpty()) {
            deviceName = m_devList.at(0);
            m_settings->setValue(key, deviceName);
        }
    }
    qDebug() << "[WlanPage] initDevice defaultDevice = " << deviceName;
    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;

}

//获取网路详情信息
void NetDetail::getConInfo(QList<ConInfo>& qlConInfo)
{
    ConInfo conInfo;
    KyConnectSetting  connetSetting;
    KyWirelessNetItem kyWirelessNetItem;
    KyWirelessNetResource *m_resource = new KyWirelessNetResource(this);
    KyConnectResourse *kyConnectResourse = new KyConnectResourse(this);

    QString deviceName;
    QString hardAddress;
    int bandWith;

    if (!m_devList.isEmpty()) {
        deviceName = m_devList.at(0);
    }
    if(isWlan) {
        if (!m_resource->getWifiNetwork(deviceName, m_name, kyWirelessNetItem)) {
            return;
        }
    }
    kyConnectResourse->getConnectionSetting(m_uuid,connetSetting);

    conInfo.strConUUID = m_uuid;
    conInfo.strIPV4ConfigType = QString("%1").arg(connetSetting.m_ipv4ConfigIpType);
    conInfo.strIPV6ConfigType = QString("%1").arg(connetSetting.m_ipv6ConfigIpType);
    qDebug()<<"conInfo.strConUUID:"<<conInfo.strConUUID<<"conInfo.strSecType"<<conInfo.strSecType;
    if (connetSetting.m_ipv4Address.length() > 0) {
        conInfo.strIPV4Address = connetSetting.m_ipv4Address.at(0).ip().toString();
        conInfo.strIPV4GateWay = connetSetting.m_ipv4Address.at(0).gateway().toString();
    } else {
        conInfo.strIPV4Address = "--";
        conInfo.strIPV4GateWay = "--";
        qDebug()<<"m_ipv4Address length is 0";
    }
    if (connetSetting.m_ipv6Address.length() > 0) {
        conInfo.strIPV6Address = connetSetting.m_ipv6Address.at(0).ip().toString();
        conInfo.strIPV6GateWay = connetSetting.m_ipv6Address.at(0).gateway().toString();
    } else {
        conInfo.strIPV6Address = "--";
        conInfo.strIPV6GateWay = "--";
        qDebug()<<"m_ipv4Address length is 0";
    }

    if (isWlan && isActive) {
        conInfo.strConType = "802-11-wireless";
        KyActiveConnectResourse *activeResourse = new KyActiveConnectResourse(this);
        QString ipv4,ipv6;
        QList<QHostAddress> ipv4Dns,ipv6Dns;
        activeResourse->getActiveConnectIpInfo(m_uuid,ipv4,ipv6);
        activeResourse->getActiveConnectDnsInfo(m_uuid,ipv4Dns,ipv6Dns);
        m_netDeviceResource->getHardwareInfo(deviceName, hardAddress, bandWith);

        qDebug()<<"802-11-wireless : "<<"deviceName:"<<deviceName<<",ssid:"<<m_ssid<<",uuid:"<<m_uuid;

        if (ipv4Dns.length() == 1) {
            conInfo.strIPV4FirDns = ipv4Dns.at(0).toString();
            conInfo.strIPV4SecDns = "--";
        } else if (ipv4Dns.length() == 2){
            conInfo.strIPV4FirDns = ipv4Dns.at(0).toString();
            conInfo.strIPV4SecDns = ipv4Dns.at(1).toString();
        } else {
            conInfo.strIPV4FirDns = "--";
            conInfo.strIPV4SecDns = "--";
            qDebug()<<"ipv4Dns length is 0";
        }
        if (ipv6Dns.length() == 1) {
            conInfo.strIPV6FirDns = ipv6Dns.at(0).toString();
            conInfo.strIPV6SecDns = "--";
        } else if (ipv4Dns.length() == 2){
            conInfo.strIPV6FirDns = ipv6Dns.at(0).toString();
            conInfo.strIPV6SecDns = ipv6Dns.at(1).toString();
        } else {
            conInfo.strIPV6FirDns = "--";
            conInfo.strIPV6SecDns = "--";
            qDebug()<<"ipv6Dns length is 0";
        }

        conInfo.strSecType = kyWirelessNetItem.m_secuType;
        conInfo.strMac = kyWirelessNetItem.m_bssid;
        conInfo.strHz = QString("%1").arg(kyWirelessNetItem.m_frequency) +" MHz";
        conInfo.strConName = kyWirelessNetItem.m_NetSsid;
        conInfo.strIPV4Address = ipv4;
        conInfo.strIPV6Address = ipv6;
        conInfo.strBandWidth = QString("%1").arg(bandWith/1000) + "Mbps";
    } else if (isWlan && !isActive) {
        conInfo.strConType = "802-11-wireless";
    } else {
        conInfo.strConType = "802-3-ethernet";
        qDebug()<<"802-11-ethernet : "<<"deviceName:"<<m_deviceName<<",ssid:"<<m_ssid<<",uuid:"<<m_uuid;
        conInfo.strConName = m_name;
        qDebug()<<"conInfo.strConName :aaaaaa"<<conInfo.strConName;
        m_netDeviceResource->getHardwareInfo(m_deviceName, hardAddress, bandWith);
        if (connetSetting.m_ipv4Dns.length() == 1) {
            conInfo.strIPV4FirDns = connetSetting.m_ipv4Dns.at(0).toString();
            conInfo.strIPV4SecDns = "--";
        } else if (connetSetting.m_ipv4Dns.length() == 2) {
            conInfo.strIPV4FirDns = connetSetting.m_ipv4Dns.at(0).toString();
            conInfo.strIPV4SecDns = connetSetting.m_ipv4Dns.at(1).toString();
        } else {
            conInfo.strIPV4FirDns = "--";
            conInfo.strIPV4SecDns = "--";
            qDebug()<<"m_ipv4DNS length is 0";
        }
        if (connetSetting.m_ipv6Dns.length() == 1) {
            conInfo.strIPV6FirDns = connetSetting.m_ipv6Dns.at(0).toString();
            conInfo.strIPV6SecDns = "--";
        } else if (connetSetting.m_ipv6Dns.length() == 2) {
            conInfo.strIPV6FirDns = connetSetting.m_ipv6Dns.at(0).toString();
            conInfo.strIPV6SecDns = connetSetting.m_ipv6Dns.at(1).toString();
        } else {
            conInfo.strIPV6FirDns = "--";
            conInfo.strIPV6SecDns = "--";
            qDebug()<<"m_ipv6DNS length is 0";
        }
        conInfo.strBandWidth = QString("%1").arg(bandWith/1000) + "Mbps";
        conInfo.strMac = hardAddress;
    }
    qlConInfo.append(conInfo);
}

//点击了保存更改网络设置的按钮
void NetDetail::on_btnConfirm_clicked()
{
    if (checkConfig()) {

    } else {

    }
}

//检测网络配置信息是否改变
bool NetDetail::checkConfig() {
    return false;
}

void NetDetail::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath rectPath;
    rectPath.addRoundedRect(this->rect().adjusted(12, 12, -12, -12), 12, 12);

    // 画一个黑底
    QPixmap pixmap(this->rect().size());
    pixmap.fill(Qt::transparent);
    QPainter pixmapPainter(&pixmap);
    pixmapPainter.setRenderHint(QPainter::Antialiasing);
    pixmapPainter.setPen(Qt::transparent);
    pixmapPainter.setBrush(Qt::black);
    pixmapPainter.setOpacity(0.65);
    pixmapPainter.drawPath(rectPath);
    pixmapPainter.end();

    // 模糊这个黑底
    QImage img = pixmap.toImage();
    qt_blurImage(img, 10, false, false);

    // 挖掉中心
    pixmap = QPixmap::fromImage(img);
    QPainter pixmapPainter2(&pixmap);
    pixmapPainter2.setRenderHint(QPainter::Antialiasing);
    pixmapPainter2.setCompositionMode(QPainter::CompositionMode_Clear);
    pixmapPainter2.setPen(Qt::transparent);
    pixmapPainter2.setBrush(Qt::transparent);
    pixmapPainter2.drawPath(rectPath);

    // 绘制阴影
    p.drawPixmap(this->rect(), pixmap, pixmap.rect());

    // 绘制一个背景
    p.save();
    p.fillPath(rectPath, palette().color(QPalette::Base));
    p.restore();
}
