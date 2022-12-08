/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "mobilehotspot.h"


#include <QDebug>


MobileHotspot::MobileHotspot() :  mFirstLoad(true) {

    QTranslator* translator = new QTranslator(this);
    qDebug() << "/usr/share/kylin-nm/mobilehotspot/" + QLocale::system().name();
    if (!translator->load("/usr/share/kylin-nm/mobilehotspot/" + QLocale::system().name())) {
        qDebug() << "load failed";
    }
    QApplication::installTranslator(translator);

    pluginName = tr("MobileHotspot");
    qDebug() << pluginName;
    pluginType = NETWORK;

    qDBusRegisterMetaType<QMap<QString, bool> >();
    qDBusRegisterMetaType<QMap<QString, int> >();
    needLoad = isExitWirelessDevice();
}

MobileHotspot::~MobileHotspot()
{
    if (!mFirstLoad) {
//        delete pluginWidget;
//        pluginWidget = nullptr;

    }
}

QString MobileHotspot::plugini18nName() {
    return pluginName;
}

int MobileHotspot::pluginTypes() {
    return pluginType;
}

QWidget *MobileHotspot::pluginUi() {
    if (mFirstLoad) {
        mFirstLoad = false;

        pluginWidget = new MobileHotspotWidget;
        pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
    }
    return pluginWidget;
}

const QString MobileHotspot::name() const {

    return QStringLiteral("mobilehotspot");
}

bool MobileHotspot::isEnable() const
{
    qDebug() << needLoad;
    return needLoad;
}


bool MobileHotspot::isShowOnHomePage() const
{
    return false;
}

QIcon MobileHotspot::icon() const
{
    return QIcon::fromTheme("ukui-hotspot-symbolic");
}

QString MobileHotspot::translationPath() const
{
    return "/usr/share/kylin-nm/mobilehotspot/%1.ts";
}

void MobileHotspot::initSearchText()
{
    //~ contents_path /mobilehotspot/mobilehotspot
    tr("mobilehotspot");
    //~ contents_path /mobilehotspot/mobilehotspot open
    tr("mobilehotspot open");
}

bool MobileHotspot::isExitWirelessDevice()
{
    QDBusInterface *interface = new QDBusInterface("com.kylin.network", "/com/kylin/network",
                                     "com.kylin.network",
                                     QDBusConnection::sessionBus());
    if (!interface->isValid()) {
        qDebug() << "/com/kylin/network is invalid";
        return false;
    }

    QDBusMessage result = interface->call(QStringLiteral("getDeviceListAndEnabled"),1);
    if(result.type() == QDBusMessage::ErrorMessage) {
        qWarning() << "getWirelessDeviceList error:" << result.errorMessage();
        return false;
    }

    auto dbusArg =  result.arguments().at(0).value<QDBusArgument>();
    QMap<QString, bool> deviceListMap;
    dbusArg >> deviceListMap;

    QDBusReply<QMap<QString, int> > capReply = interface->call("getWirelessDeviceCap");
    if (!capReply.isValid()) {
        qDebug()<<"execute dbus method 'getWirelessDeviceCap' is invalid in func initInterfaceInfo()" <<capReply.error().type() ;
        return false;
    }
    QMap<QString, int> devCapMap = capReply.value();


    if (deviceListMap.isEmpty()) {
        qDebug() << "no wireless device";
        return false;
    } else {
        QMap<QString, bool>::Iterator iter = deviceListMap.begin();
        while (iter != deviceListMap.end()) {
            QString interfaceName = iter.key();
            if (!devCapMap.contains(interfaceName)) {
                iter++;
                continue;
            }
            if (deviceListMap[interfaceName] & 0x01) {
                qDebug() << "wireless device" <<  interfaceName << "support hotspot";
                return true;
            }
            iter++;
        }
    }
    qDebug() << "no wireless device support hotspot";
    return false;
}

