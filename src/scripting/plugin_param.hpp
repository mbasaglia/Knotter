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

#ifndef PLUGIN_PARAM_HPP
#define PLUGIN_PARAM_HPP

#include <QMap>
#include <QVariant>
#include <QLabel>
#include <QDialog>

/**
    \brief Plugin parameter

    A plugin may specify some parameters that will be presented to the user.
*/
class Plugin_Param
{
    QString m_tooltip;

public:
    /// String representing the type of this parameter
    virtual QString type() const = 0;
    /// Value for the parameter
    virtual QVariant value() const = 0;
    /**
     *  \brief Create a new label describing the parameter
     *
     *  If returns NULL the label will be ignored
     */
    virtual QLabel* create_label() const = 0;

    virtual QWidget* create_widget() const = 0;

    void set_tooltip(QString tooltip ) { m_tooltip = tooltip; }
    QString tooltip() const { return m_tooltip; }

};

/**
 *  \brief Map of plugin parameters
 *
 *  Creates and stores parameters, generates dialogs to present them,
 *  binds such parameters as script values
 */
class Plugin_Params
{
    QMap<QString,Plugin_Param*> params;

    Plugin_Params(const Plugin_Params&);
    Plugin_Params& operator=(const Plugin_Params&);
public:
    ~Plugin_Params();

    QDialog* create_dialog() const;

};

#endif // PLUGIN_PARAM_HPP
