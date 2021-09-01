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

LanPage::LanPage(QWidget *parent) : TabPage(parent)
{
    m_activeResourse = new KyActiveConnectResourse;
    m_connectResourse = new KyConnectResourse;
    m_device = new KyNetworkDeviceResourse;
    initDevice();
    initUI();
    initList();
    connect(m_activeResourse, &KyActiveConnectResourse::stateChangeReason, this, &LanPage::updateLanlist);
    connect(m_connectResourse, &KyConnectResourse::connectionAdd, this, &LanPage::addConnectionSlot);
    connect(m_connectResourse, &KyConnectResourse::connectionRemove, this, &LanPage::removeConnectionSlot);
}

LanPage::~LanPage()
{

}

void LanPage::removeConnectionSlot(QString path)            //删除时后端会自动断开激活，将其从未激活列表中删除
{
    qDebug()<<"[LanPage] Removing a connection, path:"<<path;
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

            //for dbus
            qDebug() << "[LanPage] because removeConnectionSlot " << item->m_ifaceName;
            emit listUpdate(item->m_ifaceName);

            break;
        }
    }
}

void LanPage::addConnectionSlot(QString uuid)               //新增一个有线连接，将其加入到激活列表
{
    KyConnectItem * newItem = m_connectResourse->getConnectionItemByUuid(uuid, m_deviceName);
    if (newItem != nullptr) {
        qDebug()<<"[LanPage] Add a new connection, name:"<<newItem->m_connectName;
        LanListItem * newListItem = new LanListItem(newItem, m_deviceName);
        newListItem->setFixedHeight(ITEM_HEIGHT);

        m_listWidgetItem = new QListWidgetItem(m_inactivatedLanListWidget);
        m_listWidgetItem->setSizeHint(QSize(m_inactivatedLanListWidget->width(),ITEM_HEIGHT));

        m_inactivatedLanListWidget->addItem(m_listWidgetItem);
        m_inactivatedLanListWidget->setItemWidget(m_listWidgetItem, newListItem);
        m_deactiveMap.insert(newItem, m_listWidgetItem);

        //for dbus
        qDebug() << "[LanPage] because addConnectionSlot " << newItem->m_ifaceName;
        emit listUpdate(newItem->m_ifaceName);
    } else {
        qDebug()<<"[LanPage] GetConnectionItemByUuid is empty when add a new!";
    }
}

void LanPage::initDevice()
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    QString key("wired");
    m_deviceName = m_settings->value(key, "").toString();
    if (m_deviceName.isEmpty()) {
        qDebug() << "initDevice but  defalut wired card is null";
        QStringList list;
        list.empty();
        m_device->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, list);
        if (!list.isEmpty()) {
            m_deviceName = list.at(0);
            m_settings->setValue(key, m_deviceName);
        }
    }
    updateDefaultDevice(m_deviceName);
    qDebug() << "[LanPage] initDevice defaultDevice = " << m_deviceName;

    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;
}

void LanPage::initUI()
{
    m_titleLabel->setText(tr("LAN"));
    m_netSwitch->resize(SWITCH_WIDTH, SWITCH_HEIGHT);

    m_activatedNetLabel->setText(tr("Activated LAN"));
    m_activatedLanListWidget = new QListWidget(m_activatedNetFrame);
    m_activatedLanListWidget->setFrameShape(QFrame::Shape::NoFrame);
//    m_activatedLanListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_activatedLanListWidget->setSpacing(LAN_LIST_SPACING);
    m_activatedLanListWidget->setFixedHeight(ITEM_HEIGHT);              //active区域固定高度,只显示一个条目
    m_activatedNetLayout->addWidget(m_activatedLanListWidget);

    m_inactivatedNetLabel->setText(tr("Inactivated LAN"));
    m_inactivatedNetListArea->setBackgroundRole(QPalette::Base);
    m_inactivatedNetListArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_inactivatedNetListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_inactivatedNetListArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_inactivatedLanListWidget = new QListWidget(m_inactivatedNetListArea);
    m_inactivatedLanListWidget->setFrameShape(QFrame::Shape::NoFrame);
//    m_inactivatedLanListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedLanListWidget->setSpacing(LAN_LIST_SPACING);
    QVBoxLayout *inactiveLanListLayout = new QVBoxLayout(m_inactivatedNetListArea);
    inactiveLanListLayout->addWidget(m_inactivatedLanListWidget);

//    KyConnectItem *activeConnectItem = new KyConnectItem();
//    activeConnectItem->m_connectUuid = "e8f384a4-2214-361b-b328-eb36b36136ee";
//    activeConnectItem->m_connectName = "有线连接1";
//    activeConnectItem->m_connectPath = "fsfsef";
////    activeConnectItem->m_connectState = NetworkManager::ActiveConnection::State::Activated;
//    m_testLanItem = new LanListItem(activeConnectItem);
//    m_testLanItem->setMinimumHeight(ITEM_HEIGHT);

//    m_listWidgetItem = new QListWidgetItem(m_inactivatedLanListWidget);
//    m_inactivatedLanListWidget->addItem(m_listWidgetItem);
//    m_inactivatedLanListWidget->setItemWidget(m_listWidgetItem, m_testLanItem);

}

void LanPage::initList()       //程序拉起，初始化显示
{
    m_activeResourse->getActiveConnectionList(m_deviceName,NetworkManager::ConnectionSettings::Wired,m_activedList);      //激活列表的显示
    if (!m_activedList.isEmpty()) {
        for (int i = 0; i < m_activedList.size(); i++) {
            m_listWidgetItem = new QListWidgetItem(m_activatedLanListWidget);
            m_listWidgetItem->setSizeHint(QSize(m_activatedLanListWidget->width(),ITEM_HEIGHT));
            m_activatedLanListWidget->addItem(m_listWidgetItem);

            KyConnectItem *activeItemData = m_activedList.at(i);

            activeConnectItem = new LanListItem(activeItemData, m_deviceName);

            m_activatedLanListWidget->setItemWidget(m_listWidgetItem, activeConnectItem);

            m_activeMap.insert(activeItemData, m_listWidgetItem);
        }
    }

    m_connectResourse->getConnectionList(m_deviceName, NetworkManager::ConnectionSettings::Wired, m_deactivedList);      //未激活列表的显示
    if (!m_deactivedList.isEmpty()) {
        for (int i = 0; i < m_deactivedList.size(); i++) {
            m_listWidgetItem = new QListWidgetItem(m_inactivatedLanListWidget);
            m_listWidgetItem->setSizeHint(QSize(m_inactivatedLanListWidget->width(),ITEM_HEIGHT));
            m_inactivatedLanListWidget->addItem(m_listWidgetItem);

            KyConnectItem *deactiveItemData = m_deactivedList.at(i);

            deactiveConnectItem = new LanListItem(deactiveItemData ,m_deviceName);

//            deactiveConnectItem->setFixedHeight(ITEM_HEIGHT);
            m_inactivatedLanListWidget->setItemWidget(m_listWidgetItem, deactiveConnectItem);

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
            qDebug() << "[LanPage] updateLanlist but type is not wired";
            return;
        }
    } else {
        qDebug() << "[LanPage] updateLanlist but uuid is invalid";
    }



    if (state == NetworkManager::ActiveConnection::State::Activating) {
        qDebug() << "[LanPage] wiredActivating " << devName;
        emit wiredActivating(devName,uuid);
    }

    if (state == NetworkManager::ActiveConnection::State::Activated || state == NetworkManager::ActiveConnection::State::Deactivated)
    {
        qDebug() << "[LanPage] because updateLanlist " <<devName;
        emit listUpdate(devName);
    }

    if (state == NetworkManager::ActiveConnection::State::Activated) {
        qDebug()<<"Get an actived connection, begin to move it from deactive to avtive!";
        QMap<KyConnectItem *, QListWidgetItem *>::iterator iter;                                                           //在未激活列表里删除
        for (iter = m_deactiveMap.begin(); iter != m_deactiveMap.constEnd(); ++iter) {
            KyConnectItem *m_Item = iter.key();
            if (m_Item->m_connectUuid == uuid) {
                qDebug()<<"Add an active connection to activeList:"<<m_Item->m_connectName;
                m_Item->m_connectState = state;

                m_listWidgetItem = new QListWidgetItem(m_activatedLanListWidget);
                m_listWidgetItem->setSizeHint(QSize(m_inactivatedLanListWidget->width(),ITEM_HEIGHT));
                m_activatedLanListWidget->addItem(iter.value());

                m_activeItem = new LanListItem(m_Item ,m_deviceName);

                m_activatedLanListWidget->setItemWidget(m_listWidgetItem, m_activeItem);

                m_inactivatedLanListWidget->removeItemWidget(iter.value());
                delete(iter.value());

                m_activeMap.insert(m_Item, m_listWidgetItem);
                m_deactiveMap.erase(iter);
                break;
            }
        }
    } else if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        qDebug()<<"Get a deactivate, begin to remove it from activeList";
        QMap<KyConnectItem *, QListWidgetItem *>::iterator i;
        for (i = m_activeMap.begin(); i != m_activeMap.constEnd(); ++i) {                    //有新断开，若在激活列表里则删掉
            KyConnectItem *m_Item = i.key();
            if (m_Item->m_connectUuid == uuid) {
                m_activatedLanListWidget->removeItemWidget(i.value());
                delete(i.value());
                break;
            }
        }

        bool hasDeactiveNet = false;
        QMap<KyConnectItem *, QListWidgetItem *>::iterator iter;
        for (iter = m_deactiveMap.begin(); iter != m_deactiveMap.end(); ++iter) {           //检查其是否已经在未激活列表中
            KyConnectItem *m_Item = iter.key();
            if (m_Item->m_connectUuid == uuid) {
                hasDeactiveNet = true;
                break;
            }
        }
        qDebug()<<"The deactive has been in DeactiveNet:"<<hasDeactiveNet;

        QMap<KyConnectItem *, QListWidgetItem *>::iterator iters;
        for (iters = m_activeMap.begin(); iters != m_activeMap.end(); ++iters) {           //在未激活列表中增加一项连接
            KyConnectItem *m_Item = iters.key();
            if (m_Item->m_connectUuid == uuid && !hasDeactiveNet) {
                qDebug()<<"Add a deactive connection to inactiveList:"<<m_Item->m_connectName;
                m_Item->m_connectState = state;
                m_deactiveItem = new LanListItem(m_Item ,m_deviceName);
                m_deactiveItem->setFixedHeight(ITEM_HEIGHT);

                m_listWidgetItem = new QListWidgetItem(m_inactivatedLanListWidget);
                m_listWidgetItem->setSizeHint(QSize(m_inactivatedLanListWidget->width(),ITEM_HEIGHT));

                m_inactivatedLanListWidget->addItem(m_listWidgetItem);
                m_inactivatedLanListWidget->setItemWidget(m_listWidgetItem, m_deactiveItem);

                m_deactiveMap.insert(m_Item, m_listWidgetItem);

                m_activeMap.erase(iters);
                break;
            }
        }
    } else if (state == NetworkManager::ActiveConnection::State::Activating){

    }
}


bool LanPage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_netSwitch) {
//        if(event == click)
//        {

//        }
    }
    return QWidget::eventFilter(watched, event);
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
            vector.append(QStringList()<<activedList.at(0)->m_connectName<<activedList.at(0)->m_connectUuid);
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

void LanPage::activateWired(const QString& devName, const QString& connUuid)
{
    //todo:
}

void LanPage::deactivateWired(const QString& devName, const QString& connUuid)
{
    //todo:
}
