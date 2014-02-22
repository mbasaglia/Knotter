/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#include "dialog_preferences.hpp"
#include "resource_manager.hpp"
#include <QStyleFactory>
#include <QMessageBox>

Dialog_Preferences::Dialog_Preferences(QMainWindow *parent) :
    QDialog(parent)
{
    setupUi(this);
    toolbar_editor->setTargetWindow(parent);

    init_combos();


    combo_language->addItems(resource_manager().available_languages());
    combo_language->setCurrentIndex ( combo_language->findText(resource_manager().current_lang_name()) );

    connect(Resource_Manager::pointer(),SIGNAL(language_changed()),this,SLOT(retranslate()));

    check_fluid_refresh->setChecked(resource_manager().settings.fluid_refresh());
    check_cache_image->setChecked(resource_manager().settings.graph_cache());
    check_antialiasing->setChecked(resource_manager().settings.antialiasing());
    spin_timeout->setValue(resource_manager().settings.script_timeout());

    spin_recent_files->setValue(resource_manager().settings.max_recent_files());
    check_save_geometry->setChecked(resource_manager().settings.save_ui());

    check_save_grid->setChecked(resource_manager().settings.save_grid());
    check_save_toolbars->setChecked(resource_manager().settings.save_toolbars());
    check_save_style->setChecked(resource_manager().settings.save_knot_style());

    check_files_close->setChecked(resource_manager().settings.check_unsaved_files());

    check_clipboard_png->setChecked(resource_manager().settings.clipboard_feature(Settings::PNG));
    check_clipboard_tiff->setChecked(resource_manager().settings.clipboard_feature(Settings::TIFF));
    check_clipboard_svg->setChecked(resource_manager().settings.clipboard_feature(Settings::SVG));
    check_clipboard_xml->setChecked(resource_manager().settings.clipboard_feature(Settings::XML));

    stackedWidget->setCurrentIndex(0);
    tableWidget->setCurrentCell(0,0);

    foreach(QString k, QStyleFactory::keys() )
        combo_widget_style->addItem(k);
        combo_widget_style->setCurrentIndex(combo_widget_style->findText(
            QApplication::style()->objectName(),
            Qt::MatchFixedString
        ));

    color_node_highlighted->setColor(Node::color_highlighted);
    color_node_resting->setColor(Node::color_resting);
    color_node_selected->setColor(Node::color_selected);

    color_edge_highlighted->setColor(Edge::color_highlighted);
    color_edge_resting->setColor(Edge::color_resting);
    color_edge_selected->setColor(Edge::color_selected);

    color_grid_lines->setColor(Snapping_Grid::line_color);
}

void Dialog_Preferences::init_combos()
{
    combo_icon_size->clear();
    combo_icon_size->addItem(tr("Small (16x16)"),16);
    combo_icon_size->addItem(tr("Medium (22x22)"),22);
    combo_icon_size->addItem(tr("Large (48x48)"),48);
    combo_icon_size->addItem(tr("Huge (64x64)"),64);

    combo_icon_size->setCurrentIndex(combo_icon_size->findData(
                                resource_manager().settings.icon_size()));

    combo_icon_style->clear();
    combo_icon_style->addItem(tr("Icon Only"),Qt::ToolButtonIconOnly);
    combo_icon_style->addItem(tr("Text Only"),Qt::ToolButtonTextOnly);
    combo_icon_style->addItem(tr("Text Beside Icon"),Qt::ToolButtonTextBesideIcon);
    combo_icon_style->addItem(tr("Text Under Icon"),Qt::ToolButtonTextUnderIcon);
    combo_icon_style->addItem(tr("Follow System Style"),Qt::ToolButtonFollowStyle);

    combo_icon_style->setCurrentIndex(combo_icon_style->findData(
                                resource_manager().settings.button_style()));


    combo_node_size->clear();
    combo_node_size->addItem(tr("Small"),3);
    combo_node_size->addItem(tr("Medium"),5);
    combo_node_size->addItem(tr("Large"),15);
    combo_node_size->setCurrentIndex(combo_node_size->findData(Node::radius));


}

void Dialog_Preferences::set_preferences()
{
    toolbar_editor->apply();
    resource_manager().settings.icon_size(combo_icon_size->itemData(
                                    combo_icon_size->currentIndex()).toInt());
    resource_manager().settings.button_style( Qt::ToolButtonStyle (
        combo_icon_style->itemData(combo_icon_style->currentIndex()).toInt()) );


    Node::radius = combo_node_size->itemData(combo_node_size->currentIndex()).toInt();
    Node::color_resting = color_node_resting->color();
    Node::color_highlighted = color_node_highlighted->color();
    Node::color_selected = color_node_selected->color();

    Edge::color_highlighted = color_edge_highlighted->color();
    Edge::color_resting = color_edge_resting->color();
    Edge::color_selected = color_edge_selected->color();

    Snapping_Grid::line_color = color_grid_lines->color();

    QApplication::setStyle(combo_widget_style->currentText());
    combo_widget_style->blockSignals(true); //< prevent retranslate to mess up things


    resource_manager().settings.set_fluid_refresh(check_fluid_refresh->isChecked());
    resource_manager().settings.set_graph_cache(check_cache_image->isChecked());
    resource_manager().settings.set_antialiasing(check_antialiasing->isChecked());
    resource_manager().settings.set_script_timeout(spin_timeout->value());

    resource_manager().settings.set_max_recent_files(spin_recent_files->value());
    resource_manager().settings.set_save_ui(check_save_geometry->isChecked());

    resource_manager().settings.set_save_grid(check_save_grid->isChecked());
    resource_manager().settings.set_save_toolbars(check_save_toolbars->isChecked());
    resource_manager().settings.set_save_knot_style(check_save_style->isChecked());

    resource_manager().settings.set_check_unsaved_files(check_files_close->isChecked());

    resource_manager().settings.set_clipboard_feature(Settings::PNG,check_clipboard_png->isChecked());
    resource_manager().settings.set_clipboard_feature(Settings::TIFF,check_clipboard_tiff->isChecked());
    resource_manager().settings.set_clipboard_feature(Settings::SVG,check_clipboard_svg->isChecked());
    resource_manager().settings.set_clipboard_feature(Settings::XML,check_clipboard_xml->isChecked());

    // keep as last, it will change the combo boxes
    resource_manager().change_lang_name(combo_language->currentText());
}

void Dialog_Preferences::retranslate()
{
    retranslateUi(this);

    init_combos();
}



void Dialog_Preferences::on_button_clear_recent_clicked()
{
    resource_manager().settings.clear_recent_files();
}

void Dialog_Preferences::on_button_clear_settings_clicked()
{
    resource_manager().settings.clear_config();
    QMessageBox::information(this,tr("Clearing Configuration"),
        tr("Next time %1 will start with the default settings.")
        .arg(resource_manager().program.name())
    );
    //group_save->setEnabled(false);
}

void Dialog_Preferences::on_combo_widget_style_currentIndexChanged(int)
{
    QStyle* old_style = group_style_preview->style();
    QStyle* style = QStyleFactory::create(combo_widget_style->currentText());
    group_style_preview->setStyle(style);
    foreach(QWidget* c, group_style_preview->findChildren<QWidget*>() )
    {
        c->setStyle(style);
    }
    if ( old_style != QApplication::style() )
        delete old_style;
}
