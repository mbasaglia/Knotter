/****************************************************************************
** Meta object code from reading C++ file 'knotview.hpp'
**
** Created: Thu Aug 16 21:33:13 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "knotview.hpp"
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
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,    9,    9,    9, 0x0a,
      48,    9,    9,    9, 0x0a,
      67,    9,    9,    9, 0x0a,
      85,    9,    9,    9, 0x0a,
     102,    9,    9,    9, 0x0a,
     118,    9,    9,    9, 0x0a,
     135,    9,    9,    9, 0x0a,
     165,  148,    9,    9, 0x0a,
     178,    9,    9,    9, 0x2a,
     187,    9,    9,    9, 0x0a,
     207,  200,    9,    9, 0x0a,
     220,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KnotView[] = {
    "KnotView\0\0mouse_moved(QPointF)\0"
    "mode_edit_node()\0mode_insert_edge()\0"
    "mode_edge_chain()\0erase_selected()\0"
    "link_selected()\0merge_selected()\0"
    "select_all()\0recalculate_node\0"
    "redraw(bool)\0redraw()\0reset_view()\0"
    "factor\0zoom(double)\0reset_zoom()\0"
};

void KnotView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        KnotView *_t = static_cast<KnotView *>(_o);
        switch (_id) {
        case 0: _t->mouse_moved((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 1: _t->mode_edit_node(); break;
        case 2: _t->mode_insert_edge(); break;
        case 3: _t->mode_edge_chain(); break;
        case 4: _t->erase_selected(); break;
        case 5: _t->link_selected(); break;
        case 6: _t->merge_selected(); break;
        case 7: _t->select_all(); break;
        case 8: _t->redraw((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->redraw(); break;
        case 10: _t->reset_view(); break;
        case 11: _t->zoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->reset_zoom(); break;
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
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
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
