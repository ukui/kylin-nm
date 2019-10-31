/****************************************************************************
** Meta object code from reading C++ file 'backthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "backthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BackThread_t {
    QByteArrayData data[18];
    char stringdata0[186];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BackThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BackThread_t qt_meta_stringdata_BackThread = {
    {
QT_MOC_LITERAL(0, 0, 10), // "BackThread"
QT_MOC_LITERAL(1, 11, 9), // "enNetDone"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 10), // "disNetDone"
QT_MOC_LITERAL(4, 33, 10), // "enWifiDone"
QT_MOC_LITERAL(5, 44, 11), // "disWifiDone"
QT_MOC_LITERAL(6, 56, 8), // "connDone"
QT_MOC_LITERAL(7, 65, 8), // "connFlag"
QT_MOC_LITERAL(8, 74, 8), // "btFinish"
QT_MOC_LITERAL(9, 83, 9), // "execEnNet"
QT_MOC_LITERAL(10, 93, 10), // "execDisNet"
QT_MOC_LITERAL(11, 104, 10), // "execEnWifi"
QT_MOC_LITERAL(12, 115, 11), // "execDisWifi"
QT_MOC_LITERAL(13, 127, 11), // "execConnLan"
QT_MOC_LITERAL(14, 139, 8), // "connName"
QT_MOC_LITERAL(15, 148, 12), // "execConnWifi"
QT_MOC_LITERAL(16, 161, 15), // "execConnWifiPWD"
QT_MOC_LITERAL(17, 177, 8) // "password"

    },
    "BackThread\0enNetDone\0\0disNetDone\0"
    "enWifiDone\0disWifiDone\0connDone\0"
    "connFlag\0btFinish\0execEnNet\0execDisNet\0"
    "execEnWifi\0execDisWifi\0execConnLan\0"
    "connName\0execConnWifi\0execConnWifiPWD\0"
    "password"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BackThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    0,   81,    2, 0x06 /* Public */,
       5,    0,   82,    2, 0x06 /* Public */,
       6,    1,   83,    2, 0x06 /* Public */,
       8,    0,   86,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   87,    2, 0x0a /* Public */,
      10,    0,   88,    2, 0x0a /* Public */,
      11,    0,   89,    2, 0x0a /* Public */,
      12,    0,   90,    2, 0x0a /* Public */,
      13,    1,   91,    2, 0x0a /* Public */,
      15,    1,   94,    2, 0x0a /* Public */,
      16,    2,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   14,   17,

       0        // eod
};

void BackThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BackThread *_t = static_cast<BackThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->enNetDone(); break;
        case 1: _t->disNetDone(); break;
        case 2: _t->enWifiDone(); break;
        case 3: _t->disWifiDone(); break;
        case 4: _t->connDone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->btFinish(); break;
        case 6: _t->execEnNet(); break;
        case 7: _t->execDisNet(); break;
        case 8: _t->execEnWifi(); break;
        case 9: _t->execDisWifi(); break;
        case 10: _t->execConnLan((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->execConnWifi((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->execConnWifiPWD((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (BackThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackThread::enNetDone)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (BackThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackThread::disNetDone)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (BackThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackThread::enWifiDone)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (BackThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackThread::disWifiDone)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (BackThread::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackThread::connDone)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (BackThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackThread::btFinish)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject BackThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BackThread.data,
      qt_meta_data_BackThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BackThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BackThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BackThread.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BackThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void BackThread::enNetDone()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BackThread::disNetDone()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BackThread::enWifiDone()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void BackThread::disWifiDone()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void BackThread::connDone(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BackThread::btFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
