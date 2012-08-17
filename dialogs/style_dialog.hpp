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
#ifndef STYLE_DIALOG_HPP
#define STYLE_DIALOG_HPP

#include "ui_style_dialog.h"

class StyleDialog : public QDialog, private Ui::StyleDialog
{
        Q_OBJECT
        
    private:
        friend class Knot_Window;
        QMap<knot_curve_styler::style_id,int> combo_style;
        knot_curve_styler::style_id curstyle;

    public:
        explicit StyleDialog(QWidget *parent = 0);

        knot_curve_styler::style_id get_style_id() const;
        void set_style_id( knot_curve_styler::style_id );

        Qt::PenJoinStyle get_join_style() const;
        void set_join_style(Qt::PenJoinStyle pjs);

    public slots:
        void clicked ( QAbstractButton * button );

    signals:
        /// emitted when values have been changed and need to be applied
        void changed();

    private slots:
        void on_knot_width_spinner_valueChanged(double arg1);
        void on_outline_width_spinner_valueChanged(double arg1);
        void on_color_color_changed(const QColor &);
        void on_outline_color_color_changed(const QColor &);
        void on_style_combo_activated(int index);
        void on_cusp_angle_spinner_valueChanged(double arg1);
        void on_handle_length_spinner_valueChanged(double arg1);
        void on_crossing_gap_spinner_valueChanged(double arg1);
        void on_point_combo_activated(int index);
};

#endif // STYLE_DIALOG_HPP
