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


# Script to generate binary deb package
# env vars DEBFULLNAME and DEBEMAIL are used, if empty set to upstream maintainer info
# NOTE You  should run this script as root (or you'll have wrong install file permission)


user_name=`whoami`
user_id=`id -u $user_name`
if [ $user_id -ge 100 ] ; then
    echo 1>&2 "Note: You should run this as root"
fi


if  [ ! -x get_info.sh ] ; then
    ./configure.sh --prefix=/usr
fi

package=`./get_info.sh name`
version=`./get_info.sh version`
revision=2 #version of the deb package, set to 2 to avoid lintian warning
title=`./get_info.sh title`
desc=`./get_info.sh desc`
longdesc=`./get_info.sh long-desc`
website=`./get_info.sh website`

architecture=`dpkg-architecture -qDEB_BUILD_ARCH`

copyright_years=2012
maintainer=`./get_info.sh author`
maintainer_email=`./get_info.sh email`

if  [ -z "$DEBFULLNAME" ] ; then
    DEBEMAIL=$maintainer_email
    DEBFULLNAME=$maintainer
elif [ -z "$DEBEMAIL"  ] ; then
    echo 1>&2 "Please provide DEBEMAIL if you provide DEBFULLNAME"
    exit 1;
fi


rm -rf debian
mkdir -p debian/$package/DEBIAN

echo "Compiling $package $version $architecture..."
make man >/dev/null
make >/dev/null

echo "Creating package data"

export INSTALL_ROOT=debian/$package
make install >/dev/null

# create control file
control_file=debian/$package/DEBIAN/control
echo Creating $control_file
cat >$control_file <<_CONTROL_
Package: $package
Version: $version-$revision
Homepage: $website
Maintainer: $DEBFULLNAME <$DEBEMAIL>
Depends: libqt4-svg, libqt4-xml, libqtwebkit4, libqtcore4, libqtgui4, libstdc++6, libc6
Section: graphics
Priority: optional
Architecture: $architecture
Pre-Depends: 
Recommends: 
Suggests: inkscape
Enhances: 
Replaces: 
Conflicts:
Description: $desc
 $longdesc
_CONTROL_


#create menu/desktop files

dataroot=`./get_info.sh datarootdir`
bindir=`./get_info.sh bindir`
datadir=`./get_info.sh datadir`
icon=$datadir/img/knotter-logo-big.svg
docdir=`./get_info.sh docdir`


menu_icon=/usr/share/pixmaps/$package.xpm
mkdir -p debian/$package/usr/share/pixmaps
cp img/$package.xpm debian/$package/usr/share/pixmaps

mkdir -p debian/$package/$dataroot/menu
menu_file=debian/$package/$dataroot/menu/$package
echo Creating $menu_file
cat >$menu_file <<_MENU_
?package($package): needs="X11" section="Applications/Graphics" title="$title" command="$bindir/$package" longtitle="$desc" icon="$menu_icon"
_MENU_

postinst_file=debian/$package/DEBIAN/postinst
cat >$postinst_file <<_POST_INST_
#!/bin/sh
set -e
if [ -x \`which update-menus\` ] ; then update-menus ; fi
_POST_INST_
chmod a+x $postinst_file

mkdir -p debian/$package/$dataroot/applications
desktop_file=debian/$package/$dataroot/applications/$package.desktop
echo Creating $desktop_file
cat >$desktop_file <<_DESKTOP_
[Desktop Entry]
Version=$version
Type=Application
Name=$title
Comment=$desc
Exec=$package
TryExec=$package
Icon=$icon
Terminal=false
Categories=Graphics;VectorGraphics;Qt
_DESKTOP_

copyright_file=debian/$package/usr/share/doc/$package/copyright
mkdir -p debian/$package/usr/share/doc/$package
echo Creating $copyright_file
cat >$copyright_file <<_COPYRIGHT_
Format: http://dep.debian.net/deps/dep5
Upstream-Name: $package
Upstream-Contact: $maintainer <$maintainer_email>

Source: $website

Files: *
Copyright: $copyright_years $maintainer <$maintainer_email>
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


changelog_file=debian/$package/usr/share/doc/$package/changelog.Debian
changelog_temp_file=debian/changelog
echo Creating $changelog_file
cat >$changelog_temp_file <<_COPYRIGHT_
$package ($version-$revision) `lsb_release -cs 2>/dev/null || echo unknown`; urgency=low

  [ $DEBFULLNAME ]
  * Initial Release.

 -- $DEBFULLNAME <$DEBEMAIL>  `date -R`

_COPYRIGHT_
#git-dch --ignore-branch 2>/dev/null #does this ever work?
cp $changelog_temp_file $changelog_file
gzip -9 $changelog_file


cd debian

dpkg-deb --build $package
debname="${package}_${version}-${revision}_${architecture}.deb"
mv "debian/$package.deb" "debian/$debname";

echo "You can find the package as debian/$debname"

echo "Run \"lintian debian/$debname\" for quality check" # see http://lintian.debian.org/tags.html
