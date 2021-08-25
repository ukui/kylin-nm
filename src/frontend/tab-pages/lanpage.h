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

    void initLanUI();
    void initDevice();//初始化默认设备
};

#endif // LANPAGE_H
