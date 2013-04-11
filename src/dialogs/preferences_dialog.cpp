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

#include "preferences_dialog.hpp"
#include "resource_manager.hpp"

Preferences_Dialog::Preferences_Dialog(QMainWindow *parent) :
    QDialog(parent)
{
    setupUi(this);
    toolbar_editor->setTargetWindow(parent);

    combo_icon_size->addItem(tr("Small (16x16)"),16);
    combo_icon_size->addItem(tr("Medium (22x22)"),22);
    combo_icon_size->addItem(tr("Large (48x48)"),48);
    combo_icon_size->addItem(tr("Huge (64x64)"),64);

    combo_icon_size->setCurrentIndex(combo_icon_size->findData(
                                Resource_Manager::settings.icon_size()));

    combo_icon_style->addItem(tr("Icon Only"),Qt::ToolButtonIconOnly);
    combo_icon_style->addItem(tr("Text Only"),Qt::ToolButtonTextOnly);
    combo_icon_style->addItem(tr("Text Beside Icon"),Qt::ToolButtonTextBesideIcon);
    combo_icon_style->addItem(tr("Text Under Icon"),Qt::ToolButtonTextUnderIcon);
    combo_icon_style->addItem(tr("Follow System Style"),Qt::ToolButtonFollowStyle);

    combo_icon_style->setCurrentIndex(combo_icon_style->findData(
                                Resource_Manager::settings.button_style()));
}

void Preferences_Dialog::set_preferences()
{
    toolbar_editor->apply();
    Resource_Manager::settings.icon_size(combo_icon_size->itemData(
                                    combo_icon_size->currentIndex()).toInt());
    Resource_Manager::settings.button_style( Qt::ToolButtonStyle (
        combo_icon_style->itemData(combo_icon_style->currentIndex()).toInt()) );
}
