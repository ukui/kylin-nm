#include "lanpage.h"
#include <QSettings>
#include "dbusadaptor.h"

LanPage::LanPage(QWidget *parent) : TabPage(parent)
{
    m_device = new KyNetworkDeviceResourse(this);
    devList.empty();
    initDevice();
    initLanUI();

    connect(m_device, &KyNetworkDeviceResourse::deviceAdd, this, &LanPage::onDeviceAdd);
    connect(m_device, &KyNetworkDeviceResourse::deviceRemove, this, &LanPage::onDeviceRemove);
    connect(m_device, &KyNetworkDeviceResourse::deviceNameUpdate, this, &LanPage::onDeviceNameUpdate);

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
    m_device->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, devList);
    if (deviceName.isEmpty()) {
        qDebug() << "initDevice but  defalut wired card is null";
        if (!devList.isEmpty()) {
            deviceName = devList.at(0);
            m_settings->setValue(key, deviceName);
        }
    }
    updateDefaultDevice(deviceName);
    qDebug() << "[LanPage] initDevice defaultDevice = " << deviceName;

    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;

    for (int i = 0; i < devList.size(); ++i)
    {
        qDebug() << devList.at(i);
    }

}

void LanPage::onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType)
{
    qDebug() << "deviceAdd" << deviceName;
    if (deviceType !=  NetworkManager::Device::Type::Ethernet) {
        return;
    }
    devList << deviceName;
    if (getDefaultDevice().isEmpty())
    {
        updateDefaultDevice(deviceName);
        setDefaultDevice(WIRED, deviceName);
    }
    emit deviceStatusChanged();
}

void LanPage::onDeviceRemove(QString deviceName)
{
    qDebug() << "deviceRemove" << deviceName;
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
    if (devList.contains(deviceName)) {
        devList.removeOne(deviceName);
        emit deviceStatusChanged();
    }
}

void LanPage::onDeviceNameUpdate(QString oldName, QString newName)
{
   if (getDefaultDevice() == oldName) {
       updateDefaultDevice(newName);
       setDefaultDevice(WIRED, newName);
   }

   if (devList.contains(oldName)) {
       devList.removeOne(oldName);
       devList.append(newName);
       qDebug() << "LanPage emit deviceNameUpdate "  << oldName << newName;
       emit deviceNameChanged(oldName, newName);
   }

}
