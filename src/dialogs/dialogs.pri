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
    $$PWD/preferences_dialog.hpp \
    src/dialogs/export_image_dialog.hpp

SOURCES += \
    $$PWD/main_window.cpp \
    $$PWD/preferences_dialog.cpp \
    src/dialogs/export_image_dialog.cpp

FORMS += \
    $$PWD/main_window.ui \
    $$PWD/preferences_dialog.ui \
    src/dialogs/export_image_dialog.ui
