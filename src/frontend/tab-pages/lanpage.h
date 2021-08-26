#ifndef LANPAGE_H
#define LANPAGE_H

#include "tabpage.h"
#include "kylinnetworkdeviceresource.h"

class LanPage : public TabPage
{
    Q_OBJECT
public:
    explicit LanPage(QWidget *parent = nullptr);
    ~LanPage()=default;

private:
    KyNetworkDeviceResourse *m_device = nullptr;
    QStringList devList;

    void initLanUI();
    void initDevice();//初始化默认设备

private slots:
    void onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType);
    void onDeviceRemove(QString deviceName);
    void onDeviceNameUpdate(QString oldName, QString newName);
};

#endif // LANPAGE_H
