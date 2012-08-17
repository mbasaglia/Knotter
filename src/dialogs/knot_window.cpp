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
#include "knot_window.hpp"
#include "commands.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include "gridconfig.hpp"
#include "error_recovery.hpp"

Knot_Window::Knot_Window(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    ErrorRecovery::recover = canvas;

    action_New->setIcon(QIcon::fromTheme("document-new"));
    action_New->setShortcuts(QKeySequence::New);
    action_Save->setIcon(QIcon::fromTheme("document-save"));
    action_Save->setShortcuts(QKeySequence::Save);
    actionSave_As->setIcon(QIcon::fromTheme("document-save-as"));
    actionSave_As->setShortcuts(QKeySequence::SaveAs);
    action_Open->setIcon(QIcon::fromTheme("document-open"));
    action_Open->setShortcuts(QKeySequence::Open);
    action_Quit->setIcon(QIcon::fromTheme("application-exit"));
    action_Quit->setShortcuts(QKeySequence::Quit);
    action_Export->setIcon(QIcon::fromTheme("image-x-generic"));


    QAction *undos = canvas->get_undo_stack().createUndoAction(this, tr("&Undo"));
    menu_Edit->insertAction(action_Undo,undos);
    MainToolBar->insertAction(action_Undo,undos);
    delete action_Undo;
    action_Undo = undos;
    QAction *redos =  canvas->get_undo_stack().createRedoAction(this, tr("&Redo"));
    menu_Edit->insertAction(action_Redo,redos);
    MainToolBar->insertAction(action_Redo,redos);
    delete action_Redo;
    action_Redo = redos;

    action_Copy->setIcon(QIcon::fromTheme("edit-copy"));
    action_Copy->setShortcuts(QKeySequence::Copy);
    action_Paste->setIcon(QIcon::fromTheme("edit-paste"));
    action_Paste->setShortcuts(QKeySequence::Paste);
    actionCut->setIcon(QIcon::fromTheme("edit-cut"));
    actionCut->setShortcuts(QKeySequence::Cut);

    action_Undo->setIcon(QIcon::fromTheme("edit-undo"));
    action_Undo->setShortcuts(QKeySequence::Undo);
    action_Redo->setIcon(QIcon::fromTheme("edit-redo"));
    action_Redo->setShortcuts(QKeySequence::Redo);
    actionSelect_All->setIcon(QIcon::fromTheme("edit-select-all"));
    actionSelect_All->setShortcuts(QKeySequence::SelectAll);


    action_Erase->setIcon(QIcon::fromTheme("edit-delete"));
    //action_Link->setIcon(QIcon::fromTheme("insert-link"));


    actionZoom_In->setIcon(QIcon::fromTheme("zoom-in"));
    actionZoom_In->setShortcuts(QKeySequence::ZoomIn);
    actionZoom_Out->setIcon(QIcon::fromTheme("zoom-out"));
    actionZoom_Out->setShortcuts(QKeySequence::ZoomOut);
    action_Reset_Zoom->setIcon(QIcon::fromTheme("zoom-original"));
    actionRefresh_Path->setIcon(QIcon::fromTheme("view-refresh"));
    actionRefresh_Path->setShortcuts(QKeySequence::Refresh);
    action_Reset_View->setIcon(QIcon::fromTheme("view-restore"));

    action_Style->setIcon(QIcon::fromTheme("preferences-other"));


    // overcome bug in code generator from ui file
    removeToolBar(EditBar);
    removeToolBar(MainToolBar);
    addToolBar(Qt::TopToolBarArea, MainToolBar);
    addToolBar(Qt::TopToolBarArea, EditBar);
    MainToolBar->show();
    EditBar->show();


    this->connect(&canvas->get_undo_stack(),SIGNAL(cleanChanged(bool)),SLOT(update_title(bool)));
    undoView.setStack(&canvas->get_undo_stack());
    undoView.setWindowTitle("Action History");
    undoView.show();
    undoView.setAttribute(Qt::WA_QuitOnClose, false);

    style_dialog.connect(action_Style,SIGNAL(triggered()),SLOT(show()));
    style_dialog.connect(action_Style,SIGNAL(triggered()),SLOT(raise()));
    this->connect(action_Style,SIGNAL(triggered()),SLOT(update_style_dialog()));
    style_dialog.setWindowIcon(QIcon::fromTheme("preferences-other"));

    style_dialog.setAttribute(Qt::WA_QuitOnClose, false);
    update_style_dialog();
    this->connect(&style_dialog,SIGNAL(changed()),SLOT(apply_style()));
}


void Knot_Window::mode_edge_list()
{
    actionInsert_Edges->setChecked(false);
    actionEdge_List->setChecked(true);
    canvas->mode_edge_chain();
    style_dialog.connect(action_Style,SIGNAL(triggered()),SLOT(setFocus()));
}

void Knot_Window::mode_edge()
{
    actionInsert_Edges->setChecked(true);
    actionEdge_List->setChecked(false);
    canvas->mode_edit_node_edge();
}

void Knot_Window::mouse_moved(QPointF p)
{
    statusBar()->showMessage(QString("%1,%2").arg(p.x()).arg(p.y()));
}

void Knot_Window::copy()
{
    clipboard.copy ( canvas );
}

void Knot_Window::cut()
{
    canvas->get_undo_stack().beginMacro(tr("Cut"));

    clipboard.copy ( canvas );

    node_list sel = canvas->selected_nodes();
    foreach ( Node* n, sel )
        canvas->remove_node(n);

    canvas->get_undo_stack().endMacro();
}

void Knot_Window::paste()
{
    clipboard.paste ( canvas, canvas->mapToScene(
                                canvas->mapFromGlobal(QCursor::pos()) ) );
}

void Knot_Window::clear()
{
    canvas->get_undo_stack().beginMacro(tr("Clear Document"));
    filename = "";
    canvas->select_all();
    canvas->erase_selected();
    canvas->get_undo_stack().endMacro();
}


void Knot_Window::save(QString file)
{
    QFile quf(file);
    if ( ! quf.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not write to \"%1\".").arg(filename));
        return;
    }

    canvas->writeXML(&quf);
    canvas->get_undo_stack().setClean();

    quf.close();
}


void Knot_Window::save()
{
    if ( filename == "" )
        return saveAs();
    save(filename);
}

void Knot_Window::saveAs()
{
    QString fn = QFileDialog::getSaveFileName(this,tr("Save Knot"),filename,
                "Knot files (*.knot);;XML files (*.xml);;All files (*)" );
    if ( fn.isNull() )
        return;
    filename = fn;
    save(filename);
}

void Knot_Window::open()
{
    QString fn = QFileDialog::getOpenFileName(this,tr("Open Knot"),filename,
                "Knot files (*.knot);;XML files (*.xml);;All files (*)" );
    if ( fn.isNull() )
        return;
    filename = fn;
    QFile quf(filename);
    if ( ! quf.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not read \"%1\".").arg(filename));
        return;
    }
    canvas->clear();
    canvas->get_undo_stack().setClean();
    if ( !canvas->readXML(&quf) )
        QMessageBox::warning(this,tr("File Error"),tr("Error while reading \"%1\".").arg(filename));

    update_style_dialog();

}

void Knot_Window::exportSVG()
{
    QString exname = QFileDialog::getSaveFileName(this,tr("Export Knot as SVG"),filename,
                "SVG Images (*.svg);;XML files (*.xml);;All files (*)" );
    QFile quf(exname);
    if ( ! quf.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not write to \"%1\".").arg(exname));
        return;
    }
    canvas->writeSVG ( &quf );
    quf.close();
}

void Knot_Window::zoom_in()
{
    canvas->zoom(2);
}

void Knot_Window::zoom_out()
{
    canvas->zoom(0.5);
}

void Knot_Window::configure_grid()
{
    snapping_grid &grid = canvas->get_grid();
    GridConfig(&grid,this).exec();

    actionEnable_Grid->setChecked(grid.is_enabled());
    if ( grid.get_shape() == snapping_grid::TRIANGLE1 )
        actionEnable_Grid->setIcon(QIcon(":/img/triangular_grid.svg"));
    else if ( grid.get_shape() == snapping_grid::TRIANGLE2 )
        actionEnable_Grid->setIcon(QIcon(":/img/triangular_grid2.svg"));
    else
        actionEnable_Grid->setIcon(QIcon(":/img/square_grid.svg"));
    canvas->redraw(false);

}

void Knot_Window::enable_grid(bool enabled)
{
    canvas->get_grid().enable ( enabled );
    canvas->redraw(false);
}


void Knot_Window::apply_style()
{
    canvas->set_width ( style_dialog.knot_width_spinner->value() );
    canvas->set_brush ( QBrush ( style_dialog.color->color() ) );
    canvas->set_pen ( QPen ( style_dialog.outline_color->color(),
                             style_dialog.outline_width_spinner->value(),
                             style_dialog.get_pen_style()
                            ) );
    canvas->set_curve_style ( style_dialog.get_style_id() );
    canvas->set_cusp_angle ( style_dialog.cusp_angle_spinner->value() );
    canvas->set_handle_length ( style_dialog.handle_length_spinner->value() );
    canvas->set_crossing_distance ( style_dialog.crossing_gap_spinner->value() );
    canvas->set_join_style ( style_dialog.get_join_style() );

}

void Knot_Window::update_style_dialog()
{
    style_dialog.knot_width_spinner->setValue(canvas->get_width());
    style_dialog.color->set_color ( canvas->get_brush().color() );
    QPen pen = canvas->get_pen();
    style_dialog.outline_color->set_color ( pen.color() );
    style_dialog.outline_width_spinner->setValue ( pen.width() );
    style_dialog.set_pen_style(pen.style());
    style_dialog.set_join_style ( pen.joinStyle() );
    style_dialog.set_style_id(canvas->get_curve_style() );
    style_dialog.cusp_angle_spinner->setValue ( canvas->get_cusp_angle() );
    style_dialog.handle_length_spinner->setValue ( canvas->get_handle_length() );
    style_dialog.crossing_gap_spinner->setValue ( canvas->get_crossing_distance() );
    style_dialog.set_join_style( canvas->get_join_style() );
}

void Knot_Window::update_title(bool clean)
{
    setWindowTitle("Knotter - "+(filename.isEmpty()?"knot":filename)+(clean?"":"*"));
}

void Knot_Window::cause_crash()
{
    Node *nil = 0;
    nil->~Node();
}

