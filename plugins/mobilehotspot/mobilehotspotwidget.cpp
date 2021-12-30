#include "mobilehotspotwidget.h"
#include <QDebug>

#define LABEL_RECT 17, 0, 105, 23
#define CONTENTS_MARGINS 0, 0, 0, 0
#define FRAME_MIN_SIZE 550, 60
#define FRAME_MAX_SIZE 16777215, 16777215
#define CONTECT_FRAME_MAX_SIZE 16777215, 60
#define LABLE_MIN_WIDTH 140
#define COMBOBOX_MIN_WIDTH 200
#define LINE_MAX_SIZE 16777215, 1
#define LINE_MIN_SIZE 0, 1
#define LAYOUT_LEFT_MARGINS 8

#define WIRELESS   1

#define AP_NAME_MAX_LENGTH 32

const QByteArray GSETTINGS_SCHEMA = "org.ukui.kylin-nm.switch";
const QString    WIRELESS_SWITCH          = "wirelessswitch";

void MobileHotspotWidget::showDesktopNotify(const QString &message)
{
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    QList<QVariant> args;
    args<<(tr("ukui control center"))
       <<((unsigned int) 0)
       <<QString("/usr/share/icons/ukui-icon-theme-default/24x24/devices/gnome-dev-ethernet.png")
       <<tr("ukui control center desktop message") //显示的是什么类型的信息
       <<message //显示的具体信息
       <<QStringList()
       <<QVariantMap()
       <<(int)-1;
    iface.callWithArgumentList(QDBus::AutoDetect,"Notify",args);
}

MobileHotspotWidget::MobileHotspotWidget(QWidget *parent) : QWidget(parent)
{
    mVlayout = new QVBoxLayout(this);
    mVlayout->setContentsMargins(CONTENTS_MARGINS);

    qDBusRegisterMetaType<QMap<QString, bool> >();
    qDBusRegisterMetaType<QMap<QString, int> >();
    qDBusRegisterMetaType<QVector<QStringList> >();
    qDBusRegisterMetaType<QMap<QString, QVector<QStringList> >>();

    initUI();
    m_switchBtn->installEventFilter(this);
    m_interface = new QDBusInterface("com.kylin.network", "/com/kylin/network",
                                     "com.kylin.network",
                                     QDBusConnection::sessionBus());
    if(!m_interface->isValid()) {
        qDebug() << "dbus interface com.kylin.network is invaild";
        m_switchBtn->setChecked(false);
        setUiEnabled(false);
    }

    m_hostName = getHostName();

    initDbusConnect();

    initInterfaceInfo();
    getApInfo();

    connect(m_switchBtn, &SwitchButton::checkedChanged, this, &MobileHotspotWidget::setUiEnabled);
    connect(m_interfaceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
        m_interfaceName = m_interfaceComboBox->currentText();
        updateBandCombox();
    });
    connect(m_pwdShowBox, &QCheckBox::clicked, this, [=]() {
        if (m_pwdNameLine->echoMode() == QLineEdit::Password) {
            m_pwdNameLine->setEchoMode(QLineEdit::Normal);
        } else {
            m_pwdNameLine->setEchoMode(QLineEdit::Password);
        }
    });
}

MobileHotspotWidget::~MobileHotspotWidget()
{
    delete m_interface;
}

bool MobileHotspotWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick && watched == m_switchBtn) {
        return true;
    }

    if (event->type() == QEvent::MouseButtonPress) {
        if (watched == m_switchBtn) {
            if (!m_interface->isValid()) {
                return true;
            }
            if (m_switchBtn->getDisabledFlag()) {
                showDesktopNotify(tr("wirless switch is close or no wireless device"));
                return true;
            }
            if (m_switchBtn->isChecked()) {
                showDesktopNotify(tr("start to close hotspot"));
                QDBusReply<void> reply = m_interface->call("deactiveWirelessAp", m_apNameLine->text(), m_uuid);
                if (!reply.isValid()) {
                    qDebug() << "[MobileHotspotWidget] call deactiveWirelessAp failed ";
                    return true;
                }
            } else {
                if (m_apNameLine->text().isEmpty() || m_interfaceName.isEmpty())
                {
                    showDesktopNotify(tr("hotpots name or device is invalid"));
                    return true;
                }
                if (m_pwdNameLine->text().length() < 8) {
                    showDesktopNotify(tr("can not  create hotspot with password length less than eight!"));
                    return true;
                }
                showDesktopNotify(tr("start to open hotspot ") + m_apNameLine->text());
                QDBusReply<void> reply = m_interface->call("activeWirelessAp",
                                  m_apNameLine->text(),
                                  m_pwdNameLine->text(),
                                  m_freqBandComboBox->currentText(),
                                  m_interfaceComboBox->currentText());
                if (!reply.isValid()) {
                    qDebug() << "[MobileHotspotWidget] call deactiveWirelessAp failed ";
                    return true;
                }
            }
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MobileHotspotWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void MobileHotspotWidget::initUI()
{
    QFrame *hotspotFrame = new QFrame(this);
    hotspotFrame->setMinimumSize(FRAME_MIN_SIZE);
    hotspotFrame->setMaximumSize(FRAME_MAX_SIZE);
    hotspotFrame->setFrameShape(QFrame::Box);

    QVBoxLayout *hotspotLyt = new QVBoxLayout(hotspotFrame);
    hotspotLyt->setContentsMargins(0, 0, 0, 0);

    m_hotspotTitleLabel = new TitleLabel(this);
    m_hotspotTitleLabel->setText(tr("Hotspot"));

    setSwitchFrame();
    setApNameFrame();
    setPasswordFrame();
    setFreqBandFrame();
    setInterFaceFrame();

    switchAndApNameLine = myLine();
    apNameAndPwdLine = myLine();
    pwdAndfreqBandLine = myLine();
    freqBandAndInterfaceLine = myLine();

    /* add widget */
    hotspotLyt->addWidget(m_switchFrame);
    hotspotLyt->addWidget(switchAndApNameLine);
    hotspotLyt->addWidget(m_ApNameFrame);
    hotspotLyt->addWidget(apNameAndPwdLine);
    hotspotLyt->addWidget(m_passwordFrame);
    hotspotLyt->addWidget(pwdAndfreqBandLine);
    hotspotLyt->addWidget(m_freqBandFrame);
    hotspotLyt->addWidget(freqBandAndInterfaceLine);
    hotspotLyt->addWidget(m_interfaceFrame);
    hotspotLyt->setSpacing(0);

    mVlayout->addWidget(m_hotspotTitleLabel);
    mVlayout->setSpacing(8);
    mVlayout->addWidget(hotspotFrame);
    mVlayout->addStretch();
}

void MobileHotspotWidget::initDbusConnect()
{
    if(m_interface->isValid()) {
        connect(m_interface,SIGNAL(activateFailed(QString)), this, SLOT(onActivateFailed(QString)), Qt::QueuedConnection);
        connect(m_interface,SIGNAL(deactivateFailed(QString)), this, SLOT(onDeactivateFailed(QString)), Qt::QueuedConnection);
        connect(m_interface,SIGNAL(deviceStatusChanged()), this, SLOT(onDeviceStatusChanged()), Qt::QueuedConnection);
        connect(m_interface,SIGNAL(deviceNameChanged(QString, QString, int)), this, SLOT(onDeviceNameChanged(QString, QString, int)), Qt::QueuedConnection);
        connect(m_interface,SIGNAL(hotspotDeactivated(QString, QString)), this, SLOT(onHotspotDeactivated(QString, QString)), Qt::QueuedConnection);
        connect(m_interface,SIGNAL(hotspotActivated(QString, QString, QString)), this, SLOT(onHotspotActivated(QString, QString, QString)), Qt::QueuedConnection);

        connect(m_interface, SIGNAL(wlanactiveConnectionStateChanged(QString, QString, QString, int)), this, SLOT(onActiveConnectionChanged(QString, QString, QString, int)), Qt::QueuedConnection);
    }

    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA)) {
        m_switchGsettings = new QGSettings(GSETTINGS_SCHEMA);
        onGsettingChanged(WIRELESS_SWITCH);
        connect(m_switchGsettings, &QGSettings::changed, this, &MobileHotspotWidget::onGsettingChanged, Qt::QueuedConnection);
    }

    connect(m_apNameLine, &QLineEdit::textEdited, this, &MobileHotspotWidget::onApLineEditTextEdit);
}

void MobileHotspotWidget::onApLineEditTextEdit(QString text)
{
    int count = 0;
    int i = 0;

    for ( ; i < text.length(); ++i) {
        count += text.mid(i,1).toLocal8Bit().length();
        if (count > AP_NAME_MAX_LENGTH) {
            break;
        }
    }

    m_apNameLine->setText(text.left(i));
}


void MobileHotspotWidget::onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, int status)
{
    if(m_uuid == uuid && status == 4) {
        showDesktopNotify(tr("hotspot already close"));
        m_switchBtn->setChecked(false);
        setUiEnabled(false);
        m_uuid.clear();
    }
}

void MobileHotspotWidget::onGsettingChanged(const QString &key)
{
    if (key == WIRELESS_SWITCH) {
        bool status = m_switchGsettings->get(WIRELESS_SWITCH).toBool();
        if (!status) {
//                        if (m_switchBtn->isChecked()) {
//                            if(m_interface->isValid()) {
//                                QDBusReply<void> reply = m_interface->call("deactiveWirelessAp",
//                                                  m_apNameLine->text(),
//                                                  m_pwdNameLine->text(),
//                                                  m_interfaceComboBox->currentText());
//                                if (!reply.isValid()) {
//                                    qDebug() << "[MobileHotspotWidget] call deactiveWirelessAp failed ";
//                                    return true;
//                                }
//                            }
//                        }
            m_switchBtn->setChecked(status);
            m_uuid.clear();
            m_switchBtn->setDisabledFlag(true);
        } else {
            m_switchBtn->setDisabledFlag(false);
        }
    }
}

void MobileHotspotWidget::initInterfaceInfo()
{
    if(!m_interface->isValid()) {
        return;
    }
    m_interfaceComboBox->clear();
    QDBusReply<QMap<QString, bool> > reply = m_interface->call("getDeviceListAndEnabled",WIRELESS);

    if (!reply.isValid()) {
        qDebug()<<"execute dbus method 'getDeviceListAndEnabled' is invalid in func initInterfaceInfo()";
        setWidgetHidden(true);
        return;
    }
    QMap<QString, bool> devMap = reply.value();

    QDBusReply<QMap<QString, int> > capReply = m_interface->call("getWirelessDeviceCap");
    if (!capReply.isValid()) {
        qDebug()<<"execute dbus method 'getWirelessDeviceCap' is invalid in func initInterfaceInfo()" <<capReply.error().type() ;
        setWidgetHidden(true);
        return;
    }
    QMap<QString, int> devCapMap = capReply.value();


    if (devMap.isEmpty()) {
        qDebug() << "no wireless device";
        setWidgetHidden(true);
        m_switchBtn->setDisabledFlag(true);
    } else {
        QMap<QString, bool>::Iterator iter = devMap.begin();
        while (iter != devMap.end()) {
            QString interfaceName = iter.key();
            if (devCapMap[interfaceName] & 0x01) {
                m_interfaceComboBox->addItem(interfaceName);
            }
            iter++;
        }
        if (m_interfaceComboBox->count() > 0) {
            setWidgetHidden(false);
            m_interfaceName = m_interfaceComboBox->currentText();
            if (m_interfaceComboBox->count() == 1) {
                updateBandCombox();
            }
        } else {
            qDebug() << "no useable wireless device";
            setWidgetHidden(true);
        }
    }
}

void MobileHotspotWidget::getApInfo()
{
    if (!m_interface->isValid()) {
        return;
    }

    if (m_interfaceComboBox->count() <= 0) {
                m_switchBtn->setChecked(false);
                setWidgetHidden(true);
                qWarning() << "getApInfo but interface is empty";
                return;
        }


    QDBusReply<QStringList> reply = m_interface->call("getStoredApInfo");
    if (!reply.isValid()) {
        qDebug()<<"execute dbus method 'getStoredApInfo' is invalid in func getObjectPath()";
    }

    QStringList apInfo = reply.value();

    if (apInfo.isEmpty()) {
        qDebug() << "no stored hotspot info";
        m_apNameLine->setText(m_hostName);
        m_pwdNameLine->setText("12345678");
        return;
    } else {
        int index = m_interfaceComboBox->findText(apInfo.at(2));
        if (index >= 0) {
            m_apNameLine->setText(apInfo.at(0));
            m_pwdNameLine->setText(apInfo.at(1));
            m_interfaceComboBox->setCurrentIndex(index);
            m_interfaceName = apInfo.at(2);
            if (apInfo.at(3) == "true") {
                m_switchBtn->setChecked(true);
                setUiEnabled(true);
                m_uuid = apInfo.at(4);
            } else {
                m_switchBtn->setChecked(false);
                setUiEnabled(false);
            }
        } else {
            qDebug() << "no such interface " << apInfo.at(2);
        }
    }
}

void MobileHotspotWidget::setSwitchFrame()
{
    /* Open */
    m_switchFrame = new QFrame(this);
    m_switchFrame->setFrameShape(QFrame::Shape::NoFrame);
    m_switchFrame->setMinimumSize(FRAME_MIN_SIZE);
    m_switchFrame->setMaximumSize(CONTECT_FRAME_MAX_SIZE);

    QHBoxLayout *switchLayout = new QHBoxLayout();

    m_switchLabel = new QLabel(tr("Open"), this);
    m_switchLabel->setMinimumWidth(LABLE_MIN_WIDTH);
    m_switchBtn = new SwitchButton(this);
    switchLayout->addSpacing(LAYOUT_LEFT_MARGINS);
    switchLayout->addWidget(m_switchLabel);
    switchLayout->addStretch();
    switchLayout->addWidget(m_switchBtn);

    m_switchFrame->setLayout(switchLayout);
}

void MobileHotspotWidget::setApNameFrame()
{
    /* ApName */
    m_ApNameFrame = new QFrame(this);
    m_ApNameFrame->setFrameShape(QFrame::Shape::NoFrame);
    m_ApNameFrame->setMinimumSize(FRAME_MIN_SIZE);
    m_ApNameFrame->setMaximumSize(CONTECT_FRAME_MAX_SIZE);

    QHBoxLayout *apNameHLayout = new QHBoxLayout();

    m_apNameLabel = new QLabel(tr("Wi-Fi Name"), this);
    m_apNameLabel->setMinimumWidth(LABLE_MIN_WIDTH);
    m_apNameLine = new QLineEdit(this);
    m_apNameLine->setMinimumWidth(COMBOBOX_MIN_WIDTH);
    m_apNameLine->setMaxLength(AP_NAME_MAX_LENGTH);
    apNameHLayout->addSpacing(LAYOUT_LEFT_MARGINS);
    apNameHLayout->addWidget(m_apNameLabel);
    apNameHLayout->addWidget(m_apNameLine);
    m_ApNameFrame->setLayout(apNameHLayout);

}

void MobileHotspotWidget::setPasswordFrame()
{
    /* Password */
    m_passwordFrame = new QFrame(this);
    m_passwordFrame->setFrameShape(QFrame::Shape::NoFrame);
    m_passwordFrame->setMinimumSize(FRAME_MIN_SIZE);
    m_passwordFrame->setMaximumSize(CONTECT_FRAME_MAX_SIZE);

    QHBoxLayout *passwordHLayout = new QHBoxLayout();

    m_pwdLabel = new QLabel(tr("Password"), this);
    m_pwdLabel->setMinimumWidth(LABLE_MIN_WIDTH);
    m_pwdNameLine = new QLineEdit(this);
    m_pwdNameLine->setMinimumWidth(COMBOBOX_MIN_WIDTH);
    m_pwdNameLine->setEchoMode(QLineEdit::Password);
    passwordHLayout->addSpacing(LAYOUT_LEFT_MARGINS);
    passwordHLayout->addWidget(m_pwdLabel);
    passwordHLayout->addWidget(m_pwdNameLine);

    m_passwordFrame->setLayout(passwordHLayout);

    m_pwdShowBox = new QCheckBox(this);
    m_pwdShowBox->setStyleSheet("QCheckBox::indicator {width: 18px; height: 9px;}"
                                       "QCheckBox::indicator:checked {image: url(:/img/plugins/mobilehotspot/show-pwd.png);}"
                                       "QCheckBox::indicator:unchecked {image: url(:/img/plugins/mobilehotspot/hide-pwd.png);}");
    m_pwdShowBox->setCursor(Qt::PointingHandCursor);
    m_pwdShowBox->setFixedSize(30, m_pwdNameLine->height());
    //防止文本框输入内容位于按钮之下
    QMargins margins = m_pwdNameLine->textMargins();
    m_pwdNameLine->setTextMargins(margins.left(), margins.top(), m_pwdShowBox->width(), margins.bottom());
    QHBoxLayout *pSearchLayout = new QHBoxLayout();
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(m_pwdShowBox);
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(0, 0, 0, 0);
    m_pwdNameLine->setLayout(pSearchLayout);
    m_pwdNameLine->setEchoMode(QLineEdit::Password);
}

void MobileHotspotWidget::setFreqBandFrame()
{
    /* frequency band */
    m_freqBandFrame = new QFrame(this);
    m_freqBandFrame->setFrameShape(QFrame::Shape::NoFrame);
    m_freqBandFrame->setMinimumSize(FRAME_MIN_SIZE);
    m_freqBandFrame->setMaximumSize(CONTECT_FRAME_MAX_SIZE);

    QHBoxLayout *freqBandHLayout = new QHBoxLayout();

    m_freqBandLabel = new QLabel(tr("Frequency band"), this);
    m_freqBandLabel->setMinimumWidth(LABLE_MIN_WIDTH);
    m_freqBandComboBox = new QComboBox(this);
    m_freqBandComboBox->setInsertPolicy(QComboBox::NoInsert);
    m_freqBandComboBox->setMinimumWidth(COMBOBOX_MIN_WIDTH);
    m_freqBandComboBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_freqBandComboBox->addItem("2.4Ghz");
    m_freqBandComboBox->addItem("5Ghz");
    freqBandHLayout->addSpacing(LAYOUT_LEFT_MARGINS);
    freqBandHLayout->addWidget(m_freqBandLabel);
    freqBandHLayout->addWidget(m_freqBandComboBox);

    m_freqBandFrame->setLayout(freqBandHLayout);
}

void MobileHotspotWidget::setInterFaceFrame()
{
    /* key tips */
    m_interfaceFrame = new QFrame(this);
    m_interfaceFrame->setFrameShape(QFrame::Shape::NoFrame);
    m_interfaceFrame->setMinimumSize(FRAME_MIN_SIZE);
    m_interfaceFrame->setMaximumSize(CONTECT_FRAME_MAX_SIZE);

    QHBoxLayout *interfaceHLayout = new QHBoxLayout;

    m_interfaceLabel = new QLabel(tr("Net card"), this);
    m_interfaceLabel->setMinimumWidth(LABLE_MIN_WIDTH);
    m_interfaceComboBox = new QComboBox(this);
    m_interfaceComboBox->setInsertPolicy(QComboBox::NoInsert);
    m_interfaceComboBox->setMinimumWidth(COMBOBOX_MIN_WIDTH);
    m_interfaceComboBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    interfaceHLayout->addSpacing(LAYOUT_LEFT_MARGINS);
    interfaceHLayout->addWidget(m_interfaceLabel);
    interfaceHLayout->addWidget(m_interfaceComboBox);

    m_interfaceFrame->setLayout(interfaceHLayout);
}

void MobileHotspotWidget::onActivateFailed(QString errorMessage)
{
    if (errorMessage.indexOf("hotspot")) {
        //todo
//        showDesktopNotify(errorMessage);
    }
}

void MobileHotspotWidget::onDeactivateFailed(QString errorMessage)
{
    if (errorMessage.indexOf("hotspot")) {
//        showDesktopNotify(errorMessage);
    }
}

//设备插拔
void MobileHotspotWidget::onDeviceStatusChanged()
{
    initInterfaceInfo();
    getApInfo();
}

void MobileHotspotWidget::onDeviceNameChanged(QString oldName, QString newName, int type)
{
    if (WIRELESS != type) {
        return;
    }
    int index = m_interfaceComboBox->findText(oldName);
    if (index >= 0) {
        m_interfaceComboBox->setItemText(index, newName);
        if (m_interfaceName == oldName) {
            m_interfaceName = newName;
        }
    }
}

//热点断开
void MobileHotspotWidget::onHotspotDeactivated(QString devName, QString ssid)
{
    if (!m_switchBtn->isChecked()) {
        return;
    }
    if (devName == m_interfaceComboBox->currentText() && ssid == m_apNameLine->text()) {
        m_switchBtn->setChecked(false);
        m_uuid.clear();
//        setUiEnabled(true);
        showDesktopNotify(tr("hotspot already close"));
    }
}

//热点连接
void MobileHotspotWidget::onHotspotActivated(QString devName, QString ssid, QString uuid)
{
    qDebug() << "onHotspotActivated" <<devName << ssid << uuid;
    if (m_switchBtn->isChecked()) {
        return;
    }

    if (devName == m_interfaceComboBox->currentText() && ssid == m_apNameLine->text()) {
        m_switchBtn->setChecked(true);
        m_uuid = uuid;
//        setUiEnabled(false);
        showDesktopNotify(tr("hotspot already open"));
    } else {
        QStringList info;
        if (!getApInfoBySsid(devName, ssid, info)) {
            return;
        }
        int index = m_interfaceComboBox->findText(devName);
        if (index >= 0) {
            showDesktopNotify(tr("hotspot already open"));
            m_apNameLine->setText(ssid);
            m_interfaceComboBox->setCurrentIndex(index);
            m_switchBtn->setChecked(true);
            m_switchBtn->setDisabledFlag(false);
            m_pwdNameLine->setText(info.at(0));
            m_interfaceName = devName;
            updateBandCombox();
            index = m_freqBandComboBox->findText(info.at(1));
            if (index >= 0) {
                m_freqBandComboBox->setCurrentIndex(index);
            }
            m_uuid = uuid;
        } else {
            qDebug() << "no such device in combo box";
        }
    }
}

bool MobileHotspotWidget::getApInfoBySsid(QString devName, QString ssid, QStringList &info)
{
    info.clear();
    if(!m_interface->isValid()) {
        return false;
    }
    QDBusReply<QStringList> reply = m_interface->call("getApInfoBySsid", devName, ssid);
    if (!reply.isValid()) {
        qDebug()<<"execute dbus method 'getApInfoBySsid' is invalid in func getApInfoBySsid()";
    }
    info = reply.value();
    if (info.size() != 2) {
        return false;
    } else {
        return true;
    }
}

void MobileHotspotWidget::setUiEnabled(bool enable)
{
    qDebug() << "switch mode change to " << enable;
    if (enable) {
        m_pwdNameLine->setFocusPolicy(Qt::NoFocus);
        m_freqBandComboBox->setEnabled(false);
        m_interfaceComboBox->setEnabled(false);
        m_apNameLine->setFocusPolicy(Qt::NoFocus);
    } else {
        m_pwdNameLine->setFocusPolicy(Qt::ClickFocus);
        m_freqBandComboBox->setEnabled(true);
        m_interfaceComboBox->setEnabled(true);
        m_apNameLine->setFocusPolicy(Qt::ClickFocus);
    }
}

void MobileHotspotWidget::setWidgetHidden(bool isHidden)
{
    m_ApNameFrame->setHidden(isHidden);
    m_passwordFrame->setHidden(isHidden);
    m_freqBandFrame->setHidden(isHidden);
    m_interfaceFrame->setHidden(isHidden);

    switchAndApNameLine->setHidden(isHidden);
    apNameAndPwdLine->setHidden(isHidden);
    pwdAndfreqBandLine->setHidden(isHidden);
    freqBandAndInterfaceLine->setHidden(isHidden);

    if (isHidden) {
        m_switchBtn->setChecked(false);
        m_switchBtn->setDisabledFlag(true);
        m_interfaceName = "";
        m_uuid = "";
    } else {
        m_switchBtn->setDisabledFlag(false);
        onGsettingChanged(WIRELESS_SWITCH);
    }

}

void MobileHotspotWidget::updateBandCombox()
{
    m_freqBandComboBox->clear();
    QDBusReply<QMap<QString, int> > capReply = m_interface->call("getWirelessDeviceCap");
    if (!capReply.isValid()) {
        qDebug()<<"execute dbus method 'getWirelessDeviceCap' is invalid in func initInterfaceInfo()" << capReply.error().message();
        setWidgetHidden(true);
        return;
    }
    QMap<QString, int> devCapMap = capReply.value();
    if (devCapMap[m_interfaceName] & 0x02) {
         m_freqBandComboBox->addItem("2.4Ghz");
    }
    if (devCapMap[m_interfaceName] & 0x04) {
        m_freqBandComboBox->addItem("5Ghz");
    }
}

QFrame* MobileHotspotWidget::myLine()
{
    QFrame *line = new QFrame(this);
    line->setMinimumSize(QSize(LINE_MIN_SIZE));
    line->setMaximumSize(QSize(LINE_MAX_SIZE));
    line->setLineWidth(0);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    return line;
}

QString MobileHotspotWidget::getHostName()
{
    int count = 0;
    while (count < 3) {
        QDBusInterface hostInterface("org.freedesktop.hostname1",
                                     "/org/freedesktop/hostname1",
                                     "org.freedesktop.hostname1",
                                     QDBusConnection::systemBus());
        if (hostInterface.isValid()) {
            return hostInterface.property("Hostname").value<QString>();
        } else {
            count++;
        }
    }
    return "default";
}
