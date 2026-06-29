/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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

#include "uisecurityconfig.h"

/*注意该代码公用方式是代码引用方式不是库引用方式 编译虽然会在各自的引用工程中拷贝 但是源码还是同一个 多个工程使用修改需要确保影响域*/

UiSecurityConfig* UiSecurityConfig::m_pInstance = NULL;
const QMap<SDK_TYPE_PROJECT,QString> gSdkTypeProjectMap = {
    {SDK_TYPE_NONE,"none"},
    {SDK_TYPE_YDSYY,"APP_SCENE:YDSYY"},
};

UiSecurityConfig::UiSecurityConfig()
{
    m_ukuiSetingData=getModuleHideStatus();
    getSDKFieldOfProject();
    {
        QFile f("/sys/class/dmi/id/sys_vendor");
        m_isSeewoOrMaxhub = false;
        if (f.open(QIODevice::ReadOnly)) {
            QByteArray content = f.readAll().trimmed().toLower();
            QString s = QString::fromUtf8(content);
            if (s.contains("seewo") || s.contains("maxhub")) {
                m_isSeewoOrMaxhub = true;
            }
            f.close();
        }
    }
    initSysSleepMonistor();  // 初始化睡眠监控

}

UiSecurityConfig* UiSecurityConfig::getInstance()
{
    if (m_pInstance == NULL) {
        m_pInstance = new UiSecurityConfig();
    }
    return m_pInstance;
}

bool UiSecurityConfig::isSeewoOrMaxhub() const
{
    return m_isSeewoOrMaxhub;
}

/*modName预留不检查 */
QVariant UiSecurityConfig::getConnectSettingsData(QString modName,QString ruleName)
{
    if(ruleName.isEmpty()){
        qWarning() << Q_FUNC_INFO << __LINE__ <<"rulename is empty";
        return QVariant();
    }
    if (m_ukuiSetingData.contains(ruleName)) {
        return m_ukuiSetingData[ruleName].toString();
    }
    return QVariant();
}

QMap<QString, QVariant> UiSecurityConfig::getModuleHideStatus()
{
    QDBusReply<QMap<QString,QVariant>> reply_res;

    QDBusInterface iface(DBUSSERVICE_UKCC,
                         DBUSPATH_UKCC,
                         DBUSINTERFACE_UKCC,
                         QDBusConnection::sessionBus());

    if (iface.isValid())
    {
        /*
        QDBusPendingCall pcall = iface.asyncCall("getModuleHideStatus");
        pcall.waitForFinished();
        QDBusMessage res = pcall.reply();
        */
        iface.setTimeout(2000);
        QDBusMessage res = iface.call("getModuleHideStatus");

        if(res.type() == QDBusMessage::ReplyMessage)
        {
            if(res.arguments().size() > 0)
            {
                reply_res = res;
                //qInfo() << reply_res.value();
            }
        } else {
//           qWarning()<< res.errorName() << ": "<< res.errorMessage();
        }
    }
    else
    {
        qWarning()<< "dbus isValid";
    }
    return reply_res.value();
}

void UiSecurityConfig::getSDKFieldOfProject()
{
    #if 1
    char * projectStr= kdk_system_get_appScene();
    #else
    /*测试代码*/
    char * projectStr= (char*)malloc(100);
    stpcpy(projectStr,"APP_SCENE:YDSYY");
    #endif
    if(!projectStr){
        qWarning()<< "kdk_system_get_appScene return null";
        return;
    }
    QString proIdStr = QString(projectStr);
    qWarning() << Q_FUNC_INFO << __LINE__ << proIdStr;

    for (auto it = gSdkTypeProjectMap.constBegin(); it != gSdkTypeProjectMap.constEnd(); ++it) {
        if (it.value()==proIdStr) {
            m_projectID  = it.key();
            break;
        }
    }
    free(projectStr);
//    qWarning() << Q_FUNC_INFO << __LINE__ << m_projectID;
}

SDK_TYPE_PROJECT UiSecurityConfig::getProjectIdentity()
{
    return m_projectID;
}


bool UiSecurityConfig::getSysSleepState()
{
    return isSleep;
}

// LCOV_EXCL_START
void UiSecurityConfig::initSysSleepMonistor()
{
    qWarning() << Q_FUNC_INFO << __LINE__ ;

    if (QDBusConnection::systemBus().connect("org.freedesktop.login1", "/org/freedesktop/login1",
            "org.freedesktop.login1.Manager", "PrepareForSleep", this,
            SLOT(monitorSleepSlot(bool)))) {
        qDebug() << "PrepareForSleep signal connected successfully to slot";
    }

    //部分机器在系统s3/s4会对蓝牙适配器进行移除添加操作
    if (QDBusConnection::systemBus().connect("org.freedesktop.login1",
                                             "/org/freedesktop/login1",
                                             "org.freedesktop.login1.Manager",
                                             "PrepareForSleep",
                                             this,
                                             SLOT(onMonitorSleepSlot(bool)))) {
        qDebug() << "PrepareForSleep signal connected successfully to slot";
    } else {
        qDebug() << "PrepareForSleep signal connection was not successful";
    }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
void UiSecurityConfig::onMonitorSleepSlot (bool state)
{
    qWarning() << Q_FUNC_INFO << __LINE__ << state;
    isSleep = state;
}
// LCOV_EXCL_STOP
