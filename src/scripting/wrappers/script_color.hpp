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

#ifndef SCRIPT_COLOR_HPP
#define SCRIPT_COLOR_HPP

#include <QObject>
#include <QColor>
#include <QMetaType>
#include <QScriptEngine>


/**
 *  \brief Wrapper to QColor
 */
class Script_Color : public QObject, public QColor
{
    Q_OBJECT

public:
    explicit Script_Color(QColor col = Qt::transparent, QObject *parent = 0);
    Script_Color(const Script_Color &o) :QObject(), QColor(o) {}

    Q_INVOKABLE QString toString() const;

    Q_PROPERTY(int alpha READ alpha WRITE setAlpha_script)
    Q_PROPERTY(int red READ red WRITE setRed)
    Q_PROPERTY(int green READ green WRITE setGreen_script)
    Q_PROPERTY(int blue READ blue WRITE setBlue_script)
        void setAlpha_script( int v );
        void setRed_script( int v );
        void setGreen_script( int v );
        void setBlue_script( int v );

    Q_PROPERTY(int hue READ hue WRITE setHue)
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation)
    Q_PROPERTY(int value READ value WRITE setValue)
        void setHue(int v);
        void setSaturation(int v);
        void setValue(int v);

    Q_PROPERTY(int cyan READ cyan WRITE setCyan)
    Q_PROPERTY(int magenta READ magenta WRITE setMagenta)
    Q_PROPERTY(int yellow READ yellow WRITE setYellow)
    Q_PROPERTY(int black READ black WRITE setBlack)
        void setCyan(int v);
        void setMagenta(int v);
        void setYellow(int v);
        void setBlack(int v);

signals:
        void changed(QColor new_color);
};


Q_DECLARE_METATYPE(Script_Color)


/// Wrapper to color constructors
QScriptValue build_color (QScriptContext *context, QScriptEngine *engine);
/// Convert C++ object to JS object
QScriptValue color_to_script(QScriptEngine *engine, const Script_Color &c);
/// Convert JS object to C++ Object
void color_from_script(const QScriptValue &obj, Script_Color &c);

/// rgb(r,g,b[,a])
QScriptValue script_rgb(QScriptContext *context, QScriptEngine *engine);
/// hsv(h,s,v[,a])
QScriptValue script_hsv(QScriptContext *context, QScriptEngine *engine);
/// hsl(h,s,l[,a])
QScriptValue script_hsl(QScriptContext *context, QScriptEngine *engine);
/// cmyk(c,m,y,k[,a])
QScriptValue script_cmyk(QScriptContext *context, QScriptEngine *engine);

#endif // SCRIPT_COLOR_HPP
