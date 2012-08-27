#include "node_pref_dialog.hpp"
#include "translator.hpp"

node_pref_dialog::node_pref_dialog(KnotView* knot_view, Node *node, QWidget *parent) :
    QDialog(parent), node ( node ), knot_view ( knot_view )
{
    setupUi(this);
    if ( node->has_custom_style() )
    {
        use_default->setChecked ( false );
        style_form->set_style_info(node->get_custom_style());
        style_form->setEnabled(true);
    }
    else
    {
        use_default->setChecked ( true );
        style_form->set_style_info(knot_view->get_default_style());
        style_form->setEnabled(false);
    }

    setModal(true);

    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));
}

void node_pref_dialog::on_buttonBox_accepted()
{
    if ( use_default->isChecked() )
        node->unset_custom_style();
    else
        node->set_custom_style ( style_form->get_style_info() );

    knot_view->redraw(true);
}

void node_pref_dialog::retranslate()
{
    retranslateUi(this);
}
