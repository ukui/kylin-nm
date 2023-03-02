/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "netdetail.h"
//#include "../component/KylinArping/kylinipv4arping.h"
//#include "../component/KylinArping/kylinipv6arping.h"


#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

#include <QEvent>
#include <QMenu>
#include <QToolTip>
#include <QFontMetrics>

#include "windowmanager/windowmanager.h"

#define  WINDOW_WIDTH  520
#define  WINDOW_HEIGHT 602
#define  ICON_SIZE 22,22
#define  TITLE_LAYOUT_MARGINS 9,9,0,0
#define  CENTER_LAYOUT_MARGINS 24,0,0,0
#define  BOTTOM_LAYOUT_MARGINS 24,0,24,0
#define  BOTTOM_LAYOUT_SPACING 16
#define  DETAIL_PAGE_NUM 0
#define  IPV4_PAGE_NUM 1
#define  IPV6_PAGE_NUM 2
#define  SECURITY_PAGE_NUM 3
#define  CONFIG_PAGE_NUM 4
#define  CREATE_NET_PAGE_NUM 5
#define  PAGE_MIN_HEIGHT 40
#define  PAGE_WIDTH 472
#define  LAN_TAB_WIDTH 180
#define  WLAN_TAB_WIDTH 240
#define  SCRO_WIDTH 496
#define  SCRO_HEIGHT 600
#define  PEAP_SCRO_HEIGHT  300
#define  TLS_SCRO_HEIGHT  480
#define  MAX_TAB_TEXT_LENGTH 44
#define KYLIN_APP_MANAGER_NAME         "com.kylin.AppManager"
#define KYLIN_APP_MANAGER_PATH         "/com/kylin/AppManager"
#define KYLIN_APP_MANAGER_INTERFACE    "com.kylin.AppManager"

//extern void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed);

bool LaunchApp(QString desktopFile)
{
    QDBusInterface m_appManagerDbusInterface(KYLIN_APP_MANAGER_NAME,
                                             KYLIN_APP_MANAGER_PATH,
                                             KYLIN_APP_MANAGER_INTERFACE,
                                             QDBusConnection::sessionBus());//局部变量

    if (!m_appManagerDbusInterface.isValid()) {
        qWarning()<<"m_appManagerDbusInterface init error";
        return false;
    } else {
        QDBusReply<bool> reply =m_appManagerDbusInterface.call("LaunchApp",desktopFile);
        return reply;
    }
}

void NetDetail::showDesktopNotify(const QString &message, QString soundName)
{
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    QStringList actions;  //跳转动作
    actions.append("kylin-nm");
    actions.append("default");          //默认动作：点击消息体时打开麒麟录音
    QMap<QString, QVariant> hints;
    if (!soundName.isEmpty()) {
        hints.insert("sound-name", soundName); //添加声音
    }
    QList<QVariant> args;
    args<<(tr("Kylin NM"))
       <<((unsigned int) 0)
       <<QString("gnome-dev-ethernet")
       <<tr("kylin network desktop message") //显示的是什么类型的信息
       <<message //显示的具体信息
       <<actions
       <<hints
       <<(int)-1;
    iface.callWithArgumentList(QDBus::AutoDetect,"Notify",args);
}

void NetDetail::setNetdetailSomeEnable(bool on)
{
    detailPage->setEnabled(on);
    ipv4Page->setEnabled(on);
    ipv6Page->setEnabled(on);
    securityPage->setEnabled(on);
    createNetPage->setEnabled(on);
    cancelBtn->setEnabled(on);
    forgetBtn->setEnabled(on);
    confimBtn->setEnabled(on);
}

void NetDetail::startObjectThread()
{
    m_objectThread = new QThread();
    m_object = new ThreadObject(m_deviceName);
    m_object->moveToThread(m_objectThread);
    connect(m_objectThread, &QThread::finished, m_objectThread, &QObject::deleteLater);
    connect(m_objectThread, &QThread::finished, m_object, &QObject::deleteLater);
    connect(ipv4Page, &Ipv4Page::ipv4EditFinished, this, [=](){
        ipv4Page->startLoading();
    });
    connect(ipv6Page, &Ipv6Page::ipv6EditFinished, this, [=](){
        ipv6Page->startLoading();
    });

//    connect(ipv4Page, SIGNAL(ipv4EditFinished(const QString &)), m_object, SLOT(checkIpv4ConflictThread(const QString &)));
//    connect(ipv6Page, SIGNAL(ipv6EditFinished(const QString &)), m_object, SLOT(checkIpv6ConflictThread(const QString &)));
//    connect(this, SIGNAL(checkCurrentIpv4Conflict(const QString &)), m_object, SLOT(checkIpv4ConflictThread(const QString &)));
//    connect(this, SIGNAL(checkCurrentIpv6Conflict(const QString &)), m_object, SLOT(checkIpv6ConflictThread(const QString &)));

//    connect(m_object, &ThreadObject::ipv4IsConflict, this, [=](bool ipv4IsConf) {
//        ipv4Page->stopLoading();
//        ipv4Page->showIpv4AddressConflict(ipv4IsConf);
//    });
//    connect(m_object, &ThreadObject::ipv6IsConflict, this, [=](bool ipv6IsConf) {
//        ipv6Page->stopLoading();
//        ipv6Page->showIpv6AddressConflict(ipv6IsConf);
//    });

    m_objectThread->start();
}

NetDetail::NetDetail(QString interface, QString name, QString uuid, bool isActive, bool isWlan, bool isCreateNet, QWidget *parent)
    :m_deviceName(interface),
     m_name(name),
     m_uuid(uuid),
     isActive(isActive),
     isWlan(isWlan),
     m_isCreateNet(isCreateNet),
     QWidget(parent)
{
    //设置窗口无边框，阴影
//#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
//    MotifWmHints window_hints;
//    window_hints.flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
//    window_hints.functions = MWM_FUNC_ALL;
//    window_hints.decorations = MWM_DECOR_BORDER;
//    XAtomHelper::getInstance()->setWindowMotifHint(this->winId(), window_hints);
//#else
//    this->setWindowFlags(Qt::Dialog /*| Qt::FramelessWindowHint*/);
    this->setWindowFlag(Qt::Window);
//#endif
//    this->setProperty("useStyleWindowManager", false); //禁用拖动
//    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint );
//    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    centerToScreen();

    qRegisterMetaType<KyConnectSetting>("KyConnectSetting");
    qRegisterMetaType<KyWirelessConnectSetting>("KyWirelessConnectSetting");
    qRegisterMetaType<KyEapMethodTlsInfo>("KyEapMethodTlsInfo");
    qRegisterMetaType<KyEapMethodPeapInfo>("KyEapMethodPeapInfo");
    qRegisterMetaType<KyEapMethodTtlsInfo>("KyEapMethodTtlsInfo");
    qRegisterMetaType<KyEapMethodLeapInfo>("KyEapMethodLeapInfo");
    qRegisterMetaType<KyEapMethodPwdInfo>("KyEapMethodPwdInfo");
    qRegisterMetaType<KyEapMethodFastInfo>("KyEapMethodFastInfo");

    qDebug() << m_isCreateNet << name;
    if (!m_isCreateNet && name.isEmpty()) {
        m_isCreateNet = true;
    }
    qDebug() << m_isCreateNet;

    m_thread = new QThread;
    m_manager = new KyNetworkManager();
    m_manager->moveToThread(m_thread);
    connect(m_thread, &QThread::started, m_manager, &KyNetworkManager::kylinNetworkManagerInit);
    connect(m_thread, &QThread::finished, m_manager, &KyNetworkManager::deleteLater);
    m_thread->start();

    while (!m_manager->isInitFinished()) {
        ::usleep(1000);
    }

    initUI();
    loadPage();
    initComponent();
    getConInfo(m_info);
    startObjectThread();
    pagePadding(name,isWlan);
    connect(qApp, &QApplication::paletteChanged, this, &NetDetail::onPaletteChanged);

    isCreateOk = !(m_isCreateNet && !isWlan);
    isDetailOk = !(m_name.isEmpty());
    isIpv4Ok = true;
    isIpv6Ok = true;
    isSecuOk = false;
    if (!m_uuid.isEmpty() || (m_uuid.isEmpty() && m_info.secType == NONE)) {
        isSecuOk = true;
    }

    qDebug() << interface << name << uuid <<  "isWlan" << isWlan << "isCreateNet" <<m_isCreateNet;

    setConfirmEnable();
}

NetDetail::~NetDetail()
{
    if (m_objectThread->isRunning()) {
        m_objectThread->quit();
        m_objectThread->wait();
    }
}

void NetDetail::onPaletteChanged()
{
    QPalette pal = qApp->palette();

//    QGSettings * styleGsettings = nullptr;
//    const QByteArray style_id(THEME_SCHAME);
//    if (QGSettings::isSchemaInstalled(style_id)) {
//       styleGsettings = new QGSettings(style_id);
//       QString currentTheme = styleGsettings->get(COLOR_THEME).toString();
//       if(currentTheme == "ukui-default"){
//           pal = lightPalette(this);
//       }
//    }

    this->setPalette(pal);

    setFramePalette(detailPage, pal);
    setFramePalette(ipv4Page, pal);
    setFramePalette(ipv6Page, pal);
    setFramePalette(securityPage, pal);
    setFramePalette(createNetPage, pal);
    QToolTip::setPalette(pal);

    QPalette listwidget_pal(detailPage->m_listWidget->palette());
    listwidget_pal.setColor(QPalette::Base, this->palette().base().color());
    listwidget_pal.setColor(QPalette::AlternateBase, this->palette().alternateBase().color());
    detailPage->m_listWidget->setAlternatingRowColors(true);
    detailPage->m_listWidget->setPalette(listwidget_pal);

//    if (styleGsettings != nullptr) {
//        delete styleGsettings;
//        styleGsettings = nullptr;
//    }

    QColor colorTabBar = pal.color(QPalette::Disabled, QPalette::Highlight);
    m_netTabBar->setBackgroundColor(colorTabBar);
}

void NetDetail::currentRowChangeSlot(int row)
{
    if (isActive) {
        if (row < 3) {
            stackWidget->setCurrentIndex(row);
        } else {
            if(isWlan) {
                stackWidget->setCurrentIndex(row);
            } else {
                stackWidget->setCurrentIndex(CONFIG_PAGE_NUM);
            }
        }
    } else {
        stackWidget->setCurrentIndex(row);
    }
}

void NetDetail::paintEvent(QPaintEvent *event)
{
    QPalette pal = qApp->palette();
    QPainter painter(this);
    painter.setBrush(pal.color(QPalette::Base));
    painter.drawRect(this->rect());
    painter.fillRect(rect(), QBrush(pal.color(QPalette::Base)));

    return QWidget::paintEvent(event);
}

void NetDetail::closeEvent(QCloseEvent *event)
{
    Q_EMIT this->detailPageClose(m_deviceName, m_name, m_uuid);
    Q_EMIT this->createPageClose(m_deviceName);

    if (m_hasDetailPageShowed) {
        LaunchApp("ukui-control-center.desktop");
        m_hasDetailPageShowed = false;
    }
    return QWidget::closeEvent(event);
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
//    kdk::WindowManager::setGeometry(this->windowHandle(), QRect(desk_x / 2 - x / 2 + desk_rect.left(),
//                                                                desk_y / 2 - y / 2 + desk_rect.top(),
//                                                                this->width(),
//                                                                this->height()));
}

void NetDetail::initUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,9,0,24);
    mainLayout->setSpacing(22);

    this->installEventFilter(this);
    pageFrame = new QFrame(this);
    centerWidget = new QWidget(this);
    bottomWidget = new QWidget(this);

    detailPage = new DetailPage(isWlan, m_name.isEmpty(), this);
    ipv4Page = new Ipv4Page(this);
    ipv6Page = new Ipv6Page(this);
    securityPage = new SecurityPage(this);
    createNetPage = new CreatNetPage(this);
    configPage = new ConfigPage(this);

    detailPage->setFixedWidth(PAGE_WIDTH);
    ipv4Page->setFixedWidth(PAGE_WIDTH);
    ipv6Page->setFixedWidth(PAGE_WIDTH);
    securityPage->setFixedWidth(PAGE_WIDTH);
    createNetPage->setFixedWidth(PAGE_WIDTH);
    configPage->setFixedWidth(PAGE_WIDTH);


    // 滚动区域
    m_secuPageScrollArea = new QScrollArea(centerWidget);
    m_secuPageScrollArea->setFixedWidth(SCRO_WIDTH);
    m_secuPageScrollArea->setFrameShape(QFrame::NoFrame);
    m_secuPageScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_secuPageScrollArea->setWidget(securityPage);
    m_secuPageScrollArea->setWidgetResizable(true);

    m_ipv4ScrollArea = new QScrollArea(centerWidget);
    m_ipv4ScrollArea->setFixedWidth(SCRO_WIDTH);
    m_ipv4ScrollArea->setFrameShape(QFrame::NoFrame);
    m_ipv4ScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ipv4ScrollArea->setWidget(ipv4Page);
    m_ipv4ScrollArea->setWidgetResizable(true);

    m_ipv6ScrollArea = new QScrollArea(centerWidget);
    m_ipv6ScrollArea->setFixedWidth(SCRO_WIDTH);
    m_ipv6ScrollArea->setFrameShape(QFrame::NoFrame);
    m_ipv6ScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ipv6ScrollArea->setWidget(ipv6Page);
    m_ipv6ScrollArea->setWidgetResizable(true);

    QPalette pal = m_secuPageScrollArea->palette();
    pal.setBrush(QPalette::Window, Qt::transparent);
    m_secuPageScrollArea->setPalette(pal);
    m_ipv4ScrollArea->setPalette(pal);
    m_ipv6ScrollArea->setPalette(pal);

    stackWidget = new QStackedWidget(centerWidget);
    stackWidget->addWidget(detailPage);
    stackWidget->addWidget(m_ipv4ScrollArea);
    stackWidget->addWidget(m_ipv6ScrollArea);
    stackWidget->addWidget(m_secuPageScrollArea);
    stackWidget->addWidget(configPage);
    stackWidget->addWidget(createNetPage);

    m_networkMode = NetworkModeType(getNetworkModeConfig(m_uuid));
    // TabBar
    m_netTabBar = new NetTabBar(this);
    m_netTabBar->addTab(tr("Detail")); //详情
    m_netTabBar->addTab(tr("IPv4"));//Ipv4
    m_netTabBar->addTab(tr("IPv6"));//Ipv6
    if (isWlan) {
        m_netTabBar->addTab(tr("Security"));//安全
        if (isActive && m_networkMode != DBUS_INVAILD && m_networkMode != NO_CONFIG) {
            m_netTabBar->addTab(tr("Config")); //配置
            m_netTabBar->setFixedWidth(WLAN_TAB_WIDTH + TAB_WIDTH);
        } else {
            m_netTabBar->setFixedWidth(WLAN_TAB_WIDTH);
        }
    } else {
        if (isActive && m_networkMode != DBUS_INVAILD && m_networkMode != NO_CONFIG) {
            m_netTabBar->addTab(tr("Config")); //配置
            m_netTabBar->setFixedWidth(LAN_TAB_WIDTH + TAB_WIDTH);
        } else {
            m_netTabBar->setFixedWidth(LAN_TAB_WIDTH);
        }
    }

    // TabBar关联选项卡页面
    connect(m_netTabBar, SIGNAL(currentChanged(int)), this, SLOT(currentRowChangeSlot(int)));
    setNetTabToolTip();

    confimBtn = new QPushButton(this);
    confimBtn->setText(tr("Confirm"));
    cancelBtn = new QPushButton(this);
    cancelBtn->setText(tr("Cancel"));
    forgetBtn = new QPushButton(this);

    QHBoxLayout *pageLayout = new QHBoxLayout(pageFrame);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(m_netTabBar, Qt::AlignCenter);

    QVBoxLayout *centerlayout = new QVBoxLayout(centerWidget);
    centerlayout->setContentsMargins(CENTER_LAYOUT_MARGINS); // 右边距为0，为安全页滚动区域留出空间
    centerlayout->addWidget(stackWidget);

//    centerWidget->setStyleSheet("QWidget{border:1px solid rgba(255,0,0,1);}");//测试用， 画出边界线

    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(BOTTOM_LAYOUT_MARGINS);
    bottomLayout->setSpacing(BOTTOM_LAYOUT_SPACING);
    bottomLayout->addWidget(forgetBtn);
    bottomLayout->addStretch();
    bottomLayout->addWidget(cancelBtn);
    bottomLayout->addWidget(confimBtn);
    bottomWidget->setMinimumHeight(PAGE_MIN_HEIGHT);

    mainLayout->addWidget(pageFrame);
    mainLayout->addWidget(centerWidget);
    mainLayout->addWidget(bottomWidget);

    this->setAutoFillBackground(true);
    onPaletteChanged();
}

void NetDetail::loadPage()
{
    //判断是否创建网络页面
    if (m_isCreateNet && !isWlan) {
        pageFrame->hide();
        stackWidget->setCurrentIndex(CREATE_NET_PAGE_NUM);
        this->setWindowTitle(tr("Add LAN Connect"));
        setDetailPageShowed(true);
    } else {
        stackWidget->setCurrentIndex(DETAIL_PAGE_NUM);
        this->setWindowTitle(m_name);
        if (isWlan && m_name.isEmpty()) {
            this->setWindowTitle(tr("Connect Hidden Wlan"));
        }
    }
}

void NetDetail::initComponent()
{
    connect(cancelBtn, &QPushButton::clicked, this, [=] {
        close();
    });

    connect(confimBtn, SIGNAL(clicked()), this, SLOT(onBtnConfirmClicked()));
    if (!m_uuid.isEmpty()) {
        if (isWlan) {
            forgetBtn->setText(tr("Forget this network"));
        } else {
            forgetBtn->setText(tr("Delete this network"));
        }
        forgetBtn->show();
        connect(forgetBtn, SIGNAL(clicked()), this, SLOT(onBtnForgetClicked()));
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

    const QByteArray id(THEME_SCHAME);
    if(QGSettings::isSchemaInstalled(id)){
        QGSettings * fontSetting = new QGSettings(id, QByteArray(), this);
        connect(fontSetting, &QGSettings::changed,[=](QString key) {
            if ("systemFont" == key || "systemFontSize" ==key) {
                setNetTabToolTip();
            }
        });
    }

    connect(this, &NetDetail::deleteConnect, m_manager, &KyNetworkManager::onDeleteConnect);
    connect(this, &NetDetail::sigCreateWiredConnect, m_manager, &KyNetworkManager::onCreateWiredConnect);
    connect(this, &NetDetail::sigUpdateIpv4AndIpv6SettingInfo, m_manager, &KyNetworkManager::onUpdateIpv4AndIpv6SettingInfo);
    connect(this, &NetDetail::sigUpdateWirelessPersonalConnect, m_manager, &KyNetworkManager::onUpdateWirelessPersonalConnect);
    connect(this, &NetDetail::sigUpdateWirelessEnterPriseTlsConnect, m_manager, &KyNetworkManager::onUpdateWirelessEnterPriseTlsConnect);
    connect(this, &NetDetail::sigUpdateWirelessEnterPrisePeapConnect, m_manager, &KyNetworkManager::onUpdateWirelessEnterPrisePeapConnect);
    connect(this, &NetDetail::sigUpdateWirelessEnterPriseTtlsConnect, m_manager, &KyNetworkManager::onUpdateWirelessEnterPriseTtlsConnect);
    connect(this, &NetDetail::sigUpdateWirelessEnterPriseLeapConnect, m_manager, &KyNetworkManager::onUpdateWirelessEnterPriseLeapConnect);
    connect(this, &NetDetail::sigUpdateWirelessEnterPrisePwdConnect, m_manager, &KyNetworkManager::onUpdateWirelessEnterPrisePwdConnect);
    connect(this, &NetDetail::sigUpdateWirelessEnterPriseFastConnect, m_manager, &KyNetworkManager::onUpdateWirelessEnterPriseFastConnect);
    connect(this, &NetDetail::sigActivateConnection, m_manager, &KyNetworkManager::onActivateConnection);
    connect(this, &NetDetail::sigWirelessAutoConnectStateChanged, m_manager, &KyNetworkManager::onUpdateWirelessAutoConnectState);
}

//获取网路详情信息
void NetDetail::getConInfo(ConInfo &conInfo)
{
    if (m_isCreateNet && !isWlan) {
        return;
    }

    getBaseInfo(m_detailInfo, conInfo);
    getDynamicIpInfo(m_connectSetting, conInfo);
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
    ipv4Page->setIpv4Config(m_info.ipv4ConfigType);
    ipv4Page->setMulDns(m_info.ipv4DnsList);
    if (m_info.ipv4ConfigType == CONFIG_IP_MANUAL) {
        Q_EMIT checkCurrentIpv4Conflict(m_info.strIPV4Address);
        ipv4Page->setIpv4(m_info.strIPV4Address);
        ipv4Page->setNetMask(m_info.strIPV4NetMask);
        ipv4Page->setGateWay(m_info.strIPV4GateWay);
    }
    //ipv6页面填充
    ipv6Page->setIpv6Config(m_info.ipv6ConfigType);
    ipv6Page->setMulDns(m_info.ipv6DnsList);
    if (m_info.ipv6ConfigType == CONFIG_IP_MANUAL) {
        Q_EMIT checkCurrentIpv6Conflict(m_info.strIPV6Address);
        ipv6Page->setIpv6(m_info.strIPV6Address);
        ipv6Page->setIpv6Perfix(m_info.iIPV6Prefix);
        ipv6Page->setGateWay(m_info.strIPV6GateWay);
    }
    //安全页面
    if (isWlan) {
        securityPage->setSecurity(m_info.secType);
        switch (m_info.secType) {
        case UNKNOWN:
        case NONE:
            break;
        case WPA_AND_WPA2_PERSONAL:
        case WPA3_PERSONAL:
            securityPage->setPsk(m_info.strPassword);
            break;
        case WPA_AND_WPA2_ENTERPRISE:
            if (m_info.enterpriseType == TLS) {
                securityPage->setTlsInfo(m_info.tlsInfo);
            } else if (m_info.enterpriseType == PEAP) {
                securityPage->setPeapInfo(m_info.peapInfo);
            } else if (m_info.enterpriseType == TTLS) {
                securityPage->setTtlsInfo(m_info.ttlsInfo);
            } else if (m_info.enterpriseType == LEAP) {
                securityPage->setLeapInfo(m_info.leapInfo);
            } else if (m_info.enterpriseType == PWD) {
                securityPage->setPwdInfo(m_info.pwdInfo);
            } else if (m_info.enterpriseType == FAST) {
                securityPage->setFastInfo(m_info.fastInfo);
            }
            break;
        default:
            break;
        }
    }

    //配置页面
    if (isActive && m_networkMode != DBUS_INVAILD && m_networkMode != NO_CONFIG) {
        configPage->setConfigState(m_networkMode);
    }

}

//点击了保存更改网络设置的按钮
void NetDetail::onBtnConfirmClicked()
{
    qDebug() << "on_btnConfirm_clicked";
    setNetdetailSomeEnable(false);
    if (m_isCreateNet) {
        if (!isWlan) {
            //新建有线连接
            qDebug() << "Confirm create wired connect";
            if (!createWiredConnect()) {
                setNetdetailSomeEnable(true);
                return;
            }
        }
    } else {
        //更新连接
        qDebug() << "Confirm update connect";
        if (!updateConnect()) {
            setNetdetailSomeEnable(true);
            return;
        }
    }
    close();
}

//点击忘记网络
void NetDetail::onBtnForgetClicked()
{
    qDebug() << "user choose forget connection uuid = " << m_uuid;
    Q_EMIT deleteConnect(m_uuid);
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
//    qDebug() << "setConfirmEnable "<< isConfirmBtnEnable;
    confimBtn->setEnabled(isConfirmBtnEnable);
}

void NetDetail::getBaseInfo(KyDetailInfo &detailInfo, ConInfo &conInfo)
{
    if (isWlan) {
        conInfo.strConType =  "802-11-wireless";
        m_manager->getWirelessConnectDetail(m_deviceName, m_name, m_uuid, detailInfo);
        conInfo.strSecType = detailInfo.strSecType;

        //未激活WLAN的m_uuid为空，不能通过m_uuid获取安全类型
        if (!isActive) {
            if (conInfo.strSecType.isEmpty()) {
                conInfo.secType = NONE;
            } else if (conInfo.strSecType.indexOf("802.1X") != -1) {
                conInfo.secType = WPA_AND_WPA2_ENTERPRISE;
            } else {
                conInfo.secType = WPA_AND_WPA2_PERSONAL;
            }
        } else {
            conInfo.secType = m_manager->getConnectSecuType(m_uuid);
        }

        switch (conInfo.secType) {
        case UNKNOWN:
        case NONE:
            break;
        case WPA_AND_WPA2_PERSONAL:
        case WPA3_PERSONAL:
            m_manager->getNormalWifiConnectSecuInfo(m_uuid, m_pwdInfo);
            conInfo.strPassword = m_pwdInfo.pwd;
            break;
        case WPA_AND_WPA2_ENTERPRISE:
            if (!m_manager->getEnterpiseEapMethod(m_uuid, conInfo.enterpriseType)) {
                qDebug() << m_name << "not enterprise wifi";
            } else if (conInfo.enterpriseType == TLS) {
                m_manager->getEnterPriseInfoTls(m_uuid, conInfo.tlsInfo);;
            } else if (conInfo.enterpriseType == PEAP) {
                m_manager->getEnterPriseInfoPeap(m_uuid, conInfo.peapInfo);;
            } else if (conInfo.enterpriseType == TTLS) {
                m_manager->getEnterPriseInfoTtls(m_uuid, conInfo.ttlsInfo);;
            } else if (conInfo.enterpriseType == LEAP) {
                m_manager->getEnterPriseInfoLeap(m_uuid, conInfo.leapInfo);;
            } else if (conInfo.enterpriseType == PWD) {
                m_manager->getEnterPriseInfoPwd(m_uuid, conInfo.pwdInfo);;
            } else if (conInfo.enterpriseType == FAST) {
                m_manager->getEnterPriseInfoFast(m_uuid, conInfo.fastInfo);;
            }
            break;
        default:
            break;
        }

    } else {
        conInfo.strConType =  "802-3-ethernet";
        m_manager->getWiredConnectDetail(m_deviceName, m_uuid, detailInfo);
    }

    // 将网络详情信息和IP信息汇总到conInfo
    conInfo.strChan = detailInfo.strChan;
    conInfo.strMac = detailInfo.strMac;
    conInfo.strHz = detailInfo.strHz;
    conInfo.strBandWidth = detailInfo.strBandWidth;
    conInfo.strDynamicIpv4 = detailInfo.strDynamicIpv4;
    conInfo.strDynamicIpv6 = detailInfo.strDynamicIpv6;
    conInfo.strDynamicIpv4Dns = detailInfo.strDynamicIpv4Dns;
}

void NetDetail::getDynamicIpInfo(KyConnectSetting &connectSetting, ConInfo &conInfo)
{
    m_manager->getConnectIpInfo(m_uuid, connectSetting);

    // 将网络详情信息和IP信息汇总到conInfo
    conInfo.isAutoConnect = connectSetting.m_isAutoConnect;
    conInfo.ipv4ConfigType = connectSetting.m_ipv4ConfigIpType;
    conInfo.ipv4DnsList = connectSetting.m_ipv4Dns;
    if (connectSetting.m_ipv4ConfigIpType == CONFIG_IP_MANUAL) {
        if (connectSetting.m_ipv4Address.size() > 0) {
    conInfo.strIPV4Address = connectSetting.m_ipv4Address.at(0).ip().toString();
    conInfo.strIPV4NetMask = connectSetting.m_ipv4Address.at(0).netmask().toString();
    conInfo.strIPV4GateWay = connectSetting.m_ipv4Address.at(0).gateway().toString();
        }
    }
    conInfo.ipv6ConfigType = connectSetting.m_ipv6ConfigIpType;
    conInfo.ipv6DnsList = connectSetting.m_ipv6Dns;
    if (connectSetting.m_ipv6ConfigIpType == CONFIG_IP_MANUAL) {
        if (connectSetting.m_ipv6Address.size() > 0) {
    conInfo.strIPV6Address = connectSetting.m_ipv6Address.at(0).ip().toString();
    conInfo.iIPV6Prefix = ipv6Page->getPerfixLength(connectSetting.m_ipv6Address.at(0).netmask().toString());
    conInfo.strIPV6GateWay = connectSetting.m_ipv6Address.at(0).gateway().toString();
        }
    }

    QString dnsList;
    dnsList.clear();
    if (!conInfo.ipv4DnsList.isEmpty()) {
        for (QHostAddress str: conInfo.ipv4DnsList) {
            dnsList.append(str.toString());
            dnsList.append("; ");
        }
        dnsList.chop(2);
        conInfo.strDynamicIpv4Dns = dnsList;
    }

    if (!isActive) {
        conInfo.strDynamicIpv4 = conInfo.strIPV4Address.isEmpty() ? tr("Auto") : conInfo.strIPV4Address;
        conInfo.strDynamicIpv6 = conInfo.strIPV6Address.isEmpty() ? tr("Auto") : conInfo.strIPV6Address;
        conInfo.strDynamicIpv4Dns = conInfo.strDynamicIpv4Dns.isEmpty() ? tr("Auto") : conInfo.strDynamicIpv4Dns;
    }
}

bool NetDetail::createWiredConnect()
{
    KyConnectSetting connetSetting;
    connetSetting.setIfaceName(m_deviceName);
    createNetPage->constructIpv4Info(connetSetting);
    Q_EMIT sigCreateWiredConnect(connetSetting);
    return true;
}

bool NetDetail::updateConnect()
{
    KyNetResource *kyConnectResourse = new KyNetResource(this);
    KyConnectSetting  connetSetting;
    KySecuType secuType;
    KyEapMethodType enterpriseType;
    kyConnectResourse->getConnectionSetting(m_uuid, connetSetting);

    //属性页 page1 AutoConnect
    if(!m_uuid.isEmpty() && detailPage->checkIsChanged(m_info)) {
        Q_EMIT sigWirelessAutoConnectStateChanged(m_uuid, !m_info.isAutoConnect);
    }

    //属性页 page2 page3 Ipv4/Ipv6
    bool ipv4Change = ipv4Page->checkIsChanged(m_info, connetSetting);
    bool ipv6Change = ipv6Page->checkIsChanged(m_info, connetSetting);

    if (ipv4Change || ipv6Change) {
        connetSetting.dumpInfo();
        Q_EMIT sigUpdateIpv4AndIpv6SettingInfo(m_uuid, connetSetting);
    }

    //属性页 page4 wifi Security
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
            Q_EMIT sigActivateConnection(m_uuid, m_deviceName);
        }
    }

    //属性页 page5 config 网络模式配置
    if (m_networkMode != DBUS_INVAILD) {
        int configType = getNetworkModeConfig(m_uuid);
        bool configPageChange = configPage->checkIsChanged(configType);
        int currentConfigType = configPage->getConfigState();
//        qDebug () << Q_FUNC_INFO << __LINE__<< configPageChange;

        if (configPageChange) {
            setNetworkModeConfig(m_uuid, m_deviceName, m_name, currentConfigType);
//            qDebug () <<Q_FUNC_INFO << __LINE__ << m_uuid << m_deviceName << m_name << currentConfigType;
        }
    }

    return true;
}

bool NetDetail::checkWirelessSecurity(KySecuType secuType)
{
    if (secuType == WPA_AND_WPA2_ENTERPRISE) {
        if(m_info.strSecType.indexOf("802.1X") < 0) {
            showDesktopNotify(tr("this wifi no support enterprise type"), "networkwrong");
            return false;
        }
    } else {
        if (secuType == NONE && m_info.strSecType != tr("None")) {
            showDesktopNotify(tr("this wifi no support None type"), "networkwrong");
            return false;
        } else if (secuType == WPA_AND_WPA2_PERSONAL
                   && (m_info.strSecType.indexOf("WPA1") < 0 &&
                       m_info.strSecType.indexOf("WPA2") < 0)) {
            showDesktopNotify(tr("this wifi no support WPA2 type"), "networkwrong");
            return false;
        } else if (secuType == WPA3_PERSONAL && m_info.strSecType.indexOf("WPA3") < 0) {
            showDesktopNotify(tr("this wifi no support WPA3 type"), "networkwrong");
            return false;
        }
    }
    return true;
}

void NetDetail::updateWirelessPersonalConnect()
{
    KyWirelessConnectSetting setting;
    securityPage->updateSecurityChange(setting);
    bool isPwdChanged = !(m_info.strPassword == setting.m_psk);
    Q_EMIT sigUpdateWirelessPersonalConnect(m_uuid, setting, isPwdChanged);
}

void NetDetail::updateWirelessEnterPriseConnect(KyEapMethodType enterpriseType)
{
    if (enterpriseType == TLS) {
        m_info.tlsInfo.devIfaceName = m_deviceName;
        securityPage->updateTlsChange(m_info.tlsInfo);
        Q_EMIT sigUpdateWirelessEnterPriseTlsConnect(m_uuid, m_info.tlsInfo);
    } else if (enterpriseType == PEAP) {
        securityPage->updatePeapChange(m_info.peapInfo);
        Q_EMIT sigUpdateWirelessEnterPrisePeapConnect(m_uuid, m_info.peapInfo);
    } else if (enterpriseType == TTLS) {
        securityPage->updateTtlsChange(m_info.ttlsInfo);
        Q_EMIT sigUpdateWirelessEnterPriseTtlsConnect(m_uuid, m_info.ttlsInfo);
    } else if (enterpriseType == LEAP) {
        securityPage->updateLeapChange(m_info.leapInfo);
        Q_EMIT sigUpdateWirelessEnterPriseLeapConnect(m_uuid, m_info.leapInfo);
    } else if (enterpriseType == PWD) {
        securityPage->updatePwdChange(m_info.pwdInfo);
        Q_EMIT sigUpdateWirelessEnterPrisePwdConnect(m_uuid, m_info.pwdInfo);
    } else if (enterpriseType == FAST) {
        securityPage->updateFastChange(m_info.fastInfo);
        Q_EMIT sigUpdateWirelessEnterPriseFastConnect(m_uuid, m_info.fastInfo);
    }
}

bool NetDetail::eventFilter(QObject *w, QEvent *event)
{
   // 回车键触发确定按钮点击事件
   if (event->type() == QEvent::KeyPress) {
       QKeyEvent *mEvent = static_cast<QKeyEvent *>(event);
       if (mEvent->key() == Qt::Key_Enter || mEvent->key() == Qt::Key_Return) {
           if (confimBtn->isEnabled()) {
               Q_EMIT confimBtn->clicked();
           }
           return true;
       } else if (mEvent->key() == Qt::Key_Escape) {
            close();
            return true;
       }
   }
   return QWidget::eventFilter(w, event);
}

void NetDetail::setDetailPageShowed(bool state)
{
    m_hasDetailPageShowed = state;
}

void NetDetail::setNetTabToolTip()
{
    int tabCount = m_netTabBar->count();
    for (int i = 0; i< tabCount; ++i) {
        QFontMetrics fontMetrics(m_netTabBar->font());
        int fontSize = fontMetrics.width(m_netTabBar->tabText(i));
        if (fontSize > MAX_TAB_TEXT_LENGTH) {
            m_netTabBar->setTabToolTip(i, m_netTabBar->tabText(i));
        } else {
            m_netTabBar->setTabToolTip(i, "");
        }
    }
}

int NetDetail::getNetworkModeConfig(QString uuid)
{
    if (uuid.isEmpty()) {
           qWarning()<< "[NetDetail] uuid is empty, so can not get network mode config";
           return NO_CONFIG;
       }

    QDBusInterface firewallIface("com.ksc.defender",
                       "/firewall",
                       "com.ksc.defender.firewall",
                       QDBusConnection::systemBus());

    if(!firewallIface.isValid()) {
        qWarning ()<< "[NetDetail] com.ksc.defender.firewall dbus is invalid";
        return DBUS_INVAILD;
    }

    QDBusReply<int> reply = firewallIface.call("get_networkModeConfig", uuid);
    if (reply.isValid()) {
        return reply.value();
    } else {
        qWarning() << "[NetDetail] call get_networkModeConfig failed" << reply.error().message();
    }
    return NO_CONFIG;
}

void NetDetail::setNetworkModeConfig(QString uuid, QString cardName, QString ssid, int mode)
{
    QDBusInterface firewallIface("com.ksc.defender",
                       "/firewall",
                       "com.ksc.defender.firewall",
                       QDBusConnection::systemBus());

    if(!firewallIface.isValid()) {
        qWarning () << "[NetDetail] com.ksc.defender.firewall dbus is invalid";
        return;
    }

    QDBusReply<int> reply = firewallIface.call("set_networkModeConfig", uuid, cardName, ssid, mode);
    if (reply.isValid()) {
        qDebug() << "[NetDetail] set_networkModeConfig" << ssid << uuid << cardName << mode << ",result" << reply.value();
    } else {
        qWarning() << "[NetDetail] call set_networkModeConfig" << reply.error().message();
    }
}

NetTabBar::NetTabBar(QWidget *parent)
    :KTabBar(KTabBarStyle::SegmentDark, parent)
{
    //模式切换
    QDBusConnection::sessionBus().connect(QString("com.kylin.statusmanager.interface"),
                                         QString("/"),
                                         QString("com.kylin.statusmanager.interface"),
                                          QString("mode_change_signal"), this, SLOT(onModeChanged(bool)));
    //模式获取
    QDBusInterface interface(QString("com.kylin.statusmanager.interface"),
                             QString("/"),
                             QString("com.kylin.statusmanager.interface"),
                             QDBusConnection::sessionBus());
    if(!interface.isValid()) {
        this->setFixedHeight(TAB_HEIGHT);
        return;
    }
    QDBusReply<bool> reply = interface.call("get_current_tabletmode");
    if (!reply.isValid()) {
        this->setFixedHeight(TAB_HEIGHT);
        return;
    }
    onModeChanged(reply.value());
}

NetTabBar::~NetTabBar()
{

}

QSize NetTabBar::sizeHint() const
{
    QSize size = KTabBar::sizeHint();
    size.setWidth(TAB_WIDTH);
    return size;
}

QSize NetTabBar::minimumTabSizeHint(int index) const
{
    Q_UNUSED(index)
    QSize size = KTabBar::minimumTabSizeHint(index);
    size.setWidth(TAB_WIDTH);
    return size;
}

void NetTabBar::onModeChanged(bool mode)
{
    if (mode) {
        this->setFixedHeight(TAB_HEIGHT_TABLET); // 平板模式
    } else {
        this->setFixedHeight(TAB_HEIGHT); // PC模式
    }
}

ThreadObject::ThreadObject(QString deviceName, QObject *parent)
    :m_devName(deviceName), QObject(parent)
{
    m_isStop = false;
}

ThreadObject::~ThreadObject()
{

}

void ThreadObject::stop()
{
     m_isStop = true;
}

//void ThreadObject::checkIpv4ConflictThread(const QString &ipv4Address)
//{
//    if (m_isStop) {
//        return;
//    }
//    bool isConflict = false;
//    KyIpv4Arping* ipv4Arping = new KyIpv4Arping(m_devName, ipv4Address);
//    if (ipv4Arping->ipv4ConflictCheck() >= 0) {
//        isConflict =  ipv4Arping->ipv4IsConflict();
//    } else {
//        qWarning() << "checkIpv4Conflict internal error";
//    }

//    delete ipv4Arping;
//    ipv4Arping = nullptr;
//    Q_EMIT ipv4IsConflict(isConflict);
//}

//void ThreadObject::checkIpv6ConflictThread(const QString &ipv6Address)
//{
//    if (m_isStop) {
//        return;
//    }
//    bool isConflict = false;
//    KyIpv6Arping* ipv6rping = new KyIpv6Arping(m_devName, ipv6Address);

//    if (ipv6rping->ipv6ConflictCheck() >= 0) {
//        isConflict =  ipv6rping->ipv6IsConflict();
//    } else {
//        qWarning() << "checkIpv6Conflict internal error";
//    }

//    delete ipv6rping;
//    ipv6rping = nullptr;
//    Q_EMIT ipv6IsConflict(isConflict);
//}
