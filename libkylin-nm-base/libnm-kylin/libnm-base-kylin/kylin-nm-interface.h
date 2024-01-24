/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef KYLINNMINTERFACE_H
#define KYLINNMINTERFACE_H

#include "libnm-base-kylin_global.h"
#include <QObject>

class QWidget;
enum PluginType {
    SIMPLE = 0,  //锁屏 && 登录
    COMPLEX,     //托盘
};

class LIBNMBASEKYLIN_EXPORT Interface : public QObject{
    Q_OBJECT
public:
    virtual ~Interface(){}
    virtual QWidget * pluginUi() = 0;   // 插件主界面---setPluginType后调用
    virtual void setPluginType(PluginType type, bool useSwitch = true) = 0;        // 设置插件类型
    virtual void setParentWidget(QWidget* widget) = 0;
    virtual void setWidgetVisable(bool) = 0;

Q_SIGNALS:
    void needShowVirtualKeyboard();
};

#define Interface_iid "org.kylin.network"

Q_DECLARE_INTERFACE(Interface, Interface_iid)

#endif // KYLINNMINTERFACE_H
