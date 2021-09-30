#include "kylinwiredwidget.h"

KyWiredWidget::KyWiredWidget(QWidget *parent):QWidget(parent)
{
    m_activeConnectResource = new KyActiveConnectResourse();
    m_connectResource  = new KyConnectResourse();
    m_deviceResource = new KyNetworkDeviceResourse();

    connect(m_activeConnectResource, &KyActiveConnectResourse::activeConnectRemove,
                                                this, &KyWiredWidget::removeActiveConnection);
    connect(m_activeConnectResource, &KyActiveConnectResourse::stateChangeReason,
                                                this, &KyWiredWidget::activeConnectStateChange);

    connect(m_connectResource, &KyConnectResourse::connectionAdd, this, &KyWiredWidget::addConnection);
    connect(m_connectResource, &KyConnectResourse::connectionRemove, this, &KyWiredWidget::removeConnection);
    connect(m_connectResource, &KyConnectResourse::connectionUpdate, this, &KyWiredWidget::updateConnection);

    connect(m_deviceResource, &KyNetworkDeviceResourse::carrierChanage,this, &KyWiredWidget::wiredDeviceCarrierChange);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateWidgetList()));
}
KyWiredWidget::~KyWiredWidget()
{
    delete m_activeConnectResource;
    delete m_connectResource;
    delete m_deviceResource;
}

void KyWiredWidget::constructWiredActiveConnectList()
{
    qDebug()<<"constructWiredActiveConnectList";
    QList<KyConnectItem *> wiredActiveConnect;
    m_activeConnectResource->getActiveConnectionList("enp2s0",
                                                     NetworkManager::ConnectionSettings::ConnectionType::Wired,
                                                     wiredActiveConnect);
//    if (wiredActiveConnect.isEmpty()) {
//        LanListItem *activeWiredForm = new LanListItem(this, nullptr);
//        activeWiredForm->constructActiveConnectionEmptyItem();
//        m_wiredActiveFormlist<<activeWiredForm;
//    } else {
//        for (int index = 0; index < wiredActiveConnect.size(); ++index) {
//            resize(width(), height() + H_NORMAL_ITEM * index);
//            LanListItem *activeWiredForm = new LanListItem(this, wiredActiveConnect.at(index));
//            activeWiredForm->constructActiveConnectionItem(index);
//            m_wiredActiveFormlist<<activeWiredForm;
//        }
//    }

    return;
}

void KyWiredWidget::setType(WIDGETTYPE type)
{
    m_type = type;
}

void KyWiredWidget::constructWiredConnectList()
{
    qDebug()<<"constructWiredConnectList.....";
    QList<KyConnectItem *> wiredConnections;
    m_connectResource->getConnectionList("enp2s0",
                                         NetworkManager::ConnectionSettings::ConnectionType::Wired,
                                         wiredConnections);
//    if (!wiredConnections.isEmpty()) {
//        for (int index = 0; index < wiredConnections.size(); ++index) {
//            resize(W_LIST_WIDGET, height() + H_NORMAL_ITEM);
//            OneLancForm *wiredForm = new OneLancForm(this, wiredConnections.at(index));
//            wiredForm->constructConnectionItem(index);
//            m_wiredFormlist<<wiredForm;
//        }
//    }

    return;
}

void KyWiredWidget::updateWiredActiveConnection()
{
    qDebug()<<"updateWiredActiveConnection......";
    while (!m_wiredActiveFormlist.empty()) {
        LanListItem *wiredActiveForm = m_wiredActiveFormlist.at(0);
        m_wiredActiveFormlist.removeAt(0);
        delete wiredActiveForm;
        wiredActiveForm = nullptr;
    }

    constructWiredActiveConnectList();
}

void KyWiredWidget::updateWiredConnection()
{
    qDebug()<<"updateWiredConnection........";
    while (!m_wiredFormlist.empty()) {
        LanListItem *wiredForm = m_wiredFormlist.at(0);
        m_wiredFormlist.removeAt(0);
        delete wiredForm;
        wiredForm = nullptr;
    }

    constructWiredConnectList();
}

void KyWiredWidget::updateWidgetList()
{
    m_timer->stop();

    if (ACTIVECONNECTION == m_type) {
        updateWiredActiveConnection();
    } else {
        updateWiredConnection();
    }

    return;
}

void KyWiredWidget::removeActiveConnection(QString connectUuid)
{
    m_timer->start(300);
    return;
}

void KyWiredWidget::activeConnectStateChange(QString connectUuid,
                                             NetworkManager::ActiveConnection::State state,
                                             NetworkManager::ActiveConnection::Reason reason)
{
    qDebug()<<"active connection"<<connectUuid
            <<"state"<<state <<"change reason"<<reason;
    if (NetworkManager::ActiveConnection::State::Activated == state) {
        m_timer->start(300);
    }

    return;
}

void KyWiredWidget::addConnection(QString connectUuid)
{
    qDebug()<<"add connection "<<connectUuid;

    if (CONNECTION == m_type) {
        updateWiredConnection();
    }

    return;
}

void KyWiredWidget::updateConnection(QString connectUuid)
{
    qDebug() << "update connection" << connectUuid;

    if (CONNECTION == m_type) {
        updateWiredConnection();
    } else {
        updateWiredActiveConnection();
    }

    return;
}

void KyWiredWidget::removeConnection(QString path)
{
    qDebug()<<"remove connection"<< path;

    if (CONNECTION == m_type) {
        updateWiredConnection();
    }

    return;
}

void KyWiredWidget::wiredDeviceCarrierChange(QString deviceName, bool pluged)
{
    qDebug()<<"device"<< deviceName << "carrier state"<< pluged;
    return;
}

void KyWiredWidget::updateDeviceRefreshRate(QString deviceName, int ms)
{
    m_deviceResource->setDeviceRefreshRate(deviceName, ms);
    return;
}

#if 0
void KyWiredWidget::updateNetworkSpeed(KyConnectItem *wiredItem)
{
    qulonglong txBytes = wiredItem->m_upSpeed;
    qulonglong rxBytes = wiredItem->m_downSpeed;

    m_deviceResource->DeviceSpeed(wiredItem->m_ifaceName, wiredItem);

    if (wiredItem->m_upSpeed > txBytes) {
        txBytes =  wiredItem->m_upSpeed - txBytes;
    } else {
        txBytes = 0;
    }

    if (wiredItem->m_downSpeed > rxBytes) {
        rxBytes = wiredItem->m_downSpeed - rxBytes;
    } else {
        rxBytes = 0;
    }

    emit updateSpeed(rxBytes, txBytes);

    return;
}
#endif
