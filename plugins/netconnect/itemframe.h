#ifndef ITEMFRAME_H
#define ITEMFRAME_H
#include <QFrame>
#include <QVBoxLayout>
#include "deviceframe.h"
#include <addbtn.h>
#include "addnetbtn.h"
#include "lanitem.h"

class ItemFrame : public QFrame
{
    Q_OBJECT
public:
    ItemFrame(QString devName, QWidget *parent = nullptr);
    ~ItemFrame();
    //单设备整体layout
    QVBoxLayout * deviceLanLayout = nullptr;
    //单设备名称+下拉按钮Frame+d单设备开关
    DeviceFrame * deviceFrame = nullptr;
    //单设备列表Frame
    QFrame * lanItemFrame = nullptr;
    //单设备列表layout
    QVBoxLayout * lanItemLayout = nullptr;
    //新建有线连接
    AddNetBtn * addLanWidget = nullptr;
    //单设备item列表 key:uuid
    QMap<QString, LanItem *> itemMap;

private slots:
    void onDrownLabelClicked();
};

#endif // ITEMFRAME_H
