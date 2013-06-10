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

#ifndef SCRIPT_EDIT_HPP
#define SCRIPT_EDIT_HPP

#include <QPlainTextEdit>
#include "script_highlighter.hpp"

class Script_Edit : public QPlainTextEdit
{
    Q_OBJECT

private:
    class Line_Column_Widget : public QWidget
    {
        public:
            Script_Edit* owner;
            int padding;

            Line_Column_Widget( Script_Edit* parent )
                : QWidget ( parent ), owner(parent), padding(3) {}

            int text_width() const;

            QSize sizeHint() const
            {
                return QSize ( text_width(), 0 );
            }

        protected:
             void paintEvent(QPaintEvent *event);
             void mousePressEvent ( QMouseEvent * event );

    };

    Line_Column_Widget *line_col;
    Script_Highlighter *highlighter;
public:
    explicit Script_Edit(QWidget *parent = 0);

    /**
     *  \brief Move cursor to line n and marks it as error
     */
    void error_line(int line);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void lines_changed();
    void highlight_current_line();
    void update_request(const QRect &new_viewport, int scroll_y);

signals:
    void sizeChanged(QSize);

    
};

#endif // SCRIPT_EDIT_HPP
