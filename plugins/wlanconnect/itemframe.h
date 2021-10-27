#ifndef ITEMFRAME_H
#define ITEMFRAME_H
#include <QFrame>
#include <QVBoxLayout>
#include "deviceframe.h"
#include "wlanitem.h"

class ItemFrame : public QFrame
{
    Q_OBJECT
public:
    ItemFrame(QString devName, QWidget *parent = nullptr);
    ~ItemFrame();
    //单设备整体layout
    QVBoxLayout * deviceLanLayout = nullptr;
    //单设备名称+下拉按钮Frame
    DeviceFrame * deviceFrame = nullptr;
    //单设备列表Frame
    QFrame * lanItemFrame = nullptr;
    //单设备列表layout
    QVBoxLayout * lanItemLayout = nullptr;
    //单设备item列表
    QMap<QString, WlanItem *> itemMap;
    //已激活uuid
    QString uuid = "";

private slots:
    void onDrownLabelClicked();
};

#endif // ITEMFRAME_H
