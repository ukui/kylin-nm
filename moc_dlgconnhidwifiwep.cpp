/****************************************************************************
** Meta object code from reading C++ file 'dlgconnhidwifiwep.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dlgconnhidwifiwep.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgconnhidwifiwep.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DlgConnHidWifiWep_t {
    QByteArrayData data[9];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DlgConnHidWifiWep_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DlgConnHidWifiWep_t qt_meta_stringdata_DlgConnHidWifiWep = {
    {
QT_MOC_LITERAL(0, 0, 17), // "DlgConnHidWifiWep"
QT_MOC_LITERAL(1, 18, 12), // "changeDialog"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 20), // "on_btnCancel_clicked"
QT_MOC_LITERAL(4, 53, 21), // "on_btnConnect_clicked"
QT_MOC_LITERAL(5, 75, 27), // "on_checkBoxPwd_stateChanged"
QT_MOC_LITERAL(6, 103, 4), // "arg1"
QT_MOC_LITERAL(7, 108, 19), // "on_leKey_textEdited"
QT_MOC_LITERAL(8, 128, 23) // "on_leNetName_textEdited"

    },
    "DlgConnHidWifiWep\0changeDialog\0\0"
    "on_btnCancel_clicked\0on_btnConnect_clicked\0"
    "on_checkBoxPwd_stateChanged\0arg1\0"
    "on_leKey_textEdited\0on_leNetName_textEdited"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DlgConnHidWifiWep[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    1,   47,    2, 0x08 /* Private */,
       7,    1,   50,    2, 0x08 /* Private */,
       8,    1,   53,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void DlgConnHidWifiWep::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DlgConnHidWifiWep *_t = static_cast<DlgConnHidWifiWep *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeDialog(); break;
        case 1: _t->on_btnCancel_clicked(); break;
        case 2: _t->on_btnConnect_clicked(); break;
        case 3: _t->on_checkBoxPwd_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_leKey_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->on_leNetName_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject DlgConnHidWifiWep::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DlgConnHidWifiWep.data,
      qt_meta_data_DlgConnHidWifiWep,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DlgConnHidWifiWep::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DlgConnHidWifiWep::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DlgConnHidWifiWep.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DlgConnHidWifiWep::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
