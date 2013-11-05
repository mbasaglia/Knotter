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

INCLUDEPATH += $$PWD $$PWD/wrappers

HEADERS += \
    $$PWD/wrappers/script_line.hpp \
    $$PWD/wrappers/script_point.hpp \
    $$PWD/plugin.hpp \
    $$PWD/cusp_scripted.hpp \
    $$PWD/plugin_cusp.hpp \
    $$PWD/wrappers/script_path_builder.hpp \
    $$PWD/misc_script_functions.hpp \
    $$PWD/wrappers/script_node.hpp \
    $$PWD/wrappers/script_window.hpp \
    $$PWD/wrappers/script_edge.hpp \
    $$PWD/wrappers/script_graph.hpp \
    $$PWD/wrappers/script_document.hpp \
    $$PWD/wrappers/script_grid.hpp \
    $$PWD/wrappers/script_polygon.hpp \
    $$PWD/wrappers/script_node_style.hpp \
    $$PWD/wrappers/script_edge_style.hpp \
    $$PWD/wrappers/script_renderer.hpp \
    $$PWD/wrappers/script_color.hpp \
    $$PWD/wrappers/script_graph_style.hpp \
    $$PWD/plugin_crossing.hpp \
    $$PWD/edge_scripted.hpp \
    $$PWD/json_stuff.hpp \
    $$PWD/wrappers/script_qtablewidget.hpp

SOURCES += \
    $$PWD/wrappers/script_line.cpp \
    $$PWD/plugin.cpp \
    $$PWD/wrappers/script_point.cpp \
    $$PWD/cusp_scripted.cpp \
    $$PWD/plugin_cusp.cpp \
    $$PWD/wrappers/script_path_builder.cpp \
    $$PWD/misc_script_functions.cpp \
    $$PWD/wrappers/script_node.cpp \
    $$PWD/wrappers/script_window.cpp \
    $$PWD/wrappers/script_edge.cpp \
    $$PWD/wrappers/script_graph.cpp \
    $$PWD/wrappers/script_document.cpp \
    $$PWD/wrappers/script_grid.cpp \
    $$PWD/wrappers/script_polygon.cpp \
    $$PWD/wrappers/script_node_style.cpp \
    $$PWD/wrappers/script_edge_style.cpp \
    $$PWD/wrappers/script_renderer.cpp \
    $$PWD/wrappers/script_color.cpp \
    $$PWD/wrappers/script_graph_style.cpp \
    $$PWD/plugin_crossing.cpp \
    $$PWD/edge_scripted.cpp \
    $$PWD/json_stuff.cpp \
    $$PWD/wrappers/script_qtablewidget.cpp
