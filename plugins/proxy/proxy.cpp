/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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
#include "proxy.h"
#include "aptproxydialog.h"
#include "aptinfo.h"
#include <kysdk/applications/accessinfohelper.h>
#include "klineframe.h"
#include "uisecurityconfig.h"
//#include "utils.h"

#include <QDebug>
#include <QRegExpValidator>
#include <QApplication>
#include <QTranslator>
#include <QProcess>
#include <QGSettings/QGSettings>
#include <QPalette>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>

#define PROXY_SCHEMA              "org.gnome.system.proxy"
#define PROXY_MODE_KEY            "mode"
#define PROXY_AUTOCONFIG_URL_KEY  "autoconfigUrl"
#define IGNORE_HOSTS_KEY          "ignoreHosts"

#define HTTP_PROXY_SCHEMA         "org.gnome.system.proxy.http"
#define HTTP_USE_AUTH_KEY         "use-authentication"
#define HTTP_AUTH_USER_KEY        "authentication-user"
#define HTTP_AUTH_PASSWD_KEY      "authentication-password"

#define HTTPS_PROXY_SCHEMA        "org.gnome.system.proxy.https"

#define FTP_PROXY_SCHEMA          "org.gnome.system.proxy.ftp"

#define SOCKS_PROXY_SCHEMA        "org.gnome.system.proxy.socks"

#define PROXY_HOST_KEY       "host"
#define PROXY_PORT_KEY       "port"
#define THEME_SCHAME         "org.ukui.style"

#define SYS_PROXY_NONE_MODE      "none"
#define SYS_PROXY_AUTO_MODE      "auto"
#define SYS_PROXY_MANUAL_MODE    "manual"

#define FRAME_LAYOUT_MARGINS  16,0,16,0
#define FRAME_LAYOUT_SPACING  8
#define LABEL_WIDTH  136
#define LINE_EDIT_HEIGHT  36

Proxy::Proxy() : mFirstLoad(true)
{
    qDBusRegisterMetaType<QMap<QString, QStringList>>();
    qDBusRegisterMetaType<QStringList>();

    QTranslator* translator = new QTranslator(this);
    translator->load("/usr/share/kylin-nm/proxy/" + QLocale::system().name());
    QApplication::installTranslator(translator);

    pluginName = tr("Proxy");
    pluginType = NETWORK;
}

Proxy::~Proxy()
{
    if (!mFirstLoad) {
        plugin_leave();
        delete m_appProxyDbus;
    }
}

QString Proxy::plugini18nName() {
    return pluginName;
}

int Proxy::pluginTypes() {
    return pluginType;
}

QWidget *Proxy::pluginUi() {
    if (mFirstLoad) {
        mFirstLoad = false;
        pluginWidget = new QWidget;
        pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
        initUi(pluginWidget);
        retranslateUi();

        mfileWatch_1 = new QFileSystemWatcher(this);
        mfileWatch_2 = new QFileSystemWatcher(this);

        QString dir_1("/etc/apt/apt.conf.d");
        QString dir_2("/etc/profile.d");
        mfileWatch_1->addPath(dir_1);
        mfileWatch_2->addPath(dir_2);

        const QByteArray id(PROXY_SCHEMA);
        const QByteArray idd(HTTP_PROXY_SCHEMA);
        const QByteArray iddd(HTTPS_PROXY_SCHEMA);
        const QByteArray iid(FTP_PROXY_SCHEMA);
        const QByteArray iiid(SOCKS_PROXY_SCHEMA);
         const QByteArray iVd(APT_PROXY_SCHEMA);

        initSearchText();
        setupComponent();

        if (QGSettings::isSchemaInstalled(id) && QGSettings::isSchemaInstalled(idd) &&
                QGSettings::isSchemaInstalled(iddd) && QGSettings::isSchemaInstalled(iid) &&
                QGSettings::isSchemaInstalled(iiid) && QGSettings::isSchemaInstalled(iVd)){

            isExistSettings = true;
            proxysettings = new QGSettings(id,QByteArray(),this);
            httpsettings = new QGSettings(idd,QByteArray(),this);
            securesettings = new QGSettings(iddd,QByteArray(),this);
            ftpsettings = new QGSettings(iid,QByteArray(),this);
            sockssettings = new QGSettings(iiid,QByteArray(),this);
            setupConnect();
            initProxyModeStatus();
            initAutoProxyStatus();
            initManualProxyStatus();
            initIgnoreHostStatus();
            if (isKylinProxyProcessRunning()) {
                /* 只有 kylin-proxy 进程存在时才初始化应用代理 */
                initDbus();
                initAppProxyStatus();
            }
        } else {
            qCritical() << "Xml needed by Proxy is not installed";
        }
        setUkccProxySettings();
    }
    return pluginWidget;
}

void Proxy::plugin_leave()
{
//当用户手动代理设置参数有问题导致不生效时，此处不再强行将设置为自动，为用户行为
#if 0
    if (isExistSettings) {
        if (proxysettings->get(PROXY_MODE_KEY) == "manual") {
            if (!isManualProxyEnable()) {
                proxysettings->set(PROXY_MODE_KEY,"auto");
                mManualBtn->setChecked(false);
                mAutoBtn->setChecked(true);
                _setSensitivity();
            }
        }
    }
#endif
}

const QString Proxy::name() const {

    return QStringLiteral("Proxy");
}

bool Proxy::isShowOnHomePage() const
{
    return true;
}

QIcon Proxy::icon() const
{
    return QIcon::fromTheme("ukui-network-agent-symbolic");
}

QString Proxy::translationPath() const
{
    return "/usr/share/kylin-nm/proxy/%1.ts";
}

bool Proxy::isEnable() const
{
    return true;
}

void Proxy::initUi(QWidget *widget)
{
    QVBoxLayout *mverticalLayout = new QVBoxLayout(widget);
    mverticalLayout->setSpacing(8);
    mverticalLayout->setContentsMargins(0, 0, 0, 0);

    mProxyBtnGroup = new QButtonGroup(this);
    mProxyBtnGroup->setExclusive (true); // 互斥

    mTitleLabel = new KLabel(widget);
    mTitleLabel->setContentsMargins(16, 0, 0, 0);

    mProxyFrame = new QFrame(widget);
    mProxyFrame->setMinimumSize(QSize(550, 0));
    mProxyFrame->setMaximumSize(QSize(16777215, 16777215));
    mProxyFrame->setFrameShape(QFrame::Box);
    QVBoxLayout *Lyt = new QVBoxLayout(mProxyFrame);
    Lyt->setContentsMargins(0, 0, 0, 0);
    Lyt->setSpacing(0);

    mEnableFrame = new QFrame(mProxyFrame);
    setFrame_Noframe(mEnableFrame);

    QHBoxLayout *enableLyt = new QHBoxLayout(mEnableFrame);
    enableLyt->setContentsMargins(16, 0, 16, 0);
    QLabel *enableLabel = new QLabel(tr("Start using"), mEnableFrame);
    mEnableBtn = new KSwitchButton(mEnableFrame);
    KDK_EXTEND_ALL_INFO_FORMAT(mEnableBtn, "Proxy", "", "enable switch of system proxy");
    enableLyt->addWidget(enableLabel);
    enableLyt->addStretch();
    enableLyt->addWidget(mEnableBtn);

    line_8 = setLine(mProxyFrame);

    mSelectFrame = new QFrame(mProxyFrame);
    setFrame_Noframe(mSelectFrame);
    QHBoxLayout *selectLyt = new QHBoxLayout(mSelectFrame);
    selectLyt->setContentsMargins(16, 0, 16, 0);
    selectLyt->setSpacing(4);
    QLabel *selectLabel = new QLabel(tr("Proxy mode"), mSelectFrame);
    selectLabel->setFixedWidth(148);
    mAutoBtn = new QRadioButton(mSelectFrame);
    KDK_EXTEND_ALL_INFO_FORMAT(mAutoBtn, "Proxy", "", "auto mode button of system proxy");
    mProxyBtnGroup->addButton(mAutoBtn);
    QLabel *autoLabel = new QLabel(tr("Auto"), mSelectFrame);
    mManualBtn = new QRadioButton(mSelectFrame);
    KDK_EXTEND_ALL_INFO_FORMAT(mManualBtn, "Proxy", "", "manual mode button of system proxy");
    mProxyBtnGroup->addButton(mManualBtn);
    QLabel *manualLabel = new QLabel(tr("Manual"), mSelectFrame);
    selectLyt->addWidget(selectLabel);
    selectLyt->addWidget(mAutoBtn);
    selectLyt->addWidget(autoLabel);
    selectLyt->addSpacing(100);
    selectLyt->addWidget(mManualBtn);
    selectLyt->addWidget(manualLabel);
    selectLyt->addStretch();

    line_1 = setLine(mProxyFrame);

    mUrlFrame = new QFrame(mProxyFrame);
    setFrame_Noframe(mUrlFrame);

    QHBoxLayout *mUrlLayout = new QHBoxLayout(mUrlFrame);
    mUrlLayout->setContentsMargins(16, 0, 16, 0);
    mUrlLayout->setSpacing(8);

    mUrlLabel = new QLabel(mUrlFrame);
    mUrlLabel->setFixedWidth(136);

    mUrlLineEdit = new QLineEdit(mUrlFrame);
    mUrlLineEdit->setFixedHeight(36);

    mUrlLayout->addWidget(mUrlLabel);
    mUrlLayout->addWidget(mUrlLineEdit);

    line_2 = setLine(mProxyFrame);


    mHTTPFrame = new QFrame(mProxyFrame);
    setFrame_Noframe(mHTTPFrame);

    QHBoxLayout *mHTTPLayout_1 = new QHBoxLayout(mHTTPFrame);
    mHTTPLayout_1->setSpacing(8);
    mHTTPLayout_1->setContentsMargins(16, 0, 16, 0);
    mHTTPLabel = new QLabel(mHTTPFrame);
    mHTTPLabel->setFixedWidth(136);
    mHTTPPortLabel = new QLabel(mHTTPFrame);
    mHTTPPortLabel->setFixedWidth(100);
    mHTTPPortLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mHTTPLineEdit_1 = new QLineEdit(mHTTPFrame);
    mHTTPLineEdit_1->resize(300, 36);
    mHTTPLineEdit_2 = new QLineEdit(mHTTPFrame);
    mHTTPLineEdit_2->setFixedHeight(36);
    mHTTPLineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]*") , this));
    mHTTPLayout_1->addWidget(mHTTPLabel);
    mHTTPLayout_1->addWidget(mHTTPLineEdit_1);
    mHTTPLayout_1->addWidget(mHTTPPortLabel);
    mHTTPLayout_1->addWidget(mHTTPLineEdit_2);

    line_3 = setLine(mProxyFrame);

    mHTTPSFrame = new QFrame(mProxyFrame);
    setFrame_Noframe(mHTTPSFrame);

    QHBoxLayout *mHTTPSLayout = new QHBoxLayout(mHTTPSFrame);
    mHTTPSLayout->setSpacing(8);
    mHTTPSLayout->setContentsMargins(16, 0, 16, 0);
    mHTTPSLabel = new QLabel(mHTTPSFrame);
    mHTTPSLabel->setFixedWidth(136);
    mHTTPSPortLabel = new QLabel(mHTTPSFrame);
    mHTTPSPortLabel->setFixedWidth(100);
    mHTTPSPortLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mHTTPSLineEdit_1 = new QLineEdit(mHTTPSFrame);
    mHTTPSLineEdit_1->resize(300, 36);
    mHTTPSLineEdit_2 = new QLineEdit(mHTTPSFrame);
    mHTTPSLineEdit_2->setFixedHeight(36);
    mHTTPSLineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]*") , this));
    mHTTPSLayout->addWidget(mHTTPSLabel);
    mHTTPSLayout->addWidget(mHTTPSLineEdit_1);
    mHTTPSLayout->addWidget(mHTTPSPortLabel);
    mHTTPSLayout->addWidget(mHTTPSLineEdit_2);

    line_4 = setLine(mProxyFrame);

    mFTPFrame = new QFrame(mProxyFrame);
    setFrame_Noframe(mFTPFrame);

    QHBoxLayout *mFTPLayout = new QHBoxLayout(mFTPFrame);
    mFTPLayout->setSpacing(8);
    mFTPLayout->setContentsMargins(16, 0, 16, 0);
    mFTPLabel = new QLabel(mFTPFrame);
    mFTPLabel->setFixedWidth(136);
    mFTPPortLabel = new QLabel(mFTPFrame);
    mFTPPortLabel->setFixedWidth(100);
    mFTPPortLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mFTPLineEdit_1 = new QLineEdit(mFTPFrame);
    mFTPLineEdit_1->resize(300, 36);
    mFTPLineEdit_2 = new QLineEdit(mFTPFrame);
    mFTPLineEdit_2->setFixedHeight(36);
    mFTPLineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]*") , this));
    mFTPLayout->addWidget(mFTPLabel);
    mFTPLayout->addWidget(mFTPLineEdit_1);
    mFTPLayout->addWidget(mFTPPortLabel);
    mFTPLayout->addWidget(mFTPLineEdit_2);

    line_5 = setLine(mProxyFrame);

    mSOCKSFrame = new QFrame(mProxyFrame);
    setFrame_Noframe(mSOCKSFrame);

    QHBoxLayout *mSOCKSLayout = new QHBoxLayout(mSOCKSFrame);
    mSOCKSLayout->setSpacing(8);
    mSOCKSLayout->setContentsMargins(16, 0, 16, 0);
    mSOCKSLabel = new QLabel(mSOCKSFrame);
    mSOCKSLabel->setFixedWidth(136);
    mSOCKSPortLabel = new QLabel(mSOCKSFrame);
    mSOCKSPortLabel->setFixedWidth(100);
    mSOCKSPortLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mSOCKSLineEdit_1 = new QLineEdit(mSOCKSFrame);
    mSOCKSLineEdit_1->resize(300, 36);
    mSOCKSLineEdit_2 = new QLineEdit(mSOCKSFrame);
    mSOCKSLineEdit_2->setFixedHeight(36);
    mSOCKSLineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]*") , this));
    mSOCKSLayout->addWidget(mSOCKSLabel);
    mSOCKSLayout->addWidget(mSOCKSLineEdit_1);
    mSOCKSLayout->addWidget(mSOCKSPortLabel);
    mSOCKSLayout->addWidget(mSOCKSLineEdit_2);

    line_6 = setLine(mProxyFrame);

    mIgnoreFrame = new QFrame(mProxyFrame);
    mIgnoreFrame->setMinimumSize(QSize(550, 0));
    mIgnoreFrame->setMaximumSize(QSize(16777215, 16777215));
    mIgnoreFrame->setFrameShape(QFrame::NoFrame);
    QVBoxLayout *mIgnoreLayout = new QVBoxLayout(mIgnoreFrame);
    mIgnoreLayout->setSpacing(10);
    mIgnoreLayout->setContentsMargins(16, 0, 16, 24);
    mIgnoreLabel = new QLabel(mIgnoreFrame);
    mIgnoreLabel->setWordWrap(true);
    mIgnoreLineEdit = new TextEdit(mIgnoreFrame);
    mIgnoreLineEdit->setFixedHeight(120);
    mIgnoreLayout->addWidget(mIgnoreLabel);
    mIgnoreLayout->addWidget(mIgnoreLineEdit);

    Lyt->addWidget(mEnableFrame);
    Lyt->addWidget(line_8);
    Lyt->addWidget(mSelectFrame);
    Lyt->addWidget(line_1);
    Lyt->addWidget(mUrlFrame);
    Lyt->addWidget(line_2);
    Lyt->addWidget(mHTTPFrame);
    Lyt->addWidget(line_3);
    Lyt->addWidget(mHTTPSFrame);
    Lyt->addWidget(line_4);
    Lyt->addWidget(mFTPFrame);
    Lyt->addWidget(line_5);
    Lyt->addWidget(mSOCKSFrame);
    Lyt->addWidget(line_6);
    Lyt->addWidget(mIgnoreFrame);

    //应用代理模块
    m_appProxyLabel = new KLabel(widget);
    m_appProxyLabel->setText(tr("Application Proxy")); //应用代理
    m_appProxyLabel->setContentsMargins(16, 0, 0, 0);
    setAppProxyFrameUi(widget);
    setAppListFrameUi(widget);

    //APT代理模块
    mAptProxyLabel = new KLabel(widget);
    mAptProxyLabel->setContentsMargins(16, 0, 0, 0);

    mAPTFrame = new QFrame(widget);
    mAPTFrame->setMinimumSize(QSize(550, 0));
    mAPTFrame->setMaximumSize(QSize(16777215, 16777215));
    mAPTFrame->setFrameShape(QFrame::Box);

    QVBoxLayout *AptLayout = new QVBoxLayout(mAPTFrame);
    AptLayout->setContentsMargins(0, 0, 0, 0);
    AptLayout->setSpacing(0);

    mAPTFrame_1 = new QFrame(mAPTFrame);
    setFrame_Noframe(mAPTFrame_1);

    QHBoxLayout *mAptLayout_1 = new QHBoxLayout(mAPTFrame_1);
    mAptLayout_1->setContentsMargins(16, 0, 16, 0);
    mAptLayout_1->setSpacing(8);

    mAptLabel = new QLabel(mAPTFrame_1);
    mAptLabel->setFixedWidth(200);
    mAptBtn = new KSwitchButton(mAPTFrame_1);
    KDK_EXTEND_ALL_INFO_FORMAT(mAptBtn, "Proxy", "", "enable switch of apt proxy");
    mAptLayout_1->addWidget(mAptLabel);
    mAptLayout_1->addStretch();
    mAptLayout_1->addWidget(mAptBtn);

    m_pAPTHttpFrame = new QFrame(mAPTFrame);
    setFrame_Noframe(m_pAPTHttpFrame);

    QHBoxLayout *mAptHttpLayout = new QHBoxLayout(m_pAPTHttpFrame);
    mAptHttpLayout->setContentsMargins(16, 0, 16, 0);
    mAptHttpLayout->setSpacing(8);

    m_pAPTHttpHostTipsLabel = new QLabel(m_pAPTHttpFrame);
    m_pAPTHttpHostInfoLabel = new QLabel(m_pAPTHttpFrame);
    m_pAPTHttpPortTipsLabel = new QLabel(m_pAPTHttpFrame);
    m_pAPTHttpPortInfoLabel = new QLabel(m_pAPTHttpFrame);
    m_pAPTHttpHostTipsLabel->setFixedWidth(120);
    m_pAPTHttpHostInfoLabel->setFixedWidth(160);
    m_pAPTHttpPortTipsLabel->setFixedWidth(50);
    m_pAPTHttpPortInfoLabel->setFixedWidth(120);
    mAptHttpLayout->addWidget(m_pAPTHttpHostTipsLabel);
    mAptHttpLayout->addWidget(m_pAPTHttpHostInfoLabel);
    mAptHttpLayout->addStretch(1);
    mAptHttpLayout->addWidget(m_pAPTHttpPortTipsLabel);
    mAptHttpLayout->addWidget(m_pAPTHttpPortInfoLabel);
    mAptHttpLayout->addStretch(1);

    m_pLineHttp = setLine(mAPTFrame);

    m_pAPTHttpsFrame = new QFrame(mAPTFrame);
    setFrame_Noframe(m_pAPTHttpsFrame);

    QHBoxLayout *mAptHttpsLayout = new QHBoxLayout(m_pAPTHttpsFrame);
    mAptHttpsLayout->setContentsMargins(16, 0, 16, 0);
    mAptHttpsLayout->setSpacing(8);

    m_pAPTHttpsHostTipsLabel = new QLabel(m_pAPTHttpsFrame);
    m_pAPTHttpsHostInfoLabel = new QLabel(m_pAPTHttpsFrame);
    m_pAPTHttpsPortTipsLabel = new QLabel(m_pAPTHttpsFrame);
    m_pAPTHttpsPortInfoLabel = new QLabel(m_pAPTHttpsFrame);
    m_pAPTHttpsHostTipsLabel->setFixedWidth(120);
    m_pAPTHttpsHostInfoLabel->setFixedWidth(160);
    m_pAPTHttpsPortTipsLabel->setFixedWidth(50);
    m_pAPTHttpsPortInfoLabel->setFixedWidth(120);
    mAptHttpsLayout->addWidget(m_pAPTHttpsHostTipsLabel);
    mAptHttpsLayout->addWidget(m_pAPTHttpsHostInfoLabel);
    mAptHttpsLayout->addStretch(1);
    mAptHttpsLayout->addWidget(m_pAPTHttpsPortTipsLabel);
    mAptHttpsLayout->addWidget(m_pAPTHttpsPortInfoLabel);
    mAptHttpsLayout->addStretch(1);

    m_pLineHttps = setLine(mAPTFrame);

    m_pEditBtn = new QPushButton(widget);
    KDK_EXTEND_ALL_INFO_FORMAT(m_pEditBtn, "Proxy", "", "edit button of apt proxy");
    m_pEditBtn->setFixedSize(120, 36);

    AptLayout->addWidget(mAPTFrame_1);
    AptLayout->addWidget(m_pLineHttp);
    AptLayout->addWidget(m_pAPTHttpFrame);
    AptLayout->addWidget(m_pLineHttps);
    AptLayout->addWidget(m_pAPTHttpsFrame);

    m_sysSpacerFrame = new QFrame(widget);
    m_sysSpacerFrame->setFixedHeight(24);
    m_appListSpacerFrame = new QFrame(widget);
    m_appListSpacerFrame->setFixedHeight(4);
    m_appSpacerFrame = new QFrame(widget);;
    m_appSpacerFrame->setFixedHeight(24);;

    mverticalLayout->addWidget(mTitleLabel);
    mverticalLayout->addWidget(mProxyFrame);
    mverticalLayout->addWidget(m_sysSpacerFrame);
    mverticalLayout->addWidget(m_appProxyLabel);
    mverticalLayout->addWidget(m_appProxyFrame);
    //mverticalLayout->addWidget(m_appListSpacerFrame);
    mverticalLayout->addWidget(m_appListFrame);
    mverticalLayout->addWidget(m_appSpacerFrame);
    mverticalLayout->addWidget(mAptProxyLabel);
    mverticalLayout->addWidget(mAPTFrame);
    mverticalLayout->addWidget(m_pEditBtn);
    mverticalLayout->addStretch();
}

void Proxy::initSearchText() {

}

void Proxy::retranslateUi()
{
    //~ contents_path /Proxy/System Proxy
    mTitleLabel->setText(tr("System Proxy"));
    //~ contents_path /Proxy/Auto url
    mUrlLabel->setText(tr("Auto url"));
    //~ contents_path /Proxy/Http Proxy
    mHTTPLabel->setText(tr("Http Proxy"));
    //~ contents_path /Proxy/Https Proxy
    mHTTPSLabel->setText(tr("Https Proxy"));
    //~ contents_path /Proxy/Ftp Proxy
    mFTPLabel->setText(tr("Ftp Proxy"));
    //~ contents_path /Proxy/Socks Proxy
    mSOCKSLabel->setText(tr("Socks Proxy"));
    mHTTPPortLabel->setText(tr("Port"));
    mHTTPSPortLabel->setText(tr("Port"));
    mFTPPortLabel->setText(tr("Port"));
    mSOCKSPortLabel->setText(tr("Port"));
    mIgnoreLabel->setText(tr("List of ignored hosts. more than one entry, please separate with english semicolon(;)"));

    if (isKylinProxyProcessRunning()) {
        //~ contents_path /Proxy/App Proxy
        m_appProxyLabel->setText(tr("App Proxy"));
    } else {
        m_appProxyLabel->setText(tr("App Proxy"));
    }

    //~ contents_path /Proxy/APT Proxy
    mAptProxyLabel->setText(tr("APT Proxy"));
    mAptLabel->setText(tr("Open"));
    m_pAPTHttpHostTipsLabel->setText(tr("Http Address : "));
    m_pAPTHttpPortTipsLabel->setText(tr("Port : "));
    m_pAPTHttpsHostTipsLabel->setText(tr("Https Address : "));
    m_pAPTHttpsPortTipsLabel->setText(tr("Port : "));
    m_pEditBtn->setText(tr("Edit"));
}

void Proxy::setupComponent(){
    //QLineEdit 设置数据
    GSData httpHostData;
    httpHostData.schema = HTTP_PROXY_SCHEMA;
    httpHostData.key = PROXY_HOST_KEY;
    mHTTPLineEdit_1->setProperty("gData", QVariant::fromValue(httpHostData));

    GSData httpsHostData;
    httpsHostData.schema = HTTPS_PROXY_SCHEMA;
    httpsHostData.key = PROXY_HOST_KEY;
    mHTTPSLineEdit_1->setProperty("gData", QVariant::fromValue(httpsHostData));

    GSData ftpHostData;
    ftpHostData.schema = FTP_PROXY_SCHEMA;
    ftpHostData.key = PROXY_HOST_KEY;
    mFTPLineEdit_1->setProperty("gData", QVariant::fromValue(ftpHostData));

    GSData socksHostData;
    socksHostData.schema = SOCKS_PROXY_SCHEMA;
    socksHostData.key = PROXY_HOST_KEY;
    mSOCKSLineEdit_1->setProperty("gData", QVariant::fromValue(socksHostData));

    GSData httpPortData;
    httpPortData.schema = HTTP_PROXY_SCHEMA;
    httpPortData.key = PROXY_PORT_KEY;
    mHTTPLineEdit_2->setProperty("gData", QVariant::fromValue(httpPortData));

    GSData httpsPortData;
    httpsPortData.schema = HTTPS_PROXY_SCHEMA;
    httpsPortData.key = PROXY_PORT_KEY;
    mHTTPSLineEdit_2->setProperty("gData", QVariant::fromValue(httpsPortData));

    GSData ftpPortData;
    ftpPortData.schema = FTP_PROXY_SCHEMA;
    ftpPortData.key = PROXY_PORT_KEY;
    mFTPLineEdit_2->setProperty("gData", QVariant::fromValue(ftpPortData));

    GSData socksPortData;
    socksPortData.schema = SOCKS_PROXY_SCHEMA;
    socksPortData.key = PROXY_PORT_KEY;
    mSOCKSLineEdit_2->setProperty("gData", QVariant::fromValue(socksPortData));
}

void Proxy::setupConnect(){
    connect(mEnableBtn, &KSwitchButton::stateChanged, this ,[=](bool checked) {
        UkccCommon::buriedSettings(QString("Proxy"), QString("System Proxy Open"), QString("settings"), checked?"true":"false");
        refreshSystemProxyState(checked);

        /* 日志上报 */
        QString logMsg = checked ? QString("系统代理启用") : QString("系统代理关闭");
        QDBusInterface netCtlLogInterface("com.kylin.networkCtrol",
                                           "/com/kylin/networkCtrol",
                                           "com.kylin.networkCtrol",
                                           QDBusConnection::systemBus());
        if(!netCtlLogInterface.isValid()){
            qWarning() << QString("Failed to create D-Bus of com.kylin.networkCtrol");
            return;

        }

        qDebug()<<Q_FUNC_INFO<<__LINE__<< "Sent log message:" << logMsg;
        QDBusMessage reply = netCtlLogInterface.call("sendSysProxyNetCtlLog", logMsg);
        if (reply.type() == QDBusMessage::ErrorMessage) {
            qWarning()<< "D-Bus call failed:"<< reply.errorMessage();
        }
    });

    connect(m_pEditBtn ,&QPushButton::clicked, this, &Proxy::setAptProxySlot);

    connect(mProxyBtnGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton * eBtn){
        if (eBtn == mAutoBtn) {
            UkccCommon::buriedSettings(QString("Proxy"), QString("auto"), QString("clicked"));
            mManualBtn->setChecked(false);
            proxysettings->set(PROXY_MODE_KEY,"auto");
        } else if (eBtn == mManualBtn){
            UkccCommon::buriedSettings(QString("Proxy"), QString("manual"), QString("clicked"));
            mAutoBtn->setChecked(false);
            proxysettings->set(PROXY_MODE_KEY,"manual");
        }
        _setSensitivity();
    });

    connect(mUrlLineEdit, &QLineEdit::editingFinished, this, [=](){proxysettings->set(PROXY_AUTOCONFIG_URL_KEY, QVariant(mUrlLineEdit->text()));});
    connect(mHTTPLineEdit_1, &QLineEdit::editingFinished, this, [=](){manualProxyTextChanged();});
    connect(mHTTPSLineEdit_1, &QLineEdit::editingFinished, this, [=](){manualProxyTextChanged();});
    connect(mFTPLineEdit_1, &QLineEdit::editingFinished, this, [=](){manualProxyTextChanged();});
    connect(mSOCKSLineEdit_1, &QLineEdit::editingFinished, this, [=](){manualProxyTextChanged();});
    connect(mHTTPLineEdit_2, &QLineEdit::editingFinished, this, [=](){manualProxyTextChanged();});
    connect(mHTTPSLineEdit_2, &QLineEdit::editingFinished, this, [=](){manualProxyTextChanged();});
    connect(mFTPLineEdit_2, &QLineEdit::editingFinished, this, [=](){manualProxyTextChanged();});
    connect(mSOCKSLineEdit_2, &QLineEdit::editingFinished, this, [=](){manualProxyTextChanged();});

    connect(mIgnoreLineEdit, &TextEdit::editingFinished, this, [=](){
        QString text = mIgnoreLineEdit->toPlainText();
        QStringList hostStringList = text.split(";");
        proxysettings->set(IGNORE_HOSTS_KEY, QVariant(hostStringList));
    });

    connect(proxysettings, &QGSettings::changed, this, [=](QString key){
        if (PROXY_MODE_KEY == key) {
            if (proxysettings->get(key) == "manual") {
                mEnableBtn->setChecked(true);
                mManualBtn->setChecked(true);
            } else if (proxysettings->get(key) == "auto") {
                mEnableBtn->setChecked(true);
                mAutoBtn->setChecked(true);
            } else {
                mEnableBtn->setChecked(false);
            }
            _setSensitivity();
        } else if (PROXY_AUTOCONFIG_URL_KEY == key) {
            mUrlLineEdit->setText(proxysettings->get(key).toString());
        } else if (IGNORE_HOSTS_KEY == key) {
            mIgnoreLineEdit->blockSignals(true);
            QStringList ignorehost = proxysettings->get(key).toStringList();
            QString text = mIgnoreLineEdit->toPlainText();
            if (QString::compare(text, ignorehost.join(";"), Qt::CaseSensitive)) {
                mIgnoreLineEdit->setPlainText(ignorehost.join(";"));
            }
            mIgnoreLineEdit->blockSignals(false);
        }
    });

    connect(httpsettings, &QGSettings::changed, this, [=](QString key){
        if (PROXY_HOST_KEY == key) {
            mHTTPLineEdit_1->setText(httpsettings->get(key).toString());
        } else if (PROXY_PORT_KEY == key) {
            mHTTPLineEdit_2->setText(QString::number(httpsettings->get(key).toInt()));
        }
    });

    connect(securesettings, &QGSettings::changed, this, [=](QString key){
        if (PROXY_HOST_KEY == key) {
            mHTTPSLineEdit_1->setText(securesettings->get(key).toString());
        } else if (PROXY_PORT_KEY == key) {
            mHTTPSLineEdit_2->setText(QString::number(securesettings->get(key).toInt()));
        }
    });

    connect(ftpsettings, &QGSettings::changed, this, [=](QString key){
        if (PROXY_HOST_KEY == key) {
            mFTPLineEdit_1->setText(ftpsettings->get(key).toString());
        } else if (PROXY_PORT_KEY == key) {
            mFTPLineEdit_2->setText(QString::number(ftpsettings->get(key).toInt()));
        }
    });

    connect(sockssettings, &QGSettings::changed, this, [=](QString key){
        if (PROXY_HOST_KEY == key) {
            mSOCKSLineEdit_1->setText(sockssettings->get(key).toString());
        } else if (PROXY_PORT_KEY == key) {
            mSOCKSLineEdit_2->setText(QString::number(sockssettings->get(key).toInt()));
        }
    });

    connect(mAptBtn , &KSwitchButton::stateChanged, this ,[=](bool checked){
        UkccCommon::buriedSettings(QString("Proxy"), QString("Apt Proxy Open"), QString("settings"), checked?"true":"false");
        if (checked) {
            emit m_pEditBtn->click();
        } else {
            /* 关闭APT代理，但不删除配置信息, 关闭代理时传入当前配置，这样配置信息会被保留 */
            QHash<QString, QVariant> currentAptInfo = getAptProxy();
            QString httpIp = currentAptInfo["http_ip"].toString();
            QString httpPort = currentAptInfo["http_port"].toString();
            QString httpsIp = currentAptInfo["https_ip"].toString();
            QString httpsPort = currentAptInfo["https_port"].toString();
            if (QString(qgetenv("http_proxy").data()).isEmpty()) {
                setAPTProxyInfoFrameVisible(false);
                setAptProxy(httpIp, httpPort, httpsIp, httpsPort, false);
            } else {
                QMessageBox *mReboot = new QMessageBox(pluginWidget->topLevelWidget());
                mReboot->setIcon(QMessageBox::Warning);
                mReboot->setText(tr("The APT proxy has been turned off and needs to be restarted to take effect"));
                QPushButton *laterbtn =  mReboot->addButton(tr("Reboot Later"), QMessageBox::RejectRole);
                QPushButton *nowbtn =   mReboot->addButton(tr("Reboot Now"), QMessageBox::AcceptRole);
                mReboot->exec();
                if (mReboot->clickedButton() == nowbtn) {  //选择了立即重启，一秒后系统会重启
                    setAPTProxyInfoFrameVisible(false);
                    setAptProxy(httpIp, httpPort, httpsIp, httpsPort, false);
                    sleep(1);
                    reboot();
                } else {  //选择了稍后重启,修改了配置，但未改动了已生效的环境变量
                    setAPTProxyInfoFrameVisible(false);
                    setAptProxy(httpIp, httpPort, httpsIp, httpsPort, false);
                }
            }
        }
    });
}

void Proxy::initProxyModeStatus(){
    int mode = _getCurrentProxyMode();
    QHash<QString, QVariant> mAptinfo = getAptProxy();

    mAutoBtn->blockSignals(true);
    mManualBtn->blockSignals(true);
    mAptBtn->blockSignals(true);
    mEnableBtn->blockSignals(true);

    if (mode == AUTO){
        mEnableBtn->setChecked(true);
        mAutoBtn->setChecked(true);
    } else if (mode == MANUAL){
        mEnableBtn->setChecked(true);
        mManualBtn->setChecked(true);
    } else{
        mEnableBtn->setChecked(false);
        mSelectFrame->setVisible(false);
        line_8->setVisible(false);
    }

    if (QFile::exists("/etc/apt/ota_version")) {
        mAPTFrame->hide();
        mAptProxyLabel->hide();
    } else {
        if (mAptinfo["open"].toBool()) {
            mAptBtn->setChecked(true);
            m_pAPTHttpHostInfoLabel->setText(mAptinfo["http_ip"].toString());
            m_pAPTHttpPortInfoLabel->setText(mAptinfo["http_port"].toString());
            m_pAPTHttpsHostInfoLabel->setText(mAptinfo["https_ip"].toString());
            m_pAPTHttpsPortInfoLabel->setText(mAptinfo["https_port"].toString());
        } else {
            mAptBtn->setChecked(false);
            setAPTProxyInfoFrameVisible(false);
        }
    }


    mAutoBtn->blockSignals(false);
    mManualBtn->blockSignals(false);
    mAptBtn->blockSignals(false);
    mEnableBtn->blockSignals(false);

    _setSensitivity();
}

void Proxy::initAutoProxyStatus(){

    mUrlLineEdit->blockSignals(true);
    //设置当前url
    QString urlString = proxysettings->get(PROXY_AUTOCONFIG_URL_KEY).toString();
    mUrlLineEdit->setText(urlString);

    mUrlLineEdit->blockSignals(false);
}

void Proxy::initManualProxyStatus(){
    //信号阻塞
    mHTTPLineEdit_1->blockSignals(true);
    mHTTPSLineEdit_1->blockSignals(true);
    mFTPLineEdit_1->blockSignals(true);
    mSOCKSLineEdit_1->blockSignals(true);

    mHTTPLineEdit_2->blockSignals(true);
    mHTTPSLineEdit_2->blockSignals(true);
    mFTPLineEdit_2->blockSignals(true);
    mSOCKSLineEdit_2->blockSignals(true);

    //HTTP
    QString httphost = httpsettings->get(PROXY_HOST_KEY).toString();
    mHTTPLineEdit_1->setText(httphost);
    int httpport = httpsettings->get(PROXY_PORT_KEY).toInt();
    mHTTPLineEdit_2->setText(QString::number(httpport));

    //HTTPS
    QString httpshost = securesettings->get(PROXY_HOST_KEY).toString();
    mHTTPSLineEdit_1->setText(httpshost);
    int httpsport = securesettings->get(PROXY_PORT_KEY).toInt();
    mHTTPSLineEdit_2->setText(QString::number(httpsport));

    //FTP
    QString ftphost = ftpsettings->get(PROXY_HOST_KEY).toString();
    mFTPLineEdit_1->setText(ftphost);
    int ftppost = ftpsettings->get(PROXY_PORT_KEY).toInt();
    mFTPLineEdit_2->setText(QString::number(ftppost));

    //SOCKS
    QString sockshost = sockssettings->get(PROXY_HOST_KEY).toString();
    mSOCKSLineEdit_1->setText(sockshost);
    int socksport = sockssettings->get(PROXY_PORT_KEY).toInt();
    mSOCKSLineEdit_2->setText(QString::number(socksport));

    //解除信号阻塞
    mHTTPLineEdit_1->blockSignals(false);
    mHTTPSLineEdit_1->blockSignals(false);
    mFTPLineEdit_1->blockSignals(false);
    mSOCKSLineEdit_1->blockSignals(false);

    mHTTPLineEdit_2->blockSignals(false);
    mHTTPSLineEdit_2->blockSignals(false);
    mFTPLineEdit_2->blockSignals(false);
    mSOCKSLineEdit_2->blockSignals(false);
}

void Proxy::initIgnoreHostStatus(){
    mIgnoreLineEdit->blockSignals(true);

    //设置当前ignore host
    QStringList ignorehost = proxysettings->get(IGNORE_HOSTS_KEY).toStringList();
    mIgnoreLineEdit->setPlainText(ignorehost.join(";"));

    mIgnoreLineEdit->blockSignals(false);
}

void Proxy::initDbus()
{
    m_appProxyDbus = new QDBusInterface("com.kylin.network",
                                        "/com/kylin/proxy",
                                        "com.kylin.network.proxy",
                                        QDBusConnection::sessionBus());

    if(!m_appProxyDbus->isValid()) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }

    // 连接D-Bus信号
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    // 连接代理配置变化信号
    sessionBus.connect("com.kylin.network",
                      "/com/kylin/proxy",
                      "com.kylin.network.proxy",
                      "proxyConfigChanged",
                      this,
                      SLOT(onProxyConfigChanged(QStringList)));

    // 连接应用代理状态变化信号
    sessionBus.connect("com.kylin.network",
                      "/com/kylin/proxy",
                      "com.kylin.network.proxy",
                      "appProxyStateChanged",
                      this,
                      SLOT(onAppProxyStateChanged(bool)));

    // 连接应用代理列表变化信号
    sessionBus.connect("com.kylin.network",
                      "/com/kylin/proxy",
                      "com.kylin.network.proxy",
                      "appProxyAllListChanged",
                      this,
                      SLOT(onAppProxyAppListChanged()));

    // 连接单个应用变化信号
    sessionBus.connect("com.kylin.network",
                      "/com/kylin/proxy",
                      "com.kylin.network.proxy",
                      "appProxyAppListChanged",
                      this,
                      SLOT(onAppProxySingleAppChanged(QString, bool)));

    // 连接APT代理变化信号
    sessionBus.connect("com.kylin.network",
                      "/com/kylin/proxy",
                      "com.kylin.network.proxy",
                      "aptProxyChanged",
                      this,
                      SLOT(onAptProxyChanged(QHash<QString, QVariant>)));
}

void Proxy::initAppProxyStatus()
{
    bool state = getAppProxyState();
    appProxyInfoPadding();
    appListPadding();
    m_appEnableBtn->setChecked(state);
    setAppProxyUiEnable(state);
}

int Proxy::_getCurrentProxyMode(){
    GSettings * proxygsettings;
    proxygsettings = g_settings_new(PROXY_SCHEMA);
    int mode = g_settings_get_enum(proxygsettings, PROXY_MODE_KEY);
    g_object_unref(proxygsettings);

    return mode;
}

void Proxy::_setSensitivity(){
    bool autoChecked = false;
    bool manualChecked = false;
    if (mEnableBtn->isChecked()) {
        autoChecked = mAutoBtn->isChecked();
        manualChecked = mManualBtn->isChecked();
    }

    //自动配置代理界面敏感性
    mUrlFrame->setVisible(autoChecked);
    line_1->setVisible(autoChecked);


    //手动配置代理界面敏感性
    mHTTPFrame->setVisible(manualChecked);
    mHTTPSFrame->setVisible(manualChecked);
    mFTPFrame->setVisible(manualChecked);
    mSOCKSFrame->setVisible(manualChecked);
    mIgnoreFrame->setVisible(manualChecked);
    line_2->setVisible(manualChecked);
    line_3->setVisible(manualChecked);
    line_4->setVisible(manualChecked);
    line_5->setVisible(manualChecked);
    line_6->setVisible(manualChecked);
}

bool Proxy::isManualProxyEnable()
{
    if ((httpsettings->get(PROXY_HOST_KEY).toString().isEmpty() || httpsettings->get(PROXY_PORT_KEY).toInt() == 0)
        && (securesettings->get(PROXY_HOST_KEY).toString().isEmpty() || securesettings->get(PROXY_PORT_KEY).toInt() == 0)
        && (ftpsettings->get(PROXY_HOST_KEY).toString().isEmpty() || ftpsettings->get(PROXY_PORT_KEY).toInt() == 0)
        && (sockssettings->get(PROXY_HOST_KEY).toString().isEmpty() || sockssettings->get(PROXY_PORT_KEY).toInt() == 0)) {
        return false;
    }
    return true;
}

void Proxy::refreshSystemProxyState(bool isChecked)
{
    mSelectFrame->setVisible(isChecked);
    line_8->setVisible(isChecked);

    if (isChecked) {
        mAutoBtn->blockSignals(true);
        mManualBtn->blockSignals(true);
        mAptBtn->blockSignals(true);
        mEnableBtn->blockSignals(true);
        if (proxysettings->get(PROXY_MODE_KEY) == "manual" && !mManualBtn->isChecked()) {
            mManualBtn->setChecked(true);
        }
        if (proxysettings->get(PROXY_MODE_KEY) == "auto" && !mAutoBtn->isChecked()) {
            mAutoBtn->setChecked(true);
        }
        if(proxysettings->get(PROXY_MODE_KEY).toString() == SYS_PROXY_NONE_MODE){/*hotfix bug#382351 */
            if(mManualBtn->isChecked()){
                proxysettings->set(PROXY_MODE_KEY, SYS_PROXY_MANUAL_MODE);
            }else if(mAutoBtn->isChecked()){
                proxysettings->set(PROXY_MODE_KEY, SYS_PROXY_AUTO_MODE);
            }
        }//end
        mAutoBtn->blockSignals(false);
        mManualBtn->blockSignals(false);
        mAptBtn->blockSignals(false);
        mEnableBtn->blockSignals(false);
    } else {
        proxysettings->set(PROXY_MODE_KEY, "none");
    }

     _setSensitivity();
}

void Proxy::setAptProxy(QString http_host, QString http_port, QString https_host, QString https_port, bool status)
{
    QDBusInterface *proxyServiceDbus = new QDBusInterface("com.kylin.network",
                                                          "/com/kylin/proxy",
                                                          "com.kylin.network.proxy",
                                                          QDBusConnection::sessionBus());

    if (proxyServiceDbus->isValid()) {
        QDBusReply<void> reply = proxyServiceDbus->call("setAptProxy", http_host, http_port, https_host, https_port, status);

    }

    delete proxyServiceDbus;
    proxyServiceDbus = nullptr;
}

QHash<QString, QVariant> Proxy::getAptProxy()
{
     QHash<QString, QVariant> mAptInfo;
     QDBusInterface *proxyServiceDbus = new QDBusInterface("com.kylin.network",
                                                           "/com/kylin/proxy",
                                                           "com.kylin.network.proxy",
                                                           QDBusConnection::sessionBus());
    if (proxyServiceDbus->isValid()) {
        QDBusMessage result = proxyServiceDbus->call("getAptProxy");

        QList<QVariant> outArgs = result.arguments();
        QVariant first = outArgs.at(0);
        QDBusArgument dbvFirst = first.value<QDBusArgument>();
        QVariant vFirst = dbvFirst.asVariant();
        const QDBusArgument &dbusArgs = vFirst.value<QDBusArgument>();

        QVector<AptInfo> aptinfo;

        dbusArgs.beginArray();
        while (!dbusArgs.atEnd()) {
            AptInfo info;
            dbusArgs >> info;
            aptinfo.push_back(info);
        }
        dbusArgs.endArray();

        for (AptInfo it : aptinfo) {
            mAptInfo.insert(it.arg, it.out.variant());
        }
    }
    delete proxyServiceDbus;
    proxyServiceDbus = nullptr;
    return mAptInfo;
}

void Proxy::setAptInfo()
{
    QMessageBox *mReboot = new QMessageBox(pluginWidget->topLevelWidget());
    mReboot->setIcon(QMessageBox::Warning);
    mReboot->setText(tr("The system needs to be restarted to set the APT proxy, whether to reboot"));
    QPushButton *laterbtn =  mReboot->addButton(tr("Reboot Later"), QMessageBox::RejectRole);
    QPushButton *nowbtn =   mReboot->addButton(tr("Reboot Now"), QMessageBox::AcceptRole);
    mReboot->exec();
    if (mReboot->clickedButton() == nowbtn) {  //选择了立即重启，一秒后系统会重启
        sleep(1);
        reboot();
    } else  {  //选择了稍后重启或点击了关闭按钮，配置文件已写入，但是/etc/profile.d目录下新增的脚本文件未执行
        setAPTProxyInfoFrameVisible(true);
        m_pAPTHttpHostInfoLabel->setText(getAptProxy()["http_ip"].toString());
        m_pAPTHttpPortInfoLabel->setText(getAptProxy()["http_port"].toString());
        m_pAPTHttpsHostInfoLabel->setText(getAptProxy()["https_ip"].toString());
        m_pAPTHttpsPortInfoLabel->setText(getAptProxy()["https_port"].toString());
    }
}

void Proxy::reboot()
{
    QDBusInterface *rebootDbus = new QDBusInterface("org.gnome.SessionManager",
                                                             "/org/gnome/SessionManager",
                                                             "org.gnome.SessionManager",
                                                             QDBusConnection::sessionBus());
    if (rebootDbus->isValid()) {
        rebootDbus->call("reboot");
    }
    delete rebootDbus;
    rebootDbus = nullptr;
}

void Proxy::setFrame_Noframe(QFrame *frame)
{
    frame->setMinimumSize(QSize(550, 60));
    frame->setMaximumSize(QSize(16777215, 60));
    frame->setFrameShape(QFrame::NoFrame);
}

QFrame *Proxy::setLine(QFrame *frame)
{
    KHLineFrame *line = new KHLineFrame(frame);
    return line;
}

bool Proxy::getAppProxyState()
{
    bool state = true;
    if(m_appProxyDbus == nullptr || !m_appProxyDbus->isValid()) {
        qWarning ()<< "init AppProxy dbus error";
        return false;
    }

    //获取应用代理开启状态
    qDebug() << "call QDBusInterface getAppProxyState";
    QDBusReply<bool> reply = m_appProxyDbus->call("getAppProxyState");

    if (!reply.isValid()) {
        return false;
    }
    state = reply;

    return state;
}

void Proxy::setAppProxyState(bool state)
{
    if(m_appProxyDbus == nullptr || !m_appProxyDbus->isValid()) {
        qWarning ()<< "init AppProxy dbus error";
        return;
    }

    //设置应用代理开启状态
    qDebug() << "call QDBusInterface setAppProxyState" << state;
    m_appProxyDbus->call("setAppProxyState", state);
}

QStringList Proxy::getAppProxyConf()
{
    QStringList info;
    info.clear();

    QDBusInterface dbusInterface("com.kylin.network",
                                 "/com/kylin/proxy",
                                 "com.kylin.network.proxy",
                                 QDBusConnection::sessionBus());

    if(!dbusInterface.isValid()) {
        qWarning ()<< "init AppProxy dbus error";
        return info;
    }

    //获取应用代理配置信息
    qDebug() << "call QDBusInterface getAppProxyConfig";
    QDBusReply<QStringList> reply = dbusInterface.call("getAppProxyConfig");

    if (!reply.isValid()) {
        qWarning ()<< "Return empty app proxy information, getAppProxyConfig reply is invalid";
        return info;
    }

    info =  reply.value();

    if (info.isEmpty()) {
        qWarning() << "getAppProxyConfig reply is empty";
    }

    return info;
}

void Proxy::setAppProxyConf(QStringList list)
{
    //AppProxyConf 必填项数量为3
    if (list.count() < 3) {
        return;
    }

    if(m_appProxyDbus == nullptr || !m_appProxyDbus->isValid()) {
        qWarning ()<< "init AppProxy dbus error";
        return;
    }

    //写入应用代理配置信息
    qDebug() << "call QDBusInterface setAppProxyConfig"<<"currConf:"<<list;
    m_appProxyDbus->call("setAppProxyConfig", list);
}

QMap<QString, QStringList> Proxy::getAppListProxy()
{
    QMap<QString, QStringList> appList;
    appList.clear();

    QDBusInterface dbusInterface("com.kylin.network",
                                 "/com/kylin/proxy",
                                 "com.kylin.network.proxy",
                                 QDBusConnection::sessionBus());

    if(!dbusInterface.isValid()) {
        qWarning ()<< "init AppProxy dbus error";
        return appList;
    }

    //获取可以配置应用代理的应用信息
    qDebug() << "call QDBusInterface getAppProxyAppsInfo";
    QDBusReply<QMap<QString, QStringList>> reply = dbusInterface.call("getAppProxyAppsInfo");

    if (!reply.isValid()) {
        qWarning ()<< "Return empty app list, getAppProxy reply is invalid";
        return appList;
    }
    appList = reply.value();
    if (appList.isEmpty()) {
        qWarning() << "getAppProxyAppsInfo reply appList is empty";
    }

    return appList;
}

void Proxy::setUkccProxySettings()
{
    setSystemProxyFrameHidden(false);
    if(isKylinProxyProcessRunning()){
        setAppProxyFrameHidden(false);
    }else{
        /* 如果 kylin-proxy 进程不存在，隐藏应用代理相关界面 */
        setAppProxyFrameHidden(true);
        qWarning() << "kylin-proxy process is not running, hiding application proxy interface";
    }
    setAPTProxyFrameHidden(false);

    QVariant configData=UiSecurityConfig::getInstance()->getConnectSettingsData("proxy","proxySettings");
    QString proxySettings = configData.toString();
    if (proxySettings.isEmpty()) {
        return;
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__<< "get Ukcc ctrl ProxySettings" << proxySettings;

    QStringList proxySettingList = proxySettings.split(",");
    for (const QString setting : proxySettingList) {
        if (setting.contains("SystemProxyFrame:false")) {
            setSystemProxyFrameHidden(true);
        } else if (setting.contains("AppProxyFrame:false")) {
            setAppProxyFrameHidden(true);
        } else if (setting.contains("APTProxyFrame:false")) {
            setAPTProxyFrameHidden(true);
        } else if(setting.contains("SystemProxyFrameEnable:false")) {
            setSystemProxyFrameEnable(false);
        } else if (setting.contains("AppProxyFrameEnable:false")) {
            setAppProxyFrameEnable(false);
        } else if (setting.contains("APTProxyFrameEnable:false")) {
            setAPTProxyFrameEnable(false);
        }
    }
}

#if 0
bool Proxy::checkIsChanged(QStringList info)
{
    bool isChanged = false;
    info = getAppProxyConf();
    if (info.value(0) != m_proxyTypeComboBox->currentText()
            || info.value(1) != m_ipAddressLineEdit->text()
            || info.value(2) != m_portLineEdit->text()
            || info.value(3) != m_userNameLineEdit->text()
            || info.value(4) != m_pwdLineEdit->text()) {

        isChanged = true; //如果在代理界面相关信息已修改，则返回true
    }
    return isChanged;
}
#endif

void Proxy::setAppProxyFrameUi(QWidget *widget)
{
    //应用代理模块
    m_appProxyFrame = new QFrame(widget);
    m_appProxyFrame->setMinimumSize(QSize(550, 0));
    m_appProxyFrame->setMaximumSize(QSize(16777215, 16777215));
    m_appProxyFrame->setFrameShape(QFrame::Box);

    QVBoxLayout *appProxyLayout = new QVBoxLayout(m_appProxyFrame);
    appProxyLayout->setContentsMargins(0, 0, 0, 0);
    appProxyLayout->setSpacing(0);

    //开启ui布局
    m_appEnableFrame = new QFrame(m_appProxyFrame);
    setFrame_Noframe(m_appEnableFrame);
    m_appEnableLabel = new QLabel(tr("Open"), m_appEnableFrame);
    m_appEnableBtn = new KSwitchButton(m_appEnableFrame);
    KDK_EXTEND_ALL_INFO_FORMAT(m_appEnableBtn, "Proxy", "", "enable switch of app proxy");
    m_appEnableBtn->setCheckable(true);
    QHBoxLayout *appEnableLayout = new QHBoxLayout(m_appEnableFrame);
    appEnableLayout->setContentsMargins(FRAME_LAYOUT_MARGINS);
    appEnableLayout->addWidget(m_appEnableLabel);
    appEnableLayout->addStretch();
    appEnableLayout->addWidget(m_appEnableBtn);

    //代理类型ui布局
    m_proxyTypeFrame = new QFrame(m_appProxyFrame);
    setFrame_Noframe(m_proxyTypeFrame);
    m_proxyTypeLabel = new QLabel(tr("Proxy type"), m_proxyTypeFrame);
    m_proxyTypeLabel->setFixedWidth(LABEL_WIDTH);
    m_proxyTypeComboBox = new QComboBox(m_proxyTypeFrame);
    QHBoxLayout *proxyTypeLayout = new QHBoxLayout(m_proxyTypeFrame);
    proxyTypeLayout->setContentsMargins(FRAME_LAYOUT_MARGINS);
    proxyTypeLayout->setSpacing(FRAME_LAYOUT_SPACING);
    proxyTypeLayout->addWidget(m_proxyTypeLabel);
    proxyTypeLayout->addWidget(m_proxyTypeComboBox);
    m_proxyTypeComboBox->addItem("http"); //http
    m_proxyTypeComboBox->addItem("socks4"); //socks4
    m_proxyTypeComboBox->addItem("socks5"); //socks4

    //IP地址ui布局
    m_ipAddressFrame = new QFrame(m_appProxyFrame);
    m_ipAddressFrame->setMinimumSize(QSize(550, 60));
    m_ipAddressFrame->setMaximumSize(QSize(16777215, 88));
    m_ipAddressFrame->setFrameShape(QFrame::NoFrame);
    m_ipAddressLabel = new QLabel(tr("IP address"), m_ipAddressFrame);
    m_ipAddressLabel->setFixedWidth(LABEL_WIDTH);
    m_ipHintsLabel = new KLabel(m_ipAddressFrame);
    m_ipHintsLabel->setContentsMargins(8, 0, 0, 0);
    m_ipAddressLineEdit = new QLineEdit(m_ipAddressFrame);
    m_ipAddressLineEdit->setFixedHeight(LINE_EDIT_HEIGHT);
    m_ipAddressLineEdit->setPlaceholderText(tr("Required")); //必填

    QWidget *ipInputWidget = new QWidget(m_ipAddressFrame);
    QVBoxLayout *ipVLayout = new QVBoxLayout(ipInputWidget);
    ipVLayout->setContentsMargins(0, 0, 0, 0);
    ipVLayout->setSpacing(3);
    ipVLayout->addWidget(m_ipAddressLineEdit);
    ipVLayout->addWidget(m_ipHintsLabel);

    QFormLayout *ipAddressLayout = new QFormLayout(m_ipAddressFrame);
    ipAddressLayout->setContentsMargins(16, 12, 16, 12);
    ipAddressLayout->setSpacing(FRAME_LAYOUT_SPACING);
    ipAddressLayout->addRow(m_ipAddressLabel, ipInputWidget);

    // IP的正则格式限制
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    m_ipAddressLineEdit->setValidator(new QRegExpValidator(rx, this));
    //无效的IP地址提示
    m_ipHintsLabel->setFontColorRole(QPalette::WindowText);
    m_ipHintsLabel->setFontColor(Qt::red);
    m_ipHintsLabel->setText(tr("Invalid IP Address"));
    m_ipHintsLabel->hide();


    //端口ui布局
    m_portFrame = new QFrame(m_appProxyFrame);
    setFrame_Noframe(m_portFrame);
    m_portLabel = new QLabel(tr("Port"), m_portFrame);
    m_portLabel->setFixedWidth(LABEL_WIDTH);
    m_portLineEdit = new QLineEdit(m_portFrame);
    m_portLineEdit->setPlaceholderText(tr("Required")); //必填
    m_portLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*") , this));
    QHBoxLayout *portLayout = new QHBoxLayout(m_portFrame);
    portLayout->setContentsMargins(FRAME_LAYOUT_MARGINS);
    portLayout->setSpacing(FRAME_LAYOUT_SPACING);
    portLayout->addWidget(m_portLabel);
    portLayout->addWidget(m_portLineEdit);

    //用户名ui布局
    m_userNameFrame = new QFrame(m_appProxyFrame);
    setFrame_Noframe(m_userNameFrame);
    m_userNameLabel = new QLabel(tr("Username"), m_userNameFrame);
    m_userNameLabel->setFixedWidth(LABEL_WIDTH);
    m_userNameLineEdit = new QLineEdit(m_userNameFrame);
    m_userNameLineEdit->setPlaceholderText(tr("Optional")); //选填
    QHBoxLayout *userNameLayout = new QHBoxLayout(m_userNameFrame);
    userNameLayout->setContentsMargins(FRAME_LAYOUT_MARGINS);
    userNameLayout->setSpacing(FRAME_LAYOUT_SPACING);
    userNameLayout->addWidget(m_userNameLabel);
    userNameLayout->addWidget(m_userNameLineEdit);

    //密码ui布局
    m_pwdFrame = new QFrame(m_appProxyFrame);
    setFrame_Noframe(m_pwdFrame);
    m_pwdLabel = new QLabel(tr("Password"), m_pwdFrame);
    m_pwdLabel->setFixedWidth(LABEL_WIDTH);
    m_pwdLineEdit = new KPasswordEdit(m_pwdFrame);
    m_pwdLineEdit->setClearButtonEnabled(false);
    QString str = tr("Optional");
    m_pwdLineEdit->setPlaceholderText(str); //选填

    QHBoxLayout *pwdLayout = new QHBoxLayout(m_pwdFrame);
    pwdLayout->setContentsMargins(FRAME_LAYOUT_MARGINS);
    pwdLayout->setSpacing(FRAME_LAYOUT_SPACING);
    pwdLayout->addWidget(m_pwdLabel);
    pwdLayout->addWidget(m_pwdLineEdit);

    QRegExp rxPwd("^[A-Za-z0-9`~!@#$%^&*()_-+=<>,.\\\/]+$");
    QRegExpValidator *latitude = new QRegExpValidator(rxPwd, this);
    m_pwdLineEdit->setValidator(latitude);

#if 0
    //按钮ui布局
    m_appBtnFrame = new QFrame(m_appProxyFrame);
    setFrame_Noframe(m_appBtnFrame);
    m_cancelBtn = new QPushButton(m_appBtnFrame);
    m_saveBtn = new QPushButton(m_appBtnFrame);
    m_cancelBtn->setText(tr("Cancel"));
    m_saveBtn->setText(tr("Save"));

    QHBoxLayout *btnLayout = new QHBoxLayout(m_appBtnFrame);
    btnLayout->setContentsMargins(FRAME_LAYOUT_MARGINS);
    btnLayout->setSpacing(16);
    btnLayout->addStretch();
    btnLayout->addWidget(m_cancelBtn);
    btnLayout->addWidget(m_saveBtn);
#endif

    //分隔线
    m_appLine1 = setLine(m_appProxyFrame);
    m_appLine2 = setLine(m_appProxyFrame);
    m_appLine3 = setLine(m_appProxyFrame);
    m_appLine4 = setLine(m_appProxyFrame);
    m_appLine5 = setLine(m_appProxyFrame);

    m_appProxyInfoWidget = new QWidget(m_appProxyFrame);
    QVBoxLayout *widgetHLayout = new QVBoxLayout(m_appProxyInfoWidget);
    widgetHLayout->setContentsMargins(0, 0, 0, 0);
    widgetHLayout->setSpacing(0);
    widgetHLayout->addWidget(m_appLine1);
    widgetHLayout->addWidget(m_proxyTypeFrame);
    widgetHLayout->addWidget(m_appLine2);
    widgetHLayout->addWidget(m_ipAddressFrame);
    widgetHLayout->addWidget(m_appLine3);
    widgetHLayout->addWidget(m_portFrame);
    widgetHLayout->addWidget(m_appLine4);
    widgetHLayout->addWidget(m_userNameFrame);
    widgetHLayout->addWidget(m_appLine5);
    widgetHLayout->addWidget(m_pwdFrame);

    appProxyLayout->addWidget(m_appEnableFrame);
    appProxyLayout->addWidget(m_appProxyInfoWidget);

//    appProxyLayout->addWidget(line5);
//    appProxyLayout->addWidget(m_appBtnFrame);

    connect(m_appEnableBtn, &KSwitchButton::stateChanged, this, &Proxy::setAppProxyUiEnable);
    connect(m_appEnableBtn, &KSwitchButton::stateChanged, this, &Proxy::setAppProxyState);
    connect(m_appEnableBtn, &KSwitchButton::stateChanged, [=](bool checked) {
        UkccCommon::buriedSettings(QString("Proxy"), QString("App Proxy Open"), QString("settings"), checked?"true":"false");
    });
    connect(m_proxyTypeComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onAppProxyConfChanged()));
    connect(m_ipAddressLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onipEditStateChanged()));
    connect(m_ipAddressLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onAppProxyConfChanged()));
    connect(m_portLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onAppProxyConfChanged()));
    connect(m_userNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onAppProxyConfChanged()));
    connect(m_pwdLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onAppProxyConfChanged()));
    connect(m_ipAddressLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onAppProxyConfChanged()));

    connect(m_proxyTypeComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onAppProxyConfEditFinished()));
    connect(m_ipAddressLineEdit, SIGNAL(editingFinished()), this, SLOT(onAppProxyConfEditFinished()));
    connect(m_portLineEdit, SIGNAL(editingFinished()), this, SLOT(onAppProxyConfEditFinished()));
    connect(m_userNameLineEdit, SIGNAL(editingFinished()), this, SLOT(onAppProxyConfEditFinished()));
    connect(m_pwdLineEdit, SIGNAL(editingFinished()), this, SLOT(onAppProxyConfEditFinished()));
//    connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(onCancelBtnClicked()));
//    connect(m_saveBtn, SIGNAL(clicked()), this, SLOT(onSaveBtnClicked()));
}

void Proxy::setAppListFrameUi(QWidget *widget)
{
    //应用列表
    m_appListFrame = new QFrame(widget);
    m_appListFrame->setMinimumSize(QSize(550, 0));
    m_appListFrame->setMaximumSize(QSize(16777215, 336));
    m_appListFrame->setFrameShape(QFrame::Box);
    QVBoxLayout *appListLayout = new QVBoxLayout(m_appListFrame);
    appListLayout->setContentsMargins(16, 23, 16, 16);
    appListLayout->setSpacing(16);

    m_allowAppProxyLabel = new QLabel(m_appListFrame);
    m_allowAppProxyLabel->setText(tr("The following applications are allowed to use this configuration:")); //允许以下应用使用该配置：
    m_appListWidget = new QListWidget(m_appListFrame);
    m_appListWidget->setMinimumHeight(240);
    m_appListWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_appListWidget->setFrameShape(QFrame::Shape::Panel);
    m_appListWidget->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);

    appListLayout->addWidget(m_allowAppProxyLabel);
    appListLayout->addWidget(m_appListWidget);

    onPaletteChanged();
    const QByteArray style_id(THEME_SCHAME);
    if (QGSettings::isSchemaInstalled(style_id)) {
        QGSettings * styleGsettings = new QGSettings(style_id, QByteArray(), this);
        connect(styleGsettings, &QGSettings::changed, this, [=](QString key){
            if ("styleName" == key) {
                onPaletteChanged();
            }
        });
    }
}

void Proxy::appProxyInfoPadding()
{
    QStringList proxyInfo;
    proxyInfo.clear();
    proxyInfo = getAppProxyConf();

    m_proxyTypeComboBox->setCurrentText(proxyInfo.value(0));
    m_ipAddressLineEdit->setText(proxyInfo.value(1));
    m_portLineEdit->setText(proxyInfo.value(2));
    m_userNameLineEdit->setText(proxyInfo.value(3));
    m_pwdLineEdit->setText(proxyInfo.value(4));
}

void Proxy::appListPadding()
{
    QMap<QString, QStringList> appList ;
    appList.clear();
    appList = getAppListProxy(); //获取应用代理配置

    //遍历应用列表
    for (auto index : appList.keys()) {
        QStringList appInfo = appList.value(index);
        QVariant isChecked = appInfo.value(2);
        bool flag = isChecked.toBool();

        AppListWidget *appWidget = new AppListWidget(index, m_appListWidget);
        appWidget->setAppName(appInfo.value(0));
        appWidget->setAppIcon(QIcon::fromTheme(appInfo.value(1)));
        appWidget->setAppChecked(flag);
        appWidget->setProperty("desktopfp", index);// 保存桌面文件路径作为属性

        QListWidgetItem *appListWidgetItem = new QListWidgetItem(m_appListWidget);
        appListWidgetItem->setSizeHint(QSize(m_appListWidget->width(),36));
        appListWidgetItem->setFlags(Qt::NoItemFlags);
        m_appListWidget->addItem(appListWidgetItem);
        m_appListWidget->setItemWidget(appListWidgetItem, appWidget);
    }
}

bool Proxy::getipEditState(QString text)
{
    if (text.isEmpty()) {
        return true;
    }
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

    bool match = false;
    match = rx.exactMatch(text);

    return match;
}

void Proxy::setSystemProxyFrameHidden(bool state)
{
    mTitleLabel->setHidden(state);
    mProxyFrame->setHidden(state);
    m_sysSpacerFrame->setHidden(state);
}

void Proxy::setAppProxyFrameHidden(bool state)
{
    m_appProxyLabel->setHidden(state);
    m_appProxyFrame->setHidden(state);
    if (state) {
        m_appListFrame->setHidden(state);
    } else {
        m_appListFrame->setHidden(!m_appEnableBtn->isChecked());
    }
    m_appSpacerFrame->setHidden(state);
}

void Proxy::setAPTProxyFrameHidden(bool state)
{
    mAptProxyLabel->setHidden(state);
    mAPTFrame->setHidden(state);
}

bool Proxy::isKylinProxyProcessRunning()
{
    QProcess process;

    /* 方法1: 使用 pgrep 命令查找进程 */
    process.start("pgrep", QStringList() << "-f" << "kylin-proxy");
    process.waitForFinished();

    if (process.exitCode() == 0) {
        QString output = process.readAllStandardOutput();
        return !output.trimmed().isEmpty();
    }

    /* 方法2: 如果 pgrep 不可用，使用 ps 命令作为备选 */
    process.start("ps", QStringList() << "aux");
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    return output.contains("kylin-proxy");
}

void Proxy::onipEditStateChanged()
{
    if (!getipEditState(m_ipAddressLineEdit->text())) {
        m_ipAddressFrame->setFixedHeight(88);
        m_ipHintsLabel->show();
    } else {
        m_ipHintsLabel->hide();
        m_ipAddressFrame->setFixedHeight(60);
    }
}

void Proxy::onAppProxyConfChanged()
{
    if (!getipEditState(m_ipAddressLineEdit->text()) || m_portLineEdit->text().isEmpty()) {
        return;
    }

    if (m_ipAddressLineEdit->text().isEmpty()) {
        return;
    } else {
        m_appProxyInfo.clear();
        m_appProxyInfo.append(m_proxyTypeComboBox->currentText());
        m_appProxyInfo.append(m_ipAddressLineEdit->text());
        m_appProxyInfo.append(m_portLineEdit->text());
        if (!m_userNameLineEdit->text().isEmpty() && !m_pwdLineEdit->text().isEmpty()) {
            m_appProxyInfo.append(m_userNameLineEdit->text());
            m_appProxyInfo.append(m_pwdLineEdit->text());
        } else {
            m_appProxyInfo.append("");
            m_appProxyInfo.append("");
        }
    }
}

void Proxy::onAppProxyConfEditFinished()
{
   if (!m_ipAddressLineEdit->hasFocus() && !m_portLineEdit->hasFocus()) {
       setAppProxyConf(m_appProxyInfo);
   }
}

void Proxy::onPaletteChanged()
{
    QPalette mpal(m_appListWidget->palette());
    mpal.setColor(QPalette::Base, qApp->palette().base().color());
    mpal.setColor(QPalette::AlternateBase, qApp->palette().alternateBase().color());
    m_appListWidget->setBackgroundRole(QPalette::Base);
    m_appListWidget->setAlternatingRowColors(true);
    m_appListWidget->setPalette(mpal);
}

TextEdit::TextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    setFrameShape(QFrame::NoFrame);
    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);
    viewport()->installEventFilter(this);

    QScrollBar *scrollBar = verticalScrollBar();
    scrollBar->setProperty("drawScrollBarGroove", false);
    scrollBar->setContextMenuPolicy(Qt::NoContextMenu);
    scrollBar->setProperty("needTranslucent", true);

    connect(qApp, &QApplication::paletteChanged, this, [this]() {
        update();
        viewport()->update();
    });
}

bool TextEdit::eventFilter(QObject *obj, QEvent *event)
{
    // viewport 为独立子控件，需在文本绘制前先绘制圆角背景
    if (obj == viewport() && event->type() == QEvent::Paint) {
        paintBackgroundOn(viewport());
    }
    return QTextEdit::eventFilter(obj, event);
}

void TextEdit::paintBackgroundOn(QWidget *widget) const
{
    QPainter painter(widget);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(qApp->palette().color(QPalette::Button));
    painter.drawRoundedRect(widget->rect(), kRadius, kRadius);
}

void TextEdit::paintEvent(QPaintEvent *event)
{
    // 绘制整体圆角背景，覆盖滚动条区域
    paintBackgroundOn(this);
    QTextEdit::paintEvent(event);
}

void TextEdit::focusOutEvent(QFocusEvent *event)
{
    QTextEdit::focusOutEvent(event);
    emit editingFinished();
}

// 添加槽函数来处理信号
void Proxy::onProxyConfigChanged(const QStringList &configList)
{
    // 更新应用代理配置显示
    m_proxyTypeComboBox->setCurrentText(configList.value(0));
    m_ipAddressLineEdit->setText(configList.value(1));
    m_portLineEdit->setText(configList.value(2));
    m_userNameLineEdit->setText(configList.value(3));
    m_pwdLineEdit->setText(configList.value(4));
}

void Proxy::onAppProxyStateChanged(bool state)
{
    // 更新应用代理开关状态
    m_appEnableBtn->blockSignals(true);
    m_appEnableBtn->setChecked(state);
    m_appEnableBtn->blockSignals(false);

    // 更新UI
    setAppProxyUiEnable(state);
}

void Proxy::onAppProxyAppListChanged()
{
    // 清空并重新加载应用列表
    m_appListWidget->clear();
    appListPadding();
}

void Proxy::onAppProxySingleAppChanged(const QString &desktopfp, bool added)
{
    // 优化：只更新单个应用的状态
    for (int i = 0; i < m_appListWidget->count(); ++i) {
        QListWidgetItem *item = m_appListWidget->item(i);
        AppListWidget *widget = qobject_cast<AppListWidget*>(m_appListWidget->itemWidget(item));
        if (widget && widget->property("desktopfp").toString() == desktopfp) {
            widget->setAppChecked(added);
            break;
        }
    }
}

void Proxy::onAptProxyChanged(const QHash<QString, QVariant> &aptInfo)
{
    // 更新APT代理显示
    if (aptInfo["open"].toBool()) {
        mAptBtn->blockSignals(true);
        mAptBtn->setChecked(true);
        mAptBtn->blockSignals(false);

        m_pAPTHttpHostInfoLabel->setText(aptInfo["http_ip"].toString());
        m_pAPTHttpPortInfoLabel->setText(aptInfo["http_port"].toString());
        m_pAPTHttpsHostInfoLabel->setText(aptInfo["https_ip"].toString());
        m_pAPTHttpsPortInfoLabel->setText(aptInfo["https_port"].toString());
        setAPTProxyInfoFrameVisible(true);
    } else {
        mAptBtn->blockSignals(true);
        mAptBtn->setChecked(false);
        mAptBtn->blockSignals(false);
        setAPTProxyInfoFrameVisible(false);
    }
}

void Proxy::setAppProxyUiEnable(bool enable)
{
    if (enable) {
        m_appProxyInfoWidget->show();
        m_appListFrame->show();
    } else {
        m_appProxyInfoWidget->hide();
        m_appListFrame->hide();
    }
}

#if 0
void Proxy::onCancelBtnClicked()
{
    appProxyInfoPadding();
}

void Proxy::onSaveBtnClicked()
{
    if (!checkIsChanged(m_appProxyInfo)) {
        return;
    } else {
        QStringList conf;
        conf.clear();
        conf.append(m_proxyTypeComboBox->currentText());
        conf.append(m_ipAddressLineEdit->text());
        conf.append(m_portLineEdit->text());
        conf.append(m_userNameLineEdit->text());
        conf.append(m_pwdLineEdit->text());
        setAppProxyConf(conf);
    }
}

void Proxy::setBtnEnable()
{
    m_cancelBtn->setEnabled(true);

    if (getipEditState(m_ipAddressLineEdit->text()) && !m_portLineEdit->text().isEmpty()) {
        if (!m_ipAddressLineEdit->text().isEmpty()) {
            m_saveBtn->setEnabled(true);
        }
    } else {
        m_saveBtn->setEnabled(false);
    }
}
#endif

void Proxy::setAptProxySlot()
{
    mAptBtn->blockSignals(true);
    QHash<QString, QVariant> preaptinfo = getAptProxy();
    bool prestatus = preaptinfo["open"].toBool();
    AptProxyDialog *mwindow = new AptProxyDialog(pluginWidget);
    mwindow->exec();
    if (getAptProxy()["open"].toBool() && !prestatus) { // open值为true，用户点击了确定按钮，进行配置文件的写入，提示用户重启系统
        setAptInfo();
    }
    if (getAptProxy()["open"].toBool() && prestatus) {
        if (getAptProxy()["http_ip"].toString() == preaptinfo["http_ip"].toString()
                && getAptProxy()["http_port"].toString() == preaptinfo["http_port"].toString()
                && getAptProxy()["https_ip"].toString() == preaptinfo["https_ip"].toString()
                && getAptProxy()["https_port"].toString() == preaptinfo["https_port"].toString()
                && prestatus){  //点击了编辑按钮，且在设置IP和端口号的弹窗中，点击了取消或者关闭按钮
            setAPTProxyInfoFrameVisible(true);
        } else {
             setAptInfo();
        }
    }
    if(!getAptProxy()["open"].toBool() && !prestatus){ // 点击了APT开关按钮，但是在设置IP和端口号的弹窗中，点击了取消或者关闭按钮
        mAptBtn->setChecked(false);
    }
    mAptBtn->blockSignals(false);
}

void Proxy::manualProxyTextChanged(QString txt){
    //获取被修改控件
    QObject * pobject = this->sender();
    QLineEdit * who = dynamic_cast<QLineEdit *>(pobject);

    //获取控件保存的用户数据
    GSData currentData = who->property("gData").value<GSData>();
    QString schema = currentData.schema;
    QString key = currentData.key;

    qDebug()<<Q_FUNC_INFO<<__LINE__<<txt<<schema<<key;
    //构建临时QGSettings
    const QByteArray id = schema.toUtf8();
    const QByteArray iidd(id.data());
    QGSettings * setting = new QGSettings(iidd);

    setting->set(key, QVariant(txt));

    delete setting;
    setting = nullptr;
}

void Proxy::manualProxyTextChanged()
{
    //获取被修改控件
    QObject * pobject = this->sender();
    QLineEdit * who = dynamic_cast<QLineEdit *>(pobject);

    //获取控件保存的用户数据
    GSData currentData = who->property("gData").value<GSData>();
    QString schema = currentData.schema;
    qDebug()<<schema;
    QString key = currentData.key;
    QString value = who->text();

    qDebug()<<Q_FUNC_INFO<<__LINE__<<schema<<key<<value;
    //构建临时QGSettings
    const QByteArray id = schema.toUtf8();
    const QByteArray iidd(id.data());
    QGSettings * setting = new QGSettings(iidd);

    setting->set(key, QVariant(value));

    delete setting;
    setting = nullptr;
}

void Proxy::setAPTProxyInfoFrameVisible(bool state) {
    m_pLineHttp->setVisible(state);
    m_pAPTHttpFrame->setVisible(state);
    m_pLineHttps->setVisible(state);
    m_pAPTHttpsFrame->setVisible(state);
    m_pEditBtn->setVisible(state);
}

void Proxy::setSystemProxyFrameEnable(bool enable)
{
    mProxyFrame->setEnabled(enable);
}

void Proxy::setAppProxyFrameEnable(bool enable)
{
    m_appProxyFrame->setEnabled(enable);
    m_appListFrame->setEnabled(enable);
}

void Proxy::setAPTProxyFrameEnable(bool enable)
{
    mAPTFrame->setEnabled(enable);
    m_pEditBtn->setEnabled(enable);
}
