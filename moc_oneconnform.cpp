/****************************************************************************
** Meta object code from reading C++ file 'oneconnform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "oneconnform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oneconnform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OneConnForm_t {
    QByteArrayData data[17];
    char stringdata0[252];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OneConnForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OneConnForm_t qt_meta_stringdata_OneConnForm = {
    {
QT_MOC_LITERAL(0, 0, 11), // "OneConnForm"
QT_MOC_LITERAL(1, 12, 19), // "selectedOneWifiForm"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "wifiName"
QT_MOC_LITERAL(4, 42, 8), // "connDone"
QT_MOC_LITERAL(5, 51, 8), // "connFlag"
QT_MOC_LITERAL(6, 60, 17), // "disconnActiveWifi"
QT_MOC_LITERAL(7, 78, 11), // "sigConnWifi"
QT_MOC_LITERAL(8, 90, 14), // "sigConnWifiPWD"
QT_MOC_LITERAL(9, 105, 18), // "on_btnConf_clicked"
QT_MOC_LITERAL(10, 124, 18), // "on_btnConn_clicked"
QT_MOC_LITERAL(11, 143, 21), // "on_btnDisConn_clicked"
QT_MOC_LITERAL(12, 165, 12), // "slotConnWifi"
QT_MOC_LITERAL(13, 178, 15), // "slotConnWifiPWD"
QT_MOC_LITERAL(14, 194, 12), // "slotConnDone"
QT_MOC_LITERAL(15, 207, 21), // "on_btnConnPWD_clicked"
QT_MOC_LITERAL(16, 229, 22) // "on_btnHideConn_clicked"

    },
    "OneConnForm\0selectedOneWifiForm\0\0"
    "wifiName\0connDone\0connFlag\0disconnActiveWifi\0"
    "sigConnWifi\0sigConnWifiPWD\0"
    "on_btnConf_clicked\0on_btnConn_clicked\0"
    "on_btnDisConn_clicked\0slotConnWifi\0"
    "slotConnWifiPWD\0slotConnDone\0"
    "on_btnConnPWD_clicked\0on_btnHideConn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OneConnForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       6,    0,   85,    2, 0x06 /* Public */,
       7,    1,   86,    2, 0x06 /* Public */,
       8,    2,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   94,    2, 0x08 /* Private */,
      10,    0,   95,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    0,   97,    2, 0x08 /* Private */,
      13,    0,   98,    2, 0x08 /* Private */,
      14,    1,   99,    2, 0x08 /* Private */,
      15,    0,  102,    2, 0x08 /* Private */,
      16,    0,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void OneConnForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OneConnForm *_t = static_cast<OneConnForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectedOneWifiForm((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->connDone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->disconnActiveWifi(); break;
        case 3: _t->sigConnWifi((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->sigConnWifiPWD((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->on_btnConf_clicked(); break;
        case 6: _t->on_btnConn_clicked(); break;
        case 7: _t->on_btnDisConn_clicked(); break;
        case 8: _t->slotConnWifi(); break;
        case 9: _t->slotConnWifiPWD(); break;
        case 10: _t->slotConnDone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_btnConnPWD_clicked(); break;
        case 12: _t->on_btnHideConn_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (OneConnForm::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OneConnForm::selectedOneWifiForm)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (OneConnForm::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OneConnForm::connDone)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (OneConnForm::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OneConnForm::disconnActiveWifi)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (OneConnForm::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OneConnForm::sigConnWifi)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (OneConnForm::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OneConnForm::sigConnWifiPWD)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject OneConnForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OneConnForm.data,
      qt_meta_data_OneConnForm,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *OneConnForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OneConnForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OneConnForm.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int OneConnForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void OneConnForm::selectedOneWifiForm(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OneConnForm::connDone(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OneConnForm::disconnActiveWifi()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void OneConnForm::sigConnWifi(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void OneConnForm::sigConnWifiPWD(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
