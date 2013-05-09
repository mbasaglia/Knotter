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

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/main_window.hpp \
    $$PWD/dialog_preferences.hpp \
    $$PWD/export_image_dialog.hpp \
    $$PWD/dock_grid.hpp \
    $$PWD/dock_knot_display.hpp \
    $$PWD/icon_dock_style.hpp \
    $$PWD/dock_background.hpp \
    $$PWD/cusp_style_widget.hpp \
    $$PWD/about_dialog.hpp \
    $$PWD/dialog_insert_polygon.hpp

SOURCES += \
    $$PWD/main_window.cpp \
    $$PWD/dialog_preferences.cpp \
    $$PWD/export_image_dialog.cpp \
    $$PWD/dock_grid.cpp \
    $$PWD/dock_knot_display.cpp \
    $$PWD/dock_background.cpp \
    $$PWD/cusp_style_widget.cpp \
    $$PWD/about_dialog.cpp \
    $$PWD/dialog_insert_polygon.cpp

FORMS += \
    $$PWD/main_window.ui \
    $$PWD/dialog_preferences.ui \
    $$PWD/export_image_dialog.ui \
    $$PWD/dock_grid.ui \
    $$PWD/dock_knot_display.ui \
    $$PWD/dock_background.ui \
    $$PWD/cusp_style_widget.ui \
    $$PWD/about_dialog.ui \
    $$PWD/dialog_insert_polygon.ui
