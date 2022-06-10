#ifndef ITEMFRAME_H
#define ITEMFRAME_H
#include <QFrame>
#include <QVBoxLayout>
#include "../component/DeviceFrame/deviceframe.h"
#include "lanitem.h"

#include "../component/Divider/divider.h"

class ItemFrame : public QFrame
{
    Q_OBJECT
public:
    ItemFrame(QString devName, QWidget *parent = nullptr);
    //单设备整体layout
    QVBoxLayout * deviceLanLayout = nullptr;
    Divider * m_divider;
    //单设备名称 单设备开关
    DeviceFrame * deviceFrame = nullptr;
    //单设备列表Frame
    QFrame * lanItemFrame = nullptr;
    //单设备列表layout
    QVBoxLayout * lanItemLayout = nullptr;
    //单设备item列表 key:uuid
    QMap<QString, LanItem *> itemMap;
};

#endif // ITEMFRAME_H
