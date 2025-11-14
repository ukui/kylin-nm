#ifndef KNMINTERFACE_H
#define KNMINTERFACE_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariantList>
#include <QIcon>
#include <QtDBus/QtDBus>
#include <QTimer>

#include "CSingleton.h"
#include "wirelessconnectionmodel.h"

enum ConnectStatus{
    All,        // 有线无线都连接
    Wire,       // 仅有有线连接
    Wireless,   // 仅有无线连接
    NoConnect   // 都没连接
};

//前端接口类，用于调取keeper数据并展示在前端
class KnmInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList wiredDevList READ wiredDeviceList NOTIFY updateWiredDeviceList)
    Q_PROPERTY(QVariantList wirelessDevList READ wirelessDeviceList NOTIFY updateWirelessDeviceList)
    Q_PROPERTY(QVariantList wiredConList READ wiredDevConnList NOTIFY updateWiredDevConnList)
    Q_PROPERTY(QVariantList wirelessConList READ wirelessDevConnList NOTIFY updateWirelessDevConnList)
    Q_PROPERTY(bool wirelessSwitch READ wirelessSwitchState WRITE setWirelessSwitch  NOTIFY updateWirelessSwitch)
    Q_PROPERTY(bool wiredMainSwitch READ wiredMainSwitchState WRITE setWiredMainSwitch  NOTIFY updateWiredMainSwitch)
    Q_PROPERTY(QString upwareRate READ upwareRateDate WRITE setUpwareRateData  NOTIFY updateUpLoadWiredStr)
    Q_PROPERTY(QString downwareRate READ downwareRateDate WRITE setDownwareRateData  NOTIFY updateDownLoadWiredStr)
    Q_PROPERTY(WirelessConnectionModel* wirelessConLists READ wirelessConLists NOTIFY wirelessConListChanged)
    Q_PROPERTY(bool cableStatus READ getCableStatus NOTIFY updateCable)
    Q_PROPERTY(QString fontSize READ fontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(bool wirelessScanState WRITE setWirelessScanState) //wifi scan 事件

public:
    KnmInterface();

    WirelessConnectionModel* wirelessConLists() { return &mWirelessConnecModel; }
    ~KnmInterface();

public slots:
    QVariantList wiredDeviceList();

    QVariantList wirelessDeviceList();

    QVariantList wiredDevConnList();

    QVariantList wirelessDevConnList();

    bool wirelessSwitchState();

    bool wiredMainSwitchState();

    QVariant getLoadingIcon();

    void setWiredMainSwitch(bool switched);

    void setWirelessSwitch(bool switched);

    void setUpwareRateData(QString str);

    void setDownwareRateData(QString str);

    void rescanWirelessConn();

    void openNetworkSetting();

    void openwLanNetworkSetting();
    //用于切换网卡设备时显示的连接列表更新
    void getWiredDevConnList(QString devName);

    void getWiredDevConnList();

    void getWirelessDevConnList(QString devName);

    void getWirelessDevConnList();

    void rebuildCurrentWirelessList();

    QString getIconData(QString name, int size = 16);

    void activateConnect(QString devName, QString conUid, int type);

    void deActivateConnect(QString devName, QString conUid, int type);

    void passwordConnect(QString devName,
                         QString ssid,
                         QString type,
                         QString psk,
                         bool autoConnect);

    QVariant getWiFiIcon(QString signalStrength,
                        QString security,
                        QString isApConnection,
                        int category);

    QString upwareRateDate();

    QString downwareRateDate();

    bool getNetMacConnectStatus(QString devmac);

    void showPropertyWidget(QString devname, QString ssid);

    void deleteConnect(int type,QString ssid);

    void showAddOtherWlanPage(QString devName);

    void onRequestInputPasswdAgent(QString agentName,QVariantMap parm);

    int changeSelectDevice(QString deviceName);

    void setWirelessScanState(bool state);

private slots:

    void slotRefreshTimeout();
	
public:
    ConnectStatus getConnectionStatus();
    bool getCableStatusByDev(const QString &devName);
    bool getCableStatus();
    QString fontSize();

signals:
    void updateWiredDeviceList();

    void updateWirelessDeviceList();

    void updateWiredDevConnList();

    void updateWirelessDevConnList();

    void updateWiredMainSwitch();

    void updateWirelessSwitch();

    void updateLoadingIcon(QVariant icon);

    void updateUpLoadWiredStr(QString str);

    void updateUpLoadWirelessStr(QString str);

    void updateDownLoadWiredStr(QString str);

    void updateDownLoadWirelessStr(QString str);

    void wirelessConListChanged();

    void updateCable(bool status);

    void fontSizeChanged(QString fontSize);

    void triggerButtonRequested(int index);

    void changeSelectWirelessDevice(int index);
private:
    QVariantList m_wiredDevConnList;

    QVariantList m_wirelessDevConnList;

    QString m_currentWiredDevice;

    QString m_currentWirelessDevice;

    int loadCount = 0;

    QTimer *loadTimer = nullptr;
    QTimer *m_pRefreshTimer = nullptr;

    QProcess * m_pProcess = nullptr;
    WirelessConnectionModel mWirelessConnecModel;//显示模型使用QAbstractListModel 可控制显示区域与数据变化。基于object的qvariantlist数据变化或项变化都会引起界面的全量更新
    friend class SingleTon<KnmInterface>;
    
    QString m_fontSize;
    class QGSettings *m_fontSettings = nullptr;
};

typedef SingleTon<KnmInterface>  KInterface;

#endif // KNMINTERFACE_H
