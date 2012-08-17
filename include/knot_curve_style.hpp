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
#ifndef KNOT_CURVE_STYLE_HPP
#define KNOT_CURVE_STYLE_HPP

#include "node.hpp"
#include <QPainterPath>
#include <QMap>

class knot_curve_style
{
    public:
        virtual void draw_joint ( QPainterPath& path,
                                    Node *node,
                                    TraversalInfo ti,
                                    double threshold_angle,
                                    double handle_length,
                                    double crossing_distance ) = 0;
};

class knot_curve_ogee : public knot_curve_style
{
    public:
        void draw_joint ( QPainterPath& path,
                            Node *node,
                            TraversalInfo ti,
                            double threshold_angle,
                            double handle_length,
                            double crossing_distance );
};

class knot_curve_pointed : public knot_curve_style
{
    public:
        void draw_joint ( QPainterPath& path,
                            Node *node,
                            TraversalInfo ti,
                            double threshold_angle,
                            double handle_length,
                            double crossing_distance );
};

class knot_curve_simple_poly : public knot_curve_style
{
    public:
        void draw_joint ( QPainterPath& path,
                            Node *node,
                            TraversalInfo ti,
                            double threshold_angle,
                            double handle_length,
                            double crossing_distance );
};

class knot_curve_advanced_poly : public knot_curve_style
{
    public:
        void draw_joint ( QPainterPath& path,
                            Node *node,
                            TraversalInfo ti,
                            double threshold_angle,
                            double handle_length,
                            double crossing_distance );
};



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

#endif // KNOT_CURVE_STYLE_HPP
