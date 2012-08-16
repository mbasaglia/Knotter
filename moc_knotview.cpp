/****************************************************************************
** Meta object code from reading C++ file 'knotview.hpp'
**
** Created: Thu Aug 16 14:40:50 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "knotview.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'knotview.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KnotView[] = {

 // content:
       5,       // revision
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

const QMetaObject KnotView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_KnotView,
      qt_meta_data_KnotView, 0 }
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
        switch (_id) {
        case 0: mouse_moved((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 1: mode_edit_node(); break;
        case 2: mode_insert_edge(); break;
        case 3: mode_edge_chain(); break;
        case 4: erase_selected(); break;
        case 5: link_selected(); break;
        case 6: merge_selected(); break;
        case 7: select_all(); break;
        case 8: redraw((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: redraw(); break;
        case 10: reset_view(); break;
        case 11: zoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: reset_zoom(); break;
        default: ;
        }
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
