# Copyright (C) 2012-2014  Mattia Basaglia
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

# Workaround a bug in OSX Qt5
macx {
    cache()
}

# Base configutation
QT += core gui xml script svg network

TEMPLATE = app

UI_DIR = src/generated
MOC_DIR = src/generated
RCC_DIR = src/generated
OBJECTS_DIR = obj

include(knotter_info.pri)
include(src/src.pri)
include(translations.pri)

#include for lupdate but discard on compilation
!debug:!release {
    include(data/plugins/plugins.pri)
}

#non-source files
OTHER_FILES = \
    COPYING \
    Doxyfile.in \
    README \
    configure.sh \
    knotter.desktop.in \
    fix_makefile.sh \
    knotter_info.pri \
    translations.pri \
    info_preprocessor.sh \
    windows.rc \
    AUTHORS

# configured directories

isEmpty(DATADIR){
    DATADIR=./data
}
isEmpty(DATAROOTDIR){
    DATAROOTDIR=.
}
isEmpty(BINDIR){
    BINDIR=.
}
isEmpty(MANDIR){
    MANDIR=./man
}


# cpp defines

DEFINES += "VERSION=\\\"$${VERSION}\\\""
DEFINES += "DOMAIN_NAME=\\\"$${DOMAIN}\\\""
DEFINES += "TARGET=\\\"$${TARGET}\\\""
DEFINES += "DATA_DIR=\\\"$${DATADIR}\\\""
DEFINES += "BUG_URL=\\\"$${BUGS}\\\""
DEFINES += "DOC_URL=\\\"$${DOC_URL}\\\""
DEFINES += "PLUGIN_REPO=\\\"$${PLUGIN_REPO}\\\""
contains(CONFIG,debug) {
    DEFINES += "DEBUG=1"
}

#DEFINES += "BUILD_INFO=\"\\\"Knotter $$VERSION\\nBuilt on $$_DATE_\\n$${QMAKE_HOST.os} \
#$${QMAKE_HOST.version} $${QMAKE_HOST.arch}\\nQt $${QT_VERSION}\\\""\"

HAS_QT_4_8=0
HAS_QT_5=0

!lessThan(QT_MAJOR_VERSION,4) !lessThan(QT_MINOR_VERSION,8) {
    HAS_QT_4_8=1
}
greaterThan(QT_MAJOR_VERSION, 4) {
    HAS_QT_4_8=1
    HAS_QT_5=1
    QT += widgets printsupport uitools
}
else {
    CONFIG += uitools
}

DEFINES += "HAS_QT_4_8=$${HAS_QT_4_8}" "HAS_QT_5=$${HAS_QT_5}"

contains(CONFIG,c++11) {
    DEFINES += CXX_11
}


#Extra make targets

#dist
MYDISTFILES =  \$(addprefix $$PWD/, $$OTHER_FILES $${TARGET}.pro ) \
               $${TARGET}.desktop
MYDISTDIRS  =  $$PWD/src $$PWD/data $$PWD/man

MYDIST_NAME = "$$TARGET-$${VERSION}"
MYDIST_TAR_GZ = "$${MYDIST_NAME}.tar.gz"
MYDIST_TMP = ".tmp/$${MYDIST_NAME}"
mydist.depends = $${TARGET}.desktop man/$${TARGET}.1.gz
                                                                            #
mydist.commands =                                                           \
        (                                                                   \
            $(CHK_DIR_EXISTS) $$MYDIST_TMP ||                               \
            $(MKDIR) $$MYDIST_TMP                                           \
        ) &&                                                                \
        $(COPY_FILE)  $$MYDISTFILES  $$MYDIST_TMP &&                        \
        $(COPY_DIR)   $$MYDISTDIRS   $$MYDIST_TMP &&                        \
        $(DEL_FILE) -f $$MYDIST_TMP/src/generated/*.h                       \
                       $$MYDIST_TMP/src/generated/*.cpp &&                  \
        (                                                                   \
            cd `dirname $$MYDIST_TMP`  &&                                   \
            $(TAR) $$MYDIST_TAR_GZ -a $$MYDIST_NAME                         \
        ) &&                                                                \
        $(MOVE) `dirname $$MYDIST_TMP`/$$MYDIST_TAR_GZ $$MYDIST_TAR_GZ &&   \
        $(DEL_FILE) -r $$MYDIST_TMP #&&                                      \
        #md5sum $$MYDIST_TAR_GZ >$${MYDIST_TAR_GZ}.md5

#distclean
mydistclean.depends = clean
mydistclean.commands = $(DEL_FILE) $$MYDIST_TAR_GZ Makefile $(TARGET)
QMAKE_EXTRA_TARGETS += mydist mydistclean

#src_doc
Doxyfile.commands = $$PWD/info_preprocessor.sh Doxyfile.in > Doxyfile
Doxyfile.depends = Doxyfile.in
src_doc.depends = Doxyfile FORCE
src_doc.commands = doxygen Doxyfile

#desktop
$${TARGET}.desktop.depends=$$PWD/$${TARGET}.desktop.in $$PWD/knotter_info.pri
$${TARGET}.desktop.commands=$$PWD/info_preprocessor.sh $$PWD/$${TARGET}.desktop.in > $${TARGET}.desktop

#man page
man/$${TARGET}.1.gz.depends=$$PWD/man/$${TARGET}.1.in $$PWD/knotter_info.pri
man/$${TARGET}.1.gz.commands=                                                  \
        (                                                                      \
            $(CHK_DIR_EXISTS) man ||                                           \
            $(MKDIR) man                                                       \
        ) &&                                                                   \
        $$PWD/info_preprocessor.sh $$PWD/man/$${TARGET}.1.in >man/$${TARGET}.1 && \
        gzip -f -9 man/$${TARGET}.1


QMAKE_EXTRA_TARGETS += src_doc Doxyfile $${TARGET}.desktop man/$${TARGET}.1.gz


# Installs

data.files = data/*
data.path = $${DATADIR}


desktop_file.files=$${TARGET}.desktop
desktop_file.path=$${DATAROOTDIR}/applications

man_page.files=man/$${TARGET}.1.gz
man_page.path=$${MANDIR}/man1

INSTALLS += data desktop_file man_page


target.path = $$BINDIR
INSTALLS += target





# Windows-specific stuff
win32 {
    # Remove non-numeric stuff from version as Windows RC doesn't like it
    VERSION ~= s/[-_a-zA-Z]+//
    RC_FILE = windows.rc
}
