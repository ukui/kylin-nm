/****************************************************************************
** Meta object code from reading C++ file 'dlgconnhidwifisecleap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dlgconnhidwifisecleap.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgconnhidwifisecleap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DlgConnHidWifiSecLeap_t {
    QByteArrayData data[11];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DlgConnHidWifiSecLeap_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DlgConnHidWifiSecLeap_t qt_meta_stringdata_DlgConnHidWifiSecLeap = {
    {
QT_MOC_LITERAL(0, 0, 21), // "DlgConnHidWifiSecLeap"
QT_MOC_LITERAL(1, 22, 16), // "changeDialogSecu"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 16), // "changeDialogAuth"
QT_MOC_LITERAL(4, 57, 20), // "on_btnCancel_clicked"
QT_MOC_LITERAL(5, 78, 21), // "on_btnConnect_clicked"
QT_MOC_LITERAL(6, 100, 24), // "on_checkBox_stateChanged"
QT_MOC_LITERAL(7, 125, 4), // "arg1"
QT_MOC_LITERAL(8, 130, 23), // "on_leNetName_textEdited"
QT_MOC_LITERAL(9, 154, 24), // "on_leUserName_textEdited"
QT_MOC_LITERAL(10, 179, 24) // "on_lePassword_textEdited"

    },
    "DlgConnHidWifiSecLeap\0changeDialogSecu\0"
    "\0changeDialogAuth\0on_btnCancel_clicked\0"
    "on_btnConnect_clicked\0on_checkBox_stateChanged\0"
    "arg1\0on_leNetName_textEdited\0"
    "on_leUserName_textEdited\0"
    "on_lePassword_textEdited"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DlgConnHidWifiSecLeap[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    1,   58,    2, 0x08 /* Private */,
       8,    1,   61,    2, 0x08 /* Private */,
       9,    1,   64,    2, 0x08 /* Private */,
      10,    1,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,

       0        // eod
};

void DlgConnHidWifiSecLeap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DlgConnHidWifiSecLeap *_t = static_cast<DlgConnHidWifiSecLeap *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeDialogSecu(); break;
        case 1: _t->changeDialogAuth(); break;
        case 2: _t->on_btnCancel_clicked(); break;
        case 3: _t->on_btnConnect_clicked(); break;
        case 4: _t->on_checkBox_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_leNetName_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->on_leUserName_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->on_lePassword_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject DlgConnHidWifiSecLeap::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DlgConnHidWifiSecLeap.data,
      qt_meta_data_DlgConnHidWifiSecLeap,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DlgConnHidWifiSecLeap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DlgConnHidWifiSecLeap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DlgConnHidWifiSecLeap.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DlgConnHidWifiSecLeap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
