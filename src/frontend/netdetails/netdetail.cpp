#include "netdetail.h"
#include "backend/kylinipv4arping.h"
#include "backend/kylinipv6arping.h"
//#include "xatom/xatom-helper.h"

#include <QEvent>

#define  WINDOW_WIDTH  520
#define  WINDOW_HEIGHT 590
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

//extern void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed);

void NetDetail::showDesktopNotify(const QString &message)
{
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    QList<QVariant> args;
    args<<(tr("Kylin NM"))
       <<((unsigned int) 0)
       <<QString("/usr/share/icons/ukui-icon-theme-default/24x24/devices/gnome-dev-ethernet.png")
       <<tr("kylin network desktop message") //显示的是什么类型的信息
       <<message //显示的具体信息
       <<QStringList()
       <<QVariantMap()
       <<(int)-1;
    iface.callWithArgumentList(QDBus::AutoDetect,"Notify",args);
}

NetDetail::NetDetail(QString interface, QString name, QString uuid, bool isActive, bool isWlan, bool isCreateNet, QWidget *parent)
    :m_deviceName(interface),
     m_name(name),
     m_uuid(uuid),
     isActive(isActive),
     isWlan(isWlan),
     m_isCreateNet(isCreateNet),
     QDialog(parent)
{
    //设置窗口无边框，阴影
//#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
//    MotifWmHints window_hints;
//    window_hints.flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
//    window_hints.functions = MWM_FUNC_ALL;
//    window_hints.decorations = MWM_DECOR_BORDER;
//    XAtomHelper::getInstance()->setWindowMotifHint(this->winId(), window_hints);
//#else
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
//#endif
//    this->setProperty("useStyleWindowManager", false); //禁用拖动
//    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint );
//    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    centerToScreen();

    qDebug() << m_isCreateNet << name;
    if (!m_isCreateNet && name.isEmpty()) {
        m_isCreateNet = true;
    }
    qDebug() << m_isCreateNet;
    m_netDeviceResource = new KyNetworkDeviceResourse(this);
    m_wirelessConnOpration = new KyWirelessConnectOperation(this);
    m_resource = new KyWirelessNetResource(this);
    m_connectOperation = new KyConnectOperation(this);
    m_wiredConnOperation = new KyWiredConnectOperation(this);
    initUI();
    loadPage();
    initComponent();
    getConInfo(m_info);
    pagePadding(name,isWlan);

    connect(qApp, &QApplication::paletteChanged, this, &NetDetail::onPaletteChanged);

    isCreateOk = !(m_isCreateNet && !isWlan);
    isDetailOk = !(m_name.isEmpty());
    isIpv4Ok = true;
    isIpv6Ok = true;
    isSecuOk = true;

    qDebug() << interface << name << uuid <<  "isWlan" << isWlan << "isCreateNet" <<m_isCreateNet;

    setConfirmEnable();
}

NetDetail::~NetDetail()
{

}

void NetDetail::onPaletteChanged()
{
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, qApp->palette().base().color());
    this->setPalette(pal);

    QPalette listwidget_pal(detailPage->m_listWidget->palette());
    listwidget_pal.setColor(QPalette::Base, qApp->palette().base().color());
    listwidget_pal.setColor(QPalette::AlternateBase, qApp->palette().alternateBase().color());
    detailPage->m_listWidget->setAlternatingRowColors(true);
    detailPage->m_listWidget->setPalette(listwidget_pal);

}

void NetDetail::paintEvent(QPaintEvent *event)
{
    return QDialog::paintEvent(event);
}

void NetDetail::closeEvent(QCloseEvent *event)
{
    emit this->detailPageClose(false);
    return QDialog::closeEvent(event);
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

    detailPage = new DetailPage(isWlan, m_name.isEmpty(), this);
    ipv4Page = new Ipv4Page(this);
    ipv6Page = new Ipv6Page(this);
    securityPage = new SecurityPage(this);
    createNetPage = new CreatNetPage(this);

    detailPage->installEventFilter(this);
    ipv4Page->installEventFilter(this);
    ipv6Page->installEventFilter(this);
    securityPage->installEventFilter(this);
    createNetPage->installEventFilter(this);

    titleWidget = new QWidget(this);
    centerWidget = new QWidget(this);
    bottomWidget = new QWidget(this);

    stackWidget = new QStackedWidget(centerWidget);
    stackWidget->addWidget(detailPage);
    stackWidget->addWidget(ipv4Page);
    stackWidget->addWidget(ipv6Page);
    stackWidget->addWidget(securityPage);
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
    detailBtn->setCheckable(true);
    detailBtn->setChecked(true);

    ipv4Btn = new QPushButton(this);
    ipv4Btn->setText(tr("Ipv4"));
    ipv4Btn->setCheckable(true);

    ipv6Btn = new QPushButton(this);
    ipv6Btn->setText(tr("Ipv6"));
    ipv6Btn->setCheckable(true);

    securityBtn = new QPushButton(this);
    securityBtn->setText(tr("Security"));
    securityBtn->setCheckable(true);

    m_group = new QButtonGroup(this);
    m_group->addButton(detailBtn);
    m_group->addButton(ipv4Btn);
    m_group->addButton(ipv6Btn);
    if (isWlan) {
        m_group->addButton(securityBtn);
    }

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
    if (!QIcon::fromTheme("kylin-network").isNull()) {
        iconLabel->setPixmap(QIcon::fromTheme("kylin-network").pixmap(ICON_SIZE));
        iconLabel->setProperty("useIconHighlightEffect", 0x10);
    } else {
        qDebug() << "can't find preferences-system-network-symbolic in theme";
    }
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

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, qApp->palette().base().color());
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void NetDetail::loadPage()
{
    //判断是否创建网络页面
    if (m_isCreateNet && !isWlan) {
        pageFrame->hide();
        stackWidget->setCurrentIndex(CREATE_NET_PAGE_NUM);
        titleLabel->setText(tr("Add Lan Connect"));
    } else {
        stackWidget->setCurrentIndex(DETAIL_PAGE_NUM);
        titleLabel->setText(m_name);
        if (!isWlan) {
            securityBtn->hide();
        } else {
            securityBtn->show();
            if (m_name.isEmpty()) {
                titleLabel->setText(tr("connect hiddin wlan"));
            }
        }
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
    if (isWlan && !m_uuid.isEmpty()) {
        forgetBtn->show();
        connect(forgetBtn, SIGNAL(clicked()), this, SLOT(on_btnForget_clicked()));
    } else {
        forgetBtn->hide();
    }

    connect(createNetPage, &CreatNetPage::setCreatePageState, this, [=](bool status) {
       isCreateOk = status;
       setConfirmEnable();
    });

    connect(detailPage, &DetailPage::setDetailPageState, this, [=](bool status) {
       isDetailOk = status;
       setConfirmEnable();
    });

    connect(ipv4Page, &Ipv4Page::setIpv4PageState, this, [=](bool status) {
       isIpv4Ok = status;
       setConfirmEnable();
    });

    connect(ipv6Page, &Ipv6Page::setIpv6PageState, this, [=](bool status) {
       isIpv6Ok = status;
       setConfirmEnable();
    });

    connect(securityPage, &SecurityPage::setSecuPageState, this, [=](bool status) {
       isSecuOk = status;
       setConfirmEnable();
    });
}

void NetDetail::pagePadding(QString netName, bool isWlan)
{
    //网络详情页填充
    if(m_isCreateNet && !isWlan) {
        return;
    }

    detailPage->setSSID(netName);
    detailPage->setProtocol(m_info.strConType);
    detailPage->setSecType(m_info.strSecType);
    detailPage->setHz(m_info.strHz);
    detailPage->setChan(m_info.strChan);
    detailPage->setIpv4(m_info.strDynamicIpv4);
    detailPage->setIpv4Dns(m_info.strDynamicIpv4Dns);
    detailPage->setIpv6(m_info.strDynamicIpv6);
    detailPage->setMac(m_info.strMac);
    detailPage->setBandWidth(m_info.strBandWidth);
    detailPage->setAutoConnect(m_info.isAutoConnect);

    //ipv4页面填充
    if (m_info.ipv4ConfigType == CONFIG_IP_MANUAL) {
        ipv4Page->setIpv4Config(m_info.ipv4ConfigType);
        ipv4Page->setIpv4(m_info.strIPV4Address);
        ipv4Page->setNetMask(m_info.strIPV4NetMask);
        ipv4Page->setIpv4FirDns(m_info.strIPV4FirDns);
        ipv4Page->setIpv4SecDns(m_info.strIPV4SecDns);
        ipv4Page->setGateWay(m_info.strIPV4GateWay);
    } else {
        ipv4Page->setIpv4Config(m_info.ipv4ConfigType);
    }
    //ipv6页面填充
    if (m_info.ipv6ConfigType == CONFIG_IP_MANUAL) {
        ipv6Page->setIpv6Config(m_info.ipv6ConfigType);
        ipv6Page->setIpv6(m_info.strIPV6Address);
        ipv6Page->setIpv6Perfix(m_info.iIPV6Prefix);
        ipv6Page->setIpv6FirDns(m_info.strIPV6FirDns);
        ipv6Page->setIpv6SecDns(m_info.strIPV6SecDns);
        ipv6Page->setGateWay(m_info.strIPV6GateWay);
    } else {
        ipv6Page->setIpv6Config(m_info.ipv6ConfigType);
    }

    //安全页面
    if (isWlan) {
        securityPage->setSecurity(m_info.secType);
        qDebug() << "setSecurity" << m_info.secType;
        if (m_info.secType == WPA_AND_WPA2_ENTERPRISE) {
            if (m_info.enterpriseType == TLS) {
                securityPage->setTlsInfo(m_info.tlsInfo);
            } else if (m_info.enterpriseType == PEAP) {
                securityPage->setPeapInfo(m_info.peapInfo);
            } else if (m_info.enterpriseType == TTLS) {
                securityPage->setTtlsInfo(m_info.ttlsInfo);
            }
        }
    }

}

//获取网路详情信息
void NetDetail::getConInfo(ConInfo &conInfo)
{
    if (m_isCreateNet && !isWlan) {
        return;
    }
    getBaseInfo(conInfo);
    getDynamicIpInfo(conInfo, isActive);
    getStaticIpInfo(conInfo,isActive);
}


//详情ssid 带宽 物理地址 无线额外(安全性 频带 通道)
void NetDetail::getBaseInfo(ConInfo &conInfo)
{
    //有线无线公有
    conInfo.strConName = m_name;

    QString hardAddress;
    int bandWith;
    m_netDeviceResource->getHardwareInfo(m_deviceName, hardAddress, bandWith);

    if (!hardAddress.isEmpty()) {
        conInfo.strBandWidth = QString("%1").arg(bandWith/1000) + "Mbps";
        conInfo.strMac = hardAddress;
    }

    if (!isWlan) {
        conInfo.strConType = "802-3-ethernet";
    } else {
        conInfo.strConType = "802-11-wireless";
        if (!isActive) {
            KyWirelessNetItem item;
            if (!m_resource->getWifiNetwork(m_deviceName, m_name, item)) {
                qDebug() << "getWifiNetWork failed device:" << m_deviceName << " name:" << m_name;
                return;
            } else {
                    conInfo.strHz = QString::number(item.m_frequency);
                    conInfo.strChan = QString::number(item.m_channel);
                    //无线特有
                    conInfo.strSecType = item.m_secuType;
                    if (conInfo.strSecType.isEmpty()) {
                        conInfo.strSecType = tr("None");
                    }
            }
        } else {
            uint iHz,iChan;
            QString strMac;
            m_netDeviceResource->getDeviceActiveAPInfo(m_deviceName, strMac, iHz, iChan, conInfo.strSecType);
            if (conInfo.strSecType.isEmpty()) {
                conInfo.strSecType = tr("None");
            }
            conInfo.strHz = QString::number(iHz);
            conInfo.strChan = QString::number(iChan);

        }



        KyKeyMgmt type = m_wirelessConnOpration->getConnectKeyMgmt(m_uuid);
        if (type == WpaNone || type == Unknown) {
            conInfo.secType = NONE;
        } else if (type == WpaPsk) {
            conInfo.secType = WPA_AND_WPA2_PERSONAL;
        } else if (type == SAE) {
            conInfo.secType = WPA3_PERSONAL;
        } else if (type == WpaEap) {
            conInfo.secType = WPA_AND_WPA2_ENTERPRISE;
        } else {
            qDebug() << "KeyMgmt not support now " << type;
        }

        initSecuData();
    }
}

//详情ipv4 ipv6 ipv4Dns
void NetDetail::getDynamicIpInfo(ConInfo &conInfo, bool bActived)
{
    if (!bActived) {
        return;
    }
    //已激活的网络 详情页显示动态ipv4 ipv6 dns
    QString ipv4,ipv6;
    QList<QHostAddress> ipv4Dns,ipv6Dns;
    KyActiveConnectResourse *activeResourse = new KyActiveConnectResourse(this);
    activeResourse->getActiveConnectIpInfo(m_uuid,ipv4,ipv6);
    activeResourse->getActiveConnectDnsInfo(m_uuid,ipv4Dns,ipv6Dns);

    //Ipv6
    if (!ipv6.isEmpty()) {
        conInfo.strDynamicIpv6 = ipv6;
    }

    //IPv4
    if (!ipv4.isEmpty()) {
        conInfo.strDynamicIpv4 = ipv4;
    }

    if (!ipv4Dns.isEmpty()) {
        conInfo.strDynamicIpv4Dns = ipv4Dns.at(0).toString();
    }
}

//ipv4+ipv6页面
void NetDetail::getStaticIpInfo(ConInfo &conInfo, bool bActived)
{
    KyConnectResourse *kyConnectResourse = new KyConnectResourse(this);
    KyConnectSetting  connetSetting;
    kyConnectResourse->getConnectionSetting(m_uuid,connetSetting);
    connetSetting.dumpInfo();

    conInfo.ipv4ConfigType = connetSetting.m_ipv4ConfigIpType;
    conInfo.ipv6ConfigType = connetSetting.m_ipv6ConfigIpType;
    conInfo.isAutoConnect  = connetSetting.m_isAutoConnect;

    if (connetSetting.m_ipv4ConfigIpType == CONFIG_IP_MANUAL) {
        if (connetSetting.m_ipv4Address.size() > 0) {
            conInfo.strIPV4Address = connetSetting.m_ipv4Address.at(0).ip().toString();
            conInfo.strIPV4NetMask = connetSetting.m_ipv4Address.at(0).netmask().toString();
            conInfo.strIPV4GateWay = connetSetting.m_ipv4Address.at(0).gateway().toString();
        }
        if (connetSetting.m_ipv4Dns.size() == 1) {
            conInfo.strIPV4FirDns = connetSetting.m_ipv4Dns.at(0).toString();
        } else if (connetSetting.m_ipv4Dns.size() > 1) {
            conInfo.strIPV4FirDns = connetSetting.m_ipv4Dns.at(0).toString();
            conInfo.strIPV4SecDns = connetSetting.m_ipv4Dns.at(1).toString();
        }
    }

    if (connetSetting.m_ipv6ConfigIpType == CONFIG_IP_MANUAL) {
        if (connetSetting.m_ipv6Address.size() > 0) {
            conInfo.strIPV6Address = connetSetting.m_ipv6Address.at(0).ip().toString();
            conInfo.iIPV6Prefix = ipv6Page->getPerfixLength(connetSetting.m_ipv6Address.at(0).netmask().toString());
            conInfo.strIPV6GateWay = connetSetting.m_ipv6Address.at(0).gateway().toString();
        }

        if (connetSetting.m_ipv6Dns.size() == 1) {
            conInfo.strIPV6FirDns = connetSetting.m_ipv6Dns.at(0).toString();
        } else if (connetSetting.m_ipv4Dns.size() > 1) {
            conInfo.strIPV6FirDns = connetSetting.m_ipv6Dns.at(0).toString();
            conInfo.strIPV6SecDns = connetSetting.m_ipv6Dns.at(1).toString();
        }
    }

    if (!bActived) {
        conInfo.strDynamicIpv4 = conInfo.strIPV4Address.isEmpty() ? tr("Auto") : conInfo.strIPV4Address;
        conInfo.strDynamicIpv6 = conInfo.strIPV6Address.isEmpty() ? tr("Auto") : conInfo.strIPV6Address;
        conInfo.strDynamicIpv4Dns = conInfo.strIPV4FirDns.isEmpty() ? tr("Auto") : conInfo.strIPV4FirDns;
    }
}

void NetDetail::initSecuData()
{
    QString password("");
    int type = m_info.secType;
    switch (type) {
    case NONE:
        break;
    case WPA_AND_WPA2_PERSONAL:
    case WPA3_PERSONAL:
        if (!m_uuid.isEmpty()) {
            NetworkManager::Setting::SecretFlags flag;
            if (m_wirelessConnOpration->getConnSecretFlags(m_uuid, flag)) {
                if (!flag) {
                    password = m_wirelessConnOpration->getPsk(m_uuid);
                }
            }
        }
        m_info.strPassword = password;
        securityPage->setPsk(password);
        break;
    case WPA_AND_WPA2_ENTERPRISE:
        if (!m_wirelessConnOpration->getEnterpiseEapMethod(m_uuid, m_info.enterpriseType)) {
            qDebug() << m_name << "not enterprise wifi";
        } else if (m_info.enterpriseType == TLS){
            initTlsInfo(m_info);
        } else if (m_info.enterpriseType == PEAP){
            initPeapInfo(m_info);
        } else {
            initTtlsInfo(m_info);
        }
        break;
    default:
        break;
    }
}

void NetDetail::initTlsInfo(ConInfo &conInfo)
{
    m_resource->getEnterPriseInfoTls(m_uuid, conInfo.tlsInfo);
}

void NetDetail::initPeapInfo(ConInfo &conInfo)
{
    m_resource->getEnterPriseInfoPeap(m_uuid, conInfo.peapInfo);
}

void NetDetail::initTtlsInfo(ConInfo &conInfo)
{
    m_resource->getEnterPriseInfoTtls(m_uuid, conInfo.ttlsInfo);
}

//点击了保存更改网络设置的按钮
void NetDetail::on_btnConfirm_clicked()
{
    qDebug() << "on_btnConfirm_clicked";
    setEnabled(false);
    if (m_isCreateNet) {
        if (!isWlan) {
            //新建有线连接
            qDebug() << "Confirm create wired connect";
            if (!createWiredConnect()) {
                setEnabled(true);
                return;
            }
        } else {
            //新建无线连接
            qDebug() << "Confirm create wireless connect";
            if (!createWirelessConnect()) {
                setEnabled(true);
                return;
            }
        }
    } else {
        //更新连接
        qDebug() << "Confirm update connect";
        if (!updateConnect()) {
            setEnabled(true);
            return;
        }
    }
    close();
}

//点击忘记网络
void NetDetail::on_btnForget_clicked()
{
    qDebug() << "user choose forget connection uuid = " << m_uuid;
    m_connectOperation->deleteConnect(m_uuid);
    close();
}

void NetDetail::setConfirmEnable()
{
    if (m_isCreateNet && !isWlan) {
            isConfirmBtnEnable = isCreateOk;
    } else {
        if (isDetailOk && isIpv4Ok && isIpv6Ok) {
            if (isWlan && !isSecuOk) {
                isConfirmBtnEnable = false;
            } else {
                isConfirmBtnEnable = true;
            }
        } else {
            isConfirmBtnEnable = false;
        }
    }
    qDebug() << "setConfirmEnable "<< isConfirmBtnEnable;
    confimBtn->setEnabled(isConfirmBtnEnable);
}

bool NetDetail::checkIpv4Conflict(QString ipv4Address)
{
    showDesktopNotify(tr("start check ipv4 address conflict"));
    bool isConflict = false;
    KyIpv4Arping* ipv4Arping = new KyIpv4Arping(m_deviceName, ipv4Address);

    if (ipv4Arping->ipv4ConflictCheck() >= 0) {
        isConflict =  ipv4Arping->ipv4IsConflict();
    } else {
        qWarning() << "checkIpv4Conflict internal error";
    }

    delete ipv4Arping;
    ipv4Arping = nullptr;
    return isConflict;
}

bool NetDetail::checkIpv6Conflict(QString ipv6address)
{
    showDesktopNotify(tr("start check ipv6 address conflict"));
    bool isConflict = false;
    KyIpv6Arping* ipv46rping = new KyIpv6Arping(m_deviceName, ipv6address);

    if (ipv46rping->ipv6ConflictCheck() >= 0) {
        isConflict =  ipv46rping->ipv6IsConflict();
    } else {
        qWarning() << "checkIpv6Conflict internal error";
    }

    delete ipv46rping;
    ipv46rping = nullptr;
    return isConflict;
}

void NetDetail::updateWirelessPersonalConnect()
{
    KyWirelessConnectSetting setting;
    securityPage->updateSecurityChange(setting);
    bool isPwdChanged = !(m_info.strPassword == setting.m_psk);
    m_wirelessConnOpration->updateWirelessPersonalConnect(m_uuid, setting, isPwdChanged);
}

void NetDetail::updateWirelessEnterPriseConnect(KyEapMethodType enterpriseType)
{
    if (enterpriseType == TLS) {
        m_info.tlsInfo.devIfaceName = m_deviceName;
        securityPage->updateTlsChange(m_info.tlsInfo);
        m_wirelessConnOpration->updateWirelessEnterPriseTlsConnect(m_uuid, m_info.tlsInfo);
    } else if (enterpriseType == PEAP) {
        securityPage->updatePeapChange(m_info.peapInfo);
        m_wirelessConnOpration->updateWirelessEnterPrisePeapConnect(m_uuid, m_info.peapInfo);
    } else if (enterpriseType == TTLS) {
        securityPage->updateTtlsChange(m_info.ttlsInfo);
        m_wirelessConnOpration->updateWirelessEnterPriseTtlsConnect(m_uuid, m_info.ttlsInfo);
    }
}

bool NetDetail::createWiredConnect()
{
    KyWirelessConnectSetting connetSetting;
    connetSetting.setIfaceName(m_deviceName);
    createNetPage->constructIpv4Info(connetSetting);
    if (connetSetting.m_ipv4ConfigIpType != CONFIG_IP_DHCP) {
        if (checkIpv4Conflict(connetSetting.m_ipv4Address.at(0).ip().toString())) {
            qDebug() << "ipv4 conflict";
            showDesktopNotify(tr("ipv4 address conflict!"));
            return false;
        }
    }
    m_wiredConnOperation->createWiredConnect(connetSetting);
    return true;
}

bool NetDetail::createWirelessConnect()
{
    KyWirelessConnectSetting connetSetting;
    KySecuType secuType;
    KyEapMethodType enterpriseType;
    securityPage->getSecuType(secuType, enterpriseType);
    //类型判断
    if (!m_name.isEmpty()) {
        if (!checkWirelessSecurity(secuType)) {
            return false;
        }
    }

    //基本信息
    QString ssid;
    if (m_name.isEmpty()) {
        detailPage->getSsid(ssid);
    } else {
        ssid = m_name;
    }
    connetSetting.setConnectName(ssid);
    connetSetting.setIfaceName(m_deviceName);
    if (detailPage->checkIsChanged(m_info)) {
        connetSetting.isAutoConnect = !m_info.isAutoConnect;
    } else {
        connetSetting.isAutoConnect = m_info.isAutoConnect;
    }
    qDebug() << "isAutoConnect" << connetSetting.isAutoConnect;
    connetSetting.m_ssid = ssid;
//    connetSetting.m_secretFlag = NetworkManager::Setting::None;
    //由于X.h的None与此处的None有歧义，此处直接使用值
    connetSetting.m_secretFlag = 0;

    //ipv4 & ipv6
    bool ipv4Change = ipv4Page->checkIsChanged(m_info, connetSetting);
    bool ipv6Change = ipv6Page->checkIsChanged(m_info, connetSetting);

    connetSetting.dumpInfo();

    qDebug() << "ipv4Changed" << ipv4Change << "ipv6Change" << ipv6Change;
    if (ipv4Change && connetSetting.m_ipv4ConfigIpType == CONFIG_IP_MANUAL) {
        if (checkIpv4Conflict(connetSetting.m_ipv4Address.at(0).ip().toString())) {
            qDebug() << "ipv4 conflict";
            showDesktopNotify(tr("ipv4 address conflict!"));
            return false;
        }
    }

    if (ipv6Change && connetSetting.m_ipv6ConfigIpType == CONFIG_IP_MANUAL) {
        if (checkIpv6Conflict(connetSetting.m_ipv6Address.at(0).ip().toString())) {
            qDebug() << "ipv6 conflict";
            showDesktopNotify(tr("ipv6 address conflict!"));
            return false;
        }
    }
    //wifi安全性
    if (secuType == WPA_AND_WPA2_ENTERPRISE) {
        connetSetting.m_type = WpaEap;
        if (enterpriseType == TLS) {
            m_info.tlsInfo.devIfaceName = m_deviceName;
            securityPage->updateTlsChange(m_info.tlsInfo);
            if (!m_name.isEmpty()) {
                qDebug() << "add new TLS connect";
                m_wirelessConnOpration->addTlsConnect(connetSetting, m_info.tlsInfo);
            } else {
                qDebug() << "addAndConnect TLS connect";
                m_wirelessConnOpration->addAndActiveWirelessEnterPriseTlsConnect(m_info.tlsInfo, connetSetting, m_deviceName, true);
            }
        } else if (enterpriseType == PEAP) {
            securityPage->updatePeapChange(m_info.peapInfo);
            if (!m_name.isEmpty()) {
                qDebug() << "add new PEAP connect";
                m_wirelessConnOpration->addPeapConnect(connetSetting, m_info.peapInfo);
            } else {
                qDebug() << "addAndConnect PEAP connect";
                m_wirelessConnOpration->addAndActiveWirelessEnterPrisePeapConnect(m_info.peapInfo, connetSetting, m_deviceName, true);
            }
        } else if (enterpriseType == TTLS) {
            securityPage->updateTtlsChange(m_info.ttlsInfo);
            if (!m_name.isEmpty()) {
                qDebug() << "add new TTLS connect";
                m_wirelessConnOpration->addTtlsConnect(connetSetting, m_info.ttlsInfo);
            } else {
                qDebug() << "addAndConnect TTLS connect";
                m_wirelessConnOpration->addAndActiveWirelessEnterPriseTtlsConnect(m_info.ttlsInfo, connetSetting, m_deviceName, true);
            }
        }
    } else {
        securityPage->updateSecurityChange(connetSetting);
        if (!m_name.isEmpty()) {
            qDebug() << "add new personal connect";
            m_wirelessConnOpration->addConnect(connetSetting);
        } else {
            qDebug() << "addAndConnect personal connect" << m_deviceName;
            m_wirelessConnOpration->addAndActiveWirelessConnect(m_deviceName, connetSetting, true);
        }
    }
    return true;
}

bool NetDetail::updateConnect()
{
    KyConnectResourse *kyConnectResourse = new KyConnectResourse(this);
    KyConnectSetting  connetSetting;
    KySecuType secuType;
    KyEapMethodType enterpriseType;
    kyConnectResourse->getConnectionSetting(m_uuid,connetSetting);

    bool securityChange = false;
    if (isWlan) {
        securityChange = securityPage->checkIsChanged(m_info);
        if(securityChange) {
            securityPage->getSecuType(secuType, enterpriseType);
            if (!checkWirelessSecurity(secuType)) {
                return false;
            }
        }
    }

    if(!m_uuid.isEmpty() && detailPage->checkIsChanged(m_info)) {
        m_wirelessConnOpration->setWirelessAutoConnect(m_uuid, !m_info.isAutoConnect);
    }

    bool ipv4Change = ipv4Page->checkIsChanged(m_info, connetSetting);
    bool ipv6Change = ipv6Page->checkIsChanged(m_info, connetSetting);

    qDebug() << "ipv4Changed" << ipv4Change << "ipv6Change" << ipv6Change;

    if (ipv4Change && connetSetting.m_ipv4ConfigIpType == CONFIG_IP_MANUAL) {
        if (checkIpv4Conflict(connetSetting.m_ipv4Address.at(0).ip().toString())) {
            qDebug() << "ipv4 conflict";
            showDesktopNotify(tr("ipv4 address conflict!"));
            return false;
        }
    }

    if (ipv6Change && connetSetting.m_ipv6ConfigIpType == CONFIG_IP_MANUAL) {
        if (checkIpv6Conflict(connetSetting.m_ipv6Address.at(0).ip().toString())) {
            qDebug() << "ipv6 conflict";
            showDesktopNotify(tr("ipv6 address conflict!"));
            return false;
        }
    }

    if (ipv4Change || ipv6Change) {
        connetSetting.dumpInfo();
        m_wiredConnOperation->updateWiredConnect(m_uuid, connetSetting);
    }

    qDebug() << "securityChange" << securityChange;
    if (securityChange) {
        if (secuType == WPA_AND_WPA2_ENTERPRISE) {
            updateWirelessEnterPriseConnect(enterpriseType);
        } else {
            updateWirelessPersonalConnect();
        }
    }

    if (ipv4Change || ipv6Change || securityChange) {
        if (isActive) {
            //信息变化 断开-重连 更新需要時間 不可以立即重連
//            sleep(1);
            QEventLoop eventloop;
            QTimer::singleShot(1000, &eventloop, SLOT(quit()));
            eventloop.exec();
            m_wirelessConnOpration->activateConnection(m_uuid, m_deviceName);
        }
    }
    return true;
}

bool NetDetail::checkWirelessSecurity(KySecuType secuType)
{
    if (secuType == WPA_AND_WPA2_ENTERPRISE) {
        if(m_info.strSecType.indexOf("802.1X") < 0) {
            showDesktopNotify(tr("this wifi no support enterprise type"));
            return false;
        }
    } else {
        if (secuType == NONE && m_info.strSecType != "None") {
            showDesktopNotify(tr("this wifi no support None type"));
            return false;
        } else if (secuType == WPA_AND_WPA2_PERSONAL
                   && (m_info.strSecType.indexOf("WPA1") < 0 &&
                       m_info.strSecType.indexOf("WPA2") < 0)) {
            showDesktopNotify(tr("this wifi no support WPA2 type"));
            return false;
        } else if (secuType == WPA3_PERSONAL && m_info.strSecType.indexOf("WPA3") < 0) {
            showDesktopNotify(tr("this wifi no support WPA3 type"));
            return false;
        }
    }
    return true;
}

bool NetDetail::eventFilter(QObject *w, QEvent *event)
{
   // 回车键触发确定按钮点击事件
   if (event->type() == QEvent::KeyPress) {
       QKeyEvent *mEvent = static_cast<QKeyEvent *>(event);
       if (mEvent->key() == Qt::Key_Enter || mEvent->key() == Qt::Key_Return) {
           if (confimBtn->isEnabled()) {
               emit confimBtn->clicked();
           }
           return true;
       }

   }
   return QDialog::eventFilter(w, event);
}
