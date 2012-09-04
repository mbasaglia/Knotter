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
VERSION = 0.6.1debug


TEMPLATE = app


UI_DIR = src/generated
MOC_DIR = src/generated
RCC_DIR = src/generated

include(src/src.pri)
include(lib/lib.pri)
include(translations/translations.pri)

OTHER_FILES = \
    COPYING \
    Doxyfile \
    NEWS \
    doc.dox \
    README \
    configure.sh \
    AUTHORS \
    deb_builder.sh \
    fix_makefile.sh \
    man_builder.sh

DEFINES += "VERSION=\\\"$${VERSION}\\\""

DEFINES += "BUILD_INFO=\"\\\"Knotter $$VERSION\\nBuilt on $$_DATE_\\n$${QMAKE_HOST.os} \
$${QMAKE_HOST.version} $${QMAKE_HOST.arch}\\nQt $${QT_VERSION}\\\""\"

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -Werror -Wall -Wextra
}


!lessThan(QT_VERSION,"4.8"){
    DEFINES += HAS_QT_4_8
}

contains(BOOST,no) {
    DEFINES += NO_BOOST
} else {
    LIBS += -lboost_program_options
}

#Extra make targets

#dist
MYDISTFILES =  $$OTHER_FILES knotter.pro
MYDISTDIRS  =  src lib img translations user_guide man

MYDIST_NAME = "$$TARGET-$${VERSION}"
MYDIST_TAR = "$${MYDIST_NAME}.tar"
MYDIST_TAR_GZ = "$${MYDIST_TAR}.gz"
MYDIST_TMP = ".tmp/$${MYDIST_NAME}"
mydist.depends = man
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

#distclean
mydistclean.depends = clean
mydistclean.commands = $(DEL_FILE) $$MYDIST_TAR_GZ Makefile $(TARGET)
QMAKE_EXTRA_TARGETS += mydist mydistclean

#src_doc
Doxyfile.commands = doxygen -g
src_doc.depends = Doxyfile FORCE
src_doc.commands = sed s/KNOTTER_VERSION/$${VERSION}/ Doxyfile | doxygen -

#man
man.depends = man_builder.sh FORCE
man.commands = ./man_builder.sh

QMAKE_EXTRA_TARGETS += src_doc Doxyfile man


#check directories and options from configure.sh

contains (TANGO,default){
    DEFINES += TANGO_DEFAULT
    message("Using Tango icons")
}
contains(TANGO,fallback){
    DEFINES += TANGO_FALLBACK
    message("Using Tango icons as fallback")
}

contains(SINGLE_FILE,yes) {
    BINDIR=.
    DATADIR=:/data
    DOCDIR=:/doc
    MANDIR=man
    message("Compiling all data in a single executable file")
    RESOURCES += data.qrc

    !isEmpty(TANGO){
        RESOURCES += themes/tango-icons/tango.qrc
        message("Tango icons included in the executable as well")
    }

} else {
    isEmpty(DATADIR){
        DATADIR=.
    }
    isEmpty(DOCDIR){
        DOCDIR=.
    }
    isEmpty(MANDIR){
        MANDIR=man
    }


    img.files = img/*
    img.path = $${DATADIR}/img

    doc.files = user_guide/*
    doc.path = $${DOCDIR}/user_guide

    translations.files = translations/*.qm
    translations.path = $${DATADIR}/translations

    man.files = man/$${TARGET}.1.gz
    man.path = $${MANDIR}/man1

    INSTALLS += img doc translations man


    !isEmpty(TANGO){
        tango_icons.files=themes/*
        tango_icons.path = $${DATADIR}/themes
        INSTALLS += tango_icons
    }

}

isEmpty(BINDIR){
    BINDIR=.
}


DEFINES += "DATA_DIR=\\\"$${DATADIR}\\\""

DEFINES += "DOC_DIR=\\\"$${DOCDIR}\\\""

target.path = $$BINDIR
INSTALLS += target

RESOURCES +=

win32 {
    VERSION ~= s/[-_a-zA-Z]+//
}

