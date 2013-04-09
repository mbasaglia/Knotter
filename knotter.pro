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

# Base configutation
QT += core gui xml script

TEMPLATE = app

UI_DIR = src/generated
MOC_DIR = src/generated
RCC_DIR = src/generated
OBJECTS_DIR = obj

include(knotter_info.pri)
include(src/src.pri)
include(translations.pri)


#non-source files
OTHER_FILES = \
    COPYING \
    Doxyfile.in \
    README \
    configure.sh \
    knotter.desktop.in \
    knotter.desktop \
    fix_makefile.sh \
    knotter_info.pri \
    translations.pri

# configured directories

isEmpty(DATADIR){
    DATADIR=$$PWD
}
isEmpty(DATAROOTDIR){
    DATAROOTDIR=$$PWD
}
isEmpty(BINDIR){
    BINDIR=$$PWD
}


# cpp defines

DEFINES += "VERSION=\\\"$${VERSION}\\\""
DEFINES += "DATA_DIR=\\\"$${DATADIR}\\\""

#DEFINES += "BUILD_INFO=\"\\\"Knotter $$VERSION\\nBuilt on $$_DATE_\\n$${QMAKE_HOST.os} \
#$${QMAKE_HOST.version} $${QMAKE_HOST.arch}\\nQt $${QT_VERSION}\\\""\"

!lessThan(QT_MAJOR_VERSION,4) !lessThan(QT_MINOR_VERSION,8) {
    DEFINES += HAS_QT_4_8
}
greaterThan(QT_MAJOR_VERSION, 4) {
    DEFINES += HAS_QT_5
    QT += widgets
}

contains(BOOST,yes) {
    LIBS += -lboost_program_options
} else {
    DEFINES += NO_BOOST
}


# Windows-specific stuff
win32 {
    # Remove non-numeric stuff from version as Windows RC doesn't like it
    VERSION ~= s/[-_a-zA-Z]+//
}

#Extra make targets

#dist
MYDISTFILES =  $$OTHER_FILES $${TARGET}.pro
MYDISTDIRS  =  src data

MYDIST_NAME = "$$TARGET-$${VERSION}"
MYDIST_TAR_GZ = "$${MYDIST_NAME}.tar.gz"
MYDIST_TMP = ".tmp/$${MYDIST_NAME}"
mydist.depends = doc man desktop_file
                                                                            #
mydist.commands =                                                           \
        (                                                                   \
            $(CHK_DIR_EXISTS) $$MYDIST_TMP ||                               \
            $(MKDIR) $$MYDIST_TMP                                           \
        ) &&                                                                \
        $(COPY_FILE) --parents $$MYDISTFILES  $$MYDIST_TMP &&               \
        $(COPY_DIR)  --parents $$MYDISTDIRS   $$MYDIST_TMP &&               \
        $(DEL_FILE) -f $$MYDIST_TMP/src/generated/*.h                       \
                       $$MYDIST_TMP/src/generated/*.cpp &&                  \
        (                                                                   \
            cd `dirname $$MYDIST_TMP`  &&                                   \
            $(TAR) $$MYDIST_TAR_GZ -a $$MYDIST_NAME                         \
        ) &&                                                                \
        $(MOVE) `dirname $$MYDIST_TMP`/$$MYDIST_TAR_GZ $$MYDIST_TAR_GZ &&   \
        $(DEL_FILE) -r $$MYDIST_TMP &&                                      \
        md5sum $$MYDIST_TAR_GZ >$${MYDIST_TAR_GZ}.md5

#distclean
mydistclean.depends = clean
mydistclean.commands = $(DEL_FILE) $$MYDIST_TAR_GZ Makefile $(TARGET)
QMAKE_EXTRA_TARGETS += mydist mydistclean

#src_doc
Doxyfile.commands = ./info_preprocessor.sh Doxyfile.in > Doxyfile
src_doc.depends = Doxyfile FORCE
src_doc.commands = doxygen Doxyfile

#desktop
$${TARGET}.desktop.depends=$${TARGET}.desktop.in
$${TARGET}.desktop.commands=./info_preprocessor.sh $${TARGET}.desktop.in > $${TARGET}.desktop

QMAKE_EXTRA_TARGETS += src_doc Doxyfile $${TARGET}.desktop


# Installs

data.files = data/*
data.path = $${DATADIR}


$${TARGET}.desktop.files=$${TARGET}.desktop
$${TARGET}.desktop.path=$${DATAROOTDIR}/applications

INSTALLS += data $${TARGET}.desktop


target.path = $$BINDIR
INSTALLS += target


