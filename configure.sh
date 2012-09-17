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
#prefix=/usr/local
#exec_prefix=$prefix
#bindir=${exec_prefix}/bin
#libdir=${exec_prefix}/lib
#includedir=${prefix}/include/$program_name
#datarootdir=${prefix}/share
#datadir=${datarootdir}/$program_name
#docdir=${datarootdir}/doc/$program_name
#mandir=${datarootdir}/man/$program_name

#initialize misc options
single_file=no
qmake_opts=""
with_boost=yes
tango=no


dirlist="prefix
         exec_prefix
         bindir
         libdir
         includedir
         datarootdir
         datadir
         docdir
         mandir"
desclist="Non-binary files root:
          Binary files root:
          Executable files:
          Linkable files:
          Header files:
          Data root:
          Data:
          Documentation:
          Man pages:"
deflist="/usr/local
        PREFIX
        EXEC_PREFIX/bin
        EXEC_PREFIX/lib
        PREFIX/include
        PREFIX/share
        PREFIX/share/$program_name
        PREFIX/share/doc/$program_name
        PREFIX/share/man"

#print directory option, description and current value
help_dirs() {
    desctail=$desclist
    deftail=$deflist
    echo "    Option:Description:Value"
    for key in $dirlist
    do
        desc=`echo $desctail | sed "s/:.*//" `
        desctail=`echo $desctail | sed "s/[^:]*://" `

        eval "val=\$$key"

        if [ -z "$val" ]; then
            val=`echo $deftail | sed "s/\s.*//" `
        fi
        deftail=`echo $deftail | sed "s/[^ ]*\s*//"`

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
    --without-boost     Disable dependency on Boost library
    --with-boost        Use Boost without running checks

Environment variables:
    QMAKE               Override qmake command
    CXX                 Override C++ compiler
    CXXFLAG             Flags to be passed to the C++ compiler
    LINK                Override linker
    LIBDIR              Additional library path

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
            tango=fallback
            ;;
        --with-tango-default)
            tango=default
            ;;
        --without-boost)
            with_boost=no
            ;;
        --with-boost)
            with_boost=force
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
        --docdir=*) docdir=$val;;
        --datadir=*) datadir=$val;;
        --mandir=*) mandir=$val;;
        --libdir=*) libdir=$val;;
        --includedir=*) includedir=$val;;
    esac
done

# set installation directories to default value if not overriden by the user
for dir in 'prefix=/usr/local' 'exec_prefix=$prefix' 'bindir=${exec_prefix}/bin' \
           'libdir=${exec_prefix}/lib' 'includedir=${prefix}/include/$program_name' \
           'datarootdir=${prefix}/share' 'datadir=${datarootdir}/$program_name' \
           'docdir=${datarootdir}/doc/$program_name' 'mandir=${datarootdir}/man'
do
    eval "usr_dir=\$${dir%%\=*}"
    if [ -z "$usr_dir" ]
    then
        eval "$dir"
    fi
done

#create a temporary file with given extension
mk_tempfile() {
    if [ -x "`which tempfile`" ] ; then
        tempfile -s $1
    else
        name=/tmp/file-`date +%s%N`$1
        touch $name
        echo $name;
    fi
}

#run a test on whether a library is found (using the C++ compiler)
#usage check_cxx_library name header [binary]
check_cxx_library() {

    name=$1
    header=$2
    binary=$3

    cxx_test=$CXX

    if [ -z "$cxx_test" ]; then
        if  which c++; then
            cxx_test=c++
        else
            echo 1>&2 "Cannot determine whether library $name exists!"
            return 1
        fi
    fi

    tempcxx=`mk_tempfile .cpp`
    tempobj=`mk_tempfile .o`

    cat >$tempcxx <<_HEADER_
#include <$header>
int main() { return 0; }
_HEADER_

    echo "Checking library $name..."

    if  ! $cxx_test $CXXFLAGS -c $tempcxx -o $tempobj 2>/dev/null ; then
        echo 1>&2 "    Header $header not found"
        rm -f $tempcxx $tempobj
        return 1
    else
        echo "    Header $header found"
    fi


    if [ -n "$binary" ] ; then

        tempbin=`mk_tempfile .out`

        if [ -z "$LINK" ]; then
            ld_test=$cxx_test
        else
            ld_test=$LINK
        fi

        if ! $ld_test -l$binary $tempobj -o $tempbin 2>/dev/null ; then
            echo 1>&2 "    Library $binary not found"
            rm -f $tempcxx $tempobj
            return 2
        else
            echo "    Library $binary found"
        fi
    fi

    rm -f $tempcxx $tempobj $tempbin
}


##### BEGIN CONFIGURATION #####

echo "Configuring $program_name $program_version"

echo "Using the following install directories:"
dir_status | column -t -s :

# determine the command to run qmake
if [ -n "$QMAKE" ]; then
    if $QMAKE -v 2>/dev/null; then
        echo "Using $QMAKE"
        qmake_opts="$qmake_opts QMAKE_QMAKE=$QMAKE"
    else
        echo >&2 "\$QMAKE is set to $QMAKE but not working"
        exit 1
    fi
elif qmake -v 2>/dev/null; then
    QMAKE=`which qmake`
    echo "Using $QMAKE"
elif qmake-qt4 -v 2>/dev/null; then
    QMAKE=`which qmake-qt4`
    echo "Using $QMAKE"
else
    echo >&2 "cannot find qmake"
    echo >&2 -n "add its directory to PATH or "
    echo >&2 "set the env variable QMAKE to the location of qmake executable"
    exit 1
fi

#misc env vars

if [ -n "$CXX" ]; then
    qmake_opts="$qmake_opts QMAKE_CXX=$CXX"
    echo "Using $CXX compiler"
fi

if [ -n "$CXXFLAGS" ]; then
    qmake_opts="$qmake_opts QMAKE_CXXFLAGS+=$CXXFLAGS"
    echo "Extra compiler flags: $CXXFLAGS"
fi

if [ -n "$LINK" ]; then
    qmake_opts="$qmake_opts QMAKE_LINK=$LINK"
    echo "Using $LINK linker"
fi

if [ -n "$LIBDIR" ]; then
    qmake_opts="$qmake_opts QMAKE_LIBDIR+=$LIBDIR"
    echo "Extra library path: $LIBDIR"
fi

#check dependencies
if [ "$with_boost" = "no" ] ; then
    echo "Boost is disabled"
    qmake_opts="$qmake_opts BOOST=no"
elif [ "$with_boost" = "force" ] ; then
    echo "Boost is enabled"
    qmake_opts="$qmake_opts BOOST=yes"
elif  check_cxx_library "Boost::program_options" boost/program_options.hpp boost_program_options ; then
    qmake_opts="$qmake_opts BOOST=yes"
else
    echo "Dependency from Boost::program_options has been disabled"
    qmake_opts="$qmake_opts BOOST=no"
fi

#clean old make output
if [ -f Makefile ]
then
    echo "Cleaning up old build"
    make -s distclean || rm Makefile
fi

qmake_opts="$qmake_opts BINDIR=$bindir DATADIR=$datadir DOCDIR=$docdir \
            MANDIR=$mandir DATAROOTDIR=$datarootdir TANGO=$tango"
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
if ./fix_makefile.sh
then
    echo "Makefile has been generated"
else
    echo  >&2 "Error when renaming dist target"
    echo  >&2 "make may work but make dist will fail"
    exit 1
fi

cat >get_info.sh <<_GET_DIR_
#!/bin/sh
# This file has been generated by configure.sh
# It will output the installation directories and other program info
# Usage: get_info.sh info_type
# where info_type is one of prefix, bindir etc or name, version etc.

case \$1 in
    name) echo $program_name ;;
    version) echo $program_version ;;
    author) echo "Mattia Basaglia" ;;
    email) echo "mattia.basaglia@gmail.com" ;;
    title) echo Knotter ;;
    desc) echo "Celtic knot editor" ;;
    long-desc) echo "Knotter is an editor for interlace patterns."
               echo " Knots drawn with Knotter can be exported as SVG or raster images."
    ;;
    website) echo "https://sourceforge.net/projects/knotter/" ;;
    download) echo "http://sourceforge.net/projects/knotter/files/latest/download" ;;
    icon) echo "$datadir/img/$program_name-logo-big.svg" ;;
_GET_DIR_

for key in $dirlist
do
    eval "val=\$$key"
    echo "    $key) echo $val ;;" >>get_info.sh
done

echo "esac" >>get_info.sh
chmod a+x get_info.sh
