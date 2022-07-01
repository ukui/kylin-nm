#include "kylinnetworkmanagerutil.h"
#include <QSettings>
#include <QTextCodec>
#include <QDir>
#include "kylinnetworkdeviceresource.h"
#define  LOG_FLAG  "[kylin-util]"
const QString CONFIG_FILE_PATH   =  QDir::homePath() + "/.config/ukui/kylin-nm.conf";

QString getConnectTypeByDbus(QString &connectPath)
{
    QString connectType = "";

    if (connectPath.isEmpty()) {
        qWarning() << LOG_FLAG << "connect path is empty, so can not get connect type";
        return connectType;
    }

    QDBusInterface dbusInterface("org.freedesktop.NetworkManager",
                              connectPath,
                              "org.freedesktop.NetworkManager.Settings.Connection",
                              QDBusConnection::systemBus());

    QDBusMessage result = dbusInterface.call("GetSettings");
    const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
    QMap<QString, QMap<QString, QVariant>> map;
    dbusArg1st >> map;

    if (map.isEmpty()) {
        qWarning() << LOG_FLAG <<"get connection settings failed.";
        return connectType;
    }

    QMap<QString,QVariant> connectMap = map.value(KEY_CONNECTION);
    if (connectMap.isEmpty()) {
        qWarning() << LOG_FLAG <<"threre is not connection settings";
        return connectType;
    }

    connectType = connectMap.value(KEY_CONNECT_TYPE).toString();
    return connectType;
}

QString getSsidFromByteArray(QByteArray &rawSsid)
{
    QString wifiSsid = "";

    if (rawSsid.isEmpty()) {
        qWarning() << LOG_FLAG << "wifi raw ssid is empty";
        return wifiSsid;
    }

    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    codec->toUnicode( rawSsid.constData(), rawSsid.size(), &state);
    if (state.invalidChars > 0) {
        wifiSsid = QTextCodec::codecForName("GBK")->toUnicode(rawSsid);
    } else {
        wifiSsid = rawSsid;
    }

    return wifiSsid;
}

QString enumToQstring(NetworkManager::AccessPoint::Capabilities apCapability,
                      NetworkManager::AccessPoint::WpaFlags wpaFlags,
                      NetworkManager::AccessPoint::WpaFlags rsnFlags)
{
    QString out;

    if ((apCapability & AP_CAPABILITY_PRIVACY)
           && (wpaFlags == AP_SECURITY_NONE)
           && (rsnFlags == AP_SECURITY_NONE)) {
        out += "WEP ";
    }

    if (wpaFlags != AP_SECURITY_NONE) {
        out += "WPA1 ";
    }

    if ((rsnFlags & AP_SECURITY_KEY_MGMT_PSK)
            || (rsnFlags & AP_SECURITY_KEY_MGMT_802_1X)) {
        out += "WPA2 ";
    }

    if (rsnFlags & AP_SECURITY_KEY_MGMT_SAE) {
        out += "WPA3 ";
    }

    if ((wpaFlags & AP_SECURITY_KEY_MGMT_802_1X)
           || (rsnFlags & AP_SECURITY_KEY_MGMT_802_1X)) {
        out += "802.1X ";
    }

    return out;
}

void saveDeviceEnableState(QString deviceName, bool enable)
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("CARDEABLE");
    m_settings->setValue(deviceName, enable);
    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;
}

void getDeviceEnableState(QMap<QString, bool> &map)
{
    map.clear();
    if (!QFile::exists(CONFIG_FILE_PATH)) {
        qDebug() << "CONFIG_FILE_PATH not exist";
        return;
    }

    KyNetworkDeviceResourse kdr;
    QStringList wiredDevList;
    wiredDevList.clear();

    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("CARDEABLE");

    kdr.getNetworkDeviceList(DEVICE_TYPE_ETHERNET, wiredDevList);
    if (!wiredDevList.isEmpty()) {
        for (int i = 0; i < wiredDevList.size(); ++i) {
            if (!m_settings->contains(wiredDevList.at(i))) {
                saveDeviceEnableState(wiredDevList.at(i),true);
            }
            bool enable = m_settings->value(wiredDevList.at(i), true).toBool();
            map.insert(wiredDevList.at(i), enable);
        }
    }

    m_settings->endGroup();
    delete m_settings;
    m_settings = nullptr;
}

