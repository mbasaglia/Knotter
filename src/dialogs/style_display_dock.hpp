#ifndef STYLE_DISPLAY_DOCK_HPP
#define STYLE_DISPLAY_DOCK_HPP

#include "ui_style_display_dock.h"
#include "closable_dock.hpp"

class style_display_dock : public closable_dock, private Ui::style_display_dock
{
        Q_OBJECT
        
    public:
        explicit style_display_dock(QWidget *parent = 0);
};

#endif // STYLE_DISPLAY_DOCK_HPP
