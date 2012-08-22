/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef XML_SAVER_HPP
#define XML_SAVER_HPP
#include "knot_curve_style.hpp"
#include "knotview.hpp"
#include <QXmlStreamWriter>
#include <QDomDocument>
class xml_saver
{
    public:
        static const int version = 2;

    protected:
        QXmlStreamWriter xml;
        QMap<Node*,int> node_ids;

    public:
        xml_saver(QIODevice* output);

        void begin ();
        void end();

        void start_element ( QString name );
        void end_element ();

        void save_pen ( QString name, QPen pen, bool style, bool join_style );

        void save_cusp ( QString name, styleinfo style );

        void save_graph ( QString name, node_list nodes );

        void save_node ( Node* node );

        int register_node ( Node* );

};

#include <stack>

class xml_loader
{
        QDomDocument xml;
        std::stack<QDomElement> current_elements;
        int version;
        QMap<QString,Node*> node_ids;
    public:

        bool load ( QIODevice* input );

        void get_graph ( KnotView *view );

        Node* register_node ( QString id );

        bool enter( QString name);

        QPen get_pen ( QString name, QPen default_value );

        styleinfo get_cusp ( QString name, styleinfo default_value );

        void leave();
};

#endif // XML_SAVER_HPP
