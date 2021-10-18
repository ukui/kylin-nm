#include "lanpage.h"
#include <QDebug>

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define TITLE_FRAME_HEIGHT 52
#define TITLE_LAYOUT_MARGINS 24,0,24,0
#define LAN_LIST_SPACING 2
#define TEXT_MARGINS 16,0,0,0
#define SETTINGS_LAYOUT_MARGINS 24,16,24,16
#define TRANSPARENT_COLOR QColor(0,0,0,0)
#define SWITCH_WIDTH 48
#define SWITCH_HEIGHT 24
#define ITEM_HEIGHT 48

const QString INVALID_CONNECT_UUID = "emptyconnect";

const QString WIRED_SWITCH = "wiredswitch";

LanPage::LanPage(QWidget *parent) : TabPage(parent)
{
    m_activeResourse = new KyActiveConnectResourse(this);
    m_connectResourse = new KyConnectResourse(this);
    m_deviceResource = new KyNetworkDeviceResourse(this);

    initUI();
    initNetSwitch();
    initLanDevice();
    initDeviceCombox();
    initLanArea();

    connect(m_activeResourse, &KyActiveConnectResourse::stateChangeReason, this, &LanPage::onUpdateLanlist);

    connect(m_connectResourse, &KyConnectResourse::connectionAdd, this, &LanPage::onAddConnection);
    connect(m_connectResourse, &KyConnectResourse::connectionRemove, this, &LanPage::onRemoveConnection);
    connect(m_connectResourse, &KyConnectResourse::connectionUpdate, this, &LanPage::onUpdateConnection);

    connect(m_deviceResource, &KyNetworkDeviceResourse::deviceAdd, this, &LanPage::onDeviceAdd);
    connect(m_deviceResource, &KyNetworkDeviceResourse::deviceRemove, this, &LanPage::onDeviceRemove);
    connect(m_deviceResource, &KyNetworkDeviceResourse::deviceNameUpdate, this, &LanPage::onDeviceNameUpdate);

    connect(m_wiredConnectOperation, &KyWiredConnectOperation::activateConnectionError, this, &LanPage::activateFailed);
    connect(m_wiredConnectOperation, &KyWiredConnectOperation::deactivateConnectionError, this, &LanPage::deactivateFailed);

}

LanPage::~LanPage()
{

}

void LanPage::initLanDevice()
{
    m_devList.clear();
    m_deviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, m_devList);

    m_currentDeviceName = getDefaultDeviceName(WIRED);

    QMap<QString, bool> deviceStateMap;
    getDeviceEnableState(WIRED, deviceStateMap);

    QStringList disableDeviceList;
    disableDeviceList.clear();
    m_enableDeviceList.clear();
    for (int index = 0; index < m_devList.count(); ++index) {
        QString deviceName = m_devList.at(index);
        if (deviceStateMap.contains(deviceName)) {
            if (deviceStateMap[deviceName]) {
                m_enableDeviceList<<deviceName;
            } else {
                disableDeviceList<<deviceName;
            }
        } else {
            saveDeviceEnableState(deviceName, true);
            m_enableDeviceList<<deviceName;
        }
    }

    KyWiredConnectOperation wiredOperation;
    if (m_wiredSwitch) {
        for (int index = 0; index < disableDeviceList.count(); ++index) {
            wiredOperation.closeWiredNetworkWithDevice(disableDeviceList.at(index));
        }
    } else {
        for (int index = 0; index < m_devList.count(); ++index) {
            wiredOperation.closeWiredNetworkWithDevice(m_devList.at(index));
        }
    }

    return;
}

void LanPage::initNetSwitch()
{
    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA)) {
        m_switchGsettings = new QGSettings(GSETTINGS_SCHEMA);
        if (m_switchGsettings->keys().contains(WIRED_SWITCH)) {
            m_wiredSwitch = m_switchGsettings->get(WIRED_SWITCH).toBool();
            m_netSwitch->setSwitchStatus(m_wiredSwitch);
            connect(m_switchGsettings, &QGSettings::changed, this, &LanPage::onSwithGsettingsChanged);
        }
        connect(m_netSwitch, &SwitchButton::clicked, this, &LanPage::onLanSwitchClicked);
    } else {
        qDebug()<<"[LanPage] org.ukui.kylin-nm.switch is not installed!";
    }
}

void LanPage::onSwithGsettingsChanged(const QString &key)
{
    if (key == WIRED_SWITCH) {
        m_netSwitch->blockSignals(true);

        m_wiredSwitch = m_switchGsettings->get(WIRED_SWITCH).toBool();
        qDebug()<<"[LanPage] SwitchButton statue changed to:" << m_wiredSwitch;

        KyWiredConnectOperation wiredOperation;
        if (m_wiredSwitch) {
            for (int index = 0; index < m_enableDeviceList.size(); ++index) {
                qDebug()<<"[LanPage] open wired device "<< m_enableDeviceList.at(index);
                wiredOperation.openWiredNetworkWithDevice(m_enableDeviceList.at(index));
            }
        } else {
            for (int index = 0; index < m_enableDeviceList.size(); ++index) {
                qDebug()<<"[LanPage] close wired device "<< m_enableDeviceList.at(index);
                wiredOperation.closeWiredNetworkWithDevice(m_enableDeviceList.at(index));
            }
        }

        initDeviceCombox();
        initLanArea();

        m_netSwitch->setSwitchStatus(m_wiredSwitch);

        m_netSwitch->blockSignals(false);
    }
}

void LanPage::onLanSwitchClicked()
{
    qDebug()<<"[LanPage] On lan switch button clicked! Status:" <<m_netSwitch->getSwitchStatus();

    if (m_netSwitch->getSwitchStatus()) {
        m_switchGsettings->set(WIRED_SWITCH, true);
    } else {
        m_switchGsettings->set(WIRED_SWITCH, false);
    }
}

void LanPage::getEnabledDevice(QStringList &enableDeviceList)
{
    int index = 0;
    QMap<QString, bool> deviceMap;

    if (m_devList.isEmpty()) {
        qDebug()<<"[LanPage] there is not wired device.";
        return;
    }

    getDeviceEnableState(WIRED, deviceMap);
    for (index = 0; index < m_devList.size(); ++index) {
        if (deviceMap.contains(m_devList.at(index))) {
            if (deviceMap[m_devList.at(index)]) {
                enableDeviceList << m_devList.at(index);
            }
        } else {
            saveDeviceEnableState(m_devList.at(index), true);
            enableDeviceList << m_devList.at(index);
        }
    }

    return;
}

void LanPage::getDisabledDevices(QStringList &disableDeviceList)
{
    int index = 0;
    QMap<QString, bool> deviceMap;

    if (m_devList.isEmpty()) {
        qDebug()<<"[LanPage] there is not wired device.";
        return;
    }

    getDeviceEnableState(WIRED, deviceMap);
    for (index = 0; index < m_devList.size(); ++index) {
        if (deviceMap.contains(m_devList.at(index))) {
            if (!deviceMap[m_devList.at(index)]) {
                disableDeviceList << m_devList.at(index);
            }
        }
    }

    return;
}

void LanPage::initDeviceCombox()
{
    //TODO 获取设备列表，单设备时隐藏下拉框，多设备时添加到下拉框;m_devList记录插入的所有设备，deviceMap记录设备状态
    disconnect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                this, &LanPage::onDeviceComboxIndexChanged);

    m_deviceComboBox->clear();

    if (m_wiredSwitch) {
        int enableDeviceCount = m_enableDeviceList.count();
        if (enableDeviceCount > 1) {
            for (int index = 0; index < enableDeviceCount; ++index) {
                m_deviceComboBox->addItem(m_enableDeviceList.at(index));
            }

            m_deviceFrame->show();
            m_tipsLabel->hide();
            m_deviceComboBox->show();

            if (m_currentDeviceName != m_deviceComboBox->currentText()) {
                if (m_enableDeviceList.contains(m_currentDeviceName)) {
                    m_deviceComboBox->setCurrentText(m_currentDeviceName);
                } else {
                    m_currentDeviceName = m_deviceComboBox->currentText();
                    setDefaultDevice(WIRED, m_currentDeviceName);
                }
            }

        } else if (enableDeviceCount == 1) {
            m_deviceFrame->hide();

            if (m_currentDeviceName != m_enableDeviceList.at(0)) {
                m_currentDeviceName = m_enableDeviceList.at(0);
                setDefaultDevice(WIRED, m_currentDeviceName);
            }
        } else {
            m_deviceFrame->show();
            m_deviceComboBox->hide();
            m_tipsLabel->show();
            m_currentDeviceName = "";
        }
    } else {
        m_deviceFrame->hide();
        m_currentDeviceName = "";
    }

    connect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LanPage::onDeviceComboxIndexChanged);
    return;
}

bool LanPage::connectionItemIsExist(QMap<KyConnectItem *, QListWidgetItem *> &connectMap,
                                   QString uuid)
{
    QMap<KyConnectItem *, QListWidgetItem *>::iterator iter;
    for (iter = connectMap.begin(); iter != connectMap.end(); ++iter) {           //检查其是否已经在未激活列表中
        KyConnectItem *p_item = iter.key();
        if (p_item->m_connectUuid == uuid) {
            return true;
        }
    }

    return false;
}

void LanPage::addEmptyConnectItem(QMap<KyConnectItem *, QListWidgetItem *> &connectMap,
                                       QListWidget *lanListWidget)
{
    KyConnectItem *p_nullItem = new KyConnectItem();
    p_nullItem->setConnectUuid(INVALID_CONNECT_UUID);
    QListWidgetItem *p_listWidgetItem = addNewItem(nullptr, lanListWidget);
    connectMap.insert(p_nullItem, p_listWidgetItem);

    return;
}


void LanPage::deleteConnectionMapItem(QMap<KyConnectItem *, QListWidgetItem *> &connectMap,
                             QListWidget *lanListWidget, QString uuid)
{     
    QMap<KyConnectItem *, QListWidgetItem *>::iterator iter;

    iter = connectMap.begin();
    while (iter != connectMap.end()) {
        KyConnectItem *p_item = iter.key();
        if (p_item->m_connectUuid == uuid
                || INVALID_CONNECT_UUID == p_item->m_connectUuid) {
            qDebug()<<"[LanPage] delete connection map item"
                    << p_item->m_connectName << p_item->m_connectUuid;
            QListWidgetItem *p_widgetItem = iter.value();
            LanListItem *p_lanItem = (LanListItem *)lanListWidget->itemWidget(p_widgetItem);

            lanListWidget->removeItemWidget(p_widgetItem);

            delete p_lanItem;
            p_lanItem = nullptr;

            delete p_widgetItem;
            p_widgetItem = nullptr;

            iter = connectMap.erase(iter);

            delete p_item;
            p_item = nullptr;

            continue;
        }

        iter++;
    }

    return;
}

void LanPage::clearConnectionMap(QMap<KyConnectItem *, QListWidgetItem *> &connectMap,
                                 QListWidget *lanListWidget)
{
    QMap<KyConnectItem *, QListWidgetItem *>::iterator iter;

    iter = connectMap.begin();
    while (iter != connectMap.end()) {
        KyConnectItem *p_connectItem = iter.key();
        QListWidgetItem *p_widgetItem = iter.value();

        qDebug()<<"[LanPage] clear connection map item"<<p_connectItem->m_connectName;

        LanListItem *p_lanItem = (LanListItem *)lanListWidget->itemWidget(p_widgetItem);
        lanListWidget->removeItemWidget(p_widgetItem);

        delete p_lanItem;
        p_lanItem = nullptr;

        delete p_widgetItem;
        p_widgetItem = nullptr;

        delete p_connectItem;
        p_connectItem = nullptr;

        iter = connectMap.erase(iter);
    }

    return;
}

void LanPage::constructActiveConnectionArea()
{
    QList<KyConnectItem *> activedList;

    activedList.clear();
    clearConnectionMap(m_activeMap, m_activatedLanListWidget);

    m_activeResourse->getActiveConnectionList(m_currentDeviceName,
                               NetworkManager::ConnectionSettings::Wired, activedList);      //激活列表的显示
    qDebug() << "[LanPage] construct active connection area get Active list size:" << activedList.size();
    if (!activedList.isEmpty()) {
        for (int index = 0; index < activedList.size(); index++) {
            KyConnectItem *activeItemData = activedList.at(index);
            qDebug()<<"[LanPage] construct active connection area add active item"<<activeItemData->m_connectName;
            QListWidgetItem *p_listWidgetItem = addNewItem(activeItemData, m_activatedLanListWidget);
            m_activeMap.insert(activeItemData, p_listWidgetItem);
        }
    } else {
        qDebug()<<"[LanPage] there is not active";
        addEmptyConnectItem(m_activeMap, m_activatedLanListWidget);
    }

    return;
}

void LanPage::constructConnectionArea()
{
    QList<KyConnectItem *> deactivedList;

    deactivedList.clear();
    clearConnectionMap(m_deactiveMap, m_inactivatedLanListWidget);

    m_connectResourse->getConnectionList(m_currentDeviceName, NetworkManager::ConnectionSettings::Wired, deactivedList);      //未激活列表的显示
    qDebug() << "[LanPage]construct connection area get connection list size:" << deactivedList.size();
    if (!deactivedList.isEmpty()) {
        for (int index = 0; index < deactivedList.size(); index++) {
            KyConnectItem *deactiveItemData = deactivedList.at(index);
            qDebug()<<"[LanPage] construct connection area add deactive item"<<deactiveItemData->m_connectName;
            QListWidgetItem *p_listWidgetItem = addNewItem(deactiveItemData, m_inactivatedLanListWidget);
            m_deactiveMap.insert(deactiveItemData, p_listWidgetItem);
        }
    }

    return;
}

void LanPage::initLanArea()
{
    if (!m_wiredSwitch || m_currentDeviceName.isEmpty()) {
        m_activatedNetDivider->hide();
        m_activatedNetFrame->hide();

        m_inactivatedNetFrame->hide();
    } else {
        m_activatedNetDivider->show();
        m_activatedNetFrame->show();
        constructActiveConnectionArea();

        m_inactivatedNetFrame->show();
        constructConnectionArea();
    }

    return;
}

void LanPage::onRemoveConnection(QString path)            //删除时后端会自动断开激活，将其从未激活列表中删除
{
    //for dbus
    qDebug() << "[LanPage] emit lanRemove because onRemoveConnection " << path;
    emit lanRemove(path);

    QEventLoop loop;
    QTimer::singleShot(200, &loop, SLOT(quit()));
    loop.exec();

    QMap<KyConnectItem *, QListWidgetItem *>::iterator iters;
    for (iters = m_deactiveMap.begin(); iters != m_deactiveMap.end(); ++iters) {
        KyConnectItem *p_item = iters.key();
        if (p_item->m_connectPath == path) {
            qDebug()<<"[LanPage] Remove a connection from inactive list";

            LanListItem *p_lanItem = (LanListItem *)m_inactivatedLanListWidget->itemWidget(iters.value());

            m_inactivatedLanListWidget->removeItemWidget(iters.value());

            delete p_lanItem;
            p_lanItem = nullptr;

            delete(iters.value());
            m_deactiveMap.erase(iters);

            delete p_item;
            p_item = nullptr;

            break;
        }
    }

    return;
}

void LanPage::onAddConnection(QString uuid)               //新增一个有线连接，将其加入到激活列表
{
    if (!m_connectResourse->isWiredConnection(uuid)) {
        return;
    }

    KyConnectItem *p_newItem = nullptr;
    p_newItem = m_connectResourse->getConnectionItemByUuid(uuid);
    if (nullptr != p_newItem) {
        sendLanAddSignal(p_newItem);
    }

    if (p_newItem->m_ifaceName == m_currentDeviceName || p_newItem->m_ifaceName == "") {
        qDebug()<<"[LanPage] Add a new connection, name:"<<p_newItem->m_connectName;
        QListWidgetItem *p_listWidgetItem = addNewItem(p_newItem, m_inactivatedLanListWidget);
        m_deactiveMap.insert(p_newItem, p_listWidgetItem);
    } else {
        delete p_newItem;
        p_newItem = nullptr;
    }

    return;
}

void LanPage::addDeviceForCombox(QString deviceName)
{
    disconnect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                        this, &LanPage::onDeviceComboxIndexChanged);

    if (m_wiredSwitch) {
        if (1 == m_enableDeviceList.count()) {
            //1、从无到有添加第一块有线网卡
            //2、有多快网卡，但是没有使能
            m_deviceFrame->hide();
            m_currentDeviceName = deviceName;
            setDefaultDevice(WIRED, m_currentDeviceName);
        } else if (2 == m_enableDeviceList.count()) {
            //3、现在有且只有一块网卡，并已使能
            //4、有多快网卡，且使能了其中一块
            m_deviceComboBox->addItem(m_currentDeviceName);
            m_deviceComboBox->addItem(deviceName);

            m_deviceFrame->show();
            m_tipsLabel->hide();
            m_deviceComboBox->show();
        } else if (m_enableDeviceList.count() > 2) {
            //5、有多快网卡且使能了多块网卡
            m_deviceComboBox->addItem(deviceName);
        }
    }

    connect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LanPage::onDeviceComboxIndexChanged);
    return;
}

void LanPage::onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType)
{
    if (deviceType !=  NetworkManager::Device::Type::Ethernet) {
        return;
    }

    if (m_devList.contains(deviceName)) {
        return;
    }

    qDebug() << "[LanPage] Begin add device:" << deviceName;

    m_devList << deviceName;
    saveDeviceEnableState(deviceName, true);
    m_enableDeviceList<<deviceName;

    addDeviceForCombox(deviceName);
    if (m_currentDeviceName == deviceName) {
        initLanArea();
    }

    emit deviceStatusChanged();

    return;
}

void LanPage::deleteDeviceFromCombox(QString deviceName)
{
    disconnect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
               this, &LanPage::onDeviceComboxIndexChanged);

    if (m_wiredSwitch) {
        if (0 == m_enableDeviceList.count()) {
            //1、没有使能任何网卡
            goto l_out;
        } else if (1 == m_enableDeviceList.count()) {
             //2、使能了一个网卡，且当前网卡是要删除的网卡
            if (m_currentDeviceName == deviceName) {
                m_deviceFrame->show();
                m_deviceComboBox->hide();
                m_tipsLabel->show();
                m_currentDeviceName = "";
            }
        } else if (2 == m_enableDeviceList.count()) {
            //3、使能了两个网卡，且包括要删除的网卡，有可能是要删除的网卡
            if (m_deviceComboBox->findText(deviceName) != -1) {
                for (int index = m_deviceComboBox->count() - 1; index >= 0; index--) {
                    if (m_currentDeviceName == deviceName
                            && m_deviceComboBox->itemText(index) != deviceName) {
                        m_currentDeviceName = m_deviceComboBox->itemText(index);
                    }
                    m_deviceComboBox->removeItem(index);
                }

                m_tipsLabel->hide();
                m_deviceFrame->hide();
                m_deviceComboBox->hide();
            }
        } else {
            //4、使能了多个网卡，且包括要删除的网卡，有可能是正要删除的网卡
            int index = m_deviceComboBox->findText(deviceName);
            if (index != -1) {
                m_deviceComboBox->removeItem(index);
                if (m_currentDeviceName == deviceName) {
                    m_currentDeviceName = m_deviceComboBox->currentText();
                }
            }
        }
    }

l_out:
    connect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LanPage::onDeviceComboxIndexChanged);

    return;
}

void LanPage::onDeviceRemove(QString deviceName)
{
    if (!m_devList.contains(deviceName)) {
        return;
    }

    qDebug() << "[LanPage] deviceRemove:" << deviceName;

    QString nowDevice = m_currentDeviceName;
    deleteDeviceFromCombox(deviceName);
    if (nowDevice == deviceName) {
        setDefaultDevice(WIRED, m_currentDeviceName);
        initLanArea();
    }

    m_devList.removeOne(deviceName);
    if (m_enableDeviceList.contains(deviceName)) {
        m_enableDeviceList.removeOne(deviceName);
    }
    deleteDeviceEnableState(deviceName);

    emit deviceStatusChanged();

    return;
}

void LanPage::updateDeviceCombox(QString oldDeviceName, QString newDeviceName)
{   
    if (m_currentDeviceName == oldDeviceName) {
        m_currentDeviceName = newDeviceName;
    }

    int index = m_deviceComboBox->findText(oldDeviceName);
    if (index != -1) {
        m_deviceComboBox->setItemText(index, newDeviceName);
    }

    return;
}

void LanPage::onDeviceNameUpdate(QString oldName, QString newName)
{
    if (m_devList.contains(oldName)) {
        m_devList.removeOne(oldName);
        m_devList.append(newName);

        if (m_enableDeviceList.contains(oldName)) {
            m_enableDeviceList.removeOne(oldName);
            m_enableDeviceList.append(newName);
        }
        qDebug() << "[LanPage] emit deviceNameUpdate "  << oldName << newName;

        updateDeviceCombox(oldName, newName);
        if (m_currentDeviceName == newName) {
            setDefaultDevice(WIRED, m_currentDeviceName);
            initLanArea();
        }

        emit deviceNameChanged(oldName, newName);
    }
}

void LanPage::onDeviceComboxIndexChanged(int currentIndex)
{
    //TODO 设备变更时更新设备和列表
    m_currentDeviceName = m_deviceComboBox->currentText();
    qDebug() << "[LanPage]Current Device Changed to:" << m_currentDeviceName;
    initLanArea();
}

void LanPage::onShowControlCenter()
{
    QProcess process;
    process.startDetached("ukui-control-center --wiredconnect");
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
    m_settingsLabel->installEventFilter(this);
//    emit this->lanConnectChanged();
}

QListWidgetItem *LanPage::addNewItem(KyConnectItem *itemData, QListWidget *listWidget)
{
    QListWidgetItem *p_listWidgetItem = new QListWidgetItem(listWidget);
    p_listWidgetItem->setSizeHint(QSize(listWidget->width(),ITEM_HEIGHT));
    listWidget->addItem(p_listWidgetItem);

    LanListItem *p_lanItem = nullptr;
    if (itemData != nullptr) {
        p_lanItem = new LanListItem(itemData, m_currentDeviceName);
        qDebug() << "[LanPage] addNewItem, connection: " << itemData->m_connectName << "deviceName: " << m_currentDeviceName;
    }
    else {
        p_lanItem = new LanListItem();
        qDebug() << "[LanPage] Add nullItem!";
    }

    listWidget->setItemWidget(p_listWidgetItem, p_lanItem);

    return p_listWidgetItem;
}

void LanPage::updateActivatedConnectionArea(QString uuid)
{
    KyConnectItem *p_newItem = nullptr;
    p_newItem = m_activeResourse->getActiveConnectionByUuid(uuid);
    if (nullptr == p_newItem) {
        qWarning()<<"[LanPage] get active connection failed, connection uuid" << uuid;
        return;
    }

    //发送通知给控制面板
    emit lanActiveConnectionStateChanged(p_newItem->m_ifaceName, uuid, p_newItem->m_connectState);

    if (p_newItem->m_ifaceName == m_currentDeviceName) {
        qDebug()<<"[LanPage] update active connection area delete connection item "
               << p_newItem->m_connectName;
        deleteConnectionMapItem(m_deactiveMap, m_inactivatedLanListWidget, uuid);

        deleteConnectionMapItem(m_activeMap, m_activatedLanListWidget, INVALID_CONNECT_UUID);
        if (connectionItemIsExist(m_activeMap, uuid)) {
            delete p_newItem;
            p_newItem = nullptr;
            return;
        }

        qDebug()<<"[LanPage]update active connection item"<<p_newItem->m_connectName;
        QListWidgetItem *p_listWidgetItem = addNewItem(p_newItem, m_activatedLanListWidget);
        m_activeMap.insert(p_newItem, p_listWidgetItem);
    } else {
        //释放内存
        delete p_newItem;
        p_newItem = nullptr;
    }

    return;
}

void LanPage::updateConnectionArea(QString uuid)
{
    KyConnectItem *p_newItem = nullptr;
    p_newItem = m_connectResourse->getConnectionItemByUuid(uuid);
    if (nullptr == p_newItem) {
        qWarning()<<"[LanPage] get active connection failed, connection uuid" << uuid;
        return;
    }

    emit lanActiveConnectionStateChanged(p_newItem->m_ifaceName, uuid, p_newItem->m_connectState);

    deleteConnectionMapItem(m_activeMap, m_activatedLanListWidget, uuid);
    if (m_activeMap.count() <= 0) {
        addEmptyConnectItem(m_activeMap, m_activatedLanListWidget);
    }

    if (p_newItem->m_ifaceName == m_currentDeviceName || p_newItem->m_ifaceName == "") {
        qDebug()<<"[LanPage] update connection area"<<p_newItem->m_connectName;
        if (connectionItemIsExist(m_deactiveMap, uuid)) {
            delete p_newItem;
            p_newItem = nullptr;
            return;
        }

        qDebug()<<"[LanPage] update connection item"<<p_newItem->m_connectName;
        QListWidgetItem *p_listWidgetItem = addNewItem(p_newItem, m_inactivatedLanListWidget);
        m_deactiveMap.insert(p_newItem, p_listWidgetItem);

        emit this->lanConnectChanged();
    } else {
        delete p_newItem;
        p_newItem = nullptr;
    }

    return;
}

void LanPage::onUpdateLanlist(QString uuid,
                              NetworkManager::ActiveConnection::State state,
                              NetworkManager::ActiveConnection::Reason reason)
{
    //lanpage函数内持续监听连接状态的变化并记录供其他函数调用获取状态
    if (!m_connectResourse->isWiredConnection(uuid)) {
        return;
    }

    qDebug()<<"[LanPage] connection uuid"<< uuid
            << "state change slot:"<< state;

    if (state == NetworkManager::ActiveConnection::State::Activated) {
        updateActivatedConnectionArea(uuid);
    } else if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        updateConnectionArea(uuid);
    }

    return;
}

void LanPage::getWiredList(QMap<QString, QVector<QStringList> > &map)
{
    QStringList devlist;
    m_deviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, devlist);
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
                vector.append(QStringList()<<deactivedList.at(i)->m_connectName<<deactivedList.at(i)->m_connectUuid << deactivedList.at(i)->m_connectPath);
            }
        }
        map.insert(deviceName, vector);
    }
    return;
}

void LanPage::sendLanUpdateSignal(KyConnectItem *p_connectItem)
{
    QStringList info;
    info << p_connectItem->m_connectName << p_connectItem->m_connectUuid << p_connectItem->m_connectPath;
    emit lanUpdate(p_connectItem->m_ifaceName, info);

    return;
}

void LanPage::sendLanAddSignal(KyConnectItem *p_connectItem)
{
    QStringList info;
    info << p_connectItem->m_connectName << p_connectItem->m_connectUuid << p_connectItem->m_connectPath;
    qDebug() << "[LanPage] emit lanAdd because addConnection ";
    emit lanAdd(p_connectItem->m_ifaceName, info);

    return;
}

void LanPage::updateConnectionProperty(KyConnectItem *p_connectItem, bool &needDeleteItem)
{
    bool inCurrentMap = false;

    QMap<KyConnectItem *, QListWidgetItem *>::iterator iter;
    for (iter = m_deactiveMap.begin(); iter != m_deactiveMap.constEnd(); ++iter) {
        KyConnectItem *p_item = iter.key();
        if (p_item->m_connectUuid == p_connectItem->m_connectUuid) {
            inCurrentMap = true;
            if (p_connectItem->m_ifaceName != ""
                    && m_currentDeviceName != p_connectItem->m_ifaceName) {
                LanListItem *p_lanItem = (LanListItem *)m_inactivatedLanListWidget->itemWidget(iter.value());

                m_inactivatedLanListWidget->removeItemWidget(iter.value());

                delete p_lanItem;
                p_lanItem = nullptr;

                delete iter.value();
                m_deactiveMap.erase(iter);

                delete p_item;
                p_item = nullptr;
            } else {
                if (p_connectItem->m_connectName != p_item->m_connectName
                        || p_connectItem->m_connectPath != p_item->m_connectPath) {
                    LanListItem *p_lanItem = (LanListItem *)m_inactivatedLanListWidget->itemWidget(iter.value());

                    m_inactivatedLanListWidget->removeItemWidget(iter.value());

                    delete p_lanItem;
                    p_lanItem = nullptr;

                    delete iter.value();
                    m_deactiveMap.erase(iter);

                    delete p_item;
                    p_item = nullptr;

                    QListWidgetItem *p_listWidgetItem = addNewItem(p_connectItem, m_inactivatedLanListWidget);
                    m_deactiveMap.insert(p_connectItem, p_listWidgetItem);
                    needDeleteItem = false;
                }
            }
            break;
        }     
    }

    if (!inCurrentMap) {
        if (p_connectItem->m_ifaceName == m_currentDeviceName) {
            QListWidgetItem *p_listWidgetItem = addNewItem(p_connectItem, m_inactivatedLanListWidget);
            m_deactiveMap.insert(p_connectItem, p_listWidgetItem);
            needDeleteItem = false;
        }
    }

    return;
}

void LanPage::updateActiveConnectionProperty(KyConnectItem *p_connectItem, bool &needDeleteItem)
{
    QMap<KyConnectItem *, QListWidgetItem *>::iterator iters;
    for (iters = m_activeMap.begin(); iters != m_activeMap.constEnd(); ++iters) {
        KyConnectItem *p_item = iters.key();
        if (p_item->m_connectUuid == p_connectItem->m_connectUuid) {
            if (m_currentDeviceName != p_connectItem->m_ifaceName) {  //当前激活连接的设备改变
                LanListItem *p_lanItem = (LanListItem *)m_activatedLanListWidget->itemWidget(iters.value());
                m_activatedLanListWidget->removeItemWidget(iters.value());

                delete p_lanItem;
                p_lanItem = nullptr;

                delete iters.value();
                m_activeMap.erase(iters);

                delete p_item;
                p_item = nullptr;

            } else {
                if (p_item->m_connectName != p_connectItem->m_connectName
                        || p_item->m_connectPath != p_connectItem->m_connectPath) {   //当前激活连接的其他数据改变(除了激活状态外)
                    LanListItem *p_lanItem = (LanListItem *)m_activatedLanListWidget->itemWidget(iters.value());
                    m_activatedLanListWidget->removeItemWidget(iters.value());

                    delete p_lanItem;
                    p_lanItem = nullptr;

                    delete iters.value();
                    m_activeMap.erase(iters);

                    delete p_item;
                    p_item = nullptr;

                    QListWidgetItem *p_listWidgetItem = addNewItem(p_connectItem, m_activatedLanListWidget);
                    m_activeMap.insert(p_connectItem, p_listWidgetItem);
                    needDeleteItem = false;
                }
            }
            break;
        }
    }

    return;
}

void LanPage::onUpdateConnection(QString uuid)
{

    if (!m_connectResourse->isWiredConnection(uuid)) {
        return;
    }

    qDebug() << "[LanPage]:Connection property Changed." << Q_FUNC_INFO << __LINE__;

    KyConnectItem *p_newItem = nullptr;
    bool needDeleteNewItem = true;
    if (m_connectResourse->isActivatedConnection(uuid)) {
        p_newItem = m_activeResourse->getActiveConnectionByUuid(uuid);
        if (nullptr == p_newItem) {
            qWarning()<<"[LanPage] get item failed, when update activate connection."
                      <<"connection uuid" << uuid;
            return;
        }

        sendLanUpdateSignal(p_newItem);
        updateActiveConnectionProperty(p_newItem, needDeleteNewItem);
    } else {
        p_newItem = m_connectResourse->getConnectionItemByUuid(uuid);
        if (nullptr == p_newItem) {
            qWarning()<<"[LanPage] get item failed, when update connection."
                      <<"connection uuid"<<uuid;
            return;
        }

        sendLanUpdateSignal(p_newItem);
        updateConnectionProperty(p_newItem, needDeleteNewItem);
    }

    if (needDeleteNewItem) {
        qDebug()<<"[LanPage] the new item is not used, so is deleted.";
        delete p_newItem;
        p_newItem = nullptr;
    }

    return;
}

void LanPage::setWiredDeviceEnable(const QString& devName, bool enable)
{
    saveDeviceEnableState(devName, enable);

    KyWiredConnectOperation wiredOperation;
    if (enable) {
        wiredOperation.openWiredNetworkWithDevice(devName);

        m_enableDeviceList<<devName;

        addDeviceForCombox(devName);
        if (m_currentDeviceName == devName) {
            setDefaultDevice(WIRED, m_currentDeviceName);
            initLanArea();
        }
    } else {
        wiredOperation.closeWiredNetworkWithDevice(devName);

        QString nowDeviceName = m_currentDeviceName;
        deleteDeviceFromCombox(devName);
        if (nowDeviceName == devName) {
            setDefaultDevice(WIRED, m_currentDeviceName);
            initLanArea();
        }

        if (m_enableDeviceList.contains(devName)) {
            m_enableDeviceList.removeOne(devName);
        }
    }

    return;
}

bool LanPage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_settingsLabel) {
        if (event->type() == QEvent::MouseButtonPress) {
            onShowControlCenter();
        }
    }
    return QWidget::eventFilter(watched, event);
}

void LanPage::activateWired(const QString& devName, const QString& connUuid)
{
    qDebug() << "[LanPage] activateWired" << devName << connUuid;
    m_wiredConnectOperation->activateConnection(connUuid, devName);
    emit this->lanConnectChanged();
}

void LanPage::deactivateWired(const QString& devName, const QString& connUuid)
{
    qDebug() << "[LanPage] deactivateWired" << devName << connUuid;
    QString name("");
    m_wiredConnectOperation->deactivateWiredConnection(name, connUuid);
}

void LanPage::showDetailPage(QString devName, QString uuid)
{
    KyConnectItem *p_item = nullptr;
    bool isActive = true;

    if (m_connectResourse->isActivatedConnection(uuid)) {
        p_item = m_activeResourse->getActiveConnectionByUuid(uuid);
        isActive = true;
    } else {
        p_item = m_connectResourse->getConnectionItemByUuid(uuid);
        isActive = false;
    }

    if (nullptr == p_item) {
       qWarning()<<"[LanPage] GetConnectionItemByUuid is empty when showDetailPage."
                      <<"device name"<<devName
                      <<"connect uuid"<<uuid;
       return;
    }

    NetDetail *netDetail = new NetDetail(devName, p_item->m_connectName, uuid, isActive, false, false, this);
    netDetail->show();

    delete p_item;
    p_item = nullptr;
}

bool LanPage::lanIsConnected()
{
    if (m_activeMap.isEmpty()) {
        return false;
    } else {
        KyConnectItem *p_connectItem = m_activeMap.firstKey();
        if (p_connectItem->m_connectUuid == INVALID_CONNECT_UUID) {
            return false;
        }
    }

    return true;
}
