#include "wlanpage.h"
#include "wlanlistitem.h"
#include "kywirelessnetitem.h"
#include <QEvent>
#include <QDateTime>
#include <QDebug>

WlanPage::WlanPage(QWidget *parent) : TabPage(parent)
{
    m_resource = new KyWirelessNetResource(this);
    initWlanUI();
    initConnections();
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
    connect(m_resource, &KyWirelessNetResource::wifiNetworkUpadte, this, &WlanPage::onWlanUpdated);
}

/**
 * @brief WlanPage::getAllWlan 初始化所有Wlan
 */
void WlanPage::getAllWlan()
{
    qDebug() << "Started loading wlan list! time=" << QDateTime::currentDateTime().toString("hh:mm:ss.zzzz");
    m_inactivatedNetListWidget->clear();
    QMap<QString, QList<KyWirelessNetItem> > map;
    if (!m_resource->getAllDeviceWifiNetwork(map))
    {
        return;
    }
    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = map.begin();
    int height = 0;
    while (iter != map.end())
    {
        foreach (auto itemData, iter.value()) {
            KyWirelessNetItem *data = &itemData;
            WlanListItem *wlanItemWidget = new WlanListItem(m_resource, data);
            QListWidgetItem *wlanItem = new QListWidgetItem(m_inactivatedNetListWidget);
            wlanItem->setSizeHint(QSize(m_inactivatedNetListWidget->width(), wlanItemWidget->height()));
            m_inactivatedNetListWidget->addItem(wlanItem);
            m_inactivatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
            if (height == 0) height += wlanItemWidget->height();
            height += wlanItemWidget->height() + NET_LIST_SPACING;
        }
        iter++;
    }
    m_inactivatedNetListWidget->setFixedHeight(height);
    m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());
    qDebug() << "Stopped loading wlan list! time=" << QDateTime::currentDateTime().toString("hh:mm:ss.zzzz");
}

void WlanPage::onWlanAdded(QString interface, KyWirelessNetItem &item)
{
    qDebug() << "A Wlan Added! interface = " << interface << "; ssid = " << item.m_NetSsid << Q_FUNC_INFO <<__LINE__;
    getAllWlan();
}

void WlanPage::onWlanRemoved(QString interface, QString ssid)
{
    qDebug() << "A Wlan Removed! interface = " << interface << "; ssid = " << ssid << Q_FUNC_INFO <<__LINE__;
    getAllWlan();
}

void WlanPage::onWlanUpdated()
{
    getAllWlan();
}
