#ifndef NODE_PREF_DIALOG_HPP
#define NODE_PREF_DIALOG_HPP

#include "ui_node_pref_dialog.h"
#include "knotview.hpp"

class node_pref_dialog : public QDialog, private Ui::node_pref_dialog
{
        Q_OBJECT
        
        Node* node;
        KnotView* knot_view;

    public:
        explicit node_pref_dialog(KnotView* knot_view,Node* node, QWidget *parent = 0);
    private slots:
        void on_buttonBox_accepted();
};

#endif // NODE_PREF_DIALOG_HPP
