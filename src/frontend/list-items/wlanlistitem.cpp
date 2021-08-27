#include "wlanlistitem.h"
#include <QResizeEvent>

WlanListItem::WlanListItem(KyWirelessNetResource *resource, KyWirelessNetItem *data, QString device, QWidget *parent) : ListItem(parent)
{
    m_data = data;
    m_resource = resource;
    m_wlanDevice = device;
    m_connoperation = new KyWirelessConnectOperation(this);
    initWlanUI();
    setExpanded(false);
    initWlanConnection();
}

WlanListItem::WlanListItem(QWidget *parent) : ListItem(parent)
{
    m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-none-symbolic", QIcon(":/res/w/wifi-none.png")));
    const QString str="未连接任何wifi";
    setExpanded(false);
    this->setName(str);
    this->m_netButton->setEnabled(false);
    this->m_infoButton->hide();
}

WlanListItem::~WlanListItem()
{
    disconnect(m_resource, &KyWirelessNetResource::secuTypeChange, this, &WlanListItem::onSecurityChanged);
    disconnect(m_resource, &KyWirelessNetResource::connectionAdd, this, &WlanListItem::onConnectionAdd);
    disconnect(m_resource, &KyWirelessNetResource::connectionRemove, this, &WlanListItem::onConnectionRemove);
}

void WlanListItem::setWlanSignal(const int &signal)
{
    m_data->m_signalStrength = signal;
    refreshIcon();
}

void WlanListItem::setWlanState(const int &state)
{
//    m_data->m_state = state; //ZJP_TODO 后端接口待补全
    refreshIcon();
}

void WlanListItem::setExpanded(const bool &expanded)
{
    if (!m_pwdFrame || !m_autoConnectFrame) {
        this->setFixedHeight(NORMAL_HEIGHT);
        return;
    }
    this->setFixedHeight(expanded ? EXPANDED_HEIGHT : NORMAL_HEIGHT);

    m_pwdFrame->setVisible(expanded);
    m_autoConnectFrame->setVisible(expanded);
    emit this->itemHeightChanged(m_data->m_NetSsid);
}

void WlanListItem::setActivated(bool activated)
{
    this->m_isActivated = activated;
}

void WlanListItem::resizeEvent(QResizeEvent *event)
{
    this->blockSignals(true);
    if (this->height() == EXPANDED_HEIGHT) {
        this->setExpanded(true);
    } else {
        this->setExpanded(false);
    }
    this->blockSignals(false);
    return ListItem::resizeEvent(event);
}

void WlanListItem::initWlanUI()
{
    m_hasPwd = (m_data->m_secuType.isEmpty() || m_data->m_secuType == "") ? false : true;
    //设置显示的Wlan名称
    this->setName((m_data->m_connName != "") ? m_data->m_connName : m_data->m_NetSsid);
    //刷新左侧按钮图标
    refreshIcon();

#define PWD_AREA_HEIGHT 36
#define CONNECT_BUTTON_WIDTH 96
#define FRAME_CONTENT_MARGINS 56,4,16,4
#define FRAME_SPACING 8
#define PWD_CONTENT_MARGINS 8,0,32,0
#define SHOW_PWD_BUTTON_SIZE 16,16
#define PWD_LAYOUT_MARGINS 8,0,8,0
    //密码输入区域的UI
    m_pwdFrame = new QFrame(this);
    m_pwdFrameLyt = new QHBoxLayout(m_pwdFrame);
    m_pwdFrameLyt->setContentsMargins(FRAME_CONTENT_MARGINS);
    m_pwdFrameLyt->setSpacing(FRAME_SPACING);
    m_pwdFrame->setLayout(m_pwdFrameLyt);
    {
        m_pwdLineEdit  = new QLineEdit(m_pwdFrame);
        connect(m_pwdLineEdit, &QLineEdit::textChanged, this, &WlanListItem::onPwdEditorTextChanged);
        m_pwdLineEdit->setFixedHeight(PWD_AREA_HEIGHT);
        m_pwdLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
        m_pwdLineEditLyt = new QHBoxLayout(m_pwdLineEdit);
        m_pwdLineEditLyt->setContentsMargins(PWD_LAYOUT_MARGINS);
        m_pwdLineEdit->setLayout(m_pwdLineEditLyt);
        m_showPwdButton = new QPushButton(m_pwdLineEdit);
        m_showPwdButton->setFixedSize(SHOW_PWD_BUTTON_SIZE);
        connect(m_showPwdButton, &QPushButton::clicked, this, &WlanListItem::onShowPwdButtonClicked);
        m_pwdLineEditLyt->addStretch();
        m_pwdLineEditLyt->addWidget(m_showPwdButton);
    }
    m_connectButton = new QPushButton(m_pwdFrame);
    m_connectButton->setFixedSize(CONNECT_BUTTON_WIDTH, PWD_AREA_HEIGHT);
    m_connectButton->setText(tr("Connect"));
    m_connectButton->setEnabled(false);
    connect(m_connectButton, &QPushButton::clicked, this, &WlanListItem::onConnectButtonClicked);
    m_pwdFrameLyt->addWidget(m_pwdLineEdit);
    m_pwdFrameLyt->addWidget(m_connectButton);

    //自动连接选择区域UI
    m_autoConnectFrame = new QFrame(this);
    m_autoConnectFrameLyt = new QHBoxLayout(m_autoConnectFrame);
    m_autoConnectFrameLyt->setContentsMargins(FRAME_CONTENT_MARGINS);
    m_autoConnectFrameLyt->setSpacing(FRAME_SPACING);
    m_autoConnectFrame->setLayout(m_autoConnectFrameLyt);
    m_autoConnectCheckBox = new QCheckBox(m_autoConnectFrame);
    m_autoConnectCheckBox->setChecked(true);
    m_autoConnectCheckBox->setFixedSize(SHOW_PWD_BUTTON_SIZE);
    m_autoConnectLabel = new QLabel(m_autoConnectFrame);
    m_autoConnectLabel->setText(tr("Auto Connect"));
    m_autoConnectFrameLyt->addWidget(m_autoConnectCheckBox);
    m_autoConnectFrameLyt->addWidget(m_autoConnectLabel);
    m_autoConnectFrameLyt->addStretch();

    m_mainLayout->addWidget(m_pwdFrame);
    m_mainLayout->addWidget(m_autoConnectFrame);
    m_pwdFrame->hide();
    m_autoConnectFrame->hide();
}

void WlanListItem::initWlanConnection()
{
    connect(m_resource, &KyWirelessNetResource::secuTypeChange, this, &WlanListItem::onSecurityChanged);
    connect(m_resource, &KyWirelessNetResource::connectionAdd, this, &WlanListItem::onConnectionAdd);
    connect(m_resource, &KyWirelessNetResource::connectionRemove, this, &WlanListItem::onConnectionRemove);
}

void WlanListItem::refreshIcon()
{
#define FULL_SIGNAL 5
#define EXCELLENT_SIGNAL 4
#define GOOD_SIGNAL 3
#define OK_SIGNAL 2
#define LOW_SIGNAL 1
#define STEP 25
//    if (m_data->m_state) //ZJP_TODO 连接中、已连接、未连接的处理，要单独写逻辑，后端接口待补全
    if (!m_hasPwd) {
        //ZJP_TODO 无加密 注意信号格数计算方式，可能需要修改
        switch (m_data->m_signalStrength / STEP + 1) {
        case FULL_SIGNAL:
        case EXCELLENT_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-excellent-symbolic", QIcon(":/res/w/wifi-full.png")));
            break;
        case GOOD_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-good-symbolic", QIcon(":/res/w/wifi-high.png")));
            break;
        case OK_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-ok", QIcon(":/res/w/wifi-medium.png")));
            break;
        case LOW_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-low", QIcon(":/res/w/wifi-low.png")));
            break;
        default:
            qDebug() << "Set wlan(without passwd) icon failed, signal = " << m_data->m_signalStrength << Q_FUNC_INFO << __LINE__;
            break;
        }
    } else {
        //ZJP_TODO 有加密
        switch (m_data->m_signalStrength / STEP + 1) {
        case FULL_SIGNAL:
        case EXCELLENT_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-excellent-secure-symbolic", QIcon(":/res/w/wifi-full-pwd.png")));
            break;
        case GOOD_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-good-secure-symbolic", QIcon(":/res/w/wifi-high-pwd.png")));
            break;
        case OK_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-ok-secure-symbolic", QIcon(":/res/w/wifi-medium-pwd.png")));
            break;
        case LOW_SIGNAL:
            m_netButton->setIcon(QIcon::fromTheme("network-wireless-signal-low-secure-symbolic", QIcon(":/res/w/wifi-low-pwd.png")));
            break;
        default:
            qDebug() << "Set wlan(with passwd) icon failed, signal = " << m_data->m_signalStrength << Q_FUNC_INFO << __LINE__;
            break;
        }
    }
}

void WlanListItem::onInfoButtonClicked()
{
    //ZJP_TODO 呼出无线详情页
    if(m_data){
        qDebug() << "On wlan info button clicked! ssid = " << m_data->m_NetSsid << "; name = " << m_data->m_connName << "." <<Q_FUNC_INFO << __LINE__;
    }
    else{
        qDebug() << "On wlan info button clicked! But there is no wlan connect " ;
    }

}

void WlanListItem::onNetButtonClicked()
{
    if(!m_data){
        qDebug() << "On wlan  clicked! But there is no wlan connect!" << Q_FUNC_INFO << __LINE__;
        return;
    }
    qDebug() << "On wlan clicked! ssid = " << m_data->m_NetSsid << "; name = " << m_data->m_connName << "." << Q_FUNC_INFO << __LINE__;

    //判断当前item处于连接还是断开对比activessid
    QString activedssid;
    QMap<QString,QStringList> actMap;
    m_resource->getWirelessActiveConnection(NetworkManager::ActiveConnection::State::Activated, actMap);
    QMap<QString,QStringList>::iterator iter = actMap.begin();
    while (iter != actMap.end()) {
        if (iter.key() == m_wlanDevice && !iter.value().isEmpty()) {
            activedssid = iter.value().at(0);
        }
        break;
    }
    qDebug()<<"Get activated wlan succeed! ssid = " << activedssid <<Q_FUNC_INFO << __LINE__;

    //执行连接或断开
    if (m_data->m_NetSsid == activedssid) {
        m_connoperation->deActivateWirelessConnection(m_wlanDevice,m_data->m_connectUuid);
        qDebug()<<"Clicked on connected wifi, it will be inactivated. ssid = " << m_data->m_NetSsid << Q_FUNC_INFO << __LINE__;
        return;
    }

    if (m_data->m_isConfigured) {
        m_connoperation->activeWirelessConnect(m_wlanDevice,m_data->m_connectUuid);
        qDebug()<<"Has configuration, will be activated. ssid = " << m_data->m_NetSsid << Q_FUNC_INFO << __LINE__;
        return;
    }
    if (!this->m_connectButton->isVisible() && m_data->m_secuType != "") {
        this->setExpanded(true);
    } else {
        onConnectButtonClicked();
    }
}

//void WlanListItem::onNameLableClicked()
//{
//    m_mainLayout = new QVBoxLayout(this);
//    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
//    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);
//    this->setLayout(m_mainLayout);

//    m_itemFrame = new QFrame(this);

//    m_hItemLayout = new QHBoxLayout(m_itemFrame);
//    m_hItemLayout->setContentsMargins(ITEM_FRAME_MARGINS);
//    m_hItemLayout->setSpacing(ITEM_FRAME_SPACING);

//    m_lineEdit = new QLineEdit(m_itemFrame);

//    m_hItemLayout->addWidget(m_lineEdit);
//    m_hItemLayout->addStretch();

//    m_mainLayout->addWidget(m_itemFrame);
//}

void WlanListItem::onSecurityChanged(QString interface, QString ssid, QString securityType)
{
    if (ssid != m_data->m_NetSsid) {
        return;
    }
    qDebug() << "Security changed! ssid = " << m_data->m_NetSsid << "; security = " << m_data->m_secuType << "." <<Q_FUNC_INFO << __LINE__;
    m_data->m_secuType = securityType;
    bool newSecu = (m_data->m_secuType.isEmpty() || m_data->m_secuType == "") ? false : true;
    if (m_hasPwd^newSecu) {
        m_hasPwd = newSecu;
        refreshIcon();
    }
}

void WlanListItem::onPwdEditorTextChanged()
{
    if (!m_pwdLineEdit || !m_connectButton) { return; }
    if (m_pwdLineEdit->text().length() < 6) {
        m_connectButton->setEnabled(false);
    } else {
        m_connectButton->setEnabled(true);
    }
}

void WlanListItem::onShowPwdButtonClicked()
{
    if (!m_pwdLineEdit) {
        return;
    }
    if (m_pwdLineEdit->echoMode() == QLineEdit::EchoMode::Password) {
        //TODO 按钮图标要发生改变

        m_pwdLineEdit->setEchoMode(QLineEdit::EchoMode::Normal);
    } else {
        //TODO 按钮图标要发生改变
        m_pwdLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    }
}

void WlanListItem::onConnectButtonClicked()
{
    if (!m_connectButton->isEnabled() || !m_data) { return; }
    KyWirelessConnectSetting settings;
    settings.m_connectName = m_data->m_NetSsid;
    settings.m_ssid = m_data->m_NetSsid;
    settings.isAutoConnect = m_autoConnectCheckBox->isChecked();
    settings.m_psk = m_pwdLineEdit->text();
    if (m_data->m_secuType.isEmpty() || m_data->m_secuType == "") {
        settings.m_type = WpaNone;
    } else if (m_data->m_secuType.contains("WPA1") || m_data->m_secuType.contains("WPA2")) {
        settings.m_type = WpaPsk;
    } else if (m_data->m_secuType.contains("WPA3")) {
        settings.m_type = WpaEap;
    }
    qDebug() << "On button connect clicked, will connect wlan. ssid = " << m_data->m_NetSsid << Q_FUNC_INFO <<__LINE__;

    emit this->connectButtonClicked(settings, false);
}

void WlanListItem::onConnectionAdd(QString deviceName, QString ssid)
{
    if (!m_data) { return; }
    if (ssid == m_data->m_NetSsid) {
        m_data->m_isConfigured = true;
    }
}

void WlanListItem::onConnectionRemove(QString deviceName, QString ssid)
{
    if (!m_data) { return; }
    if (ssid == m_data->m_NetSsid) {
        m_data->m_isConfigured = false;
    }
}
