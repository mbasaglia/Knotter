# Copyright (C) 2012  Mattia Basaglia
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

QT       += core gui xml svg

TARGET = knotter
TEMPLATE = app


SOURCES += main.cpp\
        knot_window.cpp \
    knotview.cpp \
    node.cpp \
    edge.cpp \
    knotgraph.cpp \
    style_dialog.cpp \
    knot_curve_style.cpp

HEADERS  += knot_window.hpp \
    knotview.hpp \
    node.hpp \
    edge.hpp \
    commands.hpp \
    custom_item.hpp \
    knotgraph.hpp \
    style_dialog.hpp \
    color_widget.hpp \
    knot_curve_style.hpp

FORMS    += knot_window.ui \
    style_dialog.ui

QMAKE_CXXFLAGS += -Werror

OTHER_FILES += \
    COPYING \
    license_template \
    README
