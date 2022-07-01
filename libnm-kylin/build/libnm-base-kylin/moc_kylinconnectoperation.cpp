/****************************************************************************
** Meta object code from reading C++ file 'kylinconnectoperation.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../libnm-base-kylin/kylinconnectoperation.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kylinconnectoperation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KyConnectOperation_t {
    QByteArrayData data[8];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KyConnectOperation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KyConnectOperation_t qt_meta_stringdata_KyConnectOperation = {
    {
QT_MOC_LITERAL(0, 0, 18), // "KyConnectOperation"
QT_MOC_LITERAL(1, 19, 21), // "createConnectionError"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 12), // "errorMessage"
QT_MOC_LITERAL(4, 55, 21), // "updateConnectionError"
QT_MOC_LITERAL(5, 77, 21), // "deleteConnectionError"
QT_MOC_LITERAL(6, 99, 23), // "activateConnectionError"
QT_MOC_LITERAL(7, 123, 25) // "deactivateConnectionError"

    },
    "KyConnectOperation\0createConnectionError\0"
    "\0errorMessage\0updateConnectionError\0"
    "deleteConnectionError\0activateConnectionError\0"
    "deactivateConnectionError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KyConnectOperation[] = {

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
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,
       5,    1,   45,    2, 0x06 /* Public */,
       6,    1,   48,    2, 0x06 /* Public */,
       7,    1,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void KyConnectOperation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KyConnectOperation *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->createConnectionError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->updateConnectionError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->deleteConnectionError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->activateConnectionError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->deactivateConnectionError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (KyConnectOperation::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyConnectOperation::createConnectionError)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (KyConnectOperation::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyConnectOperation::updateConnectionError)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (KyConnectOperation::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyConnectOperation::deleteConnectionError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (KyConnectOperation::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyConnectOperation::activateConnectionError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (KyConnectOperation::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KyConnectOperation::deactivateConnectionError)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KyConnectOperation::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_KyConnectOperation.data,
    qt_meta_data_KyConnectOperation,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KyConnectOperation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KyConnectOperation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KyConnectOperation.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int KyConnectOperation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void KyConnectOperation::createConnectionError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KyConnectOperation::updateConnectionError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KyConnectOperation::deleteConnectionError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KyConnectOperation::activateConnectionError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void KyConnectOperation::deactivateConnectionError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
