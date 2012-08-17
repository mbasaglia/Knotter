/********************************************************************************
** Form generated from reading UI file 'knot_window.ui'
**
** Created: Fri Aug 17 09:18:04 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KNOT_WINDOW_H
#define UI_KNOT_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "graphics/knotview.hpp"

QT_BEGIN_NAMESPACE

class Ui_Knot_Window
{
public:
    QAction *action_Save;
    QAction *actionSave_As;
    QAction *action_Export;
    QAction *action_Open;
    QAction *action_New;
    QAction *action_Quit;
    QAction *actionInsert_Edges;
    QAction *actionEdit_Nodes;
    QAction *actionEdge_List;
    QAction *action_Link;
    QAction *action_Erase;
    QAction *action_Merge;
    QAction *action_Undo;
    QAction *action_Redo;
    QAction *actionSelect_All;
    QAction *actionRefresh_Path;
    QAction *action_Main;
    QAction *action_Knot_Editing;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *action_Reset_Zoom;
    QAction *action_Reset_View;
    QAction *actionShow_Graph;
    QAction *actionShow_Knot_line;
    QAction *action_Style;
    QAction *actionInsert_S_quare;
    QAction *actionInsert_Triangle;
    QAction *actionCut;
    QAction *action_Copy;
    QAction *action_Paste;
    QAction *action_Configure_Knotter;
    QAction *actionEnable_Grid;
    QAction *actionConfigure_G_rid;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    KnotView *canvas;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuNo_des;
    QMenu *menu_Edit;
    QMenu *menu_View;
    QMenu *menu_Zoom;
    QMenu *menu_Toolbars;
    QMenu *menu_Knot;
    QMenu *menu_Settings;
    QStatusBar *statusbar;
    QToolBar *EditBar;
    QToolBar *MainToolBar;

    void setupUi(QMainWindow *Knot_Window)
    {
        if (Knot_Window->objectName().isEmpty())
            Knot_Window->setObjectName(QString::fromUtf8("Knot_Window"));
        Knot_Window->resize(640, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/logo.svg"), QSize(), QIcon::Normal, QIcon::Off);
        Knot_Window->setWindowIcon(icon);
        action_Save = new QAction(Knot_Window);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        actionSave_As = new QAction(Knot_Window);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        action_Export = new QAction(Knot_Window);
        action_Export->setObjectName(QString::fromUtf8("action_Export"));
        action_Open = new QAction(Knot_Window);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        action_New = new QAction(Knot_Window);
        action_New->setObjectName(QString::fromUtf8("action_New"));
        action_Quit = new QAction(Knot_Window);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        actionInsert_Edges = new QAction(Knot_Window);
        actionInsert_Edges->setObjectName(QString::fromUtf8("actionInsert_Edges"));
        actionInsert_Edges->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/edit_edges.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionInsert_Edges->setIcon(icon1);
        actionEdit_Nodes = new QAction(Knot_Window);
        actionEdit_Nodes->setObjectName(QString::fromUtf8("actionEdit_Nodes"));
        actionEdit_Nodes->setCheckable(true);
        actionEdit_Nodes->setChecked(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/edit_nodes.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionEdit_Nodes->setIcon(icon2);
        actionEdge_List = new QAction(Knot_Window);
        actionEdge_List->setObjectName(QString::fromUtf8("actionEdge_List"));
        actionEdge_List->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/edge_list.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionEdge_List->setIcon(icon3);
        action_Link = new QAction(Knot_Window);
        action_Link->setObjectName(QString::fromUtf8("action_Link"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/img/link.svg"), QSize(), QIcon::Normal, QIcon::Off);
        action_Link->setIcon(icon4);
        action_Erase = new QAction(Knot_Window);
        action_Erase->setObjectName(QString::fromUtf8("action_Erase"));
        action_Merge = new QAction(Knot_Window);
        action_Merge->setObjectName(QString::fromUtf8("action_Merge"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/img/merge.svg"), QSize(), QIcon::Normal, QIcon::Off);
        action_Merge->setIcon(icon5);
        action_Undo = new QAction(Knot_Window);
        action_Undo->setObjectName(QString::fromUtf8("action_Undo"));
        action_Redo = new QAction(Knot_Window);
        action_Redo->setObjectName(QString::fromUtf8("action_Redo"));
        actionSelect_All = new QAction(Knot_Window);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionRefresh_Path = new QAction(Knot_Window);
        actionRefresh_Path->setObjectName(QString::fromUtf8("actionRefresh_Path"));
        action_Main = new QAction(Knot_Window);
        action_Main->setObjectName(QString::fromUtf8("action_Main"));
        action_Main->setCheckable(true);
        action_Main->setChecked(true);
        action_Knot_Editing = new QAction(Knot_Window);
        action_Knot_Editing->setObjectName(QString::fromUtf8("action_Knot_Editing"));
        action_Knot_Editing->setCheckable(true);
        action_Knot_Editing->setChecked(true);
        actionZoom_In = new QAction(Knot_Window);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        actionZoom_Out = new QAction(Knot_Window);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        action_Reset_Zoom = new QAction(Knot_Window);
        action_Reset_Zoom->setObjectName(QString::fromUtf8("action_Reset_Zoom"));
        action_Reset_View = new QAction(Knot_Window);
        action_Reset_View->setObjectName(QString::fromUtf8("action_Reset_View"));
        actionShow_Graph = new QAction(Knot_Window);
        actionShow_Graph->setObjectName(QString::fromUtf8("actionShow_Graph"));
        actionShow_Graph->setCheckable(true);
        actionShow_Graph->setChecked(true);
        actionShow_Knot_line = new QAction(Knot_Window);
        actionShow_Knot_line->setObjectName(QString::fromUtf8("actionShow_Knot_line"));
        actionShow_Knot_line->setCheckable(true);
        actionShow_Knot_line->setChecked(true);
        action_Style = new QAction(Knot_Window);
        action_Style->setObjectName(QString::fromUtf8("action_Style"));
        actionInsert_S_quare = new QAction(Knot_Window);
        actionInsert_S_quare->setObjectName(QString::fromUtf8("actionInsert_S_quare"));
        actionInsert_S_quare->setEnabled(false);
        actionInsert_Triangle = new QAction(Knot_Window);
        actionInsert_Triangle->setObjectName(QString::fromUtf8("actionInsert_Triangle"));
        actionInsert_Triangle->setEnabled(false);
        actionCut = new QAction(Knot_Window);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        action_Copy = new QAction(Knot_Window);
        action_Copy->setObjectName(QString::fromUtf8("action_Copy"));
        action_Paste = new QAction(Knot_Window);
        action_Paste->setObjectName(QString::fromUtf8("action_Paste"));
        action_Configure_Knotter = new QAction(Knot_Window);
        action_Configure_Knotter->setObjectName(QString::fromUtf8("action_Configure_Knotter"));
        actionEnable_Grid = new QAction(Knot_Window);
        actionEnable_Grid->setObjectName(QString::fromUtf8("actionEnable_Grid"));
        actionEnable_Grid->setCheckable(true);
        actionEnable_Grid->setChecked(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/img/square_grid.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionEnable_Grid->setIcon(icon6);
        actionConfigure_G_rid = new QAction(Knot_Window);
        actionConfigure_G_rid->setObjectName(QString::fromUtf8("actionConfigure_G_rid"));
        centralWidget = new QWidget(Knot_Window);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        canvas = new KnotView(centralWidget);
        canvas->setObjectName(QString::fromUtf8("canvas"));

        gridLayout->addWidget(canvas, 0, 0, 1, 1);

        Knot_Window->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Knot_Window);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 21));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuNo_des = new QMenu(menuBar);
        menuNo_des->setObjectName(QString::fromUtf8("menuNo_des"));
        menu_Edit = new QMenu(menuBar);
        menu_Edit->setObjectName(QString::fromUtf8("menu_Edit"));
        menu_View = new QMenu(menuBar);
        menu_View->setObjectName(QString::fromUtf8("menu_View"));
        menu_Zoom = new QMenu(menu_View);
        menu_Zoom->setObjectName(QString::fromUtf8("menu_Zoom"));
        menu_Toolbars = new QMenu(menu_View);
        menu_Toolbars->setObjectName(QString::fromUtf8("menu_Toolbars"));
        menu_Knot = new QMenu(menuBar);
        menu_Knot->setObjectName(QString::fromUtf8("menu_Knot"));
        menu_Settings = new QMenu(menuBar);
        menu_Settings->setObjectName(QString::fromUtf8("menu_Settings"));
        Knot_Window->setMenuBar(menuBar);
        statusbar = new QStatusBar(Knot_Window);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Knot_Window->setStatusBar(statusbar);
        EditBar = new QToolBar(Knot_Window);
        EditBar->setObjectName(QString::fromUtf8("EditBar"));
        EditBar->setAllowedAreas(Qt::AllToolBarAreas);
        Knot_Window->addToolBar(Qt::TopToolBarArea, EditBar);
        MainToolBar = new QToolBar(Knot_Window);
        MainToolBar->setObjectName(QString::fromUtf8("MainToolBar"));
        Knot_Window->addToolBar(Qt::TopToolBarArea, MainToolBar);
        Knot_Window->insertToolBarBreak(MainToolBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Edit->menuAction());
        menuBar->addAction(menu_View->menuAction());
        menuBar->addAction(menuNo_des->menuAction());
        menuBar->addAction(menu_Knot->menuAction());
        menuBar->addAction(menu_Settings->menuAction());
        menu_File->addAction(action_New);
        menu_File->addAction(action_Open);
        menu_File->addSeparator();
        menu_File->addAction(action_Save);
        menu_File->addAction(actionSave_As);
        menu_File->addAction(action_Export);
        menu_File->addSeparator();
        menu_File->addAction(action_Quit);
        menuNo_des->addAction(action_Link);
        menuNo_des->addAction(action_Erase);
        menuNo_des->addAction(action_Merge);
        menuNo_des->addSeparator();
        menuNo_des->addAction(actionInsert_S_quare);
        menuNo_des->addAction(actionInsert_Triangle);
        menu_Edit->addAction(action_Undo);
        menu_Edit->addAction(action_Redo);
        menu_Edit->addSeparator();
        menu_Edit->addAction(actionCut);
        menu_Edit->addAction(action_Copy);
        menu_Edit->addAction(action_Paste);
        menu_Edit->addSeparator();
        menu_Edit->addAction(actionSelect_All);
        menu_View->addAction(menu_Toolbars->menuAction());
        menu_View->addSeparator();
        menu_View->addAction(menu_Zoom->menuAction());
        menu_View->addAction(action_Reset_View);
        menu_View->addSeparator();
        menu_View->addAction(actionShow_Graph);
        menu_View->addAction(actionShow_Knot_line);
        menu_Zoom->addAction(actionZoom_In);
        menu_Zoom->addAction(actionZoom_Out);
        menu_Zoom->addAction(action_Reset_Zoom);
        menu_Toolbars->addAction(action_Main);
        menu_Toolbars->addAction(action_Knot_Editing);
        menu_Knot->addAction(action_Style);
        menu_Knot->addSeparator();
        menu_Knot->addAction(actionEdit_Nodes);
        menu_Knot->addAction(actionInsert_Edges);
        menu_Knot->addAction(actionEdge_List);
        menu_Knot->addSeparator();
        menu_Knot->addAction(actionRefresh_Path);
        menu_Settings->addAction(action_Style);
        menu_Settings->addAction(action_Configure_Knotter);
        menu_Settings->addAction(actionEnable_Grid);
        menu_Settings->addAction(actionConfigure_G_rid);
        EditBar->addAction(actionEdit_Nodes);
        EditBar->addAction(actionInsert_Edges);
        EditBar->addAction(actionEdge_List);
        EditBar->addSeparator();
        EditBar->addAction(action_Link);
        EditBar->addAction(action_Erase);
        EditBar->addAction(action_Merge);
        EditBar->addSeparator();
        EditBar->addAction(actionEnable_Grid);
        MainToolBar->addAction(action_New);
        MainToolBar->addAction(action_Open);
        MainToolBar->addAction(action_Save);
        MainToolBar->addAction(actionSave_As);
        MainToolBar->addAction(action_Export);
        MainToolBar->addSeparator();
        MainToolBar->addAction(action_Undo);
        MainToolBar->addAction(action_Redo);

        retranslateUi(Knot_Window);
        QObject::connect(action_Erase, SIGNAL(triggered()), canvas, SLOT(erase_selected()));
        QObject::connect(action_Merge, SIGNAL(triggered()), canvas, SLOT(merge_selected()));
        QObject::connect(action_Link, SIGNAL(triggered()), canvas, SLOT(link_selected()));
        QObject::connect(actionSelect_All, SIGNAL(triggered()), canvas, SLOT(select_all()));
        QObject::connect(actionEdge_List, SIGNAL(triggered()), Knot_Window, SLOT(mode_edge_list()));
        QObject::connect(actionEdit_Nodes, SIGNAL(triggered()), Knot_Window, SLOT(mode_node()));
        QObject::connect(actionInsert_Edges, SIGNAL(triggered()), Knot_Window, SLOT(mode_edge()));
        QObject::connect(canvas, SIGNAL(mouse_moved(QPointF)), Knot_Window, SLOT(mouse_moved(QPointF)));
        QObject::connect(actionRefresh_Path, SIGNAL(triggered()), canvas, SLOT(redraw()));
        QObject::connect(action_New, SIGNAL(triggered()), Knot_Window, SLOT(clear()));
        QObject::connect(actionSave_As, SIGNAL(triggered()), Knot_Window, SLOT(saveAs()));
        QObject::connect(action_Save, SIGNAL(triggered()), Knot_Window, SLOT(save()));
        QObject::connect(action_Open, SIGNAL(triggered()), Knot_Window, SLOT(open()));
        QObject::connect(action_Export, SIGNAL(triggered()), Knot_Window, SLOT(exportSVG()));
        QObject::connect(action_Reset_View, SIGNAL(triggered()), canvas, SLOT(reset_view()));
        QObject::connect(action_Main, SIGNAL(triggered(bool)), MainToolBar, SLOT(setVisible(bool)));
        QObject::connect(action_Knot_Editing, SIGNAL(triggered(bool)), EditBar, SLOT(setVisible(bool)));
        QObject::connect(action_Reset_Zoom, SIGNAL(triggered()), canvas, SLOT(reset_zoom()));
        QObject::connect(actionZoom_In, SIGNAL(triggered()), Knot_Window, SLOT(zoom_in()));
        QObject::connect(actionZoom_Out, SIGNAL(triggered()), Knot_Window, SLOT(zoom_out()));
        QObject::connect(actionCut, SIGNAL(triggered()), Knot_Window, SLOT(cut()));
        QObject::connect(action_Copy, SIGNAL(triggered()), Knot_Window, SLOT(copy()));
        QObject::connect(action_Paste, SIGNAL(triggered()), Knot_Window, SLOT(paste()));
        QObject::connect(actionConfigure_G_rid, SIGNAL(triggered()), Knot_Window, SLOT(configure_grid()));
        QObject::connect(actionEnable_Grid, SIGNAL(triggered(bool)), Knot_Window, SLOT(enable_grid(bool)));

        QMetaObject::connectSlotsByName(Knot_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Knot_Window)
    {
        Knot_Window->setWindowTitle(QApplication::translate("Knot_Window", "Knotter", 0, QApplication::UnicodeUTF8));
        action_Save->setText(QApplication::translate("Knot_Window", "&Save", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("Knot_Window", "Save &As...", 0, QApplication::UnicodeUTF8));
        action_Export->setText(QApplication::translate("Knot_Window", "E&xport...", 0, QApplication::UnicodeUTF8));
        action_Open->setText(QApplication::translate("Knot_Window", "&Open", 0, QApplication::UnicodeUTF8));
        action_New->setText(QApplication::translate("Knot_Window", "&New", 0, QApplication::UnicodeUTF8));
        action_Quit->setText(QApplication::translate("Knot_Window", "&Quit", 0, QApplication::UnicodeUTF8));
        actionInsert_Edges->setText(QApplication::translate("Knot_Window", "Edit &Edges", 0, QApplication::UnicodeUTF8));
        actionInsert_Edges->setShortcut(QApplication::translate("Knot_Window", "Alt+Shift+E", 0, QApplication::UnicodeUTF8));
        actionEdit_Nodes->setText(QApplication::translate("Knot_Window", "Edit &Nodes", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionEdit_Nodes->setToolTip(QApplication::translate("Knot_Window", "Edit Nodes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionEdit_Nodes->setShortcut(QApplication::translate("Knot_Window", "Alt+Shift+N", 0, QApplication::UnicodeUTF8));
        actionEdge_List->setText(QApplication::translate("Knot_Window", "Edge &List", 0, QApplication::UnicodeUTF8));
        actionEdge_List->setShortcut(QApplication::translate("Knot_Window", "Alt+Shift+L", 0, QApplication::UnicodeUTF8));
        action_Link->setText(QApplication::translate("Knot_Window", "&Link", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Link->setToolTip(QApplication::translate("Knot_Window", "Link Selected Nodes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_Link->setShortcut(QApplication::translate("Knot_Window", "Ctrl+L", 0, QApplication::UnicodeUTF8));
        action_Erase->setText(QApplication::translate("Knot_Window", "&Erase", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Erase->setToolTip(QApplication::translate("Knot_Window", "Erase Selected Nodes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_Erase->setShortcut(QApplication::translate("Knot_Window", "Del", 0, QApplication::UnicodeUTF8));
        action_Merge->setText(QApplication::translate("Knot_Window", "&Merge", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Merge->setToolTip(QApplication::translate("Knot_Window", "Merge Selected Nodes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_Merge->setShortcut(QApplication::translate("Knot_Window", "Ctrl+M", 0, QApplication::UnicodeUTF8));
        action_Undo->setText(QApplication::translate("Knot_Window", "&Undo", 0, QApplication::UnicodeUTF8));
        action_Redo->setText(QApplication::translate("Knot_Window", "&Redo", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setText(QApplication::translate("Knot_Window", "Select &All", 0, QApplication::UnicodeUTF8));
        actionRefresh_Path->setText(QApplication::translate("Knot_Window", "Refresh &Path", 0, QApplication::UnicodeUTF8));
        action_Main->setText(QApplication::translate("Knot_Window", "&Main", 0, QApplication::UnicodeUTF8));
        action_Knot_Editing->setText(QApplication::translate("Knot_Window", "&Knot Editing", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setText(QApplication::translate("Knot_Window", "Zoom &In", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setText(QApplication::translate("Knot_Window", "Zoom &Out", 0, QApplication::UnicodeUTF8));
        action_Reset_Zoom->setText(QApplication::translate("Knot_Window", "&Reset Zoom", 0, QApplication::UnicodeUTF8));
        action_Reset_Zoom->setShortcut(QApplication::translate("Knot_Window", "Ctrl+0", 0, QApplication::UnicodeUTF8));
        action_Reset_View->setText(QApplication::translate("Knot_Window", "&Reset View", 0, QApplication::UnicodeUTF8));
        actionShow_Graph->setText(QApplication::translate("Knot_Window", "Show &Graph", 0, QApplication::UnicodeUTF8));
        actionShow_Knot_line->setText(QApplication::translate("Knot_Window", "Show Knot&line", 0, QApplication::UnicodeUTF8));
        action_Style->setText(QApplication::translate("Knot_Window", "&Style...", 0, QApplication::UnicodeUTF8));
        actionInsert_S_quare->setText(QApplication::translate("Knot_Window", "Insert S&quare", 0, QApplication::UnicodeUTF8));
        actionInsert_Triangle->setText(QApplication::translate("Knot_Window", "Insert &Triangle", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("Knot_Window", "Cu&t", 0, QApplication::UnicodeUTF8));
        action_Copy->setText(QApplication::translate("Knot_Window", "&Copy", 0, QApplication::UnicodeUTF8));
        action_Paste->setText(QApplication::translate("Knot_Window", "&Paste", 0, QApplication::UnicodeUTF8));
        action_Configure_Knotter->setText(QApplication::translate("Knot_Window", "&Configure Knotter...", 0, QApplication::UnicodeUTF8));
        actionEnable_Grid->setText(QApplication::translate("Knot_Window", "Enable &Grid", 0, QApplication::UnicodeUTF8));
        actionConfigure_G_rid->setText(QApplication::translate("Knot_Window", "Configure G&rid...", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("Knot_Window", "&File", 0, QApplication::UnicodeUTF8));
        menuNo_des->setTitle(QApplication::translate("Knot_Window", "No&des", 0, QApplication::UnicodeUTF8));
        menu_Edit->setTitle(QApplication::translate("Knot_Window", "&Edit", 0, QApplication::UnicodeUTF8));
        menu_View->setTitle(QApplication::translate("Knot_Window", "&View", 0, QApplication::UnicodeUTF8));
        menu_Zoom->setTitle(QApplication::translate("Knot_Window", "&Zoom", 0, QApplication::UnicodeUTF8));
        menu_Toolbars->setTitle(QApplication::translate("Knot_Window", "&Toolbars", 0, QApplication::UnicodeUTF8));
        menu_Knot->setTitle(QApplication::translate("Knot_Window", "&Knot", 0, QApplication::UnicodeUTF8));
        menu_Settings->setTitle(QApplication::translate("Knot_Window", "&Settings", 0, QApplication::UnicodeUTF8));
        EditBar->setWindowTitle(QApplication::translate("Knot_Window", "EditBar", 0, QApplication::UnicodeUTF8));
        MainToolBar->setWindowTitle(QApplication::translate("Knot_Window", "MainToolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Knot_Window: public Ui_Knot_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KNOT_WINDOW_H
