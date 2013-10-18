/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

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

#ifndef SCRIPTING_NODE_STYLE_HPP
#define SCRIPTING_NODE_STYLE_HPP

#include <QObject>
#include "node_style.hpp"

/**
 * \brief wrapper to Node_Style
 */
class Script_Node_Style : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double angle READ cusp_angle WRITE set_cusp_angle )
    Q_PROPERTY(double curve READ handle_length WRITE set_handle_length )
    Q_PROPERTY(double distance READ cusp_distance WRITE set_cusp_distance )
    Q_PROPERTY(QString shape READ cusp_shape WRITE set_cusp_shape )


    Node_Style wrapped;

public:
    explicit Script_Node_Style(Node_Style wrapped, QObject *parent = 0);
    
    double cusp_angle();
    double handle_length();
    double cusp_distance();
    QString cusp_shape();

    void set_cusp_angle( double value );
    void set_handle_length( double value );
    void set_cusp_distance( double value );
    void set_cusp_shape( QString name );


    /**
     * \brief Set all style features at once
     */
    void from_style(Node_Style style);

    /**
     * \brief Disable all customized style features
     */
    Q_INVOKABLE void clear();


    Q_INVOKABLE QString toString() const { return "[node style]"; }

    Node_Style style() const { return wrapped; }

signals:
    void changed(Node_Style before,Node_Style after);
};

#endif // SCRIPTING_NODE_STYLE_HPP
