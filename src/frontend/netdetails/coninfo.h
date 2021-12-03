#ifndef CONINFO_H
#define CONINFO_H

#include <QString>
#include <QPalette>
#include <QComboBox>
#include <QAbstractItemView>
#include "kywirelessnetitem.h"
#include "kylinconnectresource.h"
#include "kylinactiveconnectresource.h"
#include "kywirelessconnectoperation.h"
#include "kywirelessnetresource.h"
#include "kyenterpricesettinginfo.h"

#define AUTO_CONFIG  0
#define MANUAL_CONFIG  1

enum PeapInnerType
{
    MSCHAPV2_PEAP = 0,
    MD5_PEAP,
    GTC_PEAP,
};

enum TtlsInnerType
{
    PAP = 0,
    MSCHAP,
    MSCHAPV2_EAP,
    MSCHAPV2,
    CHAP,
    MD5_EAP,
    GTC_EAP
};

class ConInfo {
public:
    QString strConName;
    QString strConType;
    QString strSecType;
    KySecuType secType = WPA_AND_WPA2_PERSONAL;
    QString strPassword;
    QString strChan;
    QString strMac;
    QString strHz;
    QString strBandWidth;
    QString strDynamicIpv4;
    QString strDynamicIpv6;
    QString strDynamicIpv4Dns;
    bool    isAutoConnect = false;

    KyIpConfigType ipv4ConfigType = CONFIG_IP_DHCP;
    QString strIPV4Address;
    QString strIPV4NetMask;
    QString strIPV4FirDns;
    QString strIPV4SecDns;
    QString strIPV4GateWay;

    KyIpConfigType ipv6ConfigType = CONFIG_IP_DHCP;
    QString strIPV6Address;
    int iIPV6Prefix;
    QString strIPV6FirDns;
    QString strIPV6SecDns;
    QString strIPV6GateWay;

    KyEapMethodType enterpriseType;
    KyEapMethodTlsInfo tlsInfo;
    KyEapMethodPeapInfo peapInfo;
    KyEapMethodTtlsInfo ttlsInfo;
};

static void setLineEditMenuPolicy(QFrame *widget) {
    QList<QLineEdit *> lineEditList = widget->findChildren<QLineEdit *>();
    for (int i = 0; i < lineEditList.count(); ++i) {
        lineEditList.at(i)->setContextMenuPolicy(Qt::NoContextMenu);
    }
}

static void setFramePalette(QFrame *widget, QPalette &pal) {
    QList<QLineEdit *> lineEditList = widget->findChildren<QLineEdit *>();
    for (int i = 0; i < lineEditList.count(); ++i) {
        lineEditList.at(i)->setPalette(pal);
    }
    QList<QComboBox *> comboBoxList = widget->findChildren<QComboBox *>();
    for (int i = 0; i < comboBoxList.count(); ++i) {
        comboBoxList.at(i)->setPalette(pal);
	if (comboBoxList.at(i)->view()) {
            comboBoxList.at(i)->view()->setPalette(pal);
        }
    }
}

#endif // CONINFO_H
