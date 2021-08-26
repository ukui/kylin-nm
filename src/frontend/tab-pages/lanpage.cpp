#include "lanpage.h"
#include <QDebug>

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define TITLE_FRAME_HEIGHT 52
#define TITLE_LAYOUT_MARGINS 24,0,24,0
#define LAN_LAYOUT_MARGINS 8,8,8,8
#define LAN_LAYOUT_SPACING 8
#define LAN_LIST_SPACING 2
#define TEXT_MARGINS 16,0,0,0
//#define SCROLL_AREA_HEIGHT 200
#define SETTINGS_LAYOUT_MARGINS 24,16,24,16
#define TRANSPARENT_COLOR QColor(0,0,0,0)
#define SWITCH_WIDTH 48
#define SWITCH_HEIGHT 24

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
    connect(m_connectResourse, &KyConnectResourse::connectionRemove, this, &LanPage::RemoveConnectionSlot);
}

LanPage::~LanPage()
{

}

void LanPage::RemoveConnectionSlot(QString path)
{
    qDebug()<<"___remove____";
    QEventLoop loop;
    QTimer::singleShot(200, &loop, SLOT(quit()));
    loop.exec();
    QMap<KyConnectItem *, QListWidgetItem *>::iterator iters;
    for (iters = m_deactiveMap.begin(); iters != m_deactiveMap.end(); ++iters) {
        KyConnectItem *item = iters.key();
        if (item->m_connectPath == path) {
            qDebug() << "D_E_L_E_T_E___D_E_A_C_T_I_V_E";
            m_inactivatedLanListWidget->removeItemWidget(iters.value());
            delete(iters.value());
            m_deactiveMap.erase(iters);
            break;
        }
    }
}
void LanPage::addConnectionSlot(QString uuid)
{
    KyConnectItem * newItem = m_connectResourse->getConnectionItemByUuid(uuid, deviceName);
    if (newItem != nullptr) {
        LanListItem * newListItem = new LanListItem(newItem, deviceName);
        qDebug() << "N_E_W";
        newListItem->setFixedHeight(48);
        m_listWidgetItem = new QListWidgetItem(m_inactivatedLanListWidget);
        m_listWidgetItem->setSizeHint(QSize(m_inactivatedLanListWidget->width(),48));

        m_inactivatedLanListWidget->addItem(m_listWidgetItem);
        m_inactivatedLanListWidget->setItemWidget(m_listWidgetItem, newListItem);
        m_deactiveMap.insert(newItem, m_listWidgetItem);
    } else {
        qDebug()<<"getConnectionItemByUuid is empty!";
    }
}

void LanPage::initDevice()
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    QString key("wired");
    deviceName = m_settings->value(key, "").toString();
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

void LanPage::initUI()
{
    m_titleLabel->setText(tr("LAN"));

    m_netSwitch->resize(SWITCH_WIDTH, SWITCH_HEIGHT);

    m_activatedNetLabel->setText(tr("Activated LAN"));
    m_activatedLanListWidget = new QListWidget(m_activatedNetFrame);
    m_activatedLanListWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_activatedLanListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_activatedLanListWidget->setSpacing(LAN_LIST_SPACING);
    m_activatedLanListWidget->setFixedHeight(48);              //active区域固定高度？
    m_activatedNetLayout->addWidget(m_activatedLanListWidget);

    m_inactivatedNetLabel->setText(tr("Inactivated LAN"));

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::transparent/*TRANSPARENT_COLOR*/);
    m_inactivatedNetListArea->setPalette(pal);
    m_inactivatedLanListWidget = new QListWidget(m_inactivatedNetListArea);
    m_inactivatedLanListWidget->setFrameShape(QFrame::Shape::NoFrame);
    QHBoxLayout *inLanListLayout = new QHBoxLayout(m_inactivatedNetListArea);
    inLanListLayout->addWidget(m_inactivatedLanListWidget);

    m_inactivatedLanListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_inactivatedNetListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_inactivatedNetListArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_inactivatedLanListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedLanListWidget->setSpacing(LAN_LIST_SPACING);

    m_inactivatedNetLayout->addWidget(m_inactivatedNetListArea);

//    KyConnectItem *activeConnectItem = new KyConnectItem();
//    activeConnectItem->m_connectUuid = "e8f384a4-2214-361b-b328-eb36b36136ee";
//    activeConnectItem->m_connectName = "有线连接1";
//    activeConnectItem->m_connectPath = "fsfsef";
////    activeConnectItem->m_connectState = NetworkManager::ActiveConnection::State::Activated;
//    m_testLanItem = new LanListItem(activeConnectItem);
//    m_testLanItem->setMinimumHeight(48);

//    m_listWidgetItem = new QListWidgetItem(m_inactivatedLanListWidget);
//    m_inactivatedLanListWidget->addItem(m_listWidgetItem);
//    m_inactivatedLanListWidget->setItemWidget(m_listWidgetItem, m_testLanItem);

}

void LanPage::initList()       //程序拉起，初始化显示
{
    m_activatedLanListWidget->setFixedHeight(60);
    m_activeResourse->getActiveConnectionList(deviceName,NetworkManager::ConnectionSettings::Wired,m_activedList);      //激活列表的显示
    if (!m_activedList.isEmpty()) {
        m_listWidgetItem = new QListWidgetItem(m_activatedLanListWidget);
        m_listWidgetItem->setSizeHint(QSize(m_activatedLanListWidget->width(),48));
        m_activatedLanListWidget->addItem(m_listWidgetItem);
        for (int i = 0; i < m_activedList.size(); i++) {
            KyConnectItem *activeItemData = m_activedList.at(i);
            activeConnectItem = new LanListItem(activeItemData, deviceName);
            m_activeMap.insert(activeItemData, m_listWidgetItem);
            activeConnectItem->setFixedHeight(48);
            m_activatedLanListWidget->setItemWidget(m_listWidgetItem, activeConnectItem);       //还未确保激活！
        }
    }

    m_connectResourse->getConnectionList(deviceName, NetworkManager::ConnectionSettings::Wired, m_deactivedList);      //未激活列表的显示
    if (!m_deactivedList.isEmpty()) {
        for (int i = 0; i < m_deactivedList.size(); i++) {
            m_listWidgetItem = new QListWidgetItem(m_inactivatedLanListWidget);
            m_listWidgetItem->setSizeHint(QSize(100,48));
            m_inactivatedLanListWidget->addItem(m_listWidgetItem);

            KyConnectItem *deactiveItemData = m_deactivedList.at(i);
//            qDebug()<<m_activedList.at(i);
            deactiveConnectItem = new LanListItem(deactiveItemData ,deviceName);

            m_deactiveMap.insert(deactiveItemData, m_listWidgetItem);

            deactiveConnectItem->setFixedHeight(48);
            m_inactivatedLanListWidget->setItemWidget(m_listWidgetItem, deactiveConnectItem);    //显示时要不要确保状态？
        }
    }
}

void LanPage::updateLanlist(QString uuid, NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason)
{
    qDebug() << "S_T_A_T_E:" << state;
    if (state == NetworkManager::ActiveConnection::State::Activated) {
        QMap<KyConnectItem *, QListWidgetItem *>::iterator i;                                                           //在未激活列表里删除
        for (i = m_deactiveMap.begin(); i != m_deactiveMap.constEnd(); ++i) {
            KyConnectItem *m_Item = i.key();
            if (m_Item->m_connectUuid == uuid) {
                m_Item->m_connectState = state;
                m_activeItem = new LanListItem(m_Item ,deviceName);
                m_activeItem->setFixedHeight(48);
                m_listWidgetItem = new QListWidgetItem(m_activatedLanListWidget);
                m_listWidgetItem->setSizeHint(QSize(m_inactivatedLanListWidget->width(),48));

                m_activatedLanListWidget->addItem(i.value());
                m_activatedLanListWidget->setItemWidget(m_listWidgetItem, m_activeItem);

                m_activeMap.insert(m_Item, m_listWidgetItem);
                m_inactivatedLanListWidget->removeItemWidget(i.value());
                delete(i.value());
                m_deactiveMap.erase(i);
                break;
            }
        }
    } else if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        qDebug()<<"deactivated happend, begin remove item";
        QMap<KyConnectItem *, QListWidgetItem *>::iterator i;
        for (i = m_activeMap.begin(); i != m_activeMap.constEnd(); ++i) {                                  //有新断开，在激活列表里删掉
            KyConnectItem *m_Item = i.key();
            if (m_Item->m_connectUuid == uuid) {
                m_inactivatedLanListWidget->removeItemWidget(i.value());
                delete(i.value());
                break;
            }
        }
        qDebug()<<"deactivated happend, begin add new item";
        bool hasDeactiveNet = false;
        QMap<KyConnectItem *, QListWidgetItem *>::iterator iter;
        for (iter = m_deactiveMap.begin(); iter != m_deactiveMap.end(); ++iter) {
            KyConnectItem *m_Item = iter.key();
            if (m_Item->m_connectUuid == uuid) {
                hasDeactiveNet = true;
                break;
            }
        }
        qDebug()<<"hasDeactiveNet:"<<hasDeactiveNet;

        QMap<KyConnectItem *, QListWidgetItem *>::iterator iters;
        for (iters = m_activeMap.begin(); iters != m_activeMap.end(); ++iters) {
            KyConnectItem *m_Item = iters.key();
            if (m_Item->m_connectUuid == uuid && !hasDeactiveNet) {
                m_Item->m_connectState = state;
                m_deactiveItem = new LanListItem(m_Item ,deviceName);
                m_deactiveItem->setFixedHeight(48);

                m_listWidgetItem = new QListWidgetItem(m_inactivatedLanListWidget);
                m_listWidgetItem->setSizeHint(QSize(100,48));

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

