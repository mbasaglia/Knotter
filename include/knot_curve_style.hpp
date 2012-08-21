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
/**
    \file
    \brief Classes that define and handle knot curve styles
*/
#ifndef KNOT_CURVE_STYLE_HPP
#define KNOT_CURVE_STYLE_HPP

class Node;
class TraversalInfo;
//#include "path_builder.hpp"
#include <QMap>
#include <QPainterPath>

class knot_curve_style;
/**
    \brief slightly dirty class that registers knot curve styles, used to allow extensibility
*/
struct knot_curve_styler
{
    typedef long style_id;

    static knot_curve_style* style ( style_id id );
    static knot_curve_style* style ( QString name );

    /// takes ownership
    static style_id register_style ( knot_curve_style* style );
    static void register_alias ( style_id id, QString name );

    static style_id idof ( QString name );
    static QString name ( style_id id );

    private:
        knot_curve_styler();
        ~knot_curve_styler();
        static knot_curve_styler singleton;
        static QMap<style_id,knot_curve_style*> styles;
        static QMap<QString,style_id> names;
};


struct styleinfo
{
    knot_curve_styler::style_id curve_style;///< Knot shape
    double cusp_angle;                      ///< Min cusp angle
    double handle_length;                   ///< Length of handles (line from start point to control point in SVG curves )
    double crossing_distance;               ///< Crossing gap value
    double cusp_distance;                   ///< Distance from the node to the cusp point
    styleinfo ( knot_curve_styler::style_id curve_style = 0,
               double cusp_angle = 0,
               double handle_length = 0,
               double crossing_distance = 0,
               double cusp_distance = 0 )
        : curve_style(curve_style), cusp_angle(cusp_angle),
            handle_length(handle_length), crossing_distance(crossing_distance),
            cusp_distance ( cusp_distance )
    {}
};

/// Abstract class
class knot_curve_style
{
    public:
        virtual void draw_joint ( QPainterPath& path,
                                    Node *node,
                                    const TraversalInfo& ti,
                                    styleinfo style ) = 0;

        virtual ~knot_curve_style() {}

    protected:
        /**
            \brief get line pointing to the cusp endpoint
            \param start        starning handle line
            \param finish       ending handle line
            \param node         node
            \param ti           traversal info
            \param def_dist     default distance from node (only if delta ~ 360)
        */
        QPointF get_cusp_point ( QLineF start,
                                 QLineF finish,
                                 Node *node,
                                 const TraversalInfo& ti,
                                 double def_dist ) const;
};

class knot_curve_ogee : public knot_curve_style
{
    public:
        void draw_joint ( QPainterPath& path,
                            Node *node,
                            const TraversalInfo& ti,
                            styleinfo style );
};

class knot_curve_pointed : public knot_curve_style
{
    public:
        void draw_joint ( QPainterPath& path,
                            Node *node,
                            const TraversalInfo& ti,
                            styleinfo style );
};

class knot_curve_polygonal : public knot_curve_style
{
    public:
        void draw_joint ( QPainterPath& path,
                            Node *node,
                            const TraversalInfo& ti,
                            styleinfo style );
};




#endif // KNOT_CURVE_STYLE_HPP
