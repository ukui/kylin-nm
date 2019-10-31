/****************************************************************************
** Meta object code from reading C++ file 'dlgconnhidwifi.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dlgconnhidwifi.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgconnhidwifi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DlgConnHidWifi_t {
    QByteArrayData data[15];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DlgConnHidWifi_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DlgConnHidWifi_t qt_meta_stringdata_DlgConnHidWifi = {
    {
QT_MOC_LITERAL(0, 0, 14), // "DlgConnHidWifi"
QT_MOC_LITERAL(1, 15, 13), // "reSetWifiList"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 11), // "sendMessage"
QT_MOC_LITERAL(4, 42, 11), // "execSecConn"
QT_MOC_LITERAL(5, 54, 10), // "stopSignal"
QT_MOC_LITERAL(6, 65, 12), // "changeDialog"
QT_MOC_LITERAL(7, 78, 12), // "changeWindow"
QT_MOC_LITERAL(8, 91, 10), // "emitSignal"
QT_MOC_LITERAL(9, 102, 14), // "on_execSecConn"
QT_MOC_LITERAL(10, 117, 16), // "slotStartLoading"
QT_MOC_LITERAL(11, 134, 20), // "on_btnCancel_clicked"
QT_MOC_LITERAL(12, 155, 21), // "on_btnConnect_clicked"
QT_MOC_LITERAL(13, 177, 23), // "on_leNetName_textEdited"
QT_MOC_LITERAL(14, 201, 4) // "arg1"

    },
    "DlgConnHidWifi\0reSetWifiList\0\0sendMessage\0"
    "execSecConn\0stopSignal\0changeDialog\0"
    "changeWindow\0emitSignal\0on_execSecConn\0"
    "slotStartLoading\0on_btnCancel_clicked\0"
    "on_btnConnect_clicked\0on_leNetName_textEdited\0"
    "arg1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DlgConnHidWifi[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    0,   76,    2, 0x06 /* Public */,
       5,    0,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   78,    2, 0x0a /* Public */,
       7,    0,   79,    2, 0x0a /* Public */,
       8,    0,   80,    2, 0x0a /* Public */,
       9,    0,   81,    2, 0x0a /* Public */,
      10,    0,   82,    2, 0x0a /* Public */,
      11,    0,   83,    2, 0x08 /* Private */,
      12,    0,   84,    2, 0x08 /* Private */,
      13,    1,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,

       0        // eod
};

void DlgConnHidWifi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DlgConnHidWifi *_t = static_cast<DlgConnHidWifi *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->reSetWifiList(); break;
        case 1: _t->sendMessage(); break;
        case 2: _t->execSecConn(); break;
        case 3: _t->stopSignal(); break;
        case 4: _t->changeDialog(); break;
        case 5: _t->changeWindow(); break;
        case 6: _t->emitSignal(); break;
        case 7: _t->on_execSecConn(); break;
        case 8: _t->slotStartLoading(); break;
        case 9: _t->on_btnCancel_clicked(); break;
        case 10: _t->on_btnConnect_clicked(); break;
        case 11: _t->on_leNetName_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DlgConnHidWifi::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DlgConnHidWifi::reSetWifiList)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DlgConnHidWifi::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DlgConnHidWifi::sendMessage)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DlgConnHidWifi::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DlgConnHidWifi::execSecConn)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (DlgConnHidWifi::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DlgConnHidWifi::stopSignal)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject DlgConnHidWifi::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DlgConnHidWifi.data,
      qt_meta_data_DlgConnHidWifi,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DlgConnHidWifi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DlgConnHidWifi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DlgConnHidWifi.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DlgConnHidWifi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void DlgConnHidWifi::reSetWifiList()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DlgConnHidWifi::sendMessage()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DlgConnHidWifi::execSecConn()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DlgConnHidWifi::stopSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
