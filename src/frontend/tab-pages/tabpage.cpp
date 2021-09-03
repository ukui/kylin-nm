#include "tabpage.h"
#include <qsettings.h>

TabPage::TabPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

TabPage::~TabPage()
{
    delete m_titleDivider;
    delete m_activatedNetDivider;
    delete m_inactivatedNetDivider;
}

void TabPage::initUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);
    this->setLayout(m_mainLayout);

    m_titleFrame = new QFrame(this);
    m_titleFrame->setFixedHeight(TITLE_FRAME_HEIGHT);
    m_titleLayout = new QHBoxLayout(m_titleFrame);
    m_titleLayout->setContentsMargins(TITLE_LAYOUT_MARGINS);
    m_titleLabel = new QLabel(m_titleFrame);
    m_netSwitch = new SwitchButton(m_titleFrame);
    m_titleLayout->addWidget(m_titleLabel);
    m_titleLayout->addStretch();
    m_titleLayout->addWidget(m_netSwitch);
    m_titleDivider = new Divider(this);

    m_activatedNetFrame = new QFrame(this);
    m_activatedNetLayout = new QVBoxLayout(m_activatedNetFrame);
    m_activatedNetLayout->setContentsMargins(ACTIVE_NET_LAYOUT_MARGINS);
    m_activatedNetLayout->setSpacing(NET_LAYOUT_SPACING);
    m_activatedNetLabel = new QLabel(m_activatedNetFrame);
    m_activatedNetLabel->setContentsMargins(TEXT_MARGINS);
    m_activatedNetLayout->addWidget(m_activatedNetLabel);
    m_activatedNetDivider = new Divider(this);

    m_inactivatedNetFrame = new QFrame(this);
    m_inactivatedNetFrame->setMinimumHeight(INACTIVE_AREA_MIN_HEIGHT);
    m_inactivatedNetLayout = new QVBoxLayout(m_inactivatedNetFrame);
    m_inactivatedNetLayout->setContentsMargins(NET_LAYOUT_MARGINS);
    m_inactivatedNetLayout->setSpacing(NET_LAYOUT_SPACING);
    m_inactivatedNetFrame->setLayout(m_inactivatedNetLayout);
    m_inactivatedNetLabel = new QLabel(m_inactivatedNetFrame);
    m_inactivatedNetLabel->setContentsMargins(TEXT_MARGINS);
    m_inactivatedNetListArea = new QScrollArea(m_inactivatedNetFrame);
    m_inactivatedNetListArea->setFrameShape(QFrame::Shape::NoFrame);
    m_inactivatedNetListArea->setWidgetResizable(true);
    m_inactivatedNetLayout->addWidget(m_inactivatedNetLabel);
    m_inactivatedNetLayout->addWidget(m_inactivatedNetListArea);
    m_inactivatedNetDivider = new Divider(this);

    m_settingsFrame = new QFrame(this);
    m_settingsLayout = new QHBoxLayout(m_settingsFrame);
    m_settingsLayout->setContentsMargins(SETTINGS_LAYOUT_MARGINS);
    m_settingsLabel = new QLabel(m_settingsFrame);
    m_settingsLabel->setText(tr("Settings"));
    m_settingsLayout->addWidget(m_settingsLabel);
    m_settingsFrame->setLayout(m_settingsLayout);

    m_mainLayout->addWidget(m_titleFrame);
    m_mainLayout->addWidget(m_titleDivider);
    m_mainLayout->addWidget(m_activatedNetFrame);
    m_mainLayout->addWidget(m_activatedNetDivider);
    m_mainLayout->addWidget(m_inactivatedNetFrame);
    m_mainLayout->addWidget(m_inactivatedNetDivider);
    m_mainLayout->addWidget(m_settingsFrame);
}

void TabPage::updateDefaultDevice(QString &deviceName)
{
    qDebug() << "updateDefaultDevice" << deviceName;
    defaultDevice = deviceName;
}

QString TabPage::getDefaultDevice()
{
    qDebug() << "getDefaultDevice" << defaultDevice;
    return defaultDevice;
}

void setDefaultDevice(KyDeviceType deviceType, QString deviceName)
{
    QString key;
    switch (deviceType) {
    case WIRED:
        key = "wired";
        break;
    case WIRELESS:
        key = "wireless";
        break;
    default:
        return;
        break;
    }
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    m_settings->setValue(key, deviceName);
    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;
    return;
}

bool checkDeviceExist(KyDeviceType deviceType, QString deviceName)
{
    NetworkManager::Device::Type type;
    switch (deviceType) {
    case WIRED:
        type = NetworkManager::Device::Type::Ethernet;
        break;
    case WIRELESS:
        type = NetworkManager::Device::Type::Wifi;
        break;
    default:
        return false;
        break;
    }

    KyNetworkDeviceResourse * kdr = new KyNetworkDeviceResourse();
    QStringList devList;
    devList.clear();

    kdr->getNetworkDeviceList(type, devList);
    delete kdr;
    return devList.contains(deviceName);
}
