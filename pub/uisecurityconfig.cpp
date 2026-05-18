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
    initSysSleepMonistor();  // 初始化睡眠监控

}

UiSecurityConfig* UiSecurityConfig::getInstance()
{
    if (m_pInstance == NULL) {
        m_pInstance = new UiSecurityConfig();
    }
    return m_pInstance;
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
        }
        else
        {
           qWarning()<< res.errorName() << ": "<< res.errorMessage();
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
    qWarning() << Q_FUNC_INFO << __LINE__ << m_projectID;
}

SDK_TYPE_PROJECT UiSecurityConfig::getProjectIdentity()
{
    return m_projectID;
}


bool UiSecurityConfig::getSysSleepState()
{
    return isSleep;
}

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

void UiSecurityConfig::onMonitorSleepSlot (bool state)
{
    qWarning() << Q_FUNC_INFO << __LINE__ << state;
    isSleep = state;
}
