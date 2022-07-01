/****************************************************************************
** Meta object code from reading C++ file 'kylinsecretagent.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../libnm-external-kylin/kylin-agent/kylinsecretagent.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kylinsecretagent.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KylinSecretAgent_t {
    QByteArrayData data[13];
    char stringdata0[156];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KylinSecretAgent_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KylinSecretAgent_t qt_meta_stringdata_KylinSecretAgent = {
    {
QT_MOC_LITERAL(0, 0, 16), // "KylinSecretAgent"
QT_MOC_LITERAL(1, 17, 10), // "GetSecrets"
QT_MOC_LITERAL(2, 28, 15), // "NMVariantMapMap"
QT_MOC_LITERAL(3, 44, 0), // ""
QT_MOC_LITERAL(4, 45, 10), // "connection"
QT_MOC_LITERAL(5, 56, 15), // "QDBusObjectPath"
QT_MOC_LITERAL(6, 72, 15), // "connection_path"
QT_MOC_LITERAL(7, 88, 12), // "setting_name"
QT_MOC_LITERAL(8, 101, 5), // "hints"
QT_MOC_LITERAL(9, 107, 5), // "flags"
QT_MOC_LITERAL(10, 113, 16), // "CancelGetSecrets"
QT_MOC_LITERAL(11, 130, 11), // "SaveSecrets"
QT_MOC_LITERAL(12, 142, 13) // "DeleteSecrets"

    },
    "KylinSecretAgent\0GetSecrets\0NMVariantMapMap\0"
    "\0connection\0QDBusObjectPath\0connection_path\0"
    "setting_name\0hints\0flags\0CancelGetSecrets\0"
    "SaveSecrets\0DeleteSecrets"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KylinSecretAgent[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    5,   34,    3, 0x0a /* Public */,
      10,    2,   45,    3, 0x0a /* Public */,
      11,    2,   50,    3, 0x0a /* Public */,
      12,    2,   55,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2, 0x80000000 | 2, 0x80000000 | 5, QMetaType::QString, QMetaType::QStringList, QMetaType::UInt,    4,    6,    7,    8,    9,
    QMetaType::Void, 0x80000000 | 5, QMetaType::QString,    6,    7,
    QMetaType::Void, 0x80000000 | 2, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 2, 0x80000000 | 5,    4,    6,

       0        // eod
};

void KylinSecretAgent::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<KylinSecretAgent *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { NMVariantMapMap _r = _t->GetSecrets((*reinterpret_cast< const NMVariantMapMap(*)>(_a[1])),(*reinterpret_cast< const QDBusObjectPath(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QStringList(*)>(_a[4])),(*reinterpret_cast< uint(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< NMVariantMapMap*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->CancelGetSecrets((*reinterpret_cast< const QDBusObjectPath(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->SaveSecrets((*reinterpret_cast< const NMVariantMapMap(*)>(_a[1])),(*reinterpret_cast< const QDBusObjectPath(*)>(_a[2]))); break;
        case 3: _t->DeleteSecrets((*reinterpret_cast< const NMVariantMapMap(*)>(_a[1])),(*reinterpret_cast< const QDBusObjectPath(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< NMVariantMapMap >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QDBusObjectPath >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QDBusObjectPath >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< NMVariantMapMap >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QDBusObjectPath >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< NMVariantMapMap >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QDBusObjectPath >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject KylinSecretAgent::staticMetaObject = { {
    &NetworkManager::SecretAgent::staticMetaObject,
    qt_meta_stringdata_KylinSecretAgent.data,
    qt_meta_data_KylinSecretAgent,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *KylinSecretAgent::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KylinSecretAgent::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KylinSecretAgent.stringdata0))
        return static_cast<void*>(this);
    return NetworkManager::SecretAgent::qt_metacast(_clname);
}

int KylinSecretAgent::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NetworkManager::SecretAgent::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
