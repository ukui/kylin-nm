#ifndef NETWORKSPEED_H
#define NETWORKSPEED_H

#include <QObject>
#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

class NetworkSpeed : public QObject
{
    Q_OBJECT
public:
    explicit NetworkSpeed(QObject *parent = nullptr);

    int getCurrentDownloadRates(char *netname,long int * save_rate,long int * tx_rate); //获取当前的流量，参数为将获取到的流量保
};

#endif // NETWORKSPEED_H
