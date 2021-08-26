#include "lanpage.h"
#include <QSettings>
#include "dbusadaptor.h"

LanPage::LanPage(QWidget *parent) : TabPage(parent)
{
    m_device = new KyNetworkDeviceResourse(this);
    initDevice();
    initLanUI();

    connect(m_device, &KyNetworkDeviceResourse::deviceAdd, this , [=](QString deviceName, NetworkManager::Device::Type deviceType) {
        qDebug() << "deviceAdd";
        if (deviceType !=  NetworkManager::Device::Type::Ethernet) {
            return;
        }
        if (getDefaultDevice().isEmpty())
        {
            updateDefaultDevice(deviceName);
            setDefaultDevice(WIRED, deviceName);
        }
        emit deviceStatusChanged();

    });
    connect(m_device, &KyNetworkDeviceResourse::deviceRemove, this , [=](QString deviceName) {
       //todo:check device type
        if (getDefaultDevice() == deviceName)
        {
            QStringList list;
            QString newDefaultDevice = "";
            list.empty();
            m_device->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, list);
            if (!list.isEmpty()) {
                newDefaultDevice = list.at(0);
            }
            updateDefaultDevice(newDefaultDevice);
            setDefaultDevice(WIRED, newDefaultDevice);
        }
        emit deviceStatusChanged();
    });

}


void LanPage::initLanUI()
{
    m_titleLabel->setText(tr("LAN"));
    m_activatedNetLabel->setText(tr("Activated LAN"));
    m_inactivatedNetLabel->setText(tr("Inactivated LAN"));

    m_inactivatedNetListWidget = new QListWidget(m_inactivatedNetListArea);
    m_inactivatedNetListArea->setWidget(m_inactivatedNetListWidget);
    m_inactivatedNetListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_inactivatedNetListArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_inactivatedNetListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedNetListWidget->setSpacing(NET_LIST_SPACING);
    m_inactivatedNetListWidget->setFrameShape(QFrame::Shape::NoFrame);
}


/**
 * @brief WlanPage::initDevice 初始化默认网卡
 */
void LanPage::initDevice()
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    QString key("wired");
    QString deviceName = m_settings->value(key, "").toString();
    if (deviceName.isEmpty()) {
        qDebug() << "initDevice but  defalut wired card is null";
        QStringList list;
        list.empty();
        m_device->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, list);
        if (!list.isEmpty()) {
            deviceName = list.at(0);
            m_settings->setValue(key, deviceName);
        }
    }
    updateDefaultDevice(deviceName);
    qDebug() << "[LanPage] initDevice defaultDevice = " << deviceName;

    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;

}
