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

#ifndef SCRIPT_LINE_HPP
#define SCRIPT_LINE_HPP

#include "script_point.hpp"
#include <QLineF>

/**
    Wrapper to QLineF
*/
class Script_Line : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Script_Point p1 READ p1 WRITE setP1 )
    Q_PROPERTY(Script_Point p2 READ p2 WRITE setP2 )

    Q_PROPERTY(double x1 READ x1 WRITE setX1 )
    Q_PROPERTY(double x2 READ x2 WRITE setX2 )

    Q_PROPERTY(double y1 READ y2 WRITE setY2 )
    Q_PROPERTY(double y2 READ y2 WRITE setY2 )

    Q_PROPERTY(double angle READ angle WRITE setAngle)
    Q_PROPERTY(double length READ length WRITE setLength)


    Q_PROPERTY(double dx READ dx)
    Q_PROPERTY(double dy READ dy)

    QLineF line;

    public:

        Script_Line() {}
        Script_Line ( const QLineF& line ) : line(line) {}
        Script_Line ( const Script_Line& o ) : QObject(), line(o.line) {}
        Script_Line ( const Script_Point& p1, const Script_Point& p2 ) : line(p1,p2) {}
        Script_Line& operator= ( const Script_Line&o ) { line = o.line; return *this; }
        operator QLineF() const { return line; }

        Script_Point p1() const { return line.p1(); }
        double x1() const { return line.x1(); }
        double y1() const { return line.y1(); }

        void setP1(const Script_Point& p) { line.setP1(p); }
        void setX1( double x ) { line.setP1(QPointF(x,y1())); }
        void setY1( double y ) { line.setP1(QPointF(x1(),y)); }

        Script_Point p2() const { return line.p2(); }
        double x2() const { return line.x2(); }
        double y2() const { return line.y2(); }

        void setP2(const Script_Point& p) { line.setP2(p); }
        void setX2( double x ) { line.setP2(QPointF(x,y2())); }
        void setY2( double y ) { line.setP2(QPointF(x2(),y)); }

        double angle() const { return line.angle(); }
        double length() const { return line.length(); }
        void setAngle(double a) { line.setAngle(a); }
        void setLength(double l) { line.setLength(l); }

        double dx() const { return line.dx(); }
        double dy() const { return line.dy(); }

        Q_INVOKABLE Script_Point intersect ( const Script_Line& o ) const
        {
            QPointF i;
            line.intersect(o.line,&i);
            return i;
        }

        Q_INVOKABLE Script_Line normalVector() const { return line.normalVector(); }
        Q_INVOKABLE Script_Line unitVector() const { return line.unitVector(); }

        Q_INVOKABLE Script_Point pointAt ( double t ) const { return line.pointAt(t); }

        Q_INVOKABLE void translate(const Script_Point& off) { line.translate(off); }
        Q_INVOKABLE void translate(double x, double y) { line.translate(x,y); }

        Q_INVOKABLE QString toString() const {
            return QString("(%1,%2) -- (%3,%4)").arg(x1()).arg(y1()).arg(x2()).arg(y2());
        }


};

Q_DECLARE_METATYPE(Script_Line)


/// Wrapper to line constructors
QScriptValue build_line (QScriptContext *context, QScriptEngine *engine);
/// Convert C++ object to JS object
QScriptValue line_to_script(QScriptEngine *engine, const Script_Line &l);
/// Convert JS object to C++ Object
void line_from_script(const QScriptValue &obj, Script_Line &l);


#endif // SCRIPT_LINE_HPP
