#include "lanpage.h"
#include <QDebug>

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define TITLE_FRAME_HEIGHT 52
#define TITLE_LAYOUT_MARGINS 24,0,24,0
#define LAN_LIST_SPACING 2
#define TEXT_MARGINS 16,0,0,0
//#define SCROLL_AREA_HEIGHT 200
#define SETTINGS_LAYOUT_MARGINS 24,16,24,16
#define TRANSPARENT_COLOR QColor(0,0,0,0)
#define SWITCH_WIDTH 48
#define SWITCH_HEIGHT 24
#define ITEM_HEIGHT 48

const QString WIRED_SWITCH = "wiredswitch";
//const QByteArray GSETTINGS_SCHEMA = "org.ukui.kylin-nm.switch";

LanPage::LanPage(QWidget *parent) : TabPage(parent)
{

    m_activeResourse = new KyActiveConnectResourse(this);
    m_connectResourse = new KyConnectResourse(this);
    m_device = new KyNetworkDeviceResourse(this);
    m_nullLanItem = new LanListItem();

    m_devList.empty();

    initUI();

    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA)) {
        m_switchGsettings = new QGSettings(GSETTINGS_SCHEMA);
        initNetSwitch();
    } else {
        qDebug()<<"[LanPage] org.ukui.kylin-nm.switch is not installed!";
    }

    initDeviceCombox();
    initList(m_deviceName);

    connect(m_activeResourse, &KyActiveConnectResourse::stateChangeReason, this, &LanPage::updateLanlist);
    connect(m_connectResourse, &KyConnectResourse::connectionAdd, this, &LanPage::addConnectionSlot);
    connect(m_connectResourse, &KyConnectResourse::connectionRemove, this, &LanPage::removeConnectionSlot);

    connect(m_device, &KyNetworkDeviceResourse::deviceAdd, this, &LanPage::onDeviceAdd);
    connect(m_device, &KyNetworkDeviceResourse::deviceRemove, this, &LanPage::onDeviceRemove);
    connect(m_device, &KyNetworkDeviceResourse::deviceNameUpdate, this, &LanPage::onDeviceNameUpdate);
}

LanPage::~LanPage()
{

}

void LanPage::initNetSwitch()
{
    if (m_switchGsettings->keys().contains(WIRED_SWITCH)) {
        m_netSwitch->setSwitchStatus(m_switchGsettings->get(WIRED_SWITCH).toBool());
        initDeviceCombox();
        connect(m_switchGsettings, &QGSettings::changed, this, &LanPage::onSwithGsettingsChanged);
    }
    connect(m_netSwitch, &SwitchButton::clicked, this, &LanPage::onLanSwitchClicked);
}

void LanPage::onSwithGsettingsChanged(const QString &key)
{
    if (key == WIRED_SWITCH) {
        m_netSwitch->blockSignals(true);

        qDebug()<<"[LanPage] SwitchButton statue changed to ::" <<m_switchGsettings->get(WIRED_SWITCH).toBool();

        KyWiredConnectOperation wiredOperation;

        bool isOn = m_switchGsettings->get(WIRED_SWITCH).toBool();
        if (isOn) {
            for (int index = 0; index < m_devList.size(); ++index) {
                qDebug()<<"[LanPage] open wired device "<< m_devList.at(index);
                wiredOperation.openWiredNetworkWithDevice(m_devList.at(index));
            }

            m_inactivatedLanListWidget->show();
            initDeviceCombox();
        } else {
            for (int index = 0; index < m_devList.size(); ++index) {
                qDebug()<<"[LanPage] close wired device "<< m_devList.at(index);
                wiredOperation.closeWiredNetworkWithDevice(m_devList.at(index));
            }

            m_inactivatedLanListWidget->hide();
            m_deviceFrame->hide();
        }

        m_netSwitch->setSwitchStatus(isOn);
        m_netSwitch->blockSignals(false);
    }
}

void LanPage::onLanSwitchClicked()
{
    qDebug()<<"[LanPage] On lan switch button clicked! Status:" <<m_netSwitch->getSwitchStatus();

    KyWiredConnectOperation wiredOperation;

    if (m_netSwitch->getSwitchStatus()) {
        for (int index = 0; index < m_devList.size(); ++index) {
            qDebug()<<"[LanPage] open wired device "<< m_devList.at(index);
            wiredOperation.openWiredNetworkWithDevice(m_devList.at(index));
        }

        m_inactivatedLanListWidget->show();
        m_switchGsettings->set(WIRED_SWITCH,true);
        initDeviceCombox();
    } else {
        for (int index = 0; index < m_devList.size(); ++index) {
            qDebug()<<"[LanPage] close wired device "<< m_devList.at(index);
            wiredOperation.closeWiredNetworkWithDevice(m_devList.at(index));
        }
        m_inactivatedLanListWidget->hide();
        m_deviceFrame->hide();
        m_switchGsettings->set(WIRED_SWITCH,false);
    }
}

void LanPage::removeConnectionSlot(QString path)            //删除时后端会自动断开激活，将其从未激活列表中删除
{
    //for dbus
    qDebug() << "[LanPage] emit lanRemove because removeConnectionSlot " << path;
    emit lanRemove(path);

    qDebug()<<"[LanPage] Removing a connection, path:"<<path;
    bool isLan = false;
    QEventLoop loop;
    QTimer::singleShot(200, &loop, SLOT(quit()));
    loop.exec();
    QMap<KyConnectItem *, QListWidgetItem *>::iterator iters;
    for (iters = m_deactiveMap.begin(); iters != m_deactiveMap.end(); ++iters) {
        KyConnectItem *item = iters.key();
        if (item->m_connectPath == path) {
            qDebug()<<"[LanPage] Remove a connection from inactive list";
            m_inactivatedLanListWidget->removeItemWidget(iters.value());
            delete(iters.value());
            m_deactiveMap.erase(iters);
            isLan = true;

            break;
        }
    }
}

void LanPage::addConnectionSlot(QString uuid)               //新增一个有线连接，将其加入到激活列表
{
    QString devName;
    NetworkManager::ConnectionSettings::ConnectionType type;
    if(m_connectResourse->getInterfaceByUuid(devName, type, uuid)) {
        if (type != NetworkManager::ConnectionSettings::ConnectionType::Wired) {
            qDebug() << "[LanPage] addConnectionSlot but type is not Wired";
            return;
        }
    } else {
        qDebug() << "[LanPage] addConnectionSlot but uuid is invalid";
    }

    //for dbus
    KyConnectItem *item = nullptr;
    item = m_connectResourse->getConnectionItemByUuid(uuid, devName);
    if (nullptr != item) {
        QStringList info;
        info << item->m_connectName << uuid << item->m_connectPath;
        qDebug() << "[LanPage] emit lanAdd because addConnection " << devName;
        emit lanAdd(devName, info);
    }

    KyConnectItem * newItem = m_connectResourse->getConnectionItemByUuid(uuid, m_deviceName);
    if (newItem != nullptr) {
        if (newItem->m_itemType == NetworkManager::ConnectionSettings::ConnectionType::Wired) {
            qDebug()<<"[LanPage] Add a new connection, name:"<<newItem->m_connectName;
            addNewItem(newItem, m_inactivatedLanListWidget);
            m_deactiveMap.insert(newItem, m_listWidgetItem);
        }

    } else {
        qDebug()<<"[LanPage] GetConnectionItemByUuid is empty when add a new!";
    }
}
void LanPage::connectionUpdateSlot(QString uuid)
{
    //for dbus
    QStringList devNameList;
    if (m_activeResourse->isActiveConnection(uuid, devNameList)) {
        for (int i = 0; i < devNameList.size(); ++i) {
            KyConnectItem *item = nullptr;
            item = m_activeResourse->getActiveConnectionByUuid(uuid, devNameList.at(i));
            if (nullptr != item) {
                if (item->m_itemType != NetworkManager::ConnectionSettings::ConnectionType::Wired) {
                    return;
                }
                QStringList info;
                info << item->m_connectName << uuid << item->m_connectPath;
                emit lanUpdate(devNameList.at(i), info);
            }
        }
    } else {
        QString devName;
        NetworkManager::ConnectionSettings::ConnectionType type;
        if (!m_connectResourse->getInterfaceByUuid(devName, type, uuid)) {
            return;
        }
        if (type != NetworkManager::ConnectionSettings::ConnectionType::Wired) {
            return;
        }
        KyConnectItem *item = nullptr;
        item = m_connectResourse->getConnectionItemByUuid(uuid, devName);
        if (nullptr != item) {
            QStringList info;
            info << item->m_connectName << uuid << item->m_connectPath;
            emit lanUpdate(devName, info);
        }
    }
}

//void LanPage::initDevice()
//{
//    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
//    m_settings->beginGroup("DEFAULTCARD");
//    QString key("wired");
//    m_deviceName = m_settings->value(key, "").toString();
//    if (m_deviceName.isEmpty()) {
//        qDebug() << "initDevice but  defalut wired card is null";
//        if (!m_devList.isEmpty()) {
//            m_deviceName = m_devList.at(0);
//            m_settings->setValue(key, m_deviceName);
//        }
//    }
//    updateDefaultDevice(m_deviceName);
//    qDebug() << "[LanPage] initDevice defaultDevice = " << m_deviceName;

//    m_settings->endGroup();
//    m_settings->sync();
//    delete m_settings;
//    m_settings = nullptr;
//    for (int i = 0; i < m_devList.size(); ++i) {
//            qDebug() << "[LanPage] Device: " << m_devList.at(i);
//    }
//}

void LanPage::initDeviceCombox()
{
    //TODO 获取设备列表，单设备时隐藏下拉框，多设备时添加到下拉框
    QMap<QString, bool> enableMap;
    getDeviceEnableState(0,enableMap);
    m_deviceComboBox->clear();
    m_devList.clear();

    bool isOn = m_switchGsettings->get(WIRED_SWITCH).toBool();
    if (!isOn) {
        m_deviceFrame->hide();
    } else {
        m_device->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, m_devList);
        if (m_devList.size() == 1) {
            m_deviceFrame->hide();
            m_deviceName = m_devList.at(0);
            initList(m_deviceName);
            return;
        } else {
            for (int i=0; i<m_devList.size(); ++i) {
                m_deviceFrame->show();
                m_deviceComboBox->addItem(m_devList.at(i));
            }
        }
        qDebug() << "[LanPage]Current device:" << m_deviceComboBox->currentText();
        m_deviceName = m_deviceComboBox->currentText();
        initList(m_deviceName);
    }
}

void LanPage::onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType)
{
    if (deviceType !=  NetworkManager::Device::Type::Ethernet) {
        return;
    }

    qDebug() << "[LanPage] Begin add device:" << deviceName;

    m_devList << deviceName;

    if (getDefaultDevice().isEmpty()) {
        updateDefaultDevice(deviceName);
        setDefaultDevice(WIRED, deviceName);
    }
    emit deviceStatusChanged();
    initDeviceCombox();
}

void LanPage::onDeviceRemove(QString deviceName)
{
    qDebug() << "[LanPage] deviceRemove:" << deviceName;
    if (getDefaultDevice() == deviceName) {
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

    if (m_devList.contains(deviceName)) {
        m_devList.removeOne(deviceName);
    }

    emit deviceStatusChanged();
    initDeviceCombox();
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
       qDebug() << "[LanPage] emit deviceNameUpdate "  << oldName << newName;

       emit deviceNameChanged(oldName, newName);
       initDeviceCombox();
   }
}

void LanPage::onDeviceComboxIndexChanged(int currentIndex)
{
    //TODO 设备变更时更新设备和列表
    m_deviceName = m_deviceComboBox->currentText();
    qDebug() << "[LanPage]Device Changed:" << m_deviceName;
    initList(m_deviceName);
}

void LanPage::initUI()
{
    m_titleLabel->setText(tr("LAN"));
    m_netSwitch->resize(SWITCH_WIDTH, SWITCH_HEIGHT);

    m_activatedNetLabel->setText(tr("Activated LAN"));
    m_activatedLanListWidget = new QListWidget(m_activatedNetFrame);
    m_activatedLanListWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_activatedLanListWidget->setSpacing(LAN_LIST_SPACING);
    m_activatedLanListWidget->setFixedHeight(ITEM_HEIGHT);              //active区域固定高度,只显示一个条目
    m_activatedLanListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_activatedNetLayout->addWidget(m_activatedLanListWidget);

    m_inactivatedNetLabel->setText(tr("Inactivated LAN"));

    m_inactivatedNetListArea->setBackgroundRole(QPalette::Base);
    m_inactivatedNetListArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_inactivatedNetListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    m_inactivatedNetListArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QVBoxLayout *inactiveLanListLayout = new QVBoxLayout(m_inactivatedNetListArea);
    inactiveLanListLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    inactiveLanListLayout->setSpacing(MAIN_LAYOUT_SPACING);

    m_inactivatedLanListWidget = new QListWidget(m_inactivatedNetListArea);
    m_inactivatedLanListWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_inactivatedLanListWidget->setSpacing(LAN_LIST_SPACING);
    m_inactivatedLanListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    m_inactivatedLanListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   //用了listwidget的滚动条

    inactiveLanListLayout->addWidget(m_inactivatedLanListWidget);
}

void LanPage::addNewItem(KyConnectItem *itemData, QListWidget *listWidget)
{
    m_listWidgetItem = new QListWidgetItem(listWidget);
    m_listWidgetItem->setSizeHint(QSize(listWidget->width(),ITEM_HEIGHT));
    listWidget->addItem(m_listWidgetItem);

    m_testLanItem = new LanListItem(itemData, m_deviceName);
    qDebug() << "[LanPage] addNewItem, connection: " << itemData->m_connectName << "deviceName: " << m_deviceName;
    listWidget->setItemWidget(m_listWidgetItem, m_testLanItem);
}

void LanPage::initList(QString m_deviceName)       //程序拉起，初始化显示
{
    m_activatedLanListWidget->clear();
    m_inactivatedLanListWidget->clear();
    m_activeMap.clear();
    m_deactiveMap.clear();
    m_activedList.clear();
    m_deactivedList.clear();

    m_activeResourse->getActiveConnectionList(m_deviceName,
                               NetworkManager::ConnectionSettings::Wired, m_activedList);      //激活列表的显示
    qDebug() << "[LanPage]init list! Active list:" << m_activedList.size() << "Deactive list:" << m_deactivedList.size();
    if (!m_activedList.isEmpty()) {
        for (int i = 0; i < m_activedList.size(); i++) {
            KyConnectItem *activeItemData = m_activedList.at(i);
            addNewItem(activeItemData, m_activatedLanListWidget);

            m_activeMap.insert(activeItemData, m_listWidgetItem);
        }
    } else {
        m_nullItem = new QListWidgetItem(m_activatedLanListWidget);
        m_nullItem->setSizeHint(QSize(m_activatedLanListWidget->width(),ITEM_HEIGHT));
        m_activatedLanListWidget->addItem(m_nullItem);

        m_nullLanItem = new LanListItem();
        m_activatedLanListWidget->setItemWidget(m_nullItem, m_nullLanItem);
    }

    m_connectResourse->getConnectionList(m_deviceName, NetworkManager::ConnectionSettings::Wired, m_deactivedList);      //未激活列表的显示
    if (!m_deactivedList.isEmpty()) {
        for (int i = 0; i < m_deactivedList.size(); i++) {
            KyConnectItem *deactiveItemData = m_deactivedList.at(i);
            addNewItem(deactiveItemData, m_inactivatedLanListWidget);

            m_deactiveMap.insert(deactiveItemData, m_listWidgetItem);
        }
    }
}

void LanPage::updateLanlist(QString uuid, NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason)
{
    qDebug()<<"[LanPage] State change slot:"<<state;
    QString devName;
    NetworkManager::ConnectionSettings::ConnectionType type;

    if(m_connectResourse->getInterfaceByUuid(devName, type, uuid)) {
        if (type != NetworkManager::ConnectionSettings::ConnectionType::Wired) {
            qDebug() << "[LanPage] updateLanlist but type is not Wired";
            return;
        }
    } else {
        qDebug() << "[LanPage] updateLanlist but uuid is invalid";
    }

    QStringList devNameList;
    if (m_activeResourse->isActiveConnection(uuid, devNameList)) {
        for (int i = 0; i < devNameList.size(); ++i) {
            KyConnectItem *item = nullptr;
            item = m_activeResourse->getActiveConnectionByUuid(uuid, devNameList.at(i));
            if (nullptr != item) {
                QStringList info;
                info << item->m_connectName << uuid << item->m_connectPath;
                emit lanActiveConnectionStateChanged(devNameList.at(i), uuid, state);
            }
        }
    } else {
        qDebug() << "emit lanActiveConnectionStateChanged" << devName << uuid << state;
        emit lanActiveConnectionStateChanged(devName, uuid, state);
    }

    qDebug() << "[LanPage] Update Device Name:" << m_deviceName;

    if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        qDebug()<<"Get a deactivate, begin to remove it from activeList";
        QMap<KyConnectItem *, QListWidgetItem *>::iterator i;
        for (i = m_activeMap.begin(); i != m_activeMap.constEnd(); ++i) {                    //有新断开，若在激活列表里则删掉
            KyConnectItem *m_item = i.key();
            if (m_item->m_connectUuid == uuid) {
                m_activatedLanListWidget->removeItemWidget(i.value());
                delete(i.value());
                break;
            }
        }

        bool hasDeactiveNet = false;
        QMap<KyConnectItem *, QListWidgetItem *>::iterator iter;
        for (iter = m_deactiveMap.begin(); iter != m_deactiveMap.end(); ++iter) {           //检查其是否已经在未激活列表中
            KyConnectItem *m_item = iter.key();
            if (m_item->m_connectUuid == uuid) {
                hasDeactiveNet = true;
                break;
            }
        }
        qDebug()<<"The deactive has been in DeactiveNet:"<<hasDeactiveNet;

        QMap<KyConnectItem *, QListWidgetItem *>::iterator iters;
        for (iters = m_activeMap.begin(); iters != m_activeMap.end(); ++iters) {           //在未激活列表中增加一项连接
            KyConnectItem *m_item = iters.key();
            if (m_item->m_connectUuid == uuid && !hasDeactiveNet) {
                qDebug()<<"Add a deactive connection to inactiveList:"<<m_item->m_connectName;
                m_item->m_connectState = state;
                addNewItem(m_item, m_inactivatedLanListWidget);

                m_deactiveMap.insert(m_item, m_listWidgetItem);
                m_activeMap.erase(iters);
                break;
            }
        }
    } else if (state == NetworkManager::ActiveConnection::State::Activated) {
        qDebug()<<"Get an actived connection, begin to move it from deactive to avtive!";
        QMap<KyConnectItem *, QListWidgetItem *>::iterator iter;                                                           //在未激活列表里删除
        for (iter = m_deactiveMap.begin(); iter != m_deactiveMap.constEnd(); ++iter) {
            KyConnectItem *m_item = iter.key();
            if (m_item->m_connectUuid == uuid) {
                m_activatedLanListWidget->clear();
                qDebug()<<"Add an active connection to activeList:"<<m_item->m_connectName;
                m_item->m_connectState = state;
                addNewItem(m_item, m_activatedLanListWidget);

                m_inactivatedLanListWidget->removeItemWidget(iter.value());
                delete(iter.value());

                m_activeMap.insert(m_item, m_listWidgetItem);
                m_deactiveMap.erase(iter);
                break;
            }
        }
    }
    else if (state == NetworkManager::ActiveConnection::State::Activating){

    }

    if (m_activeMap.count() <= 0) {
        m_activatedLanListWidget->clear();
        m_nullItem = new QListWidgetItem(m_activatedLanListWidget);
        m_nullItem->setSizeHint(QSize(m_activatedLanListWidget->width(),ITEM_HEIGHT));
        m_activatedLanListWidget->addItem(m_nullItem);
        m_nullLanItem = new LanListItem();
        qDebug() << "[LanPage] Add nullItem!";
        m_activatedLanListWidget->setItemWidget(m_nullItem, m_nullLanItem);
    }
}

void LanPage::getWiredList(QMap<QString, QVector<QStringList> > &map)
{
    QStringList devlist;
    m_device->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, devlist);
    if (devlist.isEmpty()) {
        return;
    }

    foreach (auto deviceName, devlist) {
        QList<KyConnectItem *> activedList;
        QList<KyConnectItem *> deactivedList;
        QVector<QStringList> vector;
        m_activeResourse->getActiveConnectionList(deviceName,NetworkManager::ConnectionSettings::Wired,activedList);
        if (!activedList.isEmpty()) {
            vector.append(QStringList() << activedList.at(0)->m_connectName << activedList.at(0)->m_connectUuid << activedList.at(0)->m_connectPath);
        } else {
            vector.append(QStringList()<<("--"));
        }

        m_connectResourse->getConnectionList(deviceName, NetworkManager::ConnectionSettings::Wired, deactivedList);      //未激活列表的显示
        if (!deactivedList.isEmpty()) {
            for (int i = 0; i < deactivedList.size(); i++) {
                vector.append(QStringList()<<deactivedList.at(i)->m_connectName<<deactivedList.at(i)->m_connectUuid);
            }
        }
        map.insert(deviceName, vector);
    }
    return;
}

void LanPage::setWiredDeviceEnable(const QString& devName, bool enable)
{
    saveDeviceEnableState(devName, enable);
    initDeviceCombox();
}

void LanPage::activateWired(const QString& devName, const QString& connUuid)
{
    qDebug() << "activateWired" << devName << connUuid;
    KyWiredConnectOperation a;
    a.activateConnection(connUuid, devName);
}

void LanPage::deactivateWired(const QString& devName, const QString& connUuid)
{
    qDebug() << "deactivateWired" << devName << connUuid;
    KyConnectItem *item = nullptr;
    item = m_activeResourse->getActiveConnectionByUuid(connUuid, devName);
    if (nullptr == item) {
        //todo: 通知桌面
        qDebug() << "not ActiveConnection";
        return;
    }

    KyWiredConnectOperation a;
    a.deactivateWiredConnection(item->m_connectName, connUuid);
}

void LanPage::showDetailPage(QString devName, QString uuid)
{
    KyConnectItem *item = nullptr;
    bool isActive = true;
    item = m_activeResourse->getActiveConnectionByUuid(uuid, devName);
    if (nullptr == item) {
        item = m_connectResourse->getConnectionItemByUuid(uuid, devName);
        if (nullptr == item) {
            qWarning()<<"[LanPage] GetConnectionItemByUuid is empty when showDetailPage";
            return;
        }
        isActive= false;
    }

    NetDetail *netDetail = new NetDetail(devName, item->m_connectName, uuid, isActive, false, false, this);
    netDetail->show();
    delete item;
    item = nullptr;
}
