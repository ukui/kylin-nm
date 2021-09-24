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

protected:
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void lanAdd(QString devName, QStringList info);
    void lanRemove(QString dbusPath);
    void lanUpdate(QString devName, QStringList info);
    void lanActiveConnectionStateChanged(QString interface, QString uuid, int status);
    void lanConnectChanged();

private:
    void initDeviceState();
    void initDeviceCombox();
    void initUI();
    void initList(QString m_deviceName);
    void addNewItem(KyConnectItem *itemData, QListWidget *listWidget);
    void addNUllItem(QListWidget *listWidget);
    void initNetSwitch();

private:
    QListWidget * m_activatedLanListWidget = nullptr;
    QListWidget * m_inactivatedLanListWidget = nullptr;

    LanListItem * m_nullLanItem = nullptr;
    QListWidgetItem *m_nullItem = nullptr;
    QListWidgetItem *m_listWidgetItem = nullptr;

    KyNetworkDeviceResourse *m_device = nullptr;
    KyWiredConnectOperation *m_wiredConnectOperation = nullptr;
    KyActiveConnectResourse *m_activeResourse = nullptr;     //激活的连接
    KyConnectResourse *m_connectResourse = nullptr;          //未激活的连接

    QList<KyConnectItem *> m_activedList;
    QList<KyConnectItem *> m_deactivedList;

//    QMap<QString, QVector<QStringList> > m_deviceMap;

    QMap<KyConnectItem *, QListWidgetItem *> m_deactiveMap;
    QMap<KyConnectItem *, QListWidgetItem *> m_activeMap;

//    QMap<LanListItem *, QListWidgetItem *> m_deactiveMap;
//    QMap<LanListItem *, QListWidgetItem *> m_activeMap;

    QString m_deviceName;
    QStringList m_devList;
    QStringList enableDevice;
    QGSettings * m_switchGsettings = nullptr;

private slots:
    void updateLanlist(QString uuid, NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason);
    void addConnectionSlot(QString uuid);
    void removeConnectionSlot(QString path);
    void connectionUpdateSlot(QString uuid);
    void onSwithGsettingsChanged(const QString &key);
    void onLanSwitchClicked();
    void onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType);
    void onDeviceRemove(QString deviceName);
    void onDeviceNameUpdate(QString oldName, QString newName);
    void onDeviceComboxIndexChanged(int currentIndex);
    void showControlCenter();
//    void onLanDataChange(QString uuid);
};

#endif // LANPAGE_H
