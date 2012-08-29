#!/bin/sh
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

#project file, used to detect program name and version and passed to qmake
qmake_pro_file=knotter.pro

#initialize values from project files
get_project_var() {
    grep -E "^\s*$1\s*=\s*([-a-zA-Z0-9._]*)" $qmake_pro_file | sed "s/\s*$1\s*=\s*//"
}
program_name=`get_project_var TARGET`
program_version=`get_project_var VERSION`

#initialize default directories
prefix=/usr/local
exec_prefix=$prefix
bindir=${exec_prefix}/bin
libdir=${exec_prefix}/lib
includedir=${prefix}/include/$program_name
datarootdir=${prefix}/share
datadir=${datarootdir}/$program_name
docdir=${datarootdir}/doc/$program_name
mandir=${datarootdir}/man/$program_name

#initialize misc options
single_file=no
qmake_opts=""

dirlist="prefix exec_prefix bindir libdir includedir datarootdir datadir docdir mandir"
desclist="Non-binary files root:Binary files root:Executable files:Linkable files:\
Header files:Data root:Data:Documentation:Man pages:"


#print directory option, description and current value
help_dirs() {
    desctail=$desclist
    echo "    Option:Description:Current Value"
    for key in $dirlist
    do
        desc=`echo $desctail | sed "s/:.*//" `
        desctail=`echo $desctail | sed "s/[^:]*://" `
        eval "val=\$$key"
        echo "    --$key=DIR:$desc:$val"
    done
}

#print the help message
show_help() {
cat <<_HELP_
Configuration script for $program_name $program_version

Usage: $0 [OPTION]... [VARIABLE=VALUE]...

Options:
    --help -h           Show this help message and exit
    --single-file       Enable compilation of data in the executable file.
                        Will override installation directories to point to
                        the internal resource location.
    --name              Print the program name ($program_name) and exit.
    --version           Print the program version ($program_version) and exit.

Installation directories:
_HELP_
    help_dirs | column -t -s :

cat <<_HELP_

Optional components:
    --without-webkit    Remove the dependency on QtWebkit
    --with-tango        Use Tango icons as fallback if no system theme is found
    --with-tango-default Use Tango icons as default icon theme

Environment variables:
    QMAKE               Override qmake command
    CXX                 Override C++ compiler
    LINK                Override linker

_HELP_
}

#print directory description and value
dir_status() {
    desctail=$desclist
    for key in $dirlist
    do
        desc=`echo $desctail | sed "s/:.*//" `
        desctail=`echo $desctail | sed "s/[^:]*://" `
        eval "val=\$$key"
        echo "$desc:$val"
    done
}

#parse input parameters
for arg in $*
do
    case $arg in
        *=*)
            val=`echo $arg | sed 's/.*=//'`
            ;;
        --help | -h)
            show_help
            exit 0
            ;;
        --single-file)
            single_file=yes
            qmake_opts="$qmake_opts SINGLE_FILE=yes"
            ;;
        --version)
            echo $program_version
            exit 0
            ;;
        --name)
            echo $program_name
            exit 0
            ;;
        --without-webkit)
            qmake_opts="$qmake_opts WEBKIT=no"
            ;;
        --with-tango)
            qmake_opts="$qmake_opts TANGO=fallback"
            ;;
        --with-tango-default)
            qmake_opts="$qmake_opts TANGO=default"
            ;;
        *)
            echo "Ignoring option $arg"
            ;;
    esac

    case $arg in
        --prefix=*) prefix=$val;;
        --exec_prefix=*) exec_prefix=$val;;
        --bindir=*) bindir=$val;;
        --datarootdir=*) datarootdir=$val;;
        --datadir=*) datadir=$val;;
        --mandir=*) mandir=$val;;
        --libdir=*) libdir=$val;;
        --includedir=*) includedir=$val;;
    esac
done

echo "Configuring $program_name $program_version"

echo "Using the following install directories:"
dir_status | column -t -s :

# determine the command to run qmake
if [ -n "$QMAKE" ]; then
    if $QMAKE -v 2>/dev/null; then
        echo "Using $QMAKE"
        qmake_opts="$qmake_opts QMAKE_QMAKE=$QMAKE"
    else
        echo >&2 "\$QMAKE is set to QMAKE but not working"
        exit 1
    fi
elif qmake -v 2>/dev/null; then
    QMAKE=qmake
    echo "Using qmake"
elif qmake-qt4 -v 2>/dev/null; then
    QMAKE=qmake-qt4
    echo "Using qmake-qt4"
else
    echo >&2 "cannot find qmake"
    echo >&2 -n "add its directory to PATH or "
    echo >&2 "set the env variable QMAKE to the location of qmake executable"
    exit 1
fi

#minc env vars
if [ -n "$CXX" ]; then
    qmake_opts="$qmake_opts QMAKE_CXX=$CXX"
    echo "Using $CXX compiler"
fi

if [ -n "$LINK" ]; then
    qmake_opts="$qmake_opts QMAKE_LINK=$LINK"
    echo "Using $LINK linker"
fi

#clean old make output
if [ -f Makefile ]
then
    echo "Cleaning up old build"
    make -s distclean || rm Makefile
fi

qmake_opts="$qmake_opts BINDIR=$bindir DATADIR=$datadir DOCDIR=$docdir "
qmake_command="$QMAKE $qmake_opts $qmake_pro_file"
echo $qmake_command
if $qmake_command && [ -f Makefile ]
then
    echo "Initial Makefile generated"
else
    echo >&2 "qmake failed"
    exit 1
fi

#fix qmake buggy dist target
if sed -i -e "s/^dist/buggydist/g" -e "s/mydist/dist/g" Makefile
then
    echo "Makefile has been generated"
else
    echo  >&2 "Error when renaming dist target"
    echo  >&2 "make may work but make dist will fail"
    exit 1
fi
