/****************************************************************************
** Meta object code from reading C++ file 'kylinactiveconnectresource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../libnm-base-kylin/kylinactiveconnectresource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kylinactiveconnectresource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KyActiveConnectResourse_t {
    QByteArrayData data[9];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KyActiveConnectResourse_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KyActiveConnectResourse_t qt_meta_stringdata_KyActiveConnectResourse = {
    {
QT_MOC_LITERAL(0, 0, 23), // "KyActiveConnectResourse"
QT_MOC_LITERAL(1, 24, 16), // "wiredStateChange"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 10), // "deviceName"
QT_MOC_LITERAL(4, 53, 4), // "uuid"
QT_MOC_LITERAL(5, 58, 14), // "KyConnectState"
QT_MOC_LITERAL(6, 73, 5), // "state"
QT_MOC_LITERAL(7, 79, 19), // "wirelessStateChange"
QT_MOC_LITERAL(8, 99, 4) // "ssid"

    },
    "KyActiveConnectResourse\0wiredStateChange\0"
    "\0deviceName\0uuid\0KyConnectState\0state\0"
    "wirelessStateChange\0ssid"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KyActiveConnectResourse[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,
       7,    4,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, 0x80000000 | 5,    3,    8,    4,    6,

       0        // eod
};

void KyActiveConnectResourse::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KyActiveConnectResourse *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->wiredStateChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< KyConnectState(*)>(_a[3]))); break;
        case 1: _t->wirelessStateChange((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< KyConnectState(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KyActiveConnectResourse::*)(QString , QString , KyConnectState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyActiveConnectResourse::wiredStateChange)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (KyActiveConnectResourse::*)(QString , QString , QString , KyConnectState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyActiveConnectResourse::wirelessStateChange)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KyActiveConnectResourse::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_KyActiveConnectResourse.data,
    qt_meta_data_KyActiveConnectResourse,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KyActiveConnectResourse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KyActiveConnectResourse::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KyActiveConnectResourse.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int KyActiveConnectResourse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void KyActiveConnectResourse::wiredStateChange(QString _t1, QString _t2, KyConnectState _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KyActiveConnectResourse::wirelessStateChange(QString _t1, QString _t2, QString _t3, KyConnectState _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
