#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP
#include "knotview.hpp"

class ClipboardItem
{
    protected:
        struct ClipboardNode
        {
            int     id;
            QPointF pos;
            explicit ClipboardNode ( int id, QPointF pos = QPointF() )
                : id ( id ), pos ( pos ) {}
        };

        struct ClipboardEdge
        {
            int n1;
            int n2;
            explicit ClipboardEdge ( int n1, int n2 )
                : n1 ( n1 ), n2 ( n2 ) {}
            bool operator== ( const ClipboardEdge& e )
                { return n1 == e.n1 && n2 == e.n2; }
        };

        QPointF              center;
        QList<ClipboardNode> nodes;
        QList<ClipboardEdge> edges;

    public:

        explicit ClipboardItem ( node_list nodes = node_list() );

        void paste ( KnotView*, QPointF p ) const;
        void copy ( KnotView* ) ;



};

#endif // CLIPBOARD_HPP
