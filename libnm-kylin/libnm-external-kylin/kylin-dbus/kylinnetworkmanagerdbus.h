#ifndef KYLINNETWORKMANAGERDBUS_H
#define KYLINNETWORKMANAGERDBUS_H

#include <QObject>
#include <QThread>

#include "kylindbusinterfrace.h"
#include "kylindbusadapter.h"
#include "libnm-external-kylin_global.h"

class LIBNMEXTERNALKYLIN_EXPORT KyNetworkManagerDbus : public QObject
{
    Q_OBJECT
public:
    explicit KyNetworkManagerDbus(QObject *parent = nullptr);
    ~KyNetworkManagerDbus();

public:
    void registerNetworkManagerDbus();

private:


};

#endif // KYLINNETWORKMANAGERDBUS_H
