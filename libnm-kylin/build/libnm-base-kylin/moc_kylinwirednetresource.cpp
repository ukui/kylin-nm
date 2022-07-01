/****************************************************************************
** Meta object code from reading C++ file 'kylinwirednetresource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../libnm-base-kylin/kylinwirednetresource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kylinwirednetresource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KyWiredConnectResourse_t {
    QByteArrayData data[15];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KyWiredConnectResourse_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KyWiredConnectResourse_t qt_meta_stringdata_KyWiredConnectResourse = {
    {
QT_MOC_LITERAL(0, 0, 22), // "KyWiredConnectResourse"
QT_MOC_LITERAL(1, 23, 18), // "wiredConnectionAdd"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 7), // "devName"
QT_MOC_LITERAL(4, 51, 11), // "connectUuid"
QT_MOC_LITERAL(5, 63, 11), // "connectName"
QT_MOC_LITERAL(6, 75, 11), // "connectPath"
QT_MOC_LITERAL(7, 87, 21), // "wiredConnectionRemove"
QT_MOC_LITERAL(8, 109, 4), // "path"
QT_MOC_LITERAL(9, 114, 14), // "wiredDeviceAdd"
QT_MOC_LITERAL(10, 129, 10), // "deviceName"
QT_MOC_LITERAL(11, 140, 17), // "wiredDeviceUpdate"
QT_MOC_LITERAL(12, 158, 13), // "oldDeviceName"
QT_MOC_LITERAL(13, 172, 13), // "newDeviceName"
QT_MOC_LITERAL(14, 186, 17) // "wiredDeviceRemove"

    },
    "KyWiredConnectResourse\0wiredConnectionAdd\0"
    "\0devName\0connectUuid\0connectName\0"
    "connectPath\0wiredConnectionRemove\0"
    "path\0wiredDeviceAdd\0deviceName\0"
    "wiredDeviceUpdate\0oldDeviceName\0"
    "newDeviceName\0wiredDeviceRemove"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KyWiredConnectResourse[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   39,    2, 0x06 /* Public */,
       7,    1,   48,    2, 0x06 /* Public */,
       9,    1,   51,    2, 0x06 /* Public */,
      11,    2,   54,    2, 0x06 /* Public */,
      14,    1,   59,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   12,   13,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void KyWiredConnectResourse::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KyWiredConnectResourse *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->wiredConnectionAdd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->wiredConnectionRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->wiredDeviceAdd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->wiredDeviceUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->wiredDeviceRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KyWiredConnectResourse::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyWiredConnectResourse::wiredConnectionAdd)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (KyWiredConnectResourse::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyWiredConnectResourse::wiredConnectionRemove)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (KyWiredConnectResourse::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyWiredConnectResourse::wiredDeviceAdd)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (KyWiredConnectResourse::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyWiredConnectResourse::wiredDeviceUpdate)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (KyWiredConnectResourse::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyWiredConnectResourse::wiredDeviceRemove)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KyWiredConnectResourse::staticMetaObject = { {
    &KyNetResource::staticMetaObject,
    qt_meta_stringdata_KyWiredConnectResourse.data,
    qt_meta_data_KyWiredConnectResourse,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KyWiredConnectResourse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KyWiredConnectResourse::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KyWiredConnectResourse.stringdata0))
        return static_cast<void*>(this);
    return KyNetResource::qt_metacast(_clname);
}

int KyWiredConnectResourse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KyNetResource::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void KyWiredConnectResourse::wiredConnectionAdd(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KyWiredConnectResourse::wiredConnectionRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KyWiredConnectResourse::wiredDeviceAdd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KyWiredConnectResourse::wiredDeviceUpdate(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void KyWiredConnectResourse::wiredDeviceRemove(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
