# Copyright (C) 2013  Mattia Basaglia
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
    $$PWD/border_widget.hpp \
    $$PWD/cusp_style_widget.hpp \
    $$PWD/border_edit_widget.hpp \
    $$PWD/copyable_text_browser.hpp \
    $$PWD/script_edit.hpp \
    $$PWD/script_highlighter.hpp

SOURCES += \
    $$PWD/border_widget.cpp \
    $$PWD/cusp_style_widget.cpp \
    $$PWD/border_edit_widget.cpp \
    $$PWD/copyable_text_browser.cpp \
    $$PWD/script_edit.cpp \
    $$PWD/script_highlighter.cpp

FORMS += \
    $$PWD/cusp_style_widget.ui \
    $$PWD/border_edit_widget.ui
