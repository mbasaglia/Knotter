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
VERSION = 0.5.7devel


TEMPLATE = app


UI_DIR = src/generated
MOC_DIR = src/generated
RCC_DIR = src/generated

include(src/src.pri)
include(include/include.pri)
include(translations/translations.pri)

OTHER_FILES = \
    COPYING \
    Doxyfile \
    NEWS \
    doc.dox \
    README \
    configure.sh \
    AUTHORS

DEFINES += "VERSION=\\\"$${VERSION}\\\""

DEFINES += "BUILD_INFO=\"\\\"Knotter $$VERSION\\nBuilt on $$_DATE_\\n$${QMAKE_HOST.os} \
$${QMAKE_HOST.version} $${QMAKE_HOST.arch}\\nQt $${QT_VERSION}\\\""\"

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -Werror -Wall -Wextra
}

MYDISTFILES =  $$OTHER_FILES knotter.pro
MYDISTDIRS  =  src include img translations user_guide

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
mydistclean.commands = $(DEL_FILE) $$MYDIST_TAR_GZ Makefile $(TARGET)
QMAKE_EXTRA_TARGETS += mydist mydistclean

Doxyfile.commands = doxygen -g
src_doc.depends = Doxyfile FORCE
src_doc.commands = sed s/KNOTTER_VERSION/$${VERSION}/ Doxyfile | doxygen -
QMAKE_EXTRA_TARGETS += src_doc Doxyfile


#check directories and options from configure.sh

contains(SINGLE_FILE,yes) {
    BINDIR=.
    DATADIR=:/data
    DOCDIR=:/doc
    message("Compiling all data in a single executable file")
    RESOURCES += data.qrc
} else {
    isEmpty(DATADIR){
        DATADIR=.
    }
    isEmpty(DOCDIR){
        DOCDIR=.
    }
    img.files = img/*
    img.path = $${DATADIR}/img
    doc.files = user_guide/*
    doc.path = $${DOCDIR}/user_guide

    INSTALLS += img doc

}

isEmpty(BINDIR){
    BINDIR=.
}

DEFINES += "DATA_DIR=\\\"$${DATADIR}\\\""

DEFINES += "DOC_DIR=\\\"$${DOCDIR}\\\""

target.path = $$BINDIR
INSTALLS += target
