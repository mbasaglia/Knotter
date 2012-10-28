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
#ifndef SCRIPT_WRAPPERS_HPP
#define SCRIPT_WRAPPERS_HPP
#include <QMetaType>
#include <QScriptEngine>
#include "knot_curve_style.hpp"

/**
    Wrapper to QPointF
*/
/*class script_point
{
    Q_OBJECT

    Q_PROPERTY(double x READ get_x WRITE set_x)
    Q_PROPERTY(double y READ get_x WRITE set_x)

    double x, y;

    public:

        script_point ( const QPointF& o ) : x(o.x()), y(o.y()) {}
        script_point ( const script_point&o ) : QObject(), x(o.x), y(o.y) {}
        script_point ( double x, double y ) : x(x), y(y) {}
        script_point () :x(0), y(0) {}
        script_point& operator= ( const script_point&o ) { x = o.x; y = o.y; return *this; }
        operator QPointF() const { return QPointF(x,y); }

        double get_x() const { return x; }
        double get_y() const { return y; }
        void set_x(double x_) { x = x_; }
        void set_y(double y_) { y = y_; }

};*/
typedef QPointF script_point;
Q_DECLARE_METATYPE(script_point)

/**
    Wrapper to QLineF
*/
class script_line : public QObject
{
    Q_OBJECT

    Q_PROPERTY(script_point p1 READ p1 WRITE setP1 )
    Q_PROPERTY(script_point p2 READ p2 WRITE setP2 )

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

        script_line() {}
        script_line ( const QLineF& line ) : line(line) {}
        script_line ( const script_line& o ) : QObject(), line(o.line) {}
        script_line ( const script_point& p1, const script_point& p2 ) : line(p1,p2) {}
        script_line& operator= ( const script_line&o ) { line = o.line; return *this; }
        operator QLineF() const { return line; }

        script_point p1() const { return line.p1(); }
        double x1() const { return line.x1(); }
        double y1() const { return line.y1(); }

        void setP1(const script_point& p) { line.setP1(p); }
        void setX1( double x ) { line.setP1(QPointF(x,y1())); }
        void setY1( double y ) { line.setP1(QPointF(x1(),y)); }

        script_point p2() const { return line.p2(); }
        double x2() const { return line.x2(); }
        double y2() const { return line.y2(); }

        void setP2(const script_point& p) { line.setP2(p); }
        void setX2( double x ) { line.setP2(QPointF(x,y2())); }
        void setY2( double y ) { line.setP2(QPointF(x2(),y)); }

        double angle() const { return line.angle(); }
        double length() const { return line.length(); }
        void setAngle(double a) { line.setAngle(a); }
        void setLength(double l) { line.setLength(l); }

        double dx() const { return line.dx(); }
        double dy() const { return line.dy(); }

        Q_INVOKABLE script_point intersect ( const script_line& o ) const
        {
            QPointF i;
            line.intersect(o.line,&i);
            return i;
        }

        Q_INVOKABLE script_line normalVector() const { return line.normalVector(); }
        Q_INVOKABLE script_line unitVector() const { return line.unitVector(); }

        Q_INVOKABLE script_point pointAt ( double t ) const { return line.pointAt(t); }

        Q_INVOKABLE void translate(const script_point& off) { line.translate(off); }
        Q_INVOKABLE void translate(double x, double y) { line.translate(x,y); }


};

Q_DECLARE_METATYPE(script_line)

//Q_SCRIPT_DECLARE_QMETAOBJECT(script_line,QObject*)

/**
    Wrapper to path_builder
*/
class script_path_builder : public QObject
{
    Q_OBJECT

    protected:
        path_builder* pb;

    public:
        script_path_builder(path_builder* pb);

        Q_INVOKABLE void add_line( script_point begin, script_point end )
        {
            pb->add_line(begin,end);
        }
        Q_INVOKABLE void add_cubic( script_point begin, script_point control1, script_point control2, script_point end )
        {
            pb->add_cubic(begin,control1,control2,end);
        }
        Q_INVOKABLE void add_quad ( script_point begin, script_point control, script_point end )
        {
            pb->add_quad(begin,control,end);
        }

        Q_INVOKABLE void new_group()
        {
            pb->new_group();
        }
};



#endif // SCRIPT_WRAPPERS_HPP
