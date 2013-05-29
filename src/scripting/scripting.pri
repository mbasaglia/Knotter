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
    $$PWD/script_line.hpp \
    $$PWD/script_point.hpp \
    $$PWD/plugin.hpp \
    $$PWD/cusp_scripted.hpp \
    $$PWD/plugin_cusp.hpp \
    $$PWD/script_path_builder.hpp \
    src/scripting/plugin_insert.hpp \
    src/scripting/plugin_param.hpp

SOURCES += \
    $$PWD/script_line.cpp \
    $$PWD/plugin.cpp \
    $$PWD/script_point.cpp \
    $$PWD/cusp_scripted.cpp \
    $$PWD/plugin_cusp.cpp \
    $$PWD/script_path_builder.cpp \
    src/scripting/plugin_insert.cpp \
    src/scripting/plugin_param.cpp

