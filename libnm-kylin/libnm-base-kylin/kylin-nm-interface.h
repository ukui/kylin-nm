#ifndef KYLINNMINTERFACE_H
#define KYLINNMINTERFACE_H

#include "libnm-base-kylin_global.h"

class QWidget;
enum PluginType {
    SIMPLE = 0,  //锁屏 && 登录
    COMPLEX,     //托盘
};

class LIBNMBASEKYLIN_EXPORT Interface{
public:
    virtual ~Interface(){}
    virtual QWidget * pluginUi() = 0;   // 插件主界面---setPluginType后调用
    virtual void setPluginType(PluginType type, bool useSwitch = true) = 0;        // 设置插件类型
    virtual void setParentWidget(QWidget* widget) = 0;
};

#define Interface_iid "org.kylin.network"

Q_DECLARE_INTERFACE(Interface, Interface_iid)

#endif // KYLINNMINTERFACE_H
