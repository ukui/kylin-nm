/****************************************************************************
** Meta object code from reading C++ file 'kylinnetworkmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../libnm-base-kylin/kylinnetworkmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kylinnetworkmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KyNetworkManager_t {
    QByteArrayData data[98];
    char stringdata0[1722];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KyNetworkManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KyNetworkManager_t qt_meta_stringdata_KyNetworkManager = {
    {
QT_MOC_LITERAL(0, 0, 16), // "KyNetworkManager"
QT_MOC_LITERAL(1, 17, 16), // "wiredStateChange"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 10), // "deviceName"
QT_MOC_LITERAL(4, 46, 4), // "uuid"
QT_MOC_LITERAL(5, 51, 14), // "KyConnectState"
QT_MOC_LITERAL(6, 66, 5), // "state"
QT_MOC_LITERAL(7, 72, 19), // "wirelessStateChange"
QT_MOC_LITERAL(8, 92, 4), // "ssid"
QT_MOC_LITERAL(9, 97, 17), // "deviceStateChange"
QT_MOC_LITERAL(10, 115, 13), // "KyDeviceState"
QT_MOC_LITERAL(11, 129, 8), // "newstate"
QT_MOC_LITERAL(12, 138, 8), // "oldstate"
QT_MOC_LITERAL(13, 147, 41), // "NetworkManager::Device::State..."
QT_MOC_LITERAL(14, 189, 6), // "reason"
QT_MOC_LITERAL(15, 196, 13), // "carrierChange"
QT_MOC_LITERAL(16, 210, 6), // "pluged"
QT_MOC_LITERAL(17, 217, 19), // "connectivityChanged"
QT_MOC_LITERAL(18, 237, 14), // "KyConnectivity"
QT_MOC_LITERAL(19, 252, 12), // "connectivity"
QT_MOC_LITERAL(20, 265, 18), // "wifiEnabledChanged"
QT_MOC_LITERAL(21, 284, 24), // "networkingEnabledChanged"
QT_MOC_LITERAL(22, 309, 18), // "wiredConnectionAdd"
QT_MOC_LITERAL(23, 328, 7), // "devName"
QT_MOC_LITERAL(24, 336, 11), // "connectUuid"
QT_MOC_LITERAL(25, 348, 11), // "connectName"
QT_MOC_LITERAL(26, 360, 11), // "connectPath"
QT_MOC_LITERAL(27, 372, 21), // "wiredConnectionRemove"
QT_MOC_LITERAL(28, 394, 4), // "path"
QT_MOC_LITERAL(29, 399, 14), // "wiredDeviceAdd"
QT_MOC_LITERAL(30, 414, 17), // "wiredDeviceUpdate"
QT_MOC_LITERAL(31, 432, 13), // "oldDeviceName"
QT_MOC_LITERAL(32, 446, 13), // "newDeviceName"
QT_MOC_LITERAL(33, 460, 17), // "wiredDeviceRemove"
QT_MOC_LITERAL(34, 478, 20), // "signalStrengthChange"
QT_MOC_LITERAL(35, 499, 8), // "strength"
QT_MOC_LITERAL(36, 508, 14), // "secuTypeChange"
QT_MOC_LITERAL(37, 523, 12), // "securityType"
QT_MOC_LITERAL(38, 536, 24), // "wirelessConnectionRemove"
QT_MOC_LITERAL(39, 561, 21), // "wirelessConnectionAdd"
QT_MOC_LITERAL(40, 583, 8), // "dbusPath"
QT_MOC_LITERAL(41, 592, 24), // "wirelessConnectionUpdate"
QT_MOC_LITERAL(42, 617, 10), // "KySecuType"
QT_MOC_LITERAL(43, 628, 15), // "connectSecuType"
QT_MOC_LITERAL(44, 644, 17), // "wifiNetworkUpdate"
QT_MOC_LITERAL(45, 662, 14), // "wifiNetworkAdd"
QT_MOC_LITERAL(46, 677, 18), // "KyWirelessNetItem&"
QT_MOC_LITERAL(47, 696, 4), // "item"
QT_MOC_LITERAL(48, 701, 17), // "wifiNetworkRemove"
QT_MOC_LITERAL(49, 719, 17), // "wirelessDeviceAdd"
QT_MOC_LITERAL(50, 737, 20), // "wirelessDeviceRemove"
QT_MOC_LITERAL(51, 758, 24), // "wirelessDeviceNameUpdate"
QT_MOC_LITERAL(52, 783, 7), // "oldName"
QT_MOC_LITERAL(53, 791, 7), // "newName"
QT_MOC_LITERAL(54, 799, 28), // "wirelessApConnectStateChange"
QT_MOC_LITERAL(55, 828, 27), // "onSetWirelessNetworkEnabled"
QT_MOC_LITERAL(56, 856, 7), // "enabled"
QT_MOC_LITERAL(57, 864, 22), // "onSetNetworkingEnabled"
QT_MOC_LITERAL(58, 887, 17), // "onSetWiredEnabled"
QT_MOC_LITERAL(59, 905, 15), // "onDeleteConnect"
QT_MOC_LITERAL(60, 921, 20), // "onActivateConnection"
QT_MOC_LITERAL(61, 942, 22), // "onDeactivateConnection"
QT_MOC_LITERAL(62, 965, 17), // "activeConnectUuid"
QT_MOC_LITERAL(63, 983, 22), // "onSetWiredDeviceEnable"
QT_MOC_LITERAL(64, 1006, 9), // "interface"
QT_MOC_LITERAL(65, 1016, 6), // "enable"
QT_MOC_LITERAL(66, 1023, 21), // "onRequestWirelessScan"
QT_MOC_LITERAL(67, 1045, 20), // "onCreateWiredConnect"
QT_MOC_LITERAL(68, 1066, 16), // "KyConnectSetting"
QT_MOC_LITERAL(69, 1083, 19), // "connectSettingsInfo"
QT_MOC_LITERAL(70, 1103, 26), // "onAddAndActivateNormalWifi"
QT_MOC_LITERAL(71, 1130, 25), // "KyWirelessConnectSetting&"
QT_MOC_LITERAL(72, 1156, 15), // "connSettingInfo"
QT_MOC_LITERAL(73, 1172, 11), // "KySecuType&"
QT_MOC_LITERAL(74, 1184, 4), // "type"
QT_MOC_LITERAL(75, 1189, 42), // "onAddAndActiveWirelessEnterPr..."
QT_MOC_LITERAL(76, 1232, 19), // "KyEapMethodTlsInfo&"
QT_MOC_LITERAL(77, 1252, 4), // "info"
QT_MOC_LITERAL(78, 1257, 43), // "onAddAndActiveWirelessEnterPr..."
QT_MOC_LITERAL(79, 1301, 20), // "KyEapMethodPeapInfo&"
QT_MOC_LITERAL(80, 1322, 43), // "onAddAndActiveWirelessEnterPr..."
QT_MOC_LITERAL(81, 1366, 20), // "KyEapMethodTtlsInfo&"
QT_MOC_LITERAL(82, 1387, 18), // "onAddNormalConnect"
QT_MOC_LITERAL(83, 1406, 24), // "KyWirelessConnectSetting"
QT_MOC_LITERAL(84, 1431, 15), // "onAddTlsConnect"
QT_MOC_LITERAL(85, 1447, 18), // "KyEapMethodTlsInfo"
QT_MOC_LITERAL(86, 1466, 7), // "tlsinfo"
QT_MOC_LITERAL(87, 1474, 16), // "onAddPeapConnect"
QT_MOC_LITERAL(88, 1491, 19), // "KyEapMethodPeapInfo"
QT_MOC_LITERAL(89, 1511, 8), // "peapInfo"
QT_MOC_LITERAL(90, 1520, 16), // "onAddTtlsConnect"
QT_MOC_LITERAL(91, 1537, 19), // "KyEapMethodTtlsInfo"
QT_MOC_LITERAL(92, 1557, 8), // "ttlsInfo"
QT_MOC_LITERAL(93, 1566, 31), // "onUpdateWirelessPersonalConnect"
QT_MOC_LITERAL(94, 1598, 10), // "bPwdChange"
QT_MOC_LITERAL(95, 1609, 36), // "onUpdateWirelessEnterPriseTls..."
QT_MOC_LITERAL(96, 1646, 37), // "onUpdateWirelessEnterPrisePea..."
QT_MOC_LITERAL(97, 1684, 37) // "onUpdateWirelessEnterPriseTtl..."

    },
    "KyNetworkManager\0wiredStateChange\0\0"
    "deviceName\0uuid\0KyConnectState\0state\0"
    "wirelessStateChange\0ssid\0deviceStateChange\0"
    "KyDeviceState\0newstate\0oldstate\0"
    "NetworkManager::Device::StateChangeReason\0"
    "reason\0carrierChange\0pluged\0"
    "connectivityChanged\0KyConnectivity\0"
    "connectivity\0wifiEnabledChanged\0"
    "networkingEnabledChanged\0wiredConnectionAdd\0"
    "devName\0connectUuid\0connectName\0"
    "connectPath\0wiredConnectionRemove\0"
    "path\0wiredDeviceAdd\0wiredDeviceUpdate\0"
    "oldDeviceName\0newDeviceName\0"
    "wiredDeviceRemove\0signalStrengthChange\0"
    "strength\0secuTypeChange\0securityType\0"
    "wirelessConnectionRemove\0wirelessConnectionAdd\0"
    "dbusPath\0wirelessConnectionUpdate\0"
    "KySecuType\0connectSecuType\0wifiNetworkUpdate\0"
    "wifiNetworkAdd\0KyWirelessNetItem&\0"
    "item\0wifiNetworkRemove\0wirelessDeviceAdd\0"
    "wirelessDeviceRemove\0wirelessDeviceNameUpdate\0"
    "oldName\0newName\0wirelessApConnectStateChange\0"
    "onSetWirelessNetworkEnabled\0enabled\0"
    "onSetNetworkingEnabled\0onSetWiredEnabled\0"
    "onDeleteConnect\0onActivateConnection\0"
    "onDeactivateConnection\0activeConnectUuid\0"
    "onSetWiredDeviceEnable\0interface\0"
    "enable\0onRequestWirelessScan\0"
    "onCreateWiredConnect\0KyConnectSetting\0"
    "connectSettingsInfo\0onAddAndActivateNormalWifi\0"
    "KyWirelessConnectSetting&\0connSettingInfo\0"
    "KySecuType&\0type\0"
    "onAddAndActiveWirelessEnterPriseTlsConnect\0"
    "KyEapMethodTlsInfo&\0info\0"
    "onAddAndActiveWirelessEnterPrisePeapConnect\0"
    "KyEapMethodPeapInfo&\0"
    "onAddAndActiveWirelessEnterPriseTtlsConnect\0"
    "KyEapMethodTtlsInfo&\0onAddNormalConnect\0"
    "KyWirelessConnectSetting\0onAddTlsConnect\0"
    "KyEapMethodTlsInfo\0tlsinfo\0onAddPeapConnect\0"
    "KyEapMethodPeapInfo\0peapInfo\0"
    "onAddTtlsConnect\0KyEapMethodTtlsInfo\0"
    "ttlsInfo\0onUpdateWirelessPersonalConnect\0"
    "bPwdChange\0onUpdateWirelessEnterPriseTlsConnect\0"
    "onUpdateWirelessEnterPrisePeapConnect\0"
    "onUpdateWirelessEnterPriseTtlsConnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KyNetworkManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      45,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      24,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,  239,    2, 0x06 /* Public */,
       7,    4,  246,    2, 0x06 /* Public */,
       9,    4,  255,    2, 0x06 /* Public */,
      15,    2,  264,    2, 0x06 /* Public */,
      17,    1,  269,    2, 0x06 /* Public */,
      20,    1,  272,    2, 0x06 /* Public */,
      21,    1,  275,    2, 0x06 /* Public */,
      22,    4,  278,    2, 0x06 /* Public */,
      27,    1,  287,    2, 0x06 /* Public */,
      29,    1,  290,    2, 0x06 /* Public */,
      30,    2,  293,    2, 0x06 /* Public */,
      33,    1,  298,    2, 0x06 /* Public */,
      34,    3,  301,    2, 0x06 /* Public */,
      36,    3,  308,    2, 0x06 /* Public */,
      38,    2,  315,    2, 0x06 /* Public */,
      39,    4,  320,    2, 0x06 /* Public */,
      41,    5,  329,    2, 0x06 /* Public */,
      44,    0,  340,    2, 0x06 /* Public */,
      45,    2,  341,    2, 0x06 /* Public */,
      48,    2,  346,    2, 0x06 /* Public */,
      49,    1,  351,    2, 0x06 /* Public */,
      50,    1,  354,    2, 0x06 /* Public */,
      51,    2,  357,    2, 0x06 /* Public */,
      54,    0,  362,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      55,    1,  363,    2, 0x0a /* Public */,
      57,    1,  366,    2, 0x0a /* Public */,
      58,    1,  369,    2, 0x0a /* Public */,
      59,    1,  372,    2, 0x0a /* Public */,
      60,    2,  375,    2, 0x0a /* Public */,
      61,    1,  380,    2, 0x0a /* Public */,
      63,    2,  383,    2, 0x0a /* Public */,
      66,    0,  388,    2, 0x0a /* Public */,
      67,    1,  389,    2, 0x0a /* Public */,
      70,    2,  392,    2, 0x0a /* Public */,
      75,    2,  397,    2, 0x0a /* Public */,
      78,    2,  402,    2, 0x0a /* Public */,
      80,    2,  407,    2, 0x0a /* Public */,
      82,    2,  412,    2, 0x0a /* Public */,
      84,    2,  417,    2, 0x0a /* Public */,
      87,    2,  422,    2, 0x0a /* Public */,
      90,    2,  427,    2, 0x0a /* Public */,
      93,    3,  432,    2, 0x0a /* Public */,
      95,    2,  439,    2, 0x0a /* Public */,
      96,    2,  444,    2, 0x0a /* Public */,
      97,    2,  449,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, 0x80000000 | 5,    3,    8,    4,    6,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10, 0x80000000 | 10, 0x80000000 | 13,    3,   11,   12,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    3,   16,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   23,   24,   25,   26,
    QMetaType::Void, QMetaType::QString,   28,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   31,   32,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    3,    8,   35,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    8,   37,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    8,    4,   40,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, 0x80000000 | 42,    3,    8,    4,   40,   43,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 46,    3,   47,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    8,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   52,   53,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   56,
    QMetaType::Void, QMetaType::Bool,   56,
    QMetaType::Void, QMetaType::Bool,   56,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   24,    3,
    QMetaType::Void, QMetaType::QString,   62,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   64,   65,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 68,   69,
    QMetaType::Void, 0x80000000 | 71, 0x80000000 | 73,   72,   74,
    QMetaType::Void, 0x80000000 | 76, 0x80000000 | 71,   77,   72,
    QMetaType::Void, 0x80000000 | 79, 0x80000000 | 71,   77,   72,
    QMetaType::Void, 0x80000000 | 81, 0x80000000 | 71,   77,   72,
    QMetaType::Void, 0x80000000 | 83, 0x80000000 | 73,   72,   74,
    QMetaType::Void, 0x80000000 | 83, 0x80000000 | 85,   72,   86,
    QMetaType::Void, 0x80000000 | 83, 0x80000000 | 88,   72,   89,
    QMetaType::Void, 0x80000000 | 83, 0x80000000 | 91,   72,   92,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 83, QMetaType::Bool,    4,   72,   94,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 85,    4,   86,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 88,    4,   89,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 91,    4,   92,

       0        // eod
};

void KyNetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KyNetworkManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->wiredStateChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< KyConnectState(*)>(_a[3]))); break;
        case 1: _t->wirelessStateChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< KyConnectState(*)>(_a[4]))); break;
        case 2: _t->deviceStateChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< KyDeviceState(*)>(_a[2])),(*reinterpret_cast< KyDeviceState(*)>(_a[3])),(*reinterpret_cast< NetworkManager::Device::StateChangeReason(*)>(_a[4]))); break;
        case 3: _t->carrierChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->connectivityChanged((*reinterpret_cast< KyConnectivity(*)>(_a[1]))); break;
        case 5: _t->wifiEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->networkingEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->wiredConnectionAdd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 8: _t->wiredConnectionRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->wiredDeviceAdd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->wiredDeviceUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 11: _t->wiredDeviceRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->signalStrengthChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 13: _t->secuTypeChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 14: _t->wirelessConnectionRemove((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 15: _t->wirelessConnectionAdd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 16: _t->wirelessConnectionUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< KySecuType(*)>(_a[5]))); break;
        case 17: _t->wifiNetworkUpdate(); break;
        case 18: _t->wifiNetworkAdd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< KyWirelessNetItem(*)>(_a[2]))); break;
        case 19: _t->wifiNetworkRemove((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 20: _t->wirelessDeviceAdd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->wirelessDeviceRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->wirelessDeviceNameUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 23: _t->wirelessApConnectStateChange(); break;
        case 24: _t->onSetWirelessNetworkEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->onSetNetworkingEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: _t->onSetWiredEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: _t->onDeleteConnect((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 28: _t->onActivateConnection((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 29: _t->onDeactivateConnection((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 30: _t->onSetWiredDeviceEnable((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 31: _t->onRequestWirelessScan(); break;
        case 32: _t->onCreateWiredConnect((*reinterpret_cast< KyConnectSetting(*)>(_a[1]))); break;
        case 33: _t->onAddAndActivateNormalWifi((*reinterpret_cast< KyWirelessConnectSetting(*)>(_a[1])),(*reinterpret_cast< KySecuType(*)>(_a[2]))); break;
        case 34: _t->onAddAndActiveWirelessEnterPriseTlsConnect((*reinterpret_cast< KyEapMethodTlsInfo(*)>(_a[1])),(*reinterpret_cast< KyWirelessConnectSetting(*)>(_a[2]))); break;
        case 35: _t->onAddAndActiveWirelessEnterPrisePeapConnect((*reinterpret_cast< KyEapMethodPeapInfo(*)>(_a[1])),(*reinterpret_cast< KyWirelessConnectSetting(*)>(_a[2]))); break;
        case 36: _t->onAddAndActiveWirelessEnterPriseTtlsConnect((*reinterpret_cast< KyEapMethodTtlsInfo(*)>(_a[1])),(*reinterpret_cast< KyWirelessConnectSetting(*)>(_a[2]))); break;
        case 37: _t->onAddNormalConnect((*reinterpret_cast< const KyWirelessConnectSetting(*)>(_a[1])),(*reinterpret_cast< KySecuType(*)>(_a[2]))); break;
        case 38: _t->onAddTlsConnect((*reinterpret_cast< const KyWirelessConnectSetting(*)>(_a[1])),(*reinterpret_cast< const KyEapMethodTlsInfo(*)>(_a[2]))); break;
        case 39: _t->onAddPeapConnect((*reinterpret_cast< const KyWirelessConnectSetting(*)>(_a[1])),(*reinterpret_cast< const KyEapMethodPeapInfo(*)>(_a[2]))); break;
        case 40: _t->onAddTtlsConnect((*reinterpret_cast< const KyWirelessConnectSetting(*)>(_a[1])),(*reinterpret_cast< const KyEapMethodTtlsInfo(*)>(_a[2]))); break;
        case 41: _t->onUpdateWirelessPersonalConnect((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const KyWirelessConnectSetting(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 42: _t->onUpdateWirelessEnterPriseTlsConnect((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const KyEapMethodTlsInfo(*)>(_a[2]))); break;
        case 43: _t->onUpdateWirelessEnterPrisePeapConnect((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const KyEapMethodPeapInfo(*)>(_a[2]))); break;
        case 44: _t->onUpdateWirelessEnterPriseTtlsConnect((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const KyEapMethodTtlsInfo(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KyNetworkManager::*)(QString , QString , KyConnectState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wiredStateChange)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString , QString , KyConnectState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wirelessStateChange)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , KyDeviceState , KyDeviceState , NetworkManager::Device::StateChangeReason );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::deviceStateChange)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::carrierChange)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(KyConnectivity );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::connectivityChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wifiEnabledChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::networkingEnabledChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wiredConnectionAdd)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wiredConnectionRemove)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wiredDeviceAdd)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wiredDeviceUpdate)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wiredDeviceRemove)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::signalStrengthChange)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::secuTypeChange)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wirelessConnectionRemove)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wirelessConnectionAdd)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString , QString , QString , KySecuType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wirelessConnectionUpdate)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wifiNetworkUpdate)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , KyWirelessNetItem & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wifiNetworkAdd)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wifiNetworkRemove)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wirelessDeviceAdd)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wirelessDeviceRemove)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wirelessDeviceNameUpdate)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (KyNetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyNetworkManager::wirelessApConnectStateChange)) {
                *result = 23;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KyNetworkManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_KyNetworkManager.data,
    qt_meta_data_KyNetworkManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KyNetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KyNetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KyNetworkManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int KyNetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 45)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 45;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 45)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 45;
    }
    return _id;
}

// SIGNAL 0
void KyNetworkManager::wiredStateChange(QString _t1, QString _t2, KyConnectState _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KyNetworkManager::wirelessStateChange(QString _t1, QString _t2, QString _t3, KyConnectState _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KyNetworkManager::deviceStateChange(QString _t1, KyDeviceState _t2, KyDeviceState _t3, NetworkManager::Device::StateChangeReason _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KyNetworkManager::carrierChange(QString _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void KyNetworkManager::connectivityChanged(KyConnectivity _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void KyNetworkManager::wifiEnabledChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void KyNetworkManager::networkingEnabledChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void KyNetworkManager::wiredConnectionAdd(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void KyNetworkManager::wiredConnectionRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void KyNetworkManager::wiredDeviceAdd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void KyNetworkManager::wiredDeviceUpdate(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void KyNetworkManager::wiredDeviceRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void KyNetworkManager::signalStrengthChange(QString _t1, QString _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void KyNetworkManager::secuTypeChange(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void KyNetworkManager::wirelessConnectionRemove(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void KyNetworkManager::wirelessConnectionAdd(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void KyNetworkManager::wirelessConnectionUpdate(QString _t1, QString _t2, QString _t3, QString _t4, KySecuType _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void KyNetworkManager::wifiNetworkUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void KyNetworkManager::wifiNetworkAdd(QString _t1, KyWirelessNetItem & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void KyNetworkManager::wifiNetworkRemove(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void KyNetworkManager::wirelessDeviceAdd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void KyNetworkManager::wirelessDeviceRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void KyNetworkManager::wirelessDeviceNameUpdate(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void KyNetworkManager::wirelessApConnectStateChange()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
