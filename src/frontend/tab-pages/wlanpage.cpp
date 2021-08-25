#include "wlanpage.h"
#include "wlanlistitem.h"
#include "kywirelessnetitem.h"
#include <QEvent>
#include <QDateTime>
#include <QDebug>
#include <QSettings>

WlanPage::WlanPage(QWidget *parent) : TabPage(parent)
{
    m_resource = new KyWirelessNetResource(this);
    m_device   = new KyNetworkDeviceResourse(this);
    initDevice();
    initWlanUI();
    initConnections();
    getActiveWlan();
    getAllWlan();
}

bool WlanPage::eventFilter(QObject *w, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        if (w == m_hiddenWlanLabel) {
            //ZJP_TODO 打开隐藏WiFi添加弹窗
        } else if (w == m_settingsLabel) {
            //ZJP_TODO 打开控制面板
        }
    }
    return QWidget::eventFilter(w,e);
}

void WlanPage::initWlanUI()
{
    m_titleLabel->setText(tr("WLAN"));
    m_activatedNetLabel->setText(tr("Activated WLAN"));
    m_inactivatedNetLabel->setText(tr("Inactivated WLAN"));

    //一些独有控件
    m_inactivatedWlanListAreaCentralWidget = new QFrame(m_inactivatedNetListArea);
    m_inactivatedNetListArea->setBackgroundRole(QPalette::Base);
    m_inactivatedNetListArea->setWidget(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedWlanListAreaLayout = new QVBoxLayout(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedWlanListAreaCentralWidget->setLayout(m_inactivatedWlanListAreaLayout);
    m_inactivatedWlanListAreaLayout->setSpacing(MAIN_LAYOUT_SPACING);
    m_inactivatedWlanListAreaLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);

    m_inactivatedNetListWidget = new QListWidget(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedNetListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedNetListWidget->setSpacing(NET_LIST_SPACING);
    m_inactivatedNetListWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_inactivatedNetListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_hiddenWlanWidget = new QFrame(m_inactivatedWlanListAreaCentralWidget);
    m_hiddenWlanLayout = new QHBoxLayout(m_hiddenWlanWidget);
    m_hiddenWlanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_hiddenWlanLabel = new QLabel(m_hiddenWlanWidget);
    m_hiddenWlanLabel->setText(tr("More..."));
    m_hiddenWlanLabel->setContentsMargins(MORE_TEXT_MARGINS);
    m_hiddenWlanLayout->addWidget(m_hiddenWlanLabel);
    m_hiddenWlanLayout->addStretch();

    m_inactivatedWlanListAreaLayout->addWidget(m_inactivatedNetListWidget);
    m_inactivatedWlanListAreaLayout->addWidget(m_hiddenWlanLabel);
    m_inactivatedWlanListAreaLayout->addStretch();

    m_activatedNetListWidget->setFixedHeight(NORMAL_HEIGHT);
//    m_inactivatedNetListArea->setFixedHeight(SCROLLAREA_HEIGHT);
    m_inactivatedNetListArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/**
 * @brief WlanPage::initConnections 初始化一些信号槽
 */
void WlanPage::initConnections()
{
    connect(m_resource, &KyWirelessNetResource::wifiNetworkAdd, this, &WlanPage::onWlanAdded);
    connect(m_resource, &KyWirelessNetResource::wifiNetworkRemove, this, &WlanPage::onWlanRemoved);
    connect(m_resource, &KyWirelessNetResource::wifiNetworkUpdate, this, &WlanPage::onWlanUpdated);
}

/**
 * @brief WlanPage::initDevice 初始化默认网卡
 */
void WlanPage::initDevice()
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    QString key("wireless");
    QString deviceName = m_settings->value(key, "").toString();
    if (deviceName.isEmpty()) {
        qDebug() << "initDevice but  defalut wireless card is null";
        QStringList list;
        list.empty();
        m_device->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, list);
        if (!list.isEmpty()) {
            deviceName = list.at(0);
            m_settings->setValue(key, deviceName);
        }
    }
    updateDefaultDevice(deviceName);
    qDebug() << "[WlanPage] initDevice defaultDevice = " << deviceName;
    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;

}

/**
 * @brief WlanPage::getActiveWlan 获取所有已激活连接
 */
void WlanPage::getActiveWlan()
{
    QMap<QString,QStringList> actMap;
    m_activatedNetListWidget->clear();
    m_resource->getWirelessActiveConnection(NetworkManager::ActiveConnection::State::Activated, actMap);
    QMap<QString,QStringList>::iterator iter = actMap.begin();
    int height = 0;
    while (iter != actMap.end()) {
        if (iter.key() == "wlp3s0" && !iter.value().isEmpty()) {
            QString ssid = iter.value().at(0);
            m_activatedWlanSSid = ssid;

            KyWirelessNetItem data;
            if (!m_resource->getWifiNetwork("wlp3s0", ssid, data)) {
                return;
            }
            KyWirelessNetItem *item_data = new KyWirelessNetItem(data);
            WlanListItem *wlanItemWidget = new WlanListItem(m_resource, item_data);
            qDebug() << "Activated wlan: ssid = " << item_data->m_NetSsid;
            QListWidgetItem *wlanItem = new QListWidgetItem(m_activatedNetListWidget);
            wlanItem->setSizeHint(QSize(m_activatedNetListWidget->width(), wlanItemWidget->height()));
            m_activatedNetListWidget->addItem(wlanItem);
            m_activatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
            height += wlanItemWidget->height();
            break;
        }
        iter ++;
    }
    if (height > 0) {
        m_activatedNetListWidget->setFixedHeight(height);
    } else {
        //ZJP_TODO 未连接任何WiFi的情况
        m_activatedWlanSSid.clear();
    }
}

/**
 * @brief WlanPage::getAllWlan 获取所有Wlan //ZJP_TODO 需要针对网卡进行筛选
 */
void WlanPage::getAllWlan()
{
    qDebug() << "Started loading wlan list! time=" << QDateTime::currentDateTime().toString("hh:mm:ss.zzzz");
    m_inactivatedNetListWidget->clear();
    m_itemsMap.clear();
    QList<KyWirelessNetItem> wlanList;
//    if (!m_resource->getAllDeviceWifiNetwork(map))
    if (!m_resource->getDeviceWifiNetwork("wlp3s0", wlanList)) //ZJP_TODO 获取默认网卡并传入
    {
        return;
    }
    int height = 0;
    foreach (auto itemData, wlanList) {
        if (itemData.m_NetSsid == this->m_activatedWlanSSid) {
            continue;
        }

        KyWirelessNetItem *data = new KyWirelessNetItem(itemData);
        WlanListItem *wlanItemWidget = new WlanListItem(m_resource, data);
        QListWidgetItem *wlanItem = new QListWidgetItem(m_inactivatedNetListWidget);
        m_itemsMap.insert(data->m_NetSsid, wlanItem);
        wlanItem->setSizeHint(QSize(m_inactivatedNetListWidget->width(), wlanItemWidget->height()));
        m_inactivatedNetListWidget->addItem(wlanItem);
        m_inactivatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
        if (height == 0) {
            height += wlanItemWidget->height();
        }
        height += wlanItemWidget->height() + NET_LIST_SPACING;
    }
    m_inactivatedNetListWidget->setFixedHeight(height);
    m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());
    qDebug() << "Stopped loading wlan list! time=" << QDateTime::currentDateTime().toString("hh:mm:ss.zzzz");
}

void WlanPage::onWlanAdded(QString interface, KyWirelessNetItem &item)
{
    qDebug() << "A Wlan Added! interface = " << interface << "; ssid = " << item.m_NetSsid << Q_FUNC_INFO <<__LINE__;

    KyWirelessNetItem *data = new KyWirelessNetItem(item);
    WlanListItem *wlanItemWidget = new WlanListItem(m_resource, data);
    QListWidgetItem *wlanItem = new QListWidgetItem(m_inactivatedNetListWidget);
    wlanItem->setSizeHint(QSize(m_inactivatedNetListWidget->width(), wlanItemWidget->height()));
    m_inactivatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
//    m_inactivatedNetListWidget->insertItem(N, wlanItem);
    m_inactivatedNetListWidget->addItem(wlanItem); //ZJP_TODO 目前会添加到列表尾部
    m_inactivatedNetListWidget->setFixedHeight(m_inactivatedNetListWidget->height() + wlanItemWidget->height() + NET_LIST_SPACING);
    m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());

    m_itemsMap.insert(data->m_NetSsid, wlanItem);
}

void WlanPage::onWlanRemoved(QString interface, QString ssid)
{
    if (!m_itemsMap.contains(ssid)) { return; }
    qDebug() << "A Wlan Removed! interface = " << interface << "; ssid = " << ssid << Q_FUNC_INFO <<__LINE__;
    m_inactivatedNetListWidget->removeItemWidget(m_itemsMap.value(ssid));
    m_inactivatedNetListWidget->setFixedHeight(m_inactivatedNetListWidget->height() -
                                               m_inactivatedNetListWidget->itemWidget(m_itemsMap.value(ssid))->height() -
                                               NET_LIST_SPACING);
    m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());
    m_itemsMap.remove(ssid);
}

void WlanPage::onWlanUpdated()
{
    //ZJP_TODO 某些特定情况下不可重绘整个列表，此处代码需要修改
    getAllWlan();
}
