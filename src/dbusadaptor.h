/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp com.kylin.weather.xml -a dbusadaptor -c DbusAdaptor -l MainWindow
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DBUSADAPTOR_H
#define DBUSADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusMetaType>
QT_BEGIN_NAMESPACE
class QByteArray;
//template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
template<class T> class QVector;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.kylin.weather
 */

#include "mainwindow.h"//added the code manually by lixiang

class DbusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylin.network")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.kylin.network\">\n"
"    <method name=\"showMainWindow\"/>\n"
"    <method name=\"requestRefreshWifiList\"/>\n"
"    <method name=\"getWifiList\">\n"
"       <arg type=\"av\" direction=\"out\"/>\n"
"    </method>\n"
"    <method name=\"showPb\">\n"
"       <arg direction=\"in\" name=\"type\" type=\"s\"/>\n"
"       <arg direction=\"in\" name=\"name\" type=\"s\"/>\n"
"    </method>\n"
"    <signal name=\"getWifiListFinished\"/>\n"
"    <signal name=\"configurationChanged\"/>\n"
"    <signal name=\"wiredConnectionAdded\"/>\n"
"    <signal name=\"wiredConnectionRemoved\"/>\n"
"    <signal name=\"actWiredConnectionChanged\"/>\n"
"  </interface>\n"
        "")
public:
    DbusAdaptor(MainWindow *parent);
    virtual ~DbusAdaptor();

    inline MainWindow *parent() const
    { return static_cast<MainWindow *>(QObject::parent()); }

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void showMainWindow();
    void showPb(QString type, QString name);
    void requestRefreshWifiList();
    QVector<QStringList> getWifiList();
Q_SIGNALS: // SIGNALS
    void getWifiListFinished();
    void configurationChanged();
    void wiredConnectionAdded();
    void wiredConnectionRemoved();
    void actWiredConnectionChanged();
};

#endif
