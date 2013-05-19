#!/bin/bash
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

# Script to generate binary deb package
# env vars DEBFULLNAME and DEBEMAIL are used, if empty set to upstream maintainer info
# NOTE You  should run this script as root or (or you'll have wrong install file permission)
# ./build-deb.sh 5 will compile for qt 5

#set -ex

user_name=`whoami`
user_id=`id -u $user_name`
if [ $user_id -ge 100 ] ; then
    echo 1>&2 "Error: Run this as fakeroot"
    exit 1;
fi


#distro=`lsb_release -a 2>/dev/null | grep Description | sed -r -e 's/Description:\s+//' -e 's/ /_/g'`

srcdir=../..
source $srcdir/knotter_info.pri
revision=2 # version of deb package
architecture=`dpkg-architecture -qDEB_BUILD_ARCH`
package=$TARGET-$VERSION
qt_version=$1
if [ -z "$1" ]
then
    qt_version=4
elif [ $qt_version = 5 ]
then
    revision=$revision-qt5
fi

if  [ -z "$DEBFULLNAME" ] ; then
    DEBEMAIL=$EMAIL
    DEBFULLNAME=$AUTHOR
elif [ -z "$DEBEMAIL"  ] ; then
    echo 1>&2 "Please provide DEBEMAIL if you provide DEBFULLNAME"
    exit 1;
fi


rm -rf $package
mkdir -p $package/DEBIAN


QMAKE=qmake-qt$qt_version $srcdir/configure.sh --prefix=/usr
echo "Compiling $TARGET $VERSION $architecture Qt$qt_version..."
make >/dev/null
echo "Creating package data"
export INSTALL_ROOT=$package
make install >/dev/null

make $TARGET.desktop
cp   $TARGET.desktop $package/usr/share/applications/$TARGET.desktop

# create control file
control_file=$package/DEBIAN/control
trunc_ver=`echo $VERSION | sed s/_/-/g`
trunc_desc=`echo $LONG_DESC | sed "s/\\\\./.\\\\n/g"`
inst_size=`du -s $package/usr/ | grep -Eo '^[0-9]+'`
trunc_size=`echo "$inst_size/1024" | bc -q`
echo Creating $control_file
cat >$control_file <<_CONTROL_
Package: $TARGET
Version: $trunc_ver-$revision
Homepage: $WEBSITE
Maintainer: $DEBFULLNAME <$DEBEMAIL>
Depends: libqt$qt_version-svg, libqt$qt_version-xml, libqtcore$qt_version, libqtgui$qt_version, libstdc++6, libc6
Section: graphics
Priority: optional
Architecture: $architecture
Installed-Size: $trunc_size
Pre-Depends: 
Recommends: 
Suggests: inkscape
Enhances: 
Replaces: 
Conflicts:
Description: $DESC
 $trunc_desc
_CONTROL_



#create menu/desktop files
dataroot=`./configured_directories.sh datarootdir`
bindir=`./configured_directories.sh bindir`
datadir=`./configured_directories.sh datadir`

menu_icon=/usr/share/pixmaps/$TARGET.xpm
mkdir -p $package/usr/share/pixmaps
cp $srcdir/data/img/$TARGET.xpm $package/usr/share/pixmaps


mkdir -p $package/$dataroot/menu
menu_file=$package/$dataroot/menu/$package
echo Creating $menu_file
cat >$menu_file <<_MENU_
?package($TARGET): needs="X11" section="Applications/Graphics" title="$TITLE" command="$bindir/$TARGET" longtitle="$DESC" icon="$menu_icon"
_MENU_


postinst_file=$package/DEBIAN/postinst
cat >$postinst_file <<_POST_INST_
#!/bin/sh
set -e
if [ -x \`which update-menus\` ] ; then update-menus ; fi
_POST_INST_
chmod 755 $postinst_file


copyright_file=$package/usr/share/doc/$TARGET/copyright
mkdir -p $package/usr/share/doc/$TARGET
echo Creating $copyright_file
cat >$copyright_file <<_COPYRIGHT_
Format: http://dep.debian.net/deps/dep5
Upstream-Name: $TITLE
Upstream-Contact: $AUTHOR <$EMAIL>

Source: $WEBSITE

Files: *
Copyright: $COPY_YEARS $AUTHOR <$EMAIL>
License: GPL-3.0+

License: GPL-3.0+
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 .
 This package is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 .
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 .
 On Debian systems, the complete text of the GNU General
 Public License version 3 can be found in "/usr/share/common-licenses/GPL-3".
_COPYRIGHT_


changelog_file=$package/usr/share/doc/$TARGET/changelog.Debian
changelog_temp_file=changelog
echo Creating $changelog_file
cat >$changelog_temp_file <<_COPYRIGHT_
$TARGET ($VERSION-$revision) `lsb_release -cs 2>/dev/null || echo unknown`; urgency=low

  [ $DEBFULLNAME ]
  * Initial Release.

 -- $DEBFULLNAME <$DEBEMAIL>  `date -R`

_COPYRIGHT_
#git-dch --ignore-branch 2>/dev/null #does this ever work?
mkdir -p $package/usr/share/doc/$TARGET/
cp $changelog_temp_file $changelog_file
gzip -9 $changelog_file

echo "Setting file permission"

find $package/usr/ -type d -exec chmod 755 {} \;
find $package/usr/ -type f -exec chmod 644 {} \;
find $package/usr/ -type f -path '*/bin/*' -exec chmod 755 {} \;
find $package/$dataroot -type f -name '*.sh' -delete
find $package/$dataroot -type f -name 'COPYING' -delete

echo "Creating package"

debname="${TARGET}_${VERSION}-${revision}_${architecture}.deb"
dpkg-deb --build $package $debname


echo "You can find the package as $debname"

echo "Run \"lintian $debname\" for quality check" # see http://lintian.debian.org/tags.html


