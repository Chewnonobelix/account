/****************************************************************************
** Meta object code from reading C++ file 'entry.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Account/Model/entry.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'entry.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Entry_t {
    QByteArrayData data[7];
    char stringdata0[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Entry_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Entry_t qt_meta_stringdata_Entry = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Entry"
QT_MOC_LITERAL(1, 6, 2), // "id"
QT_MOC_LITERAL(2, 9, 7), // "account"
QT_MOC_LITERAL(3, 17, 5), // "value"
QT_MOC_LITERAL(4, 23, 4), // "date"
QT_MOC_LITERAL(5, 28, 4), // "type"
QT_MOC_LITERAL(6, 33, 5) // "label"

    },
    "Entry\0id\0account\0value\0date\0type\0label"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Entry[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       6,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       4,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Int, 0x00095001,
       2, QMetaType::QString, 0x00095001,
       3, QMetaType::Double, 0x00095001,
       4, QMetaType::QDate, 0x00095001,
       5, QMetaType::QString, 0x00095001,
       6, QMetaType::QString, 0x00095001,

       0        // eod
};

void Entry::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        Entry *_t = reinterpret_cast<Entry *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->id(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->account(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->value(); break;
        case 3: *reinterpret_cast< QDate*>(_v) = _t->date(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->type(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->label(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Entry::staticMetaObject = {
    { nullptr, qt_meta_stringdata_Entry.data,
      qt_meta_data_Entry,  qt_static_metacall, nullptr, nullptr}
};

QT_WARNING_POP
QT_END_MOC_NAMESPACE
