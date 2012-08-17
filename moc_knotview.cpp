/****************************************************************************
** Meta object code from reading C++ file 'knotview.hpp'
**
** Created: Fri Aug 17 17:28:01 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "graphics/knotview.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'knotview.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KnotView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,    9,    9,    9, 0x0a,
      53,    9,    9,    9, 0x0a,
      71,    9,    9,    9, 0x0a,
      88,    9,    9,    9, 0x0a,
     106,    9,    9,    9, 0x0a,
     123,    9,    9,    9, 0x0a,
     139,    9,    9,    9, 0x0a,
     156,    9,    9,    9, 0x0a,
     186,  169,    9,    9, 0x0a,
     199,    9,    9,    9, 0x2a,
     208,    9,    9,    9, 0x0a,
     228,  221,    9,    9, 0x0a,
     241,    9,    9,    9, 0x0a,
     254,    9,    9,    9, 0x0a,
     272,    9,    9,    9, 0x0a,
     296,  288,    9,    9, 0x0a,
     318,  288,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KnotView[] = {
    "KnotView\0\0mouse_moved(QPointF)\0"
    "mode_edit_node_edge()\0mode_edge_chain()\0"
    "mode_move_grid()\0mode_moving_new()\0"
    "erase_selected()\0link_selected()\0"
    "merge_selected()\0select_all()\0"
    "recalculate_node\0redraw(bool)\0redraw()\0"
    "reset_view()\0factor\0zoom(double)\0"
    "reset_zoom()\0flip_horizontal()\0"
    "flip_vertical()\0visible\0toggle_knotline(bool)\0"
    "toggle_graph(bool)\0"
};

void KnotView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        KnotView *_t = static_cast<KnotView *>(_o);
        switch (_id) {
        case 0: _t->mouse_moved((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 1: _t->mode_edit_node_edge(); break;
        case 2: _t->mode_edge_chain(); break;
        case 3: _t->mode_move_grid(); break;
        case 4: _t->mode_moving_new(); break;
        case 5: _t->erase_selected(); break;
        case 6: _t->link_selected(); break;
        case 7: _t->merge_selected(); break;
        case 8: _t->select_all(); break;
        case 9: _t->redraw((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->redraw(); break;
        case 11: _t->reset_view(); break;
        case 12: _t->zoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->reset_zoom(); break;
        case 14: _t->flip_horizontal(); break;
        case 15: _t->flip_vertical(); break;
        case 16: _t->toggle_knotline((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->toggle_graph((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData KnotView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject KnotView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_KnotView,
      qt_meta_data_KnotView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KnotView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KnotView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KnotView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KnotView))
        return static_cast<void*>(const_cast< KnotView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int KnotView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void KnotView::mouse_moved(QPointF _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
