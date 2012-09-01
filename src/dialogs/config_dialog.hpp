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
#ifndef CONFIG_DIALOG_HPP
#define CONFIG_DIALOG_HPP
#include <QToolBar>
#include <QMenu>

/*
    Sorry for this hack but I'm not willing to get too much overhead just to change a few options
*/
#include <QTableWidget>
struct mytablewidget : QTableWidget
{
    mytablewidget ( QWidget* parent ) : QTableWidget ( parent ) {}
    protected:
        QStyleOptionViewItem viewOptions() const
        {
            QStyleOptionViewItem option = QTableWidget::viewOptions();
            option.decorationAlignment = Qt::AlignHCenter | Qt::AlignCenter;
            option.decorationPosition = QStyleOptionViewItem::Top;
            option.displayAlignment = Qt::AlignCenter;
            return option;
        }

};
#define QTableWidget mytablewidget

#include "ui_config_dialog.h"

#undef QTableWidget

class config_dialog : public QDialog, private Ui::config_dialog
{
        Q_OBJECT

        friend class Knot_Window;
        QList<QToolBar*> toolbars;
        QList<QMenu*> menus;
        QMenu* current_menu;
        QToolBar* current_toolbar;
        bool save_anything;
    public:
        explicit config_dialog(QWidget *parent = 0);

        void set_icon_size ( QSize size );
        QSize get_icon_size() const;

        Qt::ToolButtonStyle get_tool_button_style() const;
        void set_tool_button_style ( Qt::ToolButtonStyle style );

        void set_menu(QMenu* menu);
        void set_toolbar(QToolBar* tb);

        void add_menu(QAction *menu);
        void add_toolbar(QToolBar* tb);

        void select_current_language();


    private slots:
        void on_menu_combo_activated(const QString &arg1);
        void on_toolbar_combo_activated(const QString &arg1);
        void on_butt_move_up_clicked();
        void on_butt_move_down_clicked();
        void on_butt_insert_clicked();
        void on_butt_separator_clicked();
        void on_butt_remove_clicked();
        void on_add_toolbar_btn_clicked();

        void on_rm_toolbar_btn_clicked();

        void retranslate();

        void on_clear_settings_clicked();

    signals:
        void insert_toolbar(QToolBar*);
};

#endif // CONFIG_DIALOG_HPP
