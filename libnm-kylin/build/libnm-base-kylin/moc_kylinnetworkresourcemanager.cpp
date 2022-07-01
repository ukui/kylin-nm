/****************************************************************************
** Meta object code from reading C++ file 'kylinnetworkresourcemanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../libnm-base-kylin/kylinnetworkresourcemanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kylinnetworkresourcemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KyNetworkResourceManager_t {
    QByteArrayData data[91];
    char stringdata0[1677];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KyNetworkResourceManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KyNetworkResourceManager_t qt_meta_stringdata_KyNetworkResourceManager = {
    {
QT_MOC_LITERAL(0, 0, 24), // "KyNetworkResourceManager"
QT_MOC_LITERAL(1, 25, 18), // "wiredConnectionAdd"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 7), // "devName"
QT_MOC_LITERAL(4, 53, 11), // "connectUuid"
QT_MOC_LITERAL(5, 65, 11), // "connectName"
QT_MOC_LITERAL(6, 77, 11), // "connectPath"
QT_MOC_LITERAL(7, 89, 21), // "wiredConnectionRemove"
QT_MOC_LITERAL(8, 111, 4), // "path"
QT_MOC_LITERAL(9, 116, 13), // "connectionAdd"
QT_MOC_LITERAL(10, 130, 16), // "connectionUpdate"
QT_MOC_LITERAL(11, 147, 16), // "connectionRemove"
QT_MOC_LITERAL(12, 164, 14), // "wiredDeviceAdd"
QT_MOC_LITERAL(13, 179, 10), // "deviceName"
QT_MOC_LITERAL(14, 190, 17), // "wiredDeviceUpdate"
QT_MOC_LITERAL(15, 208, 13), // "oldDeviceName"
QT_MOC_LITERAL(16, 222, 13), // "newDeviceName"
QT_MOC_LITERAL(17, 236, 17), // "wiredDeviceRemove"
QT_MOC_LITERAL(18, 254, 17), // "wirelessDeviceAdd"
QT_MOC_LITERAL(19, 272, 20), // "wirelessDeviceUpdate"
QT_MOC_LITERAL(20, 293, 20), // "wirelessDeviceRemove"
QT_MOC_LITERAL(21, 314, 9), // "deviceAdd"
QT_MOC_LITERAL(22, 324, 12), // "KyDeviceType"
QT_MOC_LITERAL(23, 337, 10), // "deviceType"
QT_MOC_LITERAL(24, 348, 12), // "deviceUpdate"
QT_MOC_LITERAL(25, 361, 12), // "deviceRemove"
QT_MOC_LITERAL(26, 374, 18), // "deviceActiveChange"
QT_MOC_LITERAL(27, 393, 12), // "deviceActive"
QT_MOC_LITERAL(28, 406, 19), // "deviceCarrierChange"
QT_MOC_LITERAL(29, 426, 6), // "pluged"
QT_MOC_LITERAL(30, 433, 19), // "deviceBitRateChange"
QT_MOC_LITERAL(31, 453, 7), // "bitRate"
QT_MOC_LITERAL(32, 461, 22), // "deviceMacAddressChange"
QT_MOC_LITERAL(33, 484, 9), // "hwAddress"
QT_MOC_LITERAL(34, 494, 17), // "deviceStateChange"
QT_MOC_LITERAL(35, 512, 13), // "KyDeviceState"
QT_MOC_LITERAL(36, 526, 8), // "newstate"
QT_MOC_LITERAL(37, 535, 8), // "oldstate"
QT_MOC_LITERAL(38, 544, 41), // "NetworkManager::Device::State..."
QT_MOC_LITERAL(39, 586, 6), // "reason"
QT_MOC_LITERAL(40, 593, 18), // "wifiNetworkRemoved"
QT_MOC_LITERAL(41, 612, 16), // "wifiNetworkAdded"
QT_MOC_LITERAL(42, 629, 25), // "wifiNetworkPropertyChange"
QT_MOC_LITERAL(43, 655, 32), // "NetworkManager::WirelessNetwork*"
QT_MOC_LITERAL(44, 688, 3), // "net"
QT_MOC_LITERAL(45, 692, 26), // "wifiNetworkDeviceDisappear"
QT_MOC_LITERAL(46, 719, 21), // "wirelessConnectionAdd"
QT_MOC_LITERAL(47, 741, 24), // "wirelessConnectionRemove"
QT_MOC_LITERAL(48, 766, 19), // "activeConnectionAdd"
QT_MOC_LITERAL(49, 786, 4), // "uuid"
QT_MOC_LITERAL(50, 791, 22), // "activeConnectionRemove"
QT_MOC_LITERAL(51, 814, 23), // "wiredConnectStateChange"
QT_MOC_LITERAL(52, 838, 14), // "KyConnectState"
QT_MOC_LITERAL(53, 853, 5), // "state"
QT_MOC_LITERAL(54, 859, 26), // "wirelessConnectStateChange"
QT_MOC_LITERAL(55, 886, 4), // "ssid"
QT_MOC_LITERAL(56, 891, 28), // "wirelessApConnectStateChange"
QT_MOC_LITERAL(57, 920, 19), // "connectivityChanged"
QT_MOC_LITERAL(58, 940, 14), // "KyConnectivity"
QT_MOC_LITERAL(59, 955, 12), // "connectivity"
QT_MOC_LITERAL(60, 968, 18), // "wifiEnabledChanged"
QT_MOC_LITERAL(61, 987, 24), // "networkingEnabledChanged"
QT_MOC_LITERAL(62, 1012, 15), // "onServiceAppear"
QT_MOC_LITERAL(63, 1028, 13), // "onInitNetwork"
QT_MOC_LITERAL(64, 1042, 25), // "setWirelessNetworkEnabled"
QT_MOC_LITERAL(65, 1068, 7), // "enabled"
QT_MOC_LITERAL(66, 1076, 19), // "onConnectionUpdated"
QT_MOC_LITERAL(67, 1096, 25), // "onActiveConnectionChanged"
QT_MOC_LITERAL(68, 1122, 39), // "NetworkManager::ActiveConnect..."
QT_MOC_LITERAL(69, 1162, 31), // "onVpnActiveConnectChangedReason"
QT_MOC_LITERAL(70, 1194, 36), // "NetworkManager::VpnConnection..."
QT_MOC_LITERAL(71, 1231, 48), // "NetworkManager::VpnConnection..."
QT_MOC_LITERAL(72, 1280, 20), // "onDeviceActiveChange"
QT_MOC_LITERAL(73, 1301, 21), // "onDeviceCarrierChange"
QT_MOC_LITERAL(74, 1323, 21), // "onDeviceBitRateChange"
QT_MOC_LITERAL(75, 1345, 24), // "onDeviceMacAddressChange"
QT_MOC_LITERAL(76, 1370, 20), // "onDeviceStateChanged"
QT_MOC_LITERAL(77, 1391, 29), // "NetworkManager::Device::State"
QT_MOC_LITERAL(78, 1421, 21), // "onWifiNetworkAppeared"
QT_MOC_LITERAL(79, 1443, 24), // "onWifiNetworkDisappeared"
QT_MOC_LITERAL(80, 1468, 19), // "onUpdateWirelessNet"
QT_MOC_LITERAL(81, 1488, 21), // "onConnectivityChanged"
QT_MOC_LITERAL(82, 1510, 28), // "NetworkManager::Connectivity"
QT_MOC_LITERAL(83, 1539, 13), // "onDeviceAdded"
QT_MOC_LITERAL(84, 1553, 3), // "uni"
QT_MOC_LITERAL(85, 1557, 15), // "onDeviceRemoved"
QT_MOC_LITERAL(86, 1573, 15), // "onDeviceUpdated"
QT_MOC_LITERAL(87, 1589, 23), // "onActiveConnectionAdded"
QT_MOC_LITERAL(88, 1613, 25), // "onActiveConnectionRemoved"
QT_MOC_LITERAL(89, 1639, 17), // "onConnectionAdded"
QT_MOC_LITERAL(90, 1657, 19) // "onConnectionRemoved"

    },
    "KyNetworkResourceManager\0wiredConnectionAdd\0"
    "\0devName\0connectUuid\0connectName\0"
    "connectPath\0wiredConnectionRemove\0"
    "path\0connectionAdd\0connectionUpdate\0"
    "connectionRemove\0wiredDeviceAdd\0"
    "deviceName\0wiredDeviceUpdate\0oldDeviceName\0"
    "newDeviceName\0wiredDeviceRemove\0"
    "wirelessDeviceAdd\0wirelessDeviceUpdate\0"
    "wirelessDeviceRemove\0deviceAdd\0"
    "KyDeviceType\0deviceType\0deviceUpdate\0"
    "deviceRemove\0deviceActiveChange\0"
    "deviceActive\0deviceCarrierChange\0"
    "pluged\0deviceBitRateChange\0bitRate\0"
    "deviceMacAddressChange\0hwAddress\0"
    "deviceStateChange\0KyDeviceState\0"
    "newstate\0oldstate\0"
    "NetworkManager::Device::StateChangeReason\0"
    "reason\0wifiNetworkRemoved\0wifiNetworkAdded\0"
    "wifiNetworkPropertyChange\0"
    "NetworkManager::WirelessNetwork*\0net\0"
    "wifiNetworkDeviceDisappear\0"
    "wirelessConnectionAdd\0wirelessConnectionRemove\0"
    "activeConnectionAdd\0uuid\0"
    "activeConnectionRemove\0wiredConnectStateChange\0"
    "KyConnectState\0state\0wirelessConnectStateChange\0"
    "ssid\0wirelessApConnectStateChange\0"
    "connectivityChanged\0KyConnectivity\0"
    "connectivity\0wifiEnabledChanged\0"
    "networkingEnabledChanged\0onServiceAppear\0"
    "onInitNetwork\0setWirelessNetworkEnabled\0"
    "enabled\0onConnectionUpdated\0"
    "onActiveConnectionChanged\0"
    "NetworkManager::ActiveConnection::State\0"
    "onVpnActiveConnectChangedReason\0"
    "NetworkManager::VpnConnection::State\0"
    "NetworkManager::VpnConnection::StateChangeReason\0"
    "onDeviceActiveChange\0onDeviceCarrierChange\0"
    "onDeviceBitRateChange\0onDeviceMacAddressChange\0"
    "onDeviceStateChanged\0NetworkManager::Device::State\0"
    "onWifiNetworkAppeared\0onWifiNetworkDisappeared\0"
    "onUpdateWirelessNet\0onConnectivityChanged\0"
    "NetworkManager::Connectivity\0onDeviceAdded\0"
    "uni\0onDeviceRemoved\0onDeviceUpdated\0"
    "onActiveConnectionAdded\0"
    "onActiveConnectionRemoved\0onConnectionAdded\0"
    "onConnectionRemoved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KyNetworkResourceManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      55,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      33,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,  289,    2, 0x06 /* Public */,
       7,    1,  298,    2, 0x06 /* Public */,
       9,    3,  301,    2, 0x06 /* Public */,
      10,    3,  308,    2, 0x06 /* Public */,
      11,    1,  315,    2, 0x06 /* Public */,
      12,    1,  318,    2, 0x06 /* Public */,
      14,    2,  321,    2, 0x06 /* Public */,
      17,    1,  326,    2, 0x06 /* Public */,
      18,    1,  329,    2, 0x06 /* Public */,
      19,    2,  332,    2, 0x06 /* Public */,
      20,    1,  337,    2, 0x06 /* Public */,
      21,    2,  340,    2, 0x06 /* Public */,
      24,    2,  345,    2, 0x06 /* Public */,
      25,    1,  350,    2, 0x06 /* Public */,
      26,    2,  353,    2, 0x06 /* Public */,
      28,    2,  358,    2, 0x06 /* Public */,
      30,    2,  363,    2, 0x06 /* Public */,
      32,    2,  368,    2, 0x06 /* Public */,
      34,    4,  373,    2, 0x06 /* Public */,
      40,    2,  382,    2, 0x06 /* Public */,
      41,    2,  387,    2, 0x06 /* Public */,
      42,    1,  392,    2, 0x06 /* Public */,
      45,    0,  395,    2, 0x06 /* Public */,
      46,    3,  396,    2, 0x06 /* Public */,
      47,    1,  403,    2, 0x06 /* Public */,
      48,    1,  406,    2, 0x06 /* Public */,
      50,    1,  409,    2, 0x06 /* Public */,
      51,    3,  412,    2, 0x06 /* Public */,
      54,    4,  419,    2, 0x06 /* Public */,
      56,    0,  428,    2, 0x06 /* Public */,
      57,    1,  429,    2, 0x06 /* Public */,
      60,    1,  432,    2, 0x06 /* Public */,
      61,    1,  435,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      62,    3,  438,    2, 0x08 /* Private */,
      63,    0,  445,    2, 0x0a /* Public */,
      64,    1,  446,    2, 0x0a /* Public */,
      66,    0,  449,    2, 0x0a /* Public */,
      67,    1,  450,    2, 0x0a /* Public */,
      69,    2,  453,    2, 0x0a /* Public */,
      72,    0,  458,    2, 0x0a /* Public */,
      73,    1,  459,    2, 0x0a /* Public */,
      74,    1,  462,    2, 0x0a /* Public */,
      75,    1,  465,    2, 0x0a /* Public */,
      76,    3,  468,    2, 0x0a /* Public */,
      78,    1,  475,    2, 0x0a /* Public */,
      79,    1,  478,    2, 0x0a /* Public */,
      80,    0,  481,    2, 0x0a /* Public */,
      81,    1,  482,    2, 0x0a /* Public */,
      83,    1,  485,    2, 0x0a /* Public */,
      85,    1,  488,    2, 0x0a /* Public */,
      86,    0,  491,    2, 0x0a /* Public */,
      87,    1,  492,    2, 0x0a /* Public */,
      88,    1,  495,    2, 0x0a /* Public */,
      89,    1,  498,    2, 0x0a /* Public */,
      90,    1,  501,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 22,   13,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   13,   27,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   13,   29,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   13,   31,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   33,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 35, 0x80000000 | 35, 0x80000000 | 38,   13,   36,   37,   39,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 43,   44,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   49,
    QMetaType::Void, QMetaType::QString,   49,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 52,   13,   49,   53,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, 0x80000000 | 52,   13,   55,   49,   53,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 58,   59,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   65,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 68,   53,
    QMetaType::Void, 0x80000000 | 70, 0x80000000 | 71,   53,   39,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   29,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, 0x80000000 | 77, 0x80000000 | 77, 0x80000000 | 38,   36,   37,   39,
    QMetaType::Void, QMetaType::QString,   55,
    QMetaType::Void, QMetaType::QString,   55,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 82,   59,
    QMetaType::Void, QMetaType::QString,   84,
    QMetaType::Void, QMetaType::QString,   84,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

void KyNetworkResourceManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KyNetworkResourceManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->wiredConnectionAdd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->wiredConnectionRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->connectionAdd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: _t->connectionUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 4: _t->connectionRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->wiredDeviceAdd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->wiredDeviceUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->wiredDeviceRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->wirelessDeviceAdd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->wirelessDeviceUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->wirelessDeviceRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->deviceAdd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< KyDeviceType(*)>(_a[2]))); break;
        case 12: _t->deviceUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 13: _t->deviceRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->deviceActiveChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 15: _t->deviceCarrierChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 16: _t->deviceBitRateChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->deviceMacAddressChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 18: _t->deviceStateChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< KyDeviceState(*)>(_a[2])),(*reinterpret_cast< KyDeviceState(*)>(_a[3])),(*reinterpret_cast< NetworkManager::Device::StateChangeReason(*)>(_a[4]))); break;
        case 19: _t->wifiNetworkRemoved((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 20: _t->wifiNetworkAdded((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 21: _t->wifiNetworkPropertyChange((*reinterpret_cast< NetworkManager::WirelessNetwork*(*)>(_a[1]))); break;
        case 22: _t->wifiNetworkDeviceDisappear(); break;
        case 23: _t->wirelessConnectionAdd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 24: _t->wirelessConnectionRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->activeConnectionAdd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->activeConnectionRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->wiredConnectStateChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< KyConnectState(*)>(_a[3]))); break;
        case 28: _t->wirelessConnectStateChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< KyConnectState(*)>(_a[4]))); break;
        case 29: _t->wirelessApConnectStateChange(); break;
        case 30: _t->connectivityChanged((*reinterpret_cast< KyConnectivity(*)>(_a[1]))); break;
        case 31: _t->wifiEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 32: _t->networkingEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 33: _t->onServiceAppear((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 34: _t->onInitNetwork(); break;
        case 35: _t->setWirelessNetworkEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: _t->onConnectionUpdated(); break;
        case 37: _t->onActiveConnectionChanged((*reinterpret_cast< NetworkManager::ActiveConnection::State(*)>(_a[1]))); break;
        case 38: _t->onVpnActiveConnectChangedReason((*reinterpret_cast< NetworkManager::VpnConnection::State(*)>(_a[1])),(*reinterpret_cast< NetworkManager::VpnConnection::StateChangeReason(*)>(_a[2]))); break;
        case 39: _t->onDeviceActiveChange(); break;
        case 40: _t->onDeviceCarrierChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 41: _t->onDeviceBitRateChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->onDeviceMacAddressChange((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 43: _t->onDeviceStateChanged((*reinterpret_cast< NetworkManager::Device::State(*)>(_a[1])),(*reinterpret_cast< NetworkManager::Device::State(*)>(_a[2])),(*reinterpret_cast< NetworkManager::Device::StateChangeReason(*)>(_a[3]))); break;
        case 44: _t->onWifiNetworkAppeared((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 45: _t->onWifiNetworkDisappeared((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 46: _t->onUpdateWirelessNet(); break;
        case 47: _t->onConnectivityChanged((*reinterpret_cast< NetworkManager::Connectivity(*)>(_a[1]))); break;
        case 48: _t->onDeviceAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 49: _t->onDeviceRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 50: _t->onDeviceUpdated(); break;
        case 51: _t->onActiveConnectionAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 52: _t->onActiveConnectionRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 53: _t->onConnectionAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 54: _t->onConnectionRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 21:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< NetworkManager::WirelessNetwork* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wiredConnectionAdd)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wiredConnectionRemove)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::connectionAdd)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::connectionUpdate)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::connectionRemove)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wiredDeviceAdd)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wiredDeviceUpdate)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wiredDeviceRemove)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wirelessDeviceAdd)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wirelessDeviceUpdate)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wirelessDeviceRemove)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , KyDeviceType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::deviceAdd)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::deviceUpdate)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::deviceRemove)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::deviceActiveChange)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::deviceCarrierChange)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::deviceBitRateChange)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::deviceMacAddressChange)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , KyDeviceState , KyDeviceState , NetworkManager::Device::StateChangeReason );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::deviceStateChange)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wifiNetworkRemoved)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wifiNetworkAdded)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(NetworkManager::WirelessNetwork * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wifiNetworkPropertyChange)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wifiNetworkDeviceDisappear)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wirelessConnectionAdd)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wirelessConnectionRemove)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::activeConnectionAdd)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::activeConnectionRemove)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString , KyConnectState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wiredConnectStateChange)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(QString , QString , QString , KyConnectState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wirelessConnectStateChange)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wirelessApConnectStateChange)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(KyConnectivity );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::connectivityChanged)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::wifiEnabledChanged)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (KyNetworkResourceManager::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkResourceManager::networkingEnabledChanged)) {
                *result = 32;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KyNetworkResourceManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_KyNetworkResourceManager.data,
    qt_meta_data_KyNetworkResourceManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KyNetworkResourceManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KyNetworkResourceManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KyNetworkResourceManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int KyNetworkResourceManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
    }
    return _id;
}

// SIGNAL 0
void KyNetworkResourceManager::wiredConnectionAdd(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KyNetworkResourceManager::wiredConnectionRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KyNetworkResourceManager::connectionAdd(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KyNetworkResourceManager::connectionUpdate(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void KyNetworkResourceManager::connectionRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void KyNetworkResourceManager::wiredDeviceAdd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void KyNetworkResourceManager::wiredDeviceUpdate(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void KyNetworkResourceManager::wiredDeviceRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void KyNetworkResourceManager::wirelessDeviceAdd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void KyNetworkResourceManager::wirelessDeviceUpdate(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void KyNetworkResourceManager::wirelessDeviceRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void KyNetworkResourceManager::deviceAdd(QString _t1, KyDeviceType _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void KyNetworkResourceManager::deviceUpdate(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void KyNetworkResourceManager::deviceRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void KyNetworkResourceManager::deviceActiveChange(QString _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void KyNetworkResourceManager::deviceCarrierChange(QString _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void KyNetworkResourceManager::deviceBitRateChange(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void KyNetworkResourceManager::deviceMacAddressChange(QString _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void KyNetworkResourceManager::deviceStateChange(QString _t1, KyDeviceState _t2, KyDeviceState _t3, NetworkManager::Device::StateChangeReason _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void KyNetworkResourceManager::wifiNetworkRemoved(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void KyNetworkResourceManager::wifiNetworkAdded(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void KyNetworkResourceManager::wifiNetworkPropertyChange(NetworkManager::WirelessNetwork * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void KyNetworkResourceManager::wifiNetworkDeviceDisappear()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void KyNetworkResourceManager::wirelessConnectionAdd(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void KyNetworkResourceManager::wirelessConnectionRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void KyNetworkResourceManager::activeConnectionAdd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void KyNetworkResourceManager::activeConnectionRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void KyNetworkResourceManager::wiredConnectStateChange(QString _t1, QString _t2, KyConnectState _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void KyNetworkResourceManager::wirelessConnectStateChange(QString _t1, QString _t2, QString _t3, KyConnectState _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void KyNetworkResourceManager::wirelessApConnectStateChange()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void KyNetworkResourceManager::connectivityChanged(KyConnectivity _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void KyNetworkResourceManager::wifiEnabledChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void KyNetworkResourceManager::networkingEnabledChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
