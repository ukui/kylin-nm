#include "hiddenwifipage.h"
#include <QPainter>
#include <QDBusInterface>

#define  WINDOW_WIDTH  480
#define  MIN_WINDOW_HEIGHT  332
#define  PEAP_WINDOW_HEIGHT  494
#define  MAX_WINDOW_HEIGHT  540
#define  PAGE_LAYOUT_MARGINS  0,0,0,0
#define  TOP_LAYOUT_MARGINS  24,14,24,24
#define  BOTTOM_LAYOUT_MARGINS  24,24,24,24
#define  LAYOUT_SPACING  16
#define  LABEL_MIN_WIDTH  150
#define  MAX_NAME_LENGTH 32
#define  PSK_SCRO_HEIGHT  182
#define  PEAP_SCRO_HEIGHT  348
#define  TLS_SCRO_HEIGHT  540
#define  MEDIUM_WEIGHT_VALUE  57

HiddenWiFiPage::HiddenWiFiPage(QString interface, bool isSimple, QWidget *parent)
    : m_deviceName(interface),
      m_isSimple(isSimple),
      QWidget(parent)
{
    initUI();
    initComponent();

    setFixedWidth(WINDOW_WIDTH);
    setAttribute(Qt::WA_DeleteOnClose);

    if (m_isSimple) {
        setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
        setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口
        setWindowFlags(Qt::Popup);
    } else {
        setWindowFlag(Qt::Window);
    }

    setJoinBtnEnable();
    centerToScreen();
}

HiddenWiFiPage::~HiddenWiFiPage()
{
    delete m_bottomDivider;
}

void HiddenWiFiPage::getSecuType(KySecuType &secuType)
{
    secuType = (KySecuType)m_secuTypeCombox->currentData().toInt();
}

void HiddenWiFiPage::paintEvent(QPaintEvent *event)
{
    QPalette pal = qApp->palette();
    QPainter painter(this);

    if (m_isSimple) {
        //设置窗体为圆角
        QColor colorPal = pal.color(QPalette::Background);
        painter.setRenderHint(QPainter::Antialiasing); // 反锯齿;
        painter.setBrush(colorPal);
        painter.setPen(Qt::transparent);
        auto rect = this->rect();
        painter.drawRoundedRect(rect, 12, 12); // 窗口圆角
    } else {
        painter.setBrush(pal.color(QPalette::Base));
        painter.drawRect(this->rect());
        painter.fillRect(rect(), QBrush(pal.color(QPalette::Base)));
    }
    return QWidget::paintEvent(event);
}

bool HiddenWiFiPage::eventFilter(QObject *w, QEvent *event)
{
    return QWidget::eventFilter(w, event);
}

void HiddenWiFiPage::setBtnEnable(bool on)
{
    m_cancelBtn->setEnabled(on);
    m_joinBtn->setEnabled(on);
}

void HiddenWiFiPage::initUI()
{
    m_topWidget = new QWidget(this);
    m_centerWidget = new QWidget(this);
    m_bottomWidget = new QWidget(this);
    m_secuWidget = new EntSecurityWidget(true, this);

    m_descriptionLabel = new QLabel(this);
    m_nameLabel = new FixLabel(this);
    m_nameLabel->setFixedWidth(LABEL_MIN_WIDTH);
    m_secuTypeLabel = new QLabel(this);
    m_pwdLabel = new QLabel(this);
    m_emptyLabel = new QLabel(this);
    m_checkLabel = new QLabel(this);

    m_nameEdit =new LineEdit(this);
    m_secuTypeCombox = new QComboBox(this);
    m_pwdEdit = new KPasswordEdit(this);

    m_rememberCheckBox = new QCheckBox(this);
    m_bottomDivider = new Divider(this);
    m_cancelBtn =new QPushButton(this);
    m_joinBtn =new QPushButton(this);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    m_scrollArea->setStyleSheet("QWidget{border:1px solid rgba(255,0,0,1);}");//测试用

    m_pageLayout = new QVBoxLayout(this);
    m_pageLayout->setContentsMargins(PAGE_LAYOUT_MARGINS);
    m_pageLayout->setSpacing(0);
    m_pageLayout->addWidget(m_topWidget);
    m_pageLayout->addWidget(m_scrollArea);
    m_pageLayout->addWidget(m_bottomDivider);
    m_pageLayout->addWidget(m_bottomWidget);

    m_topLayout = new QHBoxLayout(m_topWidget);
    m_topLayout->setContentsMargins(TOP_LAYOUT_MARGINS);
    m_topLayout->setSpacing(0);
    m_topLayout->addWidget(m_descriptionLabel);
    m_topLayout->addStretch();

    m_centerVBoxLayout = new QVBoxLayout(m_centerWidget);
    m_centerVBoxLayout->setContentsMargins(24, 0, 24, 8);
    m_centerVBoxLayout->setSpacing(0);

    QWidget *centerWidget = new QWidget(this);
    QGridLayout *centerLayout = new QGridLayout(centerWidget);
    centerLayout->setContentsMargins(PAGE_LAYOUT_MARGINS);
    centerLayout->setColumnMinimumWidth(0, LABEL_MIN_WIDTH);
    centerLayout->setVerticalSpacing(LAYOUT_SPACING);
    //SSID Label和输入框 第0行，第0列，第1列
    centerLayout->addWidget(m_nameLabel, 0, 0);
    centerLayout->addWidget(m_nameEdit, 0, 1);
    //安全性 Label和选项框 第1行，第0列，第1列
    centerLayout->addWidget(m_secuTypeLabel, 1, 0);
    centerLayout->addWidget(m_secuTypeCombox, 1, 1);
    //密码 Label和密码框 第2行，第0列，第1列
    centerLayout->addWidget(m_pwdLabel, 2, 0);
    centerLayout->addWidget(m_pwdEdit, 2, 1);

    //记住该网络复选框
    QWidget *checkWidget = new QWidget(this);
    QHBoxLayout *checkLayout = new QHBoxLayout(checkWidget);
    checkLayout->setContentsMargins(PAGE_LAYOUT_MARGINS);
    m_emptyLabel->setMinimumWidth(LABEL_MIN_WIDTH);
    m_rememberCheckBox->setChecked(true);
    checkLayout->addWidget(m_emptyLabel);
    checkLayout->addWidget(m_rememberCheckBox);
    checkLayout->addWidget(m_checkLabel);
    checkLayout->addStretch();

    m_centerVBoxLayout->addWidget(centerWidget);
    m_centerVBoxLayout->addWidget(m_secuWidget);
    m_centerVBoxLayout->addWidget(checkWidget);
    m_centerVBoxLayout->addStretch();
    m_scrollArea->setWidget(m_centerWidget);
    m_scrollArea->setWidgetResizable(true);

    //底部按钮
    m_bottomLayout = new QHBoxLayout(m_bottomWidget);
    m_bottomLayout->setContentsMargins(BOTTOM_LAYOUT_MARGINS);
    m_bottomLayout->setSpacing(LAYOUT_SPACING);
    if (!m_isSimple) {
        m_showListBtn = new KBorderlessButton(this);
        m_showListBtn->setText(tr("Show Network List")); //显示网络列表
        m_bottomLayout->addWidget(m_showListBtn);
        connect(m_showListBtn, SIGNAL(clicked()), this, SLOT(onBtnShowListClicked()));
    }
    m_bottomLayout->addStretch();
    m_bottomLayout->addWidget(m_cancelBtn);
    m_bottomLayout->addWidget(m_joinBtn);

    m_secuTypeCombox->addItem(tr("None"),NONE);
    m_secuTypeCombox->addItem(tr("WPA&WPA2 Personal"),WPA_AND_WPA2_PERSONAL);
    m_secuTypeCombox->addItem(tr("WPA&WPA2 Enterprise"), WPA_AND_WPA2_ENTERPRISE);
    m_secuTypeCombox->addItem(tr("WPA3 Personal"), WPA3_PERSONAL);

     //请输入您想要加入网络的名称和安全类型
    m_descriptionLabel->setText(tr("Please enter the network name and security type"));
    QFont font = m_descriptionLabel->font();
    font.setWeight(MEDIUM_WEIGHT_VALUE);
    m_descriptionLabel->setFont(font);
    m_nameLabel->setText(tr("Network name(SSID)")); //网络名(SSID)
    m_secuTypeLabel->setText(tr("Security type")); //安全性
    m_pwdLabel->setText(tr("Password")); //密码
    m_checkLabel->setText(tr("Remember the Network")); //记住该网络
    m_cancelBtn->setText(tr("Cancel"));
    m_joinBtn->setText(tr("Join"));

    m_nameEdit->setMaxLength(MAX_NAME_LENGTH);
    m_nameEdit->setPlaceholderText(tr("Required")); //必填
    QString hint = tr("Required");
    m_pwdEdit->setPlaceholderText(hint);

    QRegExp rx("^[A-Za-z0-9`~!@#$%^&*()_-+=<>,.\\\/]+$");
    QRegExpValidator *latitude = new QRegExpValidator(rx, this);
    m_pwdEdit->setValidator(latitude);
    m_pwdEdit->setClearButtonEnabled(false); //禁用ClearBtn按钮

    this->setWindowTitle(tr("Find and Join Wi-Fi"));
    this->setWindowIcon(QIcon::fromTheme("kylin-network"));

    QPalette pa = m_scrollArea->palette();
    pa.setBrush(QPalette::Window, Qt::transparent);
    m_scrollArea->setPalette(pa);
    showNone();
}

void HiddenWiFiPage::centerToScreen()
{
    QDesktopWidget* m = QApplication::desktop();
    QRect desk_rect = m->screenGeometry(m->screenNumber(QCursor::pos()));
    int desk_x = desk_rect.width();
    int desk_y = desk_rect.height();
    int x = this->width();
    int y = this->height();
    this->move((desk_x - x)/ 2 , (desk_y - y)/ 2);
}

void HiddenWiFiPage::initComponent()
{
    connect(m_cancelBtn, &QPushButton::clicked, this, [=] {
        close();
    });

    connect(m_joinBtn, SIGNAL(clicked()), this, SLOT(on_btnJoin_clicked()));

    //安全类型变化
    connect(m_secuTypeCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &HiddenWiFiPage::onSecuTypeComboxIndexChanged);

    //按钮状态变化
    connect(m_nameEdit, &LineEdit::textChanged, this, &HiddenWiFiPage::setJoinBtnEnable);
    connect(m_pwdEdit, &LineEdit::textChanged, this, &HiddenWiFiPage::setJoinBtnEnable);
    connect(m_secuTypeCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &HiddenWiFiPage::setJoinBtnEnable);
    connect(m_secuWidget, &EntSecurityWidget::setSecuPageState, this, [=](bool status) {
       m_isSecuOk = status;
       setJoinBtnEnable();
    });
    connect(m_secuWidget, &EntSecurityWidget::eapTypeChanged, this, [=](KyEapMethodType type) {
       setWindowWidth(type);
    });
}

void HiddenWiFiPage::showNone()
{
    this->setFixedHeight(MIN_WINDOW_HEIGHT);
//    m_centerWidget->setFixedSize(WINDOW_WIDTH, PSK_SCRO_HEIGHT);
    m_secuWidget->hide();
    m_pwdLabel->hide();
    m_pwdEdit->hide();
    m_rememberCheckBox->hide();
    m_checkLabel->hide();
}

void HiddenWiFiPage::showPsk()
{
    this->setFixedHeight(MIN_WINDOW_HEIGHT);

    m_pwdLabel->show();
    m_pwdEdit->show();
    m_rememberCheckBox->show();
    m_checkLabel->show();

    m_secuWidget->hide();
}

void HiddenWiFiPage::showEnt()
{
    KyEapMethodType eapType;
    m_secuWidget->getEnterpriseType(eapType);
    setWindowWidth(eapType);

    m_secuWidget->show();
    m_rememberCheckBox->show();
    m_checkLabel->show();

    m_pwdLabel->hide();
    m_pwdEdit->hide();
}

void HiddenWiFiPage::setJoinBtnEnable()
{
    if (m_nameEdit->text().isEmpty()) {
        qDebug() << "network name is empty";
        m_isJoinBtnEnable = false;
    } else {
        int index = m_secuTypeCombox->currentData().toInt();
        if (index == NONE) {
            if (!m_nameEdit->text().isEmpty()) {
                m_isJoinBtnEnable = true;
            }
        } else if (index == WPA_AND_WPA2_PERSONAL || index == WPA3_PERSONAL) {
            if (m_pwdEdit->text().isEmpty() || m_pwdEdit->text().length() < 8 ) {
                qDebug() << "password is empty or length < 8";
                m_isJoinBtnEnable = false;
            } else {
                m_isJoinBtnEnable = true;
            }
        } else if (index == WPA_AND_WPA2_ENTERPRISE) {
            m_isJoinBtnEnable = m_isSecuOk;
        }
    }
    qDebug() << "setJoinBtnEnable "<< m_isJoinBtnEnable;
    m_joinBtn->setEnabled(m_isJoinBtnEnable);
}

void HiddenWiFiPage::setWindowWidth(KyEapMethodType eapType)
{
    int type = eapType;
    if (type == TLS) {
        this->setFixedHeight(MAX_WINDOW_HEIGHT);
    } else if (type == PEAP || type == TTLS) {
        this->setFixedHeight(PEAP_WINDOW_HEIGHT);
    }
}


void HiddenWiFiPage::on_btnJoin_clicked()
{
    qDebug() << "on_btnJoin_clicked";

    KyWirelessConnectSetting connSettingInfo;
    //基本信息
    connSettingInfo.m_ssid = m_nameEdit->text();
    connSettingInfo.setConnectName(connSettingInfo.m_ssid);
    connSettingInfo.setIfaceName(m_deviceName);
    connSettingInfo.isHidden = true;
    connSettingInfo.m_isAutoConnect = m_rememberCheckBox->isChecked();
    connSettingInfo.m_secretFlag = 0;

    //ipv4 ipv6
    connSettingInfo.setIpConfigType(IPADDRESS_V4, CONFIG_IP_DHCP);
    connSettingInfo.setIpConfigType(IPADDRESS_V6, CONFIG_IP_DHCP);

    int index = m_secuTypeCombox->currentData().toInt();
    if (index == NONE) {
        Q_EMIT connectHideNormalConnect(connSettingInfo, NONE);
    } else if (index == WPA_AND_WPA2_PERSONAL || index == WPA3_PERSONAL) {
        connSettingInfo.m_psk = m_pwdEdit->text();
        Q_EMIT connectHideNormalConnect(connSettingInfo, (KySecuType)index);
    } else if (index == WPA_AND_WPA2_ENTERPRISE) {
        KyEapMethodType eapType;
        m_secuWidget->getEnterpriseType(eapType);
        if (eapType == PEAP) {
            KyEapMethodPeapInfo info = m_secuWidget->assemblePeapInfo();
            Q_EMIT connectHidePeapConnect(info, connSettingInfo);
        } else if (eapType = TTLS) {
            KyEapMethodTtlsInfo info = m_secuWidget->assembleTtlsInfo();
            Q_EMIT connectHideTtlsConnect(info, connSettingInfo);
        } else {
            qWarning() << "unsupport now!!!";
        }
    }

    close();
}

void HiddenWiFiPage::onSecuTypeComboxIndexChanged()
{
    int index = m_secuTypeCombox->currentData().toInt();
    if (index == WPA_AND_WPA2_PERSONAL || index == WPA3_PERSONAL) {
        showPsk();
    } else if (index == WPA_AND_WPA2_ENTERPRISE) {
        showEnt();
    } else if (index == NONE) {
        showNone();
    }
    centerToScreen();
}

void HiddenWiFiPage::onBtnShowListClicked()
{
    QDBusInterface interface("com.kylin.network",
                             "/com/kylin/network",
                             "com.kylin.network",
                             QDBusConnection::sessionBus());
    if(interface.isValid()) {
        interface.call(QStringLiteral("showKylinNM"), 1);
    }
}
