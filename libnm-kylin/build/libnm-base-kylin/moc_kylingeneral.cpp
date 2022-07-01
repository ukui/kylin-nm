/****************************************************************************
** Meta object code from reading C++ file 'kylingeneral.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../libnm-base-kylin/kylingeneral.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kylingeneral.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KylinGeneralOpration_t {
    QByteArrayData data[7];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KylinGeneralOpration_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KylinGeneralOpration_t qt_meta_stringdata_KylinGeneralOpration = {
    {
QT_MOC_LITERAL(0, 0, 20), // "KylinGeneralOpration"
QT_MOC_LITERAL(1, 21, 19), // "connectivityChanged"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 14), // "KyConnectivity"
QT_MOC_LITERAL(4, 57, 12), // "connectivity"
QT_MOC_LITERAL(5, 70, 18), // "wifiEnabledChanged"
QT_MOC_LITERAL(6, 89, 24) // "networkingEnabledChanged"

    },
    "KylinGeneralOpration\0connectivityChanged\0"
    "\0KyConnectivity\0connectivity\0"
    "wifiEnabledChanged\0networkingEnabledChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KylinGeneralOpration[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       5,    1,   32,    2, 0x06 /* Public */,
       6,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void KylinGeneralOpration::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KylinGeneralOpration *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectivityChanged((*reinterpret_cast< KyConnectivity(*)>(_a[1]))); break;
        case 1: _t->wifiEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->networkingEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KylinGeneralOpration::*)(KyConnectivity );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KylinGeneralOpration::connectivityChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (KylinGeneralOpration::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KylinGeneralOpration::wifiEnabledChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (KylinGeneralOpration::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KylinGeneralOpration::networkingEnabledChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KylinGeneralOpration::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_KylinGeneralOpration.data,
    qt_meta_data_KylinGeneralOpration,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KylinGeneralOpration::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KylinGeneralOpration::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KylinGeneralOpration.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int KylinGeneralOpration::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void KylinGeneralOpration::connectivityChanged(KyConnectivity _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KylinGeneralOpration::wifiEnabledChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KylinGeneralOpration::networkingEnabledChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
