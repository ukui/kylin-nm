#ifndef KYLINWIREDWIDGET_H
#define KYLINWIREDWIDGET_H

#include <QWidget>
#include "backend/dbus-interface/kylinactiveconnectresource.h"
#include "backend/dbus-interface/kylinconnectresource.h"
#include "backend/dbus-interface/kylinnetworkdeviceresource.h"
#include "backend/dbus-interface/kylinconnectitem.h"
#include "onelancform.h"
#include "mainwindow.h"

typedef enum{
    ACTIVECONNECTION,
    CONNECTION,
}WIDGETTYPE;

class KyWiredWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KyWiredWidget(QWidget *parent = nullptr);
    ~KyWiredWidget();

    void setType(WIDGETTYPE type);
    void constructWiredConnectList();
    void constructWiredActiveConnectList();

    void updateDeviceRefreshRate(QString deviceName, int ms);
    void updateNetworkSpeed(KyConnectItem *wiredItem);

private:
    void updateWiredActiveConnection();
    void updateWiredConnection();

signals:
    void updateSpeed(qulonglong rx, qulonglong tx);

public slots:
    void removeActiveConnection(QString connectUuid);
    void activeConnectStateChange(QString uuid,
                                  NetworkManager::ActiveConnection::State state,
                                  NetworkManager::ActiveConnection::Reason reason);

    void addConnection(QString connectUuid);
    void updateConnection(QString connectUuid);
    void removeConnection(QString path);

    void wiredDeviceCarrierChange(QString deviceName, bool pluged);

    void updateWidgetList();
private:
    WIDGETTYPE m_type;
    QTimer *m_timer;

    KyActiveConnectResourse  *m_activeConnectResource = nullptr;
    KyConnectResourse        *m_connectResource = nullptr;
    KyNetworkDeviceResourse  *m_deviceResource = nullptr;

    QList<OneLancForm*> m_wiredActiveFormlist;
    QList<OneLancForm*> m_wiredFormlist;
};

#endif // KYLINWIREDWIDGET_H
