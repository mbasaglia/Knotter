# Copyright (C) 2012-2013  Mattia Basaglia
#
# Knotter is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Knotter is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

INCLUDEPATH += $$PWD $$PWD/dock

HEADERS += \
    $$PWD/main_window.hpp \
    $$PWD/dialog_preferences.hpp \
    $$PWD/export_image_dialog.hpp \
    $$PWD/dock/dock_knot_display.hpp \
    $$PWD/icon_dock_style.hpp \
    $$PWD/dock/dock_background.hpp \
    $$PWD/about_dialog.hpp \
    $$PWD/cusp_style_dialog.hpp \
    $$PWD/dialog_confirm_close.hpp \
    $$PWD/dialog_plugins.hpp \
    $$PWD/dock/dock_borders.hpp \
    $$PWD/dock/dock_script_log.hpp \
    $$PWD/wizard_create_plugin.hpp \
    $$PWD/dock/dock_grid.hpp \
    $$PWD/dialog_edge_properties.hpp \
    $$PWD/dock/dock_knot_style.hpp

SOURCES += \
    $$PWD/main_window.cpp \
    $$PWD/dialog_preferences.cpp \
    $$PWD/export_image_dialog.cpp \
    $$PWD/dock/dock_grid.cpp \
    $$PWD/dock/dock_knot_display.cpp \
    $$PWD/dock/dock_background.cpp \
    $$PWD/about_dialog.cpp \
    $$PWD/cusp_style_dialog.cpp \
    $$PWD/dialog_confirm_close.cpp \
    $$PWD/dialog_plugins.cpp \
    $$PWD/dock/dock_borders.cpp \
    $$PWD/dock/dock_script_log.cpp \
    $$PWD/wizard_create_plugin.cpp \
    $$PWD/dialog_edge_properties.cpp \
    $$PWDdock/dock_knot_style.cpp

FORMS += \
    $$PWD/main_window.ui \
    $$PWD/dialog_preferences.ui \
    $$PWD/export_image_dialog.ui \
    $$PWD/dock/dock_grid.ui \
    $$PWD/dock/dock_knot_display.ui \
    $$PWD/dock/dock_background.ui \
    $$PWD/about_dialog.ui \
    $$PWD/cusp_style_dialog.ui \
    $$PWD/dialog_confirm_close.ui \
    $$PWD/dialog_plugins.ui \
    $$PWD/dock/dock_borders.ui \
    $$PWD/dock/dock_script_log.ui \
    $$PWD/wizard_create_plugin.ui \
    $$PWD/dialog_edge_properties.ui \
    $$PWD/dock/dock_knot_style.ui
