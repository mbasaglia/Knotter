/****************************************************************************
** Meta object code from reading C++ file 'style_dialog.hpp'
**
** Created: Sat Aug 18 00:11:05 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialogs/style_dialog.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'style_dialog.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StyleDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   23,   12,   12, 0x0a,
      61,   56,   12,   12, 0x08,
     104,   56,   12,   12, 0x08,
     150,   12,   12,   12, 0x08,
     181,   12,   12,   12, 0x08,
     226,  220,   12,   12, 0x08,
     256,   56,   12,   12, 0x08,
     299,   56,   12,   12, 0x08,
     345,   56,   12,   12, 0x08,
     390,  220,   12,   12, 0x08,
     420,  220,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_StyleDialog[] = {
    "StyleDialog\0\0changed()\0button\0"
    "clicked(QAbstractButton*)\0arg1\0"
    "on_knot_width_spinner_valueChanged(double)\0"
    "on_outline_width_spinner_valueChanged(double)\0"
    "on_color_color_changed(QColor)\0"
    "on_outline_color_color_changed(QColor)\0"
    "index\0on_style_combo_activated(int)\0"
    "on_cusp_angle_spinner_valueChanged(double)\0"
    "on_handle_length_spinner_valueChanged(double)\0"
    "on_crossing_gap_spinner_valueChanged(double)\0"
    "on_point_combo_activated(int)\0"
    "on_outline_pattern_combo_activated(int)\0"
};

void StyleDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        StyleDialog *_t = static_cast<StyleDialog *>(_o);
        switch (_id) {
        case 0: _t->changed(); break;
        case 1: _t->clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 2: _t->on_knot_width_spinner_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->on_outline_width_spinner_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->on_color_color_changed((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 5: _t->on_outline_color_color_changed((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 6: _t->on_style_combo_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_cusp_angle_spinner_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->on_handle_length_spinner_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->on_crossing_gap_spinner_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->on_point_combo_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_outline_pattern_combo_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData StyleDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StyleDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_StyleDialog,
      qt_meta_data_StyleDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StyleDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StyleDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StyleDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StyleDialog))
        return static_cast<void*>(const_cast< StyleDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int StyleDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void StyleDialog::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
