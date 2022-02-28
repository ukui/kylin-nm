#include "kylinutil.h"
#include <QTextCodec>

#define  LOG_FLAG  "[kylin-util]"

QString getConnectTypeByDbus(QString &connectPath)
{
    QString connectType = "";

    if (connectPath.isEmpty()) {
        qWarning()<< LOG_FLAG << "connect path is empty, so can not get connect type";
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

//    qDebug() << LOG_FLAG << "connection type" << connectType;

    return connectType;

}

QString getSsidFromByteArray(QByteArray &rawSsid)
{
    QString wifiSsid = rawSsid;

#if 0

    if (rawSsid.isEmpty()) {
        qWarning() << LOG_FLAG << "wifi raw ssid is empty";
        return wifiSsid;
    }

    /*
    * 由于区分GB2312和UTF-8的方法比较困难，加之会存在中英文混合的情况，所以暂时
    * 不区分，统一经过gb2312转换，经过测试没有影响。
    */
    QTextCodec *p_textGBK = QTextCodec::codecForName("GB2312");
    wifiSsid = p_textGBK->toUnicode(rawSsid);

//    if (!QString::fromUtf8(rawSsid).contains("?")) {
//        QTextCodec *p_textGBK = QTextCodec::codecForName("GB2312");
//        wifiSsid = p_textGBK->toUnicode(rawSsid);

//        qDebug() << LOG_FLAG <<"gb2312  to string ssid" << wifiSsid;
//        //qDebug() << LOG_FLAG << "-------------> GB2312 " << byteArrayGB;
//    } else {
//        wifiSsid = QString::fromUtf8(rawSsid);

//        qDebug()<< LOG_FLAG <<" UTF-8 ssid: " <<wifiSsid;
//        //qDebug()<< "-------------> UTF-8 " << bytearray;
//    }
#endif
    return wifiSsid;
}
