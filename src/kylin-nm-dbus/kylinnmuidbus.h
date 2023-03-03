#ifndef KYLINNMUIDBUS_H
#define KYLINNMUIDBUS_H

#include <QObject>
#include <QDBusConnection>

class KylinNmUiDbus : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylin.network.interface")
public:
    KylinNmUiDbus(QObject *parent = nullptr);
    ~KylinNmUiDbus() = default;

public Q_SLOTS:
    //唤起属性页 根据网卡类型 参数2 为ssid/uuid
    void showPropertyWidget(const QString devName, const QString ssid);
    //唤起新建有线连接界面
    void showCreateWiredConnectWidget(const QString devName);
    //唤起加入其他无线网络界面
    void showAddOtherWlanWidget(const QString devName);
    //连接 根据网卡类型 参数1 0:lan 1:wlan 参数3 为ssid/uuid
    Q_NOREPLY void activateConnect(int type, QString devName, QString ssid);

Q_SIGNALS:
    void signalShowPropertyWidget(const QString devName, const QString ssid);
    void signalShowCreateWiredConnectWidget(const QString devName);
    void signalShowAddOtherWlanWidget(const QString devName);
    //激活无线网络
    void signalActivateWireless(const QString devName, const QString ssid);
};

#endif // KYLINNMUIDBUS_H
