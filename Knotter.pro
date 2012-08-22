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

QT       += core gui xml


TARGET = knotter
VERSION = 0.5.2_devel

TEMPLATE = app

OTHER_FILES += \
    COPYING \
    Doxyfile \
    NEWS \
    doc.dox \
    README

UI_DIR = src/generated
MOC_DIR = src/generated
RCC_DIR = src/generated

include(src/src.pri)
include(include/include.pri)
include(img/img.pri)
include(translations/translations.pri)
include(user_guide/user_guide.pri)

DEFINES += "VERSION=\\\"$${VERSION}\\\""

DEFINES += "BUILD_INFO=\"\\\"Knotter $$VERSION\\nBuilt on $$_DATE_\\n$${QMAKE_HOST.os} \
$${QMAKE_HOST.version} $${QMAKE_HOST.arch}\\nQt $${QT_VERSION}\\\""\"

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -Werror -Wall -Wextra
}

MYDISTFILES =  COPYING README Doxyfile NEWS Makefile Knotter.pro
MYDISTDIRS  =  src include img translations

MYDIST_NAME = "$$TARGET-$${VERSION}"
MYDIST_TAR = "$${MYDIST_NAME}.tar"
MYDIST_TAR_GZ = "$${MYDIST_TAR}.gz"
MYDIST_TMP = ".tmp/$${MYDIST_NAME}"
                                                                            #
mydist.commands =                                                           \
        (                                                                   \
            $(CHK_DIR_EXISTS) $$MYDIST_TMP ||                               \
            $(MKDIR) $$MYDIST_TMP                                           \
        ) &&                                                                \
        $(COPY_FILE) --parents $$MYDISTFILES  $$MYDIST_TMP &&               \
        $(COPY_DIR)  --parents $$MYDISTDIRS   $$MYDIST_TMP &&               \
        (                                                                   \
            cd `dirname $$MYDIST_TMP`  &&                                   \
            $(TAR) $$MYDIST_TAR $$MYDIST_NAME &&                            \
            $(COMPRESS) $$MYDIST_TAR                                        \
        ) &&                                                                \
        $(MOVE) `dirname $$MYDIST_TMP`/$$MYDIST_TAR_GZ $$MYDIST_TAR_GZ &&   \
        $(DEL_FILE) -r $$MYDIST_TMP                                         #


mydistclean.depends = clean
mydistclean.commands = $(DEL_FILE) $$MYDIST_TAR_GZ Makefile
QMAKE_EXTRA_TARGETS += mydist mydistclean

Doxyfile.commands = doxygen -g
doc.depends = Doxyfile FORCE
doc.commands = sed s/KNOTTER_VERSION/$${VERSION}/ Doxyfile | doxygen -
QMAKE_EXTRA_TARGETS += doc Doxyfile


target.path = /bin
INSTALLS += target
