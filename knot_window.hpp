#ifndef KNOT_WINDOW_HPP
#define KNOT_WINDOW_HPP
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
#include "ui_knot_window.h"
#include <QUndoStack>
#include <QUndoView>
#include "dialogs/style_dialog.hpp"
#include "graphics/clipboard.hpp"

class Knot_Window : public QMainWindow, private Ui::Knot_Window
{
        Q_OBJECT

    protected:
        QString         filename;
        QUndoView       undoView;
        StyleDialog     style_dialog;
        ClipboardItem   clipboard;

    public:
        explicit Knot_Window(QWidget *parent = 0);

    protected:
        void save ( QString file );

    protected slots:
        /// set dialog values from canvas settings
        void update_style_dialog();

        void update_title(bool clean);

    public slots:
        void mode_edge_list();
        void mode_node();
        void mode_edge();

        void mouse_moved(QPointF);

        void copy();
        void cut();
        void paste();

        void clear();

        void save();
        void saveAs();
        void open();
        void exportSVG();

        /// set canvas style based on style dialog
        void apply_style();

        void zoom_in();
        void zoom_out();

        void configure_grid();

        void enable_grid ( bool enabled );
};

#endif // KNOT_WINDOW_HPP
