#ifndef LANPAGE_H
#define LANPAGE_H

#include "divider.h"
#include "switchbutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QListWidget>
#include <QMap>
#include <QGSettings>

#include "list-items/listitem.h"
#include "list-items/lanlistitem.h"
#include "tab-pages/tabpage.h"

class LanPage : public TabPage
{
    Q_OBJECT
public:
    explicit LanPage(QWidget *parent = nullptr);
    ~LanPage();
    bool m_isLanConnected = false;

    //for dbus
    void getWiredList(QMap<QString, QVector<QStringList> > &map);
    void activateWired(const QString& devName, const QString& connUuid);
    void deactivateWired(const QString& devName, const QString& connUuid);
    void showDetailPage(QString devName, QString uuid);
    void setWiredDeviceEnable(const QString& devName, bool enable);

private:
    void initDeviceState();
    void initUI();
    void initLanArea();
    void initNetSwitch();

    void initDeviceCombox();
    void updateDeviceCombox(QString oldDeviceName, QString newDeviceName);
    void deleteDeviceFromCombox(QString deviceName);
    void addDeviceForCombox(QString deviceName);

    QListWidgetItem *addNewItem(KyConnectItem *itemData, QListWidget *listWidget);

    void getEnabledDevice(QStringList &enableDeviceList);
    void getDisabledDevices(QStringList &disableDeviceList);

    void constructConnectionArea();
    void constructActiveConnectionArea();

    void updateConnectionArea(QString uuid);
    void updateActivatedConnectionArea(QString uuid);

    void updateActiveConnectionProperty(KyConnectItem *p_connectItem);
    void updateConnectionProperty(KyConnectItem *p_connectItem);

    void sendLanUpdateSignal(KyConnectItem *p_connectItem);
    void sendLanAddSignal(KyConnectItem *p_connectItem);

    void addEmptyConnectItem(QMap<KyConnectItem *, QListWidgetItem *> &connectMap,
                                           QListWidget *lanListWidget);
    void clearConnectionMap(QMap<KyConnectItem *, QListWidgetItem *> &connectMap,
                            QListWidget *lanListWidget);
    void deleteConnectionMapItem(QMap<KyConnectItem *, QListWidgetItem *> &connectMap,
                                 QListWidget *lanListWidget, QString uuid);
    bool connectionItemIsExist(QMap<KyConnectItem *, QListWidgetItem *> &connectMap,
                                       QString uuid);

signals:
    void lanAdd(QString devName, QStringList info);
    void lanRemove(QString dbusPath);
    void lanUpdate(QString devName, QStringList info);

    void lanActiveConnectionStateChanged(QString interface, QString uuid, int status);
    void lanConnectChanged();

private slots:
    void onUpdateLanlist(QString uuid, NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason);

    void onAddConnection(QString uuid);
    void onRemoveConnection(QString path);
    void onUpdateConnection(QString uuid);

    void onSwithGsettingsChanged(const QString &key);
    void onLanSwitchClicked();

    void onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType);
    void onDeviceRemove(QString deviceName);
    void onDeviceNameUpdate(QString oldName, QString newName);

    void onDeviceComboxIndexChanged(int currentIndex);

private:
    QListWidget * m_activatedLanListWidget = nullptr;
    QListWidget * m_inactivatedLanListWidget = nullptr;

    KyNetworkDeviceResourse *m_deviceResource = nullptr;
    KyWiredConnectOperation *m_wiredConnectOperation = nullptr;
    KyActiveConnectResourse *m_activeResourse = nullptr;     //激活的连接
    KyConnectResourse *m_connectResourse = nullptr;          //未激活的连接 

    QMap<KyConnectItem *, QListWidgetItem *> m_deactiveMap;
    QMap<KyConnectItem *, QListWidgetItem *> m_activeMap;

    QString m_currentDeviceName;
    QStringList m_devList;
    QGSettings *m_switchGsettings = nullptr;

    bool m_wiredSwitch = true;
};

#endif // LANPAGE_H
