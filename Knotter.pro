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
VERSION = 0.2.0

TEMPLATE = app


SOURCES += main.cpp\
        knot_window.cpp \
    knotview.cpp \
    node.cpp \
    edge.cpp \
    knotgraph.cpp \
    style_dialog.cpp \
    knot_curve_style.cpp \
    clipboard.cpp

HEADERS  += knot_window.hpp \
    knotview.hpp \
    node.hpp \
    edge.hpp \
    commands.hpp \
    custom_item.hpp \
    knotgraph.hpp \
    style_dialog.hpp \
    color_widget.hpp \
    knot_curve_style.hpp \
    clipboard.hpp

FORMS    += knot_window.ui \
    style_dialog.ui

QMAKE_CXXFLAGS += -Werror

OTHER_FILES += \
    COPYING \
    README \
    Doxyfile \
    img/logo.svg \
    img/link.svg \
    img/icon.png \
    img/edit_nodes.svg \
    img/edit_edges.svg \
    img/edge_list.svg \
    img/merge.svg

RESOURCES += \
    icons.qrc

MYDISTFILES = $$FORMS $$OTHER_FILES $$RESOURCES Makefile Knotter.pro
MYDIST_NAME = "$$TARGET-$$VERSION"
MYDIST_TAR = "$${MYDIST_NAME}.tar"
MYDIST_TAR_GZ = "$${MYDIST_TAR}.gz"
MYDIST_TMP = ".tmp/$$MYDIST_NAME"
mydist.depends = $$TARGET
                                                                                            #
mydist.commands =                                                                           \
                (                                                                           \
                    $(CHK_DIR_EXISTS) $$MYDIST_TMP ||                                       \
                    $(MKDIR) $$MYDIST_TMP                                                   \
                ) &&                                                                        \
                $(COPY_FILE) --parents $$MYDISTFILES `ls *.hpp *.cpp *.h` $$MYDIST_TMP &&   \
                (                                                                           \
                    cd `dirname $$MYDIST_TMP`  &&                                           \
                    $(TAR) $$MYDIST_TAR $$MYDIST_NAME" &&                                   \
                    $(COMPRESS) $$MYDIST_TAR                                                \
                ) &&                                                                        \
                $(MOVE) `dirname $$MYDIST_TMP`/$$MYDIST_TAR_GZ $$MYDIST_TAR_GZ &&           \
                $(DEL_FILE) -r $$MYDIST_TMP                                                 #


mydistclean.depends = clean
mydistclean.commands = $(DEL_FILE) $$MYDIST_TAR_GZ Makefile
QMAKE_EXTRA_TARGETS += mydist mydistclean

Doxyfile.commands = doxygen -g
doc.depends = $$TARGET Doxyfile
doc.commands = doxygen
QMAKE_EXTRA_TARGETS += doc Doxyfile
