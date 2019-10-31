/****************************************************************************
** Meta object code from reading C++ file 'ksimplenm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ksimplenm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ksimplenm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_KSimpleNM_t {
    QByteArrayData data[8];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KSimpleNM_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KSimpleNM_t qt_meta_stringdata_KSimpleNM = {
    {
QT_MOC_LITERAL(0, 0, 9), // "KSimpleNM"
QT_MOC_LITERAL(1, 10, 18), // "getLanListFinished"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 5), // "slist"
QT_MOC_LITERAL(4, 36, 19), // "getWifiListFinished"
QT_MOC_LITERAL(5, 56, 11), // "readProcess"
QT_MOC_LITERAL(6, 68, 15), // "finishedProcess"
QT_MOC_LITERAL(7, 84, 3) // "msg"

    },
    "KSimpleNM\0getLanListFinished\0\0slist\0"
    "getWifiListFinished\0readProcess\0"
    "finishedProcess\0msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KSimpleNM[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   40,    2, 0x0a /* Public */,
       6,    1,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::QStringList,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void KSimpleNM::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        KSimpleNM *_t = static_cast<KSimpleNM *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getLanListFinished((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 1: _t->getWifiListFinished((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 2: _t->readProcess(); break;
        case 3: _t->finishedProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (KSimpleNM::*_t)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KSimpleNM::getLanListFinished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (KSimpleNM::*_t)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&KSimpleNM::getWifiListFinished)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject KSimpleNM::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KSimpleNM.data,
      qt_meta_data_KSimpleNM,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *KSimpleNM::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KSimpleNM::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_KSimpleNM.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int KSimpleNM::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void KSimpleNM::getLanListFinished(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KSimpleNM::getWifiListFinished(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
