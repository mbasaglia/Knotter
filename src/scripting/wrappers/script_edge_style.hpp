/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#ifndef SCRIPT_EDGE_STYLE_HPP
#define SCRIPT_EDGE_STYLE_HPP

#include <QObject>
#include "edge_style.hpp"
/**
 * \brief wrapper to Node_Style
 */
class Script_Edge_Style : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double curve READ handle_length WRITE set_handle_length )
    Q_PROPERTY(double gap READ crossing_distance WRITE set_crossing_distance )
    Q_PROPERTY(QString type READ edge_type WRITE set_edge_type )
    Q_PROPERTY(double slide READ slide WRITE set_slide )


    Edge_Style wrapped;

public:
    explicit Script_Edge_Style(Edge_Style wrapped=Edge_Style(), QObject *parent = 0);


    double handle_length();
    double crossing_distance();
    double slide();
    QString edge_type();

    void set_crossing_distance( double value );
    void set_handle_length( double value );
    void set_slide ( double value );
    void set_edge_type( QString name );

    /**
     * \brief Set all style features at once
     */
    void from_style(Edge_Style style);

    /**
     * \brief Disable all customized style features
     */
    Q_INVOKABLE void clear();


    Q_INVOKABLE QString toString() const { return "[edge style]"; }

    Edge_Style style() const { return wrapped; }

signals:
    void changed(Edge_Style before,Edge_Style after);

};

#endif // SCRIPT_EDGE_STYLE_HPP
