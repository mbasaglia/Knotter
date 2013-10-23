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
    $$PWD/node.hpp \
    $$PWD/graph.hpp \
    $$PWD/edge.hpp \
    $$PWD/edge_type.hpp \
    $$PWD/graph_item.hpp \
    $$PWD/node_style.hpp \
    $$PWD/path_builder.hpp \
    $$PWD/path_item.hpp \
    $$PWD/point_math.hpp \
    $$PWD/traversal_info.hpp \
    $$PWD/node_cusp_shape.hpp \
    $$PWD/knot_border.hpp \
    $$PWD/edge_style.hpp

SOURCES += \
    $$PWD/node.cpp \
    $$PWD/graph.cpp \
    $$PWD/edge.cpp \
    $$PWD/edge_type.cpp \
    $$PWD/graph_item.cpp \
    $$PWD/node_style.cpp \
    $$PWD/path_builder.cpp \
    $$PWD/path_item.cpp \
    $$PWD/node_cusp_shape.cpp \
    $$PWD/knot_border.cpp \
    $$PWD/edge_style.cpp
