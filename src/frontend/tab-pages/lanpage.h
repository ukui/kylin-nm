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

    //for dbus
    void getWiredList(QMap<QString, QVector<QStringList> > &map);
    void activateWired(const QString& devName, const QString& connUuid);
    void deactivateWired(const QString& devName, const QString& connUuid);
    void showDetailPage(QString devName, QString uuid);
    void setWiredDeviceEnable(const QString& devName, bool enable);
signals:
    void wiredActivating(QString devName, QString ssid);

private:
    void initDevice();//初始化默认设备
    void initDeviceCombox();
    void initUI();
    void initList();
    void addNewItem(KyConnectItem *itemData, QListWidget *listWidget);
    void initNetSwitch();

private:
    QListWidget * m_activatedLanListWidget = nullptr;
    QListWidget * m_inactivatedLanListWidget = nullptr;

    LanListItem * m_testLanItem = nullptr;
    QListWidgetItem *m_listWidgetItem = nullptr;

    KyNetworkDeviceResourse *m_device = nullptr;
    KyActiveConnectResourse *m_activeResourse = nullptr;     //激活的连接
    KyConnectResourse *m_connectResourse = nullptr;          //未激活的连接

    QList<KyConnectItem *> m_activedList;
    QList<KyConnectItem *> m_deactivedList;

    QMap<KyConnectItem *, QListWidgetItem *> m_deactiveMap;
    QMap<KyConnectItem *, QListWidgetItem *> m_activeMap;
    QString m_deviceName;
    QStringList devList;
    QGSettings * m_switchGsettings = nullptr;

private slots:
    void updateLanlist(QString uuid, NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason);
    void addConnectionSlot(QString uuid);
    void removeConnectionSlot(QString path);
    void onSwithGsettingsChanged(const QString &key);
    void onLanSwitchClicked();
    void onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType);
    void onDeviceRemove(QString deviceName);
    void onDeviceNameUpdate(QString oldName, QString newName);
    void onDeviceComboxIndexChanged(int currentIndex);
};

#endif // LANPAGE_H
