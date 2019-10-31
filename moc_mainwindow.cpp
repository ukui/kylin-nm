/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[44];
    char stringdata0[637];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "iconActivated"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(4, 60, 6), // "reason"
QT_MOC_LITERAL(5, 67, 11), // "nativeEvent"
QT_MOC_LITERAL(6, 79, 9), // "eventType"
QT_MOC_LITERAL(7, 89, 7), // "message"
QT_MOC_LITERAL(8, 97, 5), // "long*"
QT_MOC_LITERAL(9, 103, 6), // "result"
QT_MOC_LITERAL(10, 110, 17), // "on_btnNet_clicked"
QT_MOC_LITERAL(11, 128, 18), // "on_btnWifi_clicked"
QT_MOC_LITERAL(12, 147, 21), // "on_btnNetList_clicked"
QT_MOC_LITERAL(13, 169, 4), // "flag"
QT_MOC_LITERAL(14, 174, 22), // "on_btnWifiList_clicked"
QT_MOC_LITERAL(15, 197, 14), // "getLanListDone"
QT_MOC_LITERAL(16, 212, 5), // "slist"
QT_MOC_LITERAL(17, 218, 15), // "getWifiListDone"
QT_MOC_LITERAL(18, 234, 16), // "loadWifiListDone"
QT_MOC_LITERAL(19, 251, 18), // "updateWifiListDone"
QT_MOC_LITERAL(20, 270, 21), // "on_btnAdvConf_clicked"
QT_MOC_LITERAL(21, 292, 21), // "on_btnNetList_pressed"
QT_MOC_LITERAL(22, 314, 22), // "on_btnWifiList_pressed"
QT_MOC_LITERAL(23, 337, 18), // "oneLanFormSelected"
QT_MOC_LITERAL(24, 356, 7), // "lanName"
QT_MOC_LITERAL(25, 364, 19), // "oneWifiFormSelected"
QT_MOC_LITERAL(26, 384, 8), // "wifiName"
QT_MOC_LITERAL(27, 393, 19), // "oneHideFormSelected"
QT_MOC_LITERAL(28, 413, 16), // "activeLanDisconn"
QT_MOC_LITERAL(29, 430, 17), // "activeWifiDisconn"
QT_MOC_LITERAL(30, 448, 21), // "on_btnAdvConf_pressed"
QT_MOC_LITERAL(31, 470, 22), // "on_btnAdvConf_released"
QT_MOC_LITERAL(32, 493, 15), // "on_isLanConnect"
QT_MOC_LITERAL(33, 509, 16), // "on_isWifiConnect"
QT_MOC_LITERAL(34, 526, 10), // "on_isNetOn"
QT_MOC_LITERAL(35, 537, 9), // "enNetDone"
QT_MOC_LITERAL(36, 547, 10), // "disNetDone"
QT_MOC_LITERAL(37, 558, 10), // "enWifiDone"
QT_MOC_LITERAL(38, 569, 11), // "disWifiDone"
QT_MOC_LITERAL(39, 581, 16), // "keepDisWifiState"
QT_MOC_LITERAL(40, 598, 11), // "connLanDone"
QT_MOC_LITERAL(41, 610, 8), // "connFlag"
QT_MOC_LITERAL(42, 619, 8), // "connDone"
QT_MOC_LITERAL(43, 628, 8) // "iconStep"

    },
    "MainWindow\0iconActivated\0\0"
    "QSystemTrayIcon::ActivationReason\0"
    "reason\0nativeEvent\0eventType\0message\0"
    "long*\0result\0on_btnNet_clicked\0"
    "on_btnWifi_clicked\0on_btnNetList_clicked\0"
    "flag\0on_btnWifiList_clicked\0getLanListDone\0"
    "slist\0getWifiListDone\0loadWifiListDone\0"
    "updateWifiListDone\0on_btnAdvConf_clicked\0"
    "on_btnNetList_pressed\0on_btnWifiList_pressed\0"
    "oneLanFormSelected\0lanName\0"
    "oneWifiFormSelected\0wifiName\0"
    "oneHideFormSelected\0activeLanDisconn\0"
    "activeWifiDisconn\0on_btnAdvConf_pressed\0"
    "on_btnAdvConf_released\0on_isLanConnect\0"
    "on_isWifiConnect\0on_isNetOn\0enNetDone\0"
    "disNetDone\0enWifiDone\0disWifiDone\0"
    "keepDisWifiState\0connLanDone\0connFlag\0"
    "connDone\0iconStep"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  174,    2, 0x08 /* Private */,
       5,    3,  177,    2, 0x08 /* Private */,
      10,    0,  184,    2, 0x08 /* Private */,
      11,    0,  185,    2, 0x08 /* Private */,
      12,    1,  186,    2, 0x08 /* Private */,
      12,    0,  189,    2, 0x28 /* Private | MethodCloned */,
      14,    0,  190,    2, 0x08 /* Private */,
      15,    1,  191,    2, 0x08 /* Private */,
      17,    1,  194,    2, 0x08 /* Private */,
      18,    1,  197,    2, 0x08 /* Private */,
      19,    1,  200,    2, 0x08 /* Private */,
      20,    0,  203,    2, 0x08 /* Private */,
      21,    0,  204,    2, 0x08 /* Private */,
      22,    0,  205,    2, 0x08 /* Private */,
      23,    1,  206,    2, 0x08 /* Private */,
      25,    1,  209,    2, 0x08 /* Private */,
      27,    1,  212,    2, 0x08 /* Private */,
      28,    0,  215,    2, 0x08 /* Private */,
      29,    0,  216,    2, 0x08 /* Private */,
      30,    0,  217,    2, 0x08 /* Private */,
      31,    0,  218,    2, 0x08 /* Private */,
      32,    0,  219,    2, 0x08 /* Private */,
      33,    0,  220,    2, 0x08 /* Private */,
      34,    0,  221,    2, 0x08 /* Private */,
      35,    0,  222,    2, 0x08 /* Private */,
      36,    0,  223,    2, 0x08 /* Private */,
      37,    0,  224,    2, 0x08 /* Private */,
      38,    0,  225,    2, 0x08 /* Private */,
      39,    0,  226,    2, 0x08 /* Private */,
      40,    1,  227,    2, 0x08 /* Private */,
      42,    1,  230,    2, 0x08 /* Private */,
      43,    0,  233,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Bool, QMetaType::QByteArray, QMetaType::VoidStar, 0x80000000 | 8,    6,    7,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,   16,
    QMetaType::Void, QMetaType::QStringList,   16,
    QMetaType::Void, QMetaType::QStringList,   16,
    QMetaType::Void, QMetaType::QStringList,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void, QMetaType::Int,   41,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->iconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 1: { bool _r = _t->nativeEvent((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2])),(*reinterpret_cast< long*(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->on_btnNet_clicked(); break;
        case 3: _t->on_btnWifi_clicked(); break;
        case 4: _t->on_btnNetList_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_btnNetList_clicked(); break;
        case 6: _t->on_btnWifiList_clicked(); break;
        case 7: _t->getLanListDone((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 8: _t->getWifiListDone((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 9: _t->loadWifiListDone((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 10: _t->updateWifiListDone((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 11: _t->on_btnAdvConf_clicked(); break;
        case 12: _t->on_btnNetList_pressed(); break;
        case 13: _t->on_btnWifiList_pressed(); break;
        case 14: _t->oneLanFormSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->oneWifiFormSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->oneHideFormSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->activeLanDisconn(); break;
        case 18: _t->activeWifiDisconn(); break;
        case 19: _t->on_btnAdvConf_pressed(); break;
        case 20: _t->on_btnAdvConf_released(); break;
        case 21: _t->on_isLanConnect(); break;
        case 22: _t->on_isWifiConnect(); break;
        case 23: _t->on_isNetOn(); break;
        case 24: _t->enNetDone(); break;
        case 25: _t->disNetDone(); break;
        case 26: _t->enWifiDone(); break;
        case 27: _t->disWifiDone(); break;
        case 28: _t->keepDisWifiState(); break;
        case 29: _t->connLanDone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->connDone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->iconStep(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 32;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
