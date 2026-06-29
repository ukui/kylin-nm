#ifndef UISECURITYCONFIG_H
#define UISECURITYCONFIG_H

#include <QMap>
#include <QFile>
#include <QDebug>
#include <QObject>
#include <QDBusReply>
#include <QDBusMessage>
#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QDBusConnection>

#include "libkysysinfo.h"

//ui 显示
#define DBUSSERVICE_UKCC        "org.ukui.ukcc.session"
#define DBUSPATH_UKCC           "/"
#define DBUSINTERFACE_UKCC      "org.ukui.ukcc.session.interface"


//SDK项目类型
enum SDK_TYPE_PROJECT
{
    SDK_TYPE_NONE,
    SDK_TYPE_YDSYY,//苏研院-移动云

    SDK_TYPE_SUM
};
Q_DECLARE_METATYPE(SDK_TYPE_PROJECT)

class UiSecurityConfig : public QObject
{
    Q_OBJECT

public:
    UiSecurityConfig();

    SDK_TYPE_PROJECT getProjectIdentity();
    static UiSecurityConfig* getInstance();

    bool isSeewoOrMaxhub() const;

    QVariant getConnectSettingsData(QString modName,QString ruleName);

    bool getSysSleepState();

private:
    QMap<QString, QVariant> m_ukuiSetingData;
    SDK_TYPE_PROJECT m_projectID=SDK_TYPE_NONE;
    static UiSecurityConfig* m_pInstance;

    bool isSleep = false;
    bool m_isSeewoOrMaxhub = false;

public:
    QMap<QString, QVariant> getModuleHideStatus();

private:

    void getSDKFieldOfProject();
    //睡眠休眠唤醒

    void initSysSleepMonistor();

private Q_SLOTS:

    void onMonitorSleepSlot(bool);


};

#endif // UISECURITYCONFIG_H
