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

include($$PWD/dialogs/dialogs.pri)
include($$PWD/graph/graph.pri)
include($$PWD/scripting/scripting.pri)
include($$PWD/io/io.pri)

#widgets
include($$PWD/widgets/color/color_widgets.pri)
include($$PWD/widgets/toolbar_editor/toolbar_editor.pri)
include($$PWD/widgets/tabwidget/extended_tabwidget.pri)
include($$PWD/widgets/knot_view/knot_view.pri)


SOURCES += \
    $$PWD/main.cpp \
    $$PWD/resource_manager.cpp \
    $$PWD/settings.cpp \
    $$PWD/string_toolbar.cpp \
    $$PWD/command_line.cpp

HEADERS += \
    $$PWD/resource_manager.hpp \
    $$PWD/settings.hpp \
    $$PWD/string_toolbar.hpp \
    $$PWD/c++.hpp \
    $$PWD/command_line.hpp
