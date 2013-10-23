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

HEADERS += \
    $$PWD/knot_view.hpp \
    $$PWD/commands.hpp \
    $$PWD/snapping_grid.hpp \
    $$PWD/background_image.hpp \
    $$PWD/node_mover.hpp \
    $$PWD/pen_join_style_metatype.hpp \
    $$PWD/context_menu_node.hpp \
    $$PWD/context_menu_edge.hpp \
    $$PWD/transform_handle.hpp \
    $$PWD/knot_tool.hpp

SOURCES += \
    $$PWD/knot_view.cpp \
    $$PWD/commands.cpp \
    $$PWD/snapping_grid.cpp \
    $$PWD/background_image.cpp \
    $$PWD/node_mover.cpp \
    $$PWD/context_menu_node.cpp \
    $$PWD/context_menu_edge.cpp \
    $$PWD/transform_handle.cpp \
    $$PWD/knot_tool.cpp

INCLUDEPATH += $$PWD
