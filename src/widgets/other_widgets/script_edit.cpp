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

#include "script_edit.hpp"
#include <cmath>
#include <QPainter>
#include <QTextBlock>
#include <QFont>
#include "resource_manager.hpp"

Script_Edit::Script_Edit(QWidget *parent) :
    QPlainTextEdit(parent), line_col(new Line_Column_Widget(this)),
    highlighter(new Script_Highlighter(this))
{
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(lines_changed()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(update_request(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlight_current_line()));

    lines_changed();
    highlight_current_line();

    setWordWrapMode(QTextOption::NoWrap);
    QFont msfont = font();
    msfont.setStyleHint(QFont::Monospace);
    msfont.setFamily("monospace");
    setFont(msfont);

    highlighter->setDocument(document());
    highlighter->style_from_file(Resource_Manager::data("style/syntax_highlighter.ini"));
}

void Script_Edit::error_line(int line)
{

    QTextCursor cur = textCursor();
    cur.movePosition(QTextCursor::Start);
    cur.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,line-1);
    setTextCursor( cur );


    QList<QTextEdit::ExtraSelection> extra_selections;

    QTextEdit::ExtraSelection selection;

    selection.format.setBackground ( QColor(255,200,200) );
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = cur;
    selection.cursor.clearSelection();
    extra_selections.append(selection);

    setExtraSelections(extra_selections);
}

void Script_Edit::resizeEvent(QResizeEvent *event)
{

    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    cr.setWidth ( line_col->text_width() );
    line_col->setGeometry ( cr );
}

void Script_Edit::lines_changed()
{
    setViewportMargins(line_col->text_width(), 0, 0, 0);
}

void Script_Edit::highlight_current_line()
{

    QList<QTextEdit::ExtraSelection> extra_selections;

    QTextEdit::ExtraSelection selection;

    selection.format.setBackground ( palette().toolTipBase() );
    //selection.format.setForeground ( palette().toolTipText() );
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extra_selections.append(selection);

    setExtraSelections(extra_selections);
}

void Script_Edit::update_request(const QRect & new_viewport, int scroll_y)
{

    if ( scroll_y )
        // signal specified a scroll delta
        line_col->scroll(0, scroll_y);
    else
        // signal specified a viewport
        line_col->update ( 0, new_viewport.y(), line_col->width(), new_viewport.height() );

    if ( new_viewport.contains ( viewport()->rect() ) )
        lines_changed();
}



int Script_Edit::Line_Column_Widget::text_width() const
{
    double bc = owner->blockCount()+1; // ensure nonzero
    int digits = int(log10(bc))+1;
    return padding + fontMetrics().width('0') * digits;
}

void Script_Edit::Line_Column_Widget::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    painter.fillRect ( event->rect(),
                      palette().color(QPalette::Disabled,QPalette::Window) );

    /// \notice TextBlock = line as seen on the editor ( => wrapped lines? )
    QTextBlock block = owner->firstVisibleBlock();
    int block_index = block.blockNumber();
    int top = owner->blockBoundingGeometry(block).translated(owner->contentOffset()).top();
    int bottom = top + owner->blockBoundingRect(block).height();

    QTextCursor b = owner->textCursor(), e = b;
    b.setPosition(b.selectionStart());
    e.setPosition(e.selectionEnd());
    int low_selected_block = b.blockNumber();
    int high_selected_block = e.blockNumber();

    QFont f = painter.font();
    painter.setPen ( palette().color(QPalette::Disabled,QPalette::Text) );

    while ( block.isValid() && top <= event->rect().bottom() )
    {
        if ( block.isVisible() && bottom >= event->rect().top() )
        {

            if ( block_index >= low_selected_block &&
                 block_index <= high_selected_block )
                f.setWeight(QFont::Bold);
            else
                f.setWeight(QFont::Normal);
            painter.setFont(f);

            painter.drawText ( 0, top, width()-padding, fontMetrics().height(),
                               Qt::AlignRight,
                               QString::number ( block_index + 1 )
                            );
        }

        block = block.next();
        top = bottom;
        bottom = top + owner->blockBoundingRect(block).height();
        ++block_index;
    }
}

void Script_Edit::Line_Column_Widget::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {

        QTextBlock block = owner->firstVisibleBlock();
        int top = owner->blockBoundingGeometry(block).translated(owner->contentOffset()).top();
        int bottom = top + owner->blockBoundingRect(block).height();

        while ( block.isValid() )
        {
            if ( event->y() >= top && event->y() <= bottom )
            {
                QTextCursor cur(owner->document());
                cur.setPosition ( block.position() );
                cur.select(QTextCursor::LineUnderCursor);
                owner->setFocus();
                owner->setTextCursor(cur);
                break;
            }
            block = block.next();
            top = bottom;
            bottom = top + owner->blockBoundingRect(block).height();
        }
    }
}

