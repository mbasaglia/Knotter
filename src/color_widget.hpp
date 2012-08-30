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

/**
    \brief Widget to select and preview colors

    Too bad Qt doesn't provide one by default.

    Used QLineEdit as base because it's easy to change its background and keep nice formatting.

    This class is short enough so it's kept in a header with no source file.

*/
class ColorWidget : public QLineEdit
{
        Q_OBJECT

    protected:
        QColorDialog color_dialog;
        QColor clr;
    public:
        ColorWidget ( QWidget* parent = NULL ) : QLineEdit(parent)
        {
            setReadOnly(true);
            connect(&color_dialog,SIGNAL(currentColorChanged(QColor)),
                            SLOT(update_color(QColor)) );
            connect(&color_dialog,SIGNAL(colorSelected(QColor)),
                            SLOT(update_color(QColor)) );
            //connect(this,SIGNAL(editingFinished()),SLOT(update_from_text()));
            connect(&color_dialog,SIGNAL(finished(int)),SIGNAL(dialog_closed()));
            color_dialog.setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::NoButtons);
            update_color(color());
        }

        QColor color() const
        {
            return clr;//color_dialog.currentColor();
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
            clr = c;
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
        /// emitted each time the user alters the color shown in the dialog
        void color_changed ( QColor );
        /// emitted when the user closes the dialog
        void dialog_closed();

};

#endif // COLOR_WIDGET_HPP
