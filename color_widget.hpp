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
#ifndef COLOR_WIDGET_HPP
#define COLOR_WIDGET_HPP
#include <QLineEdit>
#include <QColorDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOptionFrameV3>

class ColorWidget : public QLineEdit
{
        Q_OBJECT

    protected:
        QColorDialog color_dialog;

    public:
        ColorWidget ( QWidget* parent = NULL ) : QLineEdit(parent)
        {
            setReadOnly(true);
            this->connect(&color_dialog,SIGNAL(currentColorChanged(QColor)),
                            SLOT(update_color(QColor)) );
            this->connect(&color_dialog,SIGNAL(colorSelected(QColor)),
                            SLOT(update_color(QColor)) );
            //this->connect(this,SIGNAL(editingFinished()),SLOT(update_from_text()));

            update_color(color());
        }

        QColor color() const
        {
            return color_dialog.currentColor();
        }
        void set_color( QColor c)
        {
            color_dialog.setCurrentColor(c);
        }




    protected:
        void mouseReleaseEvent(QMouseEvent * )
        {
            color_dialog.show();
        }


    protected slots:
        void update_color(QColor c)
        {
            QPalette pal = palette();
            pal.setColor(QPalette::Base, c);
            /*QColor complement;//(255-c.red(),255-c.green(),255-c.blue()) ;
            complement.setHsv(360-c.hue(),255-c.saturation(),255-c.value()) ;
            pal.setColor(QPalette::Text, complement);
            setText(c.name());*/
            setPalette(pal);

            update();
            emit color_changed(c);
        }


        /*void update_from_text()
        {
            QColor c(text());
            if ( c.isValid() )
                set_color(c);
        }*/

    signals:
        void color_changed ( QColor );

};

#endif // COLOR_WIDGET_HPP
