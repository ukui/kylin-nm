#include "enterprisewlandialog.h"
#include <QApplication>
#include <QDesktopWidget>
#include "xatom-helper.h"
#define MAIN_SIZE_EXPAND 400,500
#define MAIN_SIZE_NARROW 400,400

#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

EnterpriseWlanDialog::EnterpriseWlanDialog(KyWirelessNetItem &wirelessNetItem, QString device, QWidget *parent) : QWidget(parent)
{
    //设置窗口无边框，阴影
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    MotifWmHints window_hints;
    window_hints.flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
    window_hints.functions = MWM_FUNC_ALL;
    window_hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(this->winId(), window_hints);
#else
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
#endif
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlag(Qt::Window);
    this->setWindowTitle(tr("Connect Enterprise WLAN"));

    m_wirelessNetItem = wirelessNetItem;
    m_deviceName = device;
    m_connectOperation = new KyWirelessConnectOperation();
    m_resource = new KyWirelessNetResource();

    initUI();
    initData();
    centerToScreen();
}

EnterpriseWlanDialog::~EnterpriseWlanDialog() {
    if (m_securityPage) {
        delete m_securityPage;
        m_securityPage = nullptr;
    }
    if (m_connectOperation) {
        delete m_connectOperation;
        m_connectOperation = nullptr;
    }
}

void EnterpriseWlanDialog::closeEvent(QCloseEvent *event)
{
    emit this->enterpriseWlanDialogClose(false);
    return QWidget::closeEvent(event);
}

void EnterpriseWlanDialog::initUI()
{
#define MAIN_LAYOUT_MARGINS 8,8,8,8
#define SSID_LAYOUT_MARGINS 8,8,8,0
#define MAIN_LAYOUT_SPACING 0
#define BUTTON_SPACING 8

    m_mainLayout = new QVBoxLayout(this);
    this->setLayout(m_mainLayout);
    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);

    m_ssidLayout = new QHBoxLayout();
    m_ssidLayout->setContentsMargins(SSID_LAYOUT_MARGINS);
    m_ssidTitleLabel = new QLabel(this);
    m_ssidTitleLabel->setText("SSID");
    m_ssidLabel = new QLabel(this);
    m_ssidLabel->setText(m_wirelessNetItem.m_NetSsid);

    m_ssidLayout->addWidget(m_ssidTitleLabel);
    m_ssidLayout->addStretch();
    m_ssidLayout->addWidget(m_ssidLabel);

    m_securityPage = new SecurityPage(this);
    m_securityPage->setSecurity(KySecuType::WPA_AND_WPA2_ENTERPRISE);
    m_securityPage->setSecurityVisible(false);

    m_btnLayout = new QHBoxLayout();
    m_btnLayout->setSpacing(BUTTON_SPACING);
    m_cancelBtn = new QPushButton(this);
    m_connectBtn = new QPushButton(this);
    m_cancelBtn->setText(tr("Cancel"));
    m_connectBtn->setText(tr("Connect"));
    m_connectBtn->setEnabled(false);
    m_btnLayout->addStretch();
    m_btnLayout->addWidget(m_cancelBtn);
    m_btnLayout->addWidget(m_connectBtn);
    m_mainLayout->addLayout(m_ssidLayout);
    m_mainLayout->addWidget(m_securityPage);
    m_mainLayout->addLayout(m_btnLayout);
    m_mainLayout->addStretch();

    this->setFixedSize(MAIN_SIZE_EXPAND);
    initConnections();
    onPaletteChanged();
}

void EnterpriseWlanDialog::centerToScreen()
{
    QDesktopWidget* m = QApplication::desktop();
    QRect desk_rect = m->screenGeometry(m->screenNumber(QCursor::pos()));
    int desk_x = desk_rect.width();
    int desk_y = desk_rect.height();
    int x = this->width();
    int y = this->height();
    this->move(desk_x / 2 - x / 2 + desk_rect.left(), desk_y / 2 - y / 2 + desk_rect.top());
}

void EnterpriseWlanDialog::initConnections()
{
    connect(m_closeBtn, &QPushButton::clicked, this, &EnterpriseWlanDialog::close);
    connect(m_cancelBtn, &QPushButton::clicked, this, &EnterpriseWlanDialog::close);
    connect(m_connectBtn, &QPushButton::clicked, this, &EnterpriseWlanDialog::onBtnConnectClicked);
    connect(m_securityPage, &SecurityPage::eapTypeChanged, this, &EnterpriseWlanDialog::onEapTypeChanged);
    connect(m_securityPage, &SecurityPage::setSecuPageState, this, [ = ](bool status) {
       m_connectBtn->setEnabled(status);
    });

    connect(qApp, &QApplication::paletteChanged, this, &EnterpriseWlanDialog::onPaletteChanged);
}

void EnterpriseWlanDialog::onPaletteChanged()
{
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

    this->setPalette(pal);

    setFramePalette(m_securityPage, pal);
}

void EnterpriseWlanDialog::initData()
{
    if (m_wirelessNetItem.m_isConfigured) {
        KyEapMethodType type;
        m_connectOperation->getEnterpiseEapMethod(m_wirelessNetItem.m_connectUuid, type);
        if (type) {
            onEapTypeChanged(type);
        } else {
            qWarning() << "Get eap type failed!" << Q_FUNC_INFO << __LINE__;
        }
    }
}

void EnterpriseWlanDialog::onBtnConnectClicked()
{
    qDebug() << "Clicked connectBtn, will connect enterprise WLAN, ssid = "
             << m_wirelessNetItem.m_NetSsid << "." << Q_FUNC_INFO << __LINE__;
    KySecuType secuType;
    KyEapMethodType eapType;
    m_securityPage->getSecuType(secuType, eapType);

    KyWirelessConnectSetting connetSetting;
    connetSetting.setConnectName(m_wirelessNetItem.m_NetSsid);
    connetSetting.setIfaceName(m_deviceName);
    connetSetting.isAutoConnect = true; //ZJP_TODO 自动连接选项
    connetSetting.m_type = KyKeyMgmt::WpaEap;
    connetSetting.m_ssid = m_wirelessNetItem.m_NetSsid;
    connetSetting.m_secretFlag = 0;
    connetSetting.dumpInfo();

    if (eapType  == KyEapMethodType::TLS) {
        m_info.tlsInfo.devIfaceName = m_deviceName;
         m_securityPage->updateTlsChange(m_info.tlsInfo);
        m_connectOperation->addAndActiveWirelessEnterPriseTlsConnect(m_info.tlsInfo, connetSetting, m_deviceName, false);
    } else if (eapType == KyEapMethodType::PEAP) {
        m_securityPage->updatePeapChange(m_info.peapInfo);
        m_connectOperation->addAndActiveWirelessEnterPrisePeapConnect(m_info.peapInfo, connetSetting, m_deviceName, false);
    } else if (eapType == KyEapMethodType::TTLS) {
        m_securityPage->updateTtlsChange(m_info.ttlsInfo);
        m_connectOperation->addAndActiveWirelessEnterPriseTtlsConnect(m_info.ttlsInfo, connetSetting, m_deviceName, false);
    } else {
        qWarning() << "Connect enterprise wlan failed!(Unknown eap type)" << Q_FUNC_INFO << __LINE__;
    }
    close();
}

void EnterpriseWlanDialog::onEapTypeChanged(const KyEapMethodType &type)
{
    switch (type) {
    case KyEapMethodType::TLS:
        if (!m_wirelessNetItem.m_connectUuid.isEmpty()) {
            m_resource->getEnterPriseInfoTls(m_wirelessNetItem.m_connectUuid, m_info.tlsInfo);
        }
        this->setFixedSize(MAIN_SIZE_EXPAND);
        break;
    case KyEapMethodType::PEAP:
        if (m_wirelessNetItem.m_connectUuid.isEmpty()) {
            m_resource->getEnterPriseInfoPeap(m_wirelessNetItem.m_connectUuid, m_info.peapInfo);
            }
        this->setFixedSize(MAIN_SIZE_NARROW);
        break;
    case KyEapMethodType::TTLS:
        if (!m_wirelessNetItem.m_connectUuid.isEmpty()) {
            m_resource->getEnterPriseInfoTtls(m_wirelessNetItem.m_connectUuid, m_info.ttlsInfo);
        }
        this->setFixedSize(MAIN_SIZE_NARROW);
        break;
    default:
        break;
    }
}
