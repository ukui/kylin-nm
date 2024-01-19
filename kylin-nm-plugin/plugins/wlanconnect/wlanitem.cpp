#include "wlanitem.h"
#include <QPainter>
#include <QRegExpValidator>
#include <QPainterPath>
#define FRAME_SPEED 150
#define LIMIT_TIME 60*1000
#define TOTAL_PAGE 8
#define RADIUS 6.0

#define UNEXPEND_HEIGHT  48
#define EXPEND_HEIGHT  120

#define ENABLE_BUTTON_COLOR qApp->palette().highlight().color()
#define UNABLE_BUTTON_COLOR qApp->palette().button().color()

WlanItem::WlanItem(bool isSimple, QWidget *parent)
    : isSimple(isSimple), QFrame(parent)
{
    this->setFixedSize(404, UNEXPEND_HEIGHT);
    QVBoxLayout *m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(16,0,0,0);
    m_mainLayout->setSpacing(0);

    //icon + name
    m_nameFrame = new QFrame(this);
    QHBoxLayout *lanLyt = new QHBoxLayout(m_nameFrame);
    lanLyt->setContentsMargins(0,8,16,4);
    lanLyt->setSpacing(0);
    radioBtn = new RadioItemButton(this);
    titileLabel = new FixLabel(this);
    titileLabel->setMinimumWidth(282);;

    lanLyt->addWidget(radioBtn);
    lanLyt->addSpacing(10);
    lanLyt->addWidget(titileLabel,Qt::AlignLeft);
    lanLyt->addStretch();
    if (!isSimple) {
        infoLabel = new InfoButton(this);
        lanLyt->addSpacing(8);
        lanLyt->addWidget(infoLabel);
        connect(infoLabel, &InfoButton::released, this, &WlanItem::infoButtonClick);
    }


    m_expendFrame = new QFrame(this);
    QVBoxLayout *expendnLyt = new QVBoxLayout(m_expendFrame);
    expendnLyt->setContentsMargins(40, 0, 16, 0);
    expendnLyt->setSpacing(0);


    //pwdline + connect Button
    m_pwdFrame = new QFrame(m_expendFrame);
    m_pwdFrameLyt = new QHBoxLayout(m_pwdFrame);
    m_pwdFrameLyt->setContentsMargins(0,0,0,0);
    m_pwdFrameLyt->setSpacing(0);
    m_pwdFrame->setLayout(m_pwdFrameLyt);

    m_pwdLineEdit = new KPasswordEdit(m_pwdFrame);
    m_pwdLineEdit->setState(Ordinary);
//    m_pwdLineEdit->setEchoModeBtnVisible(true);
    m_pwdLineEdit->setContextMenuPolicy(Qt::NoContextMenu);

    QRegExp rx("^[A-Za-z0-9`~!@#$%^&*()_-+=<>,.\\\/]+$");
    QRegExpValidator *latitude = new QRegExpValidator(rx, this);
    m_pwdLineEdit->setValidator(latitude);

    m_pwdLineEdit->installEventFilter(this);

    connect(m_pwdLineEdit, &KPasswordEdit::textChanged, this, &WlanItem::onPwdEditorTextChanged);
    m_pwdLineEdit->setFixedSize(228, 36);
    m_pwdLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    m_pwdFrameLyt->addWidget(m_pwdLineEdit);
    m_pwdFrameLyt->addSpacing(8);

    m_connectButton = new QPushButton(m_pwdFrame);
    m_connectButton->setFixedSize(96, 36);
    m_connectButton->setText(tr("Connect"));
    m_connectButton->setEnabled(false);
    connect(m_connectButton, &QPushButton::clicked, this, &WlanItem::onConnectButtonClicked);
    m_pwdFrameLyt->addWidget(m_connectButton);

    //自动连接选择区域UI
    m_autoConnectFrame = new QFrame(m_expendFrame);

    m_autoConnectFrameLyt = new QHBoxLayout(m_autoConnectFrame);
    m_autoConnectFrameLyt->setContentsMargins(0,8,150,8);
    m_autoConnectFrameLyt->setSpacing(0);
    m_autoConnectFrame->setLayout(m_autoConnectFrameLyt);

    m_autoConnectCheckBox = new QCheckBox(m_autoConnectFrame);
    m_autoConnectCheckBox->setChecked(true);
    m_autoConnectCheckBox->setFixedSize(16, 16);
    m_autoConnectFrameLyt->addWidget(m_autoConnectCheckBox);
    m_autoConnectFrameLyt->addSpacing(8);

    m_autoConnectLabel = new QLabel(m_autoConnectFrame);
    m_autoConnectLabel->setText(tr("Auto Connect"));
    m_autoConnectFrameLyt->addWidget(m_autoConnectLabel);

    expendnLyt->addWidget(m_pwdFrame);
    expendnLyt->addWidget(m_autoConnectFrame);

    m_mainLayout->addWidget(m_nameFrame);
    m_mainLayout->addWidget(m_expendFrame);

    m_expendFrame->hide();

    loadIcons.append(QIcon::fromTheme("ukui-loading-1-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-2-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-3-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-4-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-5-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-6-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-7-symbolic"));
    waitTimer = new QTimer(this);
    connect(waitTimer, &QTimer::timeout, this, &WlanItem::updateIcon);

    m_menu = new QMenu(this);//右键菜单
    m_menu->setWindowFlag(Qt::X11BypassWindowManagerHint);
    connect(m_menu, &QMenu::triggered, this, &WlanItem::itemClick);
}

void WlanItem::updateIcon()
{
    if (currentIconIndex > 6) {
        currentIconIndex = 0;
    }
    radioBtn->setButtonIcon(loadIcons.at(currentIconIndex));
    currentIconIndex ++;
}

void WlanItem::showEnterPricePage(QString devName, QWidget *widget)
{
    if (nullptr != m_enterPirsePage) {
        m_enterPirsePage->show();
    } else {
        QString ssid = getName();
        m_enterPirsePage = new EnterpriseWlanPage(ssid, devName, true, widget);
        connect(m_enterPirsePage, &EnterpriseWlanPage::destroyed, [=](){
            m_enterPirsePage->disconnect(this);
            m_enterPirsePage = nullptr;
        });
        connect(m_enterPirsePage, &EnterpriseWlanPage::connectPeapConnect, this, &WlanItem::connectPeapConnect);
        connect(m_enterPirsePage, &EnterpriseWlanPage::connectTtlsConnect, this, &WlanItem::connectTtlsConnect);
        m_enterPirsePage->show();
    }
}

void WlanItem::startLoading()
{
    waitTimer->start(FRAME_SPEED);
    m_loading = true;
}

void WlanItem::stopLoading(){
    waitTimer->stop();
    m_loading = false;
}

void WlanItem::mousePressEvent(QMouseEvent *event)
{
    return QFrame::mousePressEvent(event);
}
void WlanItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        if (!m_menu) {
            return QFrame::mouseReleaseEvent(event);
        }

        m_menu->clear();
        if (m_isAcitve || m_loading) {
            m_menu->addAction(new QAction(tr("Disconnect"), this));
        } else if (!m_isAcitve && !m_loading) {
            m_menu->addAction(new QAction(tr("Connect"), this));
        }
        m_menu->move(cursor().pos());
        m_menu->show();

    } else {
        if (!m_isAcitve && !m_loading) {
            Q_EMIT itemClick();
        }
    }
    return QFrame::mouseReleaseEvent(event);
}

void WlanItem::enterEvent(QEvent *event)
{
    m_isIn = true;
    update();
    return QFrame::enterEvent(event);
}

void WlanItem::leaveEvent(QEvent *event)
{
    m_isIn = false;
    update();
    return QFrame::leaveEvent(event);
}

void WlanItem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        if (m_expendFrame && m_expendFrame->isVisible() && m_pwdLineEdit->text().length() >= 8) {
            onConnectButtonClicked();
        }
    }
    return QFrame::keyPressEvent(event);
}

void WlanItem::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    if (m_isIn) {
        QColor color(240, 240, 240);
        color.setAlphaF(0.39);
        painter.setBrush(color);
    }
    else
        painter.setBrush(pal.color(QPalette::Base));

    QRect rect = this->rect();
    QPainterPath path;
    path.addRoundedRect(rect, RADIUS, RADIUS);
    painter.drawPath(path);
    return QFrame::paintEvent(event);
}

bool WlanItem::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_pwdLineEdit && event->type() == QEvent::FocusIn) {
        Q_EMIT passwordFocusIn();
    }
    return QFrame::eventFilter(watched,event);
}

void WlanItem::setExpend(bool enable)
{
    if (enable) {
        this->setFixedHeight(EXPEND_HEIGHT);
        m_expendFrame->show();
        m_pwdLineEdit->setFocus();
    } else {
        m_expendFrame->hide();
        this->setFixedHeight(UNEXPEND_HEIGHT);
        m_pwdLineEdit->clear();
        m_pwdLineEdit->setState(Ordinary);
    }
}

bool WlanItem::getExpend()
{
    return !m_expendFrame->isHidden();
}

void WlanItem::onPwdEditorTextChanged()
{
    QPalette btnPal;
    if (m_pwdLineEdit->text().length() < 8) {
        m_connectButton->setEnabled(false);
        btnPal.setColor(QPalette::Button, UNABLE_BUTTON_COLOR);
        m_connectButton->setPalette(btnPal);
    } else {
        m_connectButton->setEnabled(true);
        btnPal.setColor(QPalette::Button, ENABLE_BUTTON_COLOR);
        m_connectButton->setPalette(btnPal);
    }
}

void WlanItem::onConnectButtonClicked()
{
    connectButtonClick(m_pwdLineEdit->text(), m_autoConnectCheckBox->isChecked());
}
