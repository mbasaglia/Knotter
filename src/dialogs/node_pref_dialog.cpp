#include "node_pref_dialog.hpp"
#include "translator.hpp"

node_pref_dialog::node_pref_dialog(KnotView* knot_view, Node *node, QWidget *parent) :
    QDialog(parent), node ( node ), knot_view ( knot_view )
{
    setupUi(this);

    style_form->from_single_node(node->get_custom_style(),knot_view->get_default_style());


    setModal(true);

    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));
}

void node_pref_dialog::on_buttonBox_accepted()
{
    knot_view->set_custom_style ( node, style_form->get_style_info() );

    knot_view->redraw(true);
}

void node_pref_dialog::retranslate()
{
    retranslateUi(this);
}
