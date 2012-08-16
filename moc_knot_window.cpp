/****************************************************************************
** Meta object code from reading C++ file 'knot_window.hpp'
**
** Created: Thu Aug 16 14:40:49 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "knot_window.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'knot_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Knot_Window[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x09,
      41,   35,   12,   12, 0x09,
      60,   12,   12,   12, 0x0a,
      77,   12,   12,   12, 0x0a,
      89,   12,   12,   12, 0x0a,
     101,   12,   12,   12, 0x0a,
     122,   12,   12,   12, 0x0a,
     129,   12,   12,   12, 0x0a,
     135,   12,   12,   12, 0x0a,
     143,   12,   12,   12, 0x0a,
     151,   12,   12,   12, 0x0a,
     158,   12,   12,   12, 0x0a,
     167,   12,   12,   12, 0x0a,
     174,   12,   12,   12, 0x0a,
     186,   12,   12,   12, 0x0a,
     200,   12,   12,   12, 0x0a,
     210,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Knot_Window[] = {
    "Knot_Window\0\0update_style_dialog()\0"
    "clean\0update_title(bool)\0mode_edge_list()\0"
    "mode_node()\0mode_edge()\0mouse_moved(QPointF)\0"
    "copy()\0cut()\0paste()\0clear()\0save()\0"
    "saveAs()\0open()\0exportSVG()\0apply_style()\0"
    "zoom_in()\0zoom_out()\0"
};

const QMetaObject Knot_Window::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Knot_Window,
      qt_meta_data_Knot_Window, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Knot_Window::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Knot_Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Knot_Window::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Knot_Window))
        return static_cast<void*>(const_cast< Knot_Window*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Knot_Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: update_style_dialog(); break;
        case 1: update_title((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: mode_edge_list(); break;
        case 3: mode_node(); break;
        case 4: mode_edge(); break;
        case 5: mouse_moved((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 6: copy(); break;
        case 7: cut(); break;
        case 8: paste(); break;
        case 9: clear(); break;
        case 10: save(); break;
        case 11: saveAs(); break;
        case 12: open(); break;
        case 13: exportSVG(); break;
        case 14: apply_style(); break;
        case 15: zoom_in(); break;
        case 16: zoom_out(); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
