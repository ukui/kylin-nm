#include "wlanlistitem.h"
#include <QResizeEvent>
#include "enterprisewlandialog.h"

#define EMPTY_SSID "EMPTY_SSID"
#define LOG_FLAG "[WlanListItem]"
#define WAIT_US  10*1000

WlanListItem::WlanListItem(KyWirelessNetItem &wirelessNetItem, QString device, QWidget *parent) : ListItem(parent)
{
    m_wlanDevice = device;
    m_wirelessNetItem = wirelessNetItem;

    qDebug()<<"[WlanPage] wlan list item is created." << m_wirelessNetItem.m_NetSsid;

    initWlanUI();
    setExpanded(false);

    connect(this->m_infoButton, &InfoButton::clicked, this, &WlanListItem::onInfoButtonClicked);
    connect(m_menu, &QMenu::triggered, this, &WlanListItem::onMenuTriggered);

    m_wirelessConnectOperation = new KyWirelessConnectOperation(this);
}

WlanListItem::WlanListItem(QWidget *parent) : ListItem(parent)
{
    m_wirelessNetItem.m_NetSsid = EMPTY_SSID;

    qDebug()<<"[WlanPage] wlan list item is created." << m_wirelessNetItem.m_NetSsid;

    m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-none-symbolic"));
    m_netButton->setDefaultPixmap();
    const QString name = tr("Not connected");
    setExpanded(false);
    this->setName(name);
    this->m_netButton->setEnabled(false);
    this->m_infoButton->hide();
}

WlanListItem::~WlanListItem()
{
    qDebug()<<"[WlanPage] wlan list item is deleted." << m_wirelessNetItem.m_NetSsid;
}

QString WlanListItem::getSsid()
{
    return m_wirelessNetItem.m_NetSsid;
}

void WlanListItem::setSignalStrength(const int &signal)
{
    m_wirelessNetItem.m_signalStrength = signal;
    if (Activated == m_connectState) {
        refreshIcon(true);
    } else if (Deactivated == m_connectState) {
        refreshIcon(false);
    }

    return;
}

int WlanListItem::getSignalStrength()
{
    return m_wirelessNetItem.m_signalStrength;
}

bool WlanListItem::isConfigured()
{
    return m_wirelessNetItem.m_isConfigured;
}

void WlanListItem::setWlanState(const int &state)
{
//    m_wirelessNetItem.m_state = state; //ZJP_TODO 后端接口待补全
    refreshIcon(false);
}

void WlanListItem::setExpanded(const bool &expanded)
{
    if (!m_pwdFrame || !m_autoConnectFrame) {
        this->setFixedHeight(NORMAL_HEIGHT);
        return;
    }

    m_pwdFrame->setVisible(expanded);
    m_autoConnectFrame->setVisible(expanded);

    if (expanded) {
        m_pwdLineEdit->setFocus();
        setFixedHeight(EXPANDED_HEIGHT);
    } else {
        setFixedHeight(NORMAL_HEIGHT);
        QPalette pal = qApp->palette();
        pal.setColor(QPalette::Window, qApp->palette().base().color());
        this->setPalette(pal);
    }

    emit this->itemHeightChanged(expanded, m_wirelessNetItem.m_NetSsid);

    return;
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

void WlanListItem::onRightButtonClicked()
{
    qDebug()<< LOG_FLAG <<"onRightButtonClicked";

    if (!m_menu) {
        return;
    }

    m_menu->clear();

    if (Activated == m_connectState || Activating == m_connectState) {
        m_menu->addAction(new QAction(tr("Disconnect"), this));
    } else if (Deactivated == m_connectState) {
        m_menu->addAction(new QAction(tr("Connect"), this));
        qDebug() << "add connect action";
    } else {
        return;
    }

    if (m_wirelessNetItem.m_isConfigured) {
        m_menu->addAction(new QAction(tr("Forget"), this));
    }

    m_menu->move(cursor().pos());
    m_menu->show();

    return;
}

void WlanListItem::enterEvent(QEvent *event)
{
    //qDebug()<< LOG_FLAG <<"enterEvent" << m_wirelessNetItem.m_NetSsid;
    m_mouseIsOut = false;
    return ListItem::enterEvent(event);
}

void WlanListItem::leaveEvent(QEvent *event)
{
    //qDebug()<< LOG_FLAG <<"leaveEvent"<< m_wirelessNetItem.m_NetSsid;
    m_mouseIsOut = true;
    if (m_pwdFrame && m_pwdFrame->isVisible()) {
        if (m_focusIsOut) {
            setExpanded(false);
        }
        return QFrame::leaveEvent(event);
    }

    return ListItem::leaveEvent(event);
}

bool WlanListItem::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_pwdLineEdit) {
        if (event->type() == QEvent::FocusOut) {
            m_focusIsOut = true;
            //qDebug()<< LOG_FLAG <<"focusOutEvent" << m_wirelessNetItem.m_NetSsid;
            if (m_mouseIsOut) {
                setExpanded(false);
            }
        } else if (event->type() == QEvent::FocusIn) {
            //qDebug()<< LOG_FLAG <<"focusInEvent" << m_wirelessNetItem.m_NetSsid;
            m_focusIsOut = false;
        }
    }

    return QFrame::eventFilter(watched, event);
}

void WlanListItem::initWlanUI()
{
    m_hasPwd = (m_wirelessNetItem.m_secuType.isEmpty() || m_wirelessNetItem.m_secuType == "") ? false : true;
    //设置显示的Wlan名称
//    this->setName((m_wirelessNetItem.m_connName != "") ? m_wirelessNetItem.m_connName : m_wirelessNetItem.m_NetSsid);
    this->setName(m_wirelessNetItem.m_NetSsid);
    //刷新左侧按钮图标
    refreshIcon(false);

#define PWD_AREA_HEIGHT 36
#define CONNECT_BUTTON_WIDTH 96
#define FRAME_CONTENT_MARGINS 56,4,16,4
#define FRAME_SPACING 8
#define PWD_CONTENT_MARGINS 8,0,34,0
#define SHOW_PWD_BUTTON_SIZE 24,24
#define PWD_LAYOUT_MARGINS 8,0,8,0
    //密码输入区域的UI
    m_pwdFrame = new QFrame(this);
    m_pwdFrameLyt = new QHBoxLayout(m_pwdFrame);
    m_pwdFrameLyt->setContentsMargins(FRAME_CONTENT_MARGINS);
    m_pwdFrameLyt->setSpacing(FRAME_SPACING);
    m_pwdFrame->setLayout(m_pwdFrameLyt);

    m_pwdLineEdit  = new QLineEdit(m_pwdFrame);
    m_pwdLineEdit->installEventFilter(this);
    connect(m_pwdLineEdit, &QLineEdit::textChanged, this, &WlanListItem::onPwdEditorTextChanged);
    m_pwdLineEdit->setFixedHeight(PWD_AREA_HEIGHT);
    m_pwdLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    m_pwdLineEdit->setTextMargins(PWD_CONTENT_MARGINS);
    m_pwdFrameLyt->addWidget(m_pwdLineEdit);

    m_pwdLineEditLyt = new QHBoxLayout(m_pwdLineEdit);
    m_pwdLineEditLyt->setContentsMargins(PWD_LAYOUT_MARGINS);
    m_pwdLineEdit->setLayout(m_pwdLineEditLyt);

    m_showPwdButton = new QPushButton(m_pwdLineEdit);
    m_showPwdButton->setFixedSize(SHOW_PWD_BUTTON_SIZE);
    m_showPwdButton->setAutoFillBackground(false);
    m_showPwdButton->setIcon(QIcon(":/res/h/hide-pwd.png"));
    m_showPwdButton->setCursor(Qt::PointingHandCursor);
    connect(m_showPwdButton, &QPushButton::clicked, this, &WlanListItem::onShowPwdButtonClicked);
    m_pwdLineEditLyt->addStretch();
    m_pwdLineEditLyt->addWidget(m_showPwdButton);

    m_connectButton = new QPushButton(m_pwdFrame);
    m_connectButton->setFixedSize(CONNECT_BUTTON_WIDTH, PWD_AREA_HEIGHT);
    m_connectButton->setText(tr("Connect"));
    m_connectButton->setEnabled(false);
    connect(m_connectButton, &QPushButton::clicked, this, &WlanListItem::onConnectButtonClicked);
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
    m_autoConnectFrameLyt->addWidget(m_autoConnectCheckBox);

    m_autoConnectLabel = new QLabel(m_autoConnectFrame);
    m_autoConnectLabel->setText(tr("Auto Connect"));
    m_autoConnectFrameLyt->addWidget(m_autoConnectLabel);
    m_autoConnectFrameLyt->addStretch();

    m_mainLayout->addWidget(m_pwdFrame);
    m_mainLayout->addWidget(m_autoConnectFrame);

    m_pwdFrame->hide();
    m_autoConnectFrame->hide();
}

void WlanListItem::refreshIcon(bool isActivated)
{
#define FULL_SIGNAL 5
#define EXCELLENT_SIGNAL 4
#define GOOD_SIGNAL 3
#define OK_SIGNAL 2
#define LOW_SIGNAL 1
#define STEP 25
    if (!m_hasPwd) {
        //ZJP_TODO 无加密 注意信号格数计算方式，可能需要修改
        switch (m_wirelessNetItem.m_signalStrength / STEP + 1) {
        case FULL_SIGNAL:
        case EXCELLENT_SIGNAL:
            m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-excellent-symbolic",
                                                        QIcon(":/res/w/wifi-full.png")));
            break;
        case GOOD_SIGNAL:
            m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-good-symbolic",
                                                        QIcon(":/res/w/wifi-high.png")));
            break;
        case OK_SIGNAL:
            m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-ok",
                                                        QIcon(":/res/w/wifi-medium.png")));
            break;
        case LOW_SIGNAL:
            m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-low",
                                                        QIcon(":/res/w/wifi-low.png")));
            break;
        default:
            qDebug() << "Set wlan(without passwd) icon failed, signal = "
                     << m_wirelessNetItem.m_signalStrength << Q_FUNC_INFO << __LINE__;
            break;
        }
    } else {
        //ZJP_TODO 有加密
        switch (m_wirelessNetItem.m_signalStrength / STEP + 1) {
        case FULL_SIGNAL:
        case EXCELLENT_SIGNAL:
            m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-excellent-secure-symbolic",
                                                        QIcon(":/res/w/wifi-full-pwd.png")));
            break;
        case GOOD_SIGNAL:
            m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-good-secure-symbolic",
                                                        QIcon(":/res/w/wifi-high-pwd.png")));
            break;
        case OK_SIGNAL:
            m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-ok-secure-symbolic",
                                                        QIcon(":/res/w/wifi-medium-pwd.png")));
            break;
        case LOW_SIGNAL:
            m_netButton->setButtonIcon(QIcon::fromTheme("network-wireless-signal-low-secure-symbolic",
                                                        QIcon(":/res/w/wifi-low-pwd.png")));
            break;
        default:
            qDebug() << "Set wlan(with passwd) icon failed, signal = "
                     << m_wirelessNetItem.m_signalStrength << Q_FUNC_INFO << __LINE__;
            break;
        }
    }
    m_netButton->setActive(isActivated);
}

void WlanListItem::onInfoButtonClicked()
{
    //ZJP_TODO 呼出无线详情页
    if(isDetailShow){
        qDebug() << "has show the detail page,and do not show again" << Q_FUNC_INFO << __LINE__;
        return;
    }

    qDebug() << LOG_FLAG << "Net active or not:"<< m_connectState;
    qDebug() << LOG_FLAG << "On wlan info button clicked! ssid = "
             << m_wirelessNetItem.m_NetSsid << "; name = "
             << m_wirelessNetItem.m_connName << "." <<Q_FUNC_INFO << __LINE__;


    bool isActive = false;
    if (Activated == m_connectState) {
        isActive = true;
    }

    NetDetail *netDetail = new NetDetail(m_wlanDevice, m_wirelessNetItem.m_NetSsid,
                                         m_wirelessNetItem.m_connectUuid, isActive, true,
                                         !m_wirelessNetItem.m_isConfigured, this);
    connect(netDetail, &NetDetail::detailPageClose, this, &WlanListItem::onDetailShow);
    netDetail->show();
    emit this->detailShow(true);
}

void WlanListItem::onNetButtonClicked()
{
    qDebug() << LOG_FLAG << "onNetButtonClicked";
    if (m_wirelessNetItem.m_NetSsid == EMPTY_SSID) {
        return;
    }

    if (Deactivated != m_connectState) {
        qDebug() << LOG_FLAG <<"the connection" << m_wirelessNetItem.m_connName
                 << "is not deactived, so it can not be operation." << Q_FUNC_INFO << __LINE__;
        return;
    }

    //有配置或者无密码的wifi直接连接
    if (m_wirelessNetItem.m_isConfigured) {
        m_wirelessConnectOperation->activeWirelessConnect(m_wlanDevice, m_wirelessNetItem.m_connectUuid);
        qDebug()<<"[WlanListItem] Has configuration, will be activated. ssid = "
               << m_wirelessNetItem.m_NetSsid << Q_FUNC_INFO << __LINE__;
        return;
    }

    if (!this->m_connectButton->isVisible() && m_wirelessNetItem.m_secuType != "") {
        if (m_wirelessNetItem.m_secuType.contains("802.1x", Qt::CaseInsensitive)) {
            EnterpriseWlanDialog *enterpriseWlanDialog = new EnterpriseWlanDialog(m_wirelessNetItem, m_wlanDevice, this);
            enterpriseWlanDialog->show();
        } else {
            this->setExpanded(true);
        }
    } else {
        onConnectButtonClicked();
    }

    return;
}

void WlanListItem::updateWirelessNetSecurity(QString ssid, QString securityType)
{
    if (ssid != m_wirelessNetItem.m_NetSsid) {
        return;
    }

    qDebug() << LOG_FLAG << "Security changed! ssid = " << m_wirelessNetItem.m_NetSsid
             << "; security = " << m_wirelessNetItem.m_secuType << "." <<Q_FUNC_INFO << __LINE__;

    m_wirelessNetItem.m_secuType = securityType;
    bool newSecu = (m_wirelessNetItem.m_secuType.isEmpty() || m_wirelessNetItem.m_secuType == "") ? false : true;
    if (m_hasPwd^newSecu) {
        m_hasPwd = newSecu;
        refreshIcon(false);
    }

    return;
}


void WlanListItem::updateWirelessNetItem(KyWirelessNetItem &wirelessNetItem)
{
    m_wirelessNetItem = wirelessNetItem;
    return;
}

void WlanListItem::onPwdEditorTextChanged()
{
    if (!m_pwdLineEdit || !m_connectButton) {
        return;
    }

    if (m_pwdLineEdit->text().length() < PWD_LENGTH_LIMIT) {
        m_connectButton->setEnabled(false);
    } else {
        m_connectButton->setEnabled(true);
    }

    return;
}

void WlanListItem::onShowPwdButtonClicked()
{
    qDebug()<< LOG_FLAG << "onShowPwdButtonClicked";
    if (!m_pwdLineEdit) {
        return;
    }

    if (m_pwdLineEdit->echoMode() == QLineEdit::EchoMode::Password) {
        m_showPwdButton->setIcon(QIcon(":/res/h/show-pwd.png"));
        m_pwdLineEdit->setEchoMode(QLineEdit::EchoMode::Normal);
    } else {
        m_showPwdButton->setIcon(QIcon(":/res/h/hide-pwd.png"));
        m_pwdLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    }

    return;
}

void WlanListItem::onConnectButtonClicked()
{
    qDebug()<< LOG_FLAG << "onConnectButtonClicked";
    if ((Activating == m_connectState || Deactivating == m_connectState)) {
        qDebug() << LOG_FLAG << "On wlan  clicked! But there is nothing to do because it is already activating/deactivating!"
                 << Q_FUNC_INFO << __LINE__;
        return;
    }

    if (m_connectButton->isVisible() && !m_connectButton->isEnabled()) {
        qWarning() << "Connect wlan failed because of null pointer or button state!" << Q_FUNC_INFO << __LINE__;
        return;
    }

    KyWirelessConnectSetting settings;
    settings.m_connectName = m_wirelessNetItem.m_NetSsid;
    settings.m_ssid = m_wirelessNetItem.m_NetSsid;
    settings.isAutoConnect = m_autoConnectCheckBox->isChecked();
    settings.m_psk = m_pwdLineEdit->text();  
    if (m_wirelessNetItem.m_secuType.isEmpty() || m_wirelessNetItem.m_secuType == "") {
        settings.m_type = WpaNone;
    } else if (m_wirelessNetItem.m_secuType.contains("WPA1") || m_wirelessNetItem.m_secuType.contains("WPA2")) {
        settings.m_type = WpaPsk;
    } else if (m_wirelessNetItem.m_secuType.contains("WPA3")) {
        settings.m_type = SAE;
    }

    qDebug() << "[WlanListItem] On button connect clicked, will connect wlan. ssid = "
             << m_wirelessNetItem.m_NetSsid << Q_FUNC_INFO <<__LINE__;

    m_wirelessConnectOperation->addAndActiveWirelessConnect(m_wlanDevice, settings, false);
    setExpanded(false);
    return;
}

ConnectState WlanListItem::getConnectionState()
{
    return m_connectState;
}

void WlanListItem::updateConnectState(ConnectState state)
{
    m_connectState = state;

    if (Activated == state) {
        m_netButton->stopLoading();
        m_netButton->setActive(true);
    } else if(Deactivated == state) {
        qDebug() << "[WlanListItem] stop loading connect state:" << state;
        m_netButton->stopLoading();
        m_netButton->setActive(false);
    } else {
        qDebug() << "[WlanListItem] start loading connect state:" << state;
        m_netButton->startLoading();
    }

    return;
}

void WlanListItem::onMenuTriggered(QAction *action)
{
    if (action->text() == tr("Connect")) {
        this->onNetButtonClicked();
    } else if (action->text() == tr("Disconnect")) {
        m_wirelessConnectOperation->deActivateWirelessConnection(m_wlanDevice, m_wirelessNetItem.m_connectUuid);
        qDebug()<<"[WlanListItem] Clicked on connected wifi, it will be inactivated. ssid = "
                        << m_wirelessNetItem.m_NetSsid << Q_FUNC_INFO << __LINE__;
    } else if (action->text() == tr("Forget")) {
        m_wirelessConnectOperation->deleteWirelessConnect(m_wirelessNetItem.m_connectUuid);
    }

    return;
}

void WlanListItem::forgetPwd()
{
    if (!this->isConfigured()) {
        m_pwdLineEdit->setText("");
        return;
    }
}


