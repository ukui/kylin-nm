#ifndef ITEMFRAME_H
#define ITEMFRAME_H
#include <QFrame>
#include <QVBoxLayout>

#include "wlanitem.h"

#include "../component/DeviceFrame/deviceframe.h"
#include "../component/Divider/divider.h"
#include "hiddenwifi/hiddenwifipage.h"

class AddNetItem : public QFrame
{
    Q_OBJECT
public:
    AddNetItem(QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QLabel* titleLabel = nullptr;
    bool m_isIn = false;

Q_SIGNALS:
    void itemClick();
};

class ItemFrame : public QFrame
{
    Q_OBJECT
public:
    ItemFrame(QString devName, QWidget *parent = nullptr);
    //单设备整体layout
    QVBoxLayout * deviceLanLayout = nullptr;
    //单设备名称+下拉按钮Frame
    Divider * m_divider;
    DeviceFrame * deviceFrame = nullptr;
    //单设备列表Frame
    QFrame * lanItemFrame = nullptr;
    //单设备列表layout
    QVBoxLayout * lanItemLayout = nullptr;
    //单设备item列表
    QMap<QString, WlanItem *> itemMap;
    //加入其他网络
    AddNetItem * addNetItem = nullptr;
    //已激活uuid
    QString uuid = "";
public Q_SLOTS:
    void showJoinPage(bool, QWidget *);
private:
    HiddenWiFiPage* joinPage = nullptr;

Q_SIGNALS:
    void addNetItemClick();
    void connectHidePeapConnect(KyEapMethodPeapInfo info, KyWirelessConnectSetting connSettingInfo);
    void connectHideTtlsConnect(KyEapMethodTtlsInfo info, KyWirelessConnectSetting connSettingInfo);
    void connectHideLeapConnect(KyEapMethodLeapInfo info, KyWirelessConnectSetting connSettingInfo);
    void connectHidePwdConnect(KyEapMethodPwdInfo info, KyWirelessConnectSetting connSettingInfo);
    void connectHideNormalConnect(KyWirelessConnectSetting connSettingInfo, KySecuType type);
};

#endif // ITEMFRAME_H
