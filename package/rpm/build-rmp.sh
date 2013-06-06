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


# Script to generate binary rpm package
# env vars RPMEMAIL and RPMEMAIL are used, if empty set to upstream maintainer info
# WARNING This will edit files under ~/rpmbuild

set -ex

srcdir=../..
source $srcdir/knotter_info.pri
revision=1 # version of rpm package

tarball=$TARGET-$VERSION.tar.gz

if  [ -z "$RPMFULLNAME" ] ; then
    RPMEMAIL=$EMAIL
    RPMFULLNAME=$AUTHOR
elif [ -z "$RPMEMAIL"  ] ; then
    echo 1>&2 "Please provide RPMEMAIL if you provide RPMFULLNAME"
    exit 1;
fi


rpmbuild=~/rpmbuild

rpmdev-setuptree
( cd $srcdir && ./configure.sh --prefix=/usr && make dist )
cp $srcdir/$tarball $rpmbuild/SOURCES

specs_file=$rpmbuild/SPECS/$TARGET.spec
trunc_desc=`echo $LONG_DESC | sed s/\\\\./.\\\\n/g`
cat >$specs_file <<_SPECS_
Name:           $TARGET
Version:        $VERSION
Release:        $revision
Summary:        $DESC

License:        GPLv3+
URL:            $WEBSITE
Source0:        $DOWNLOAD/$tarball

BuildRequires:  qt-devel
Requires:       qt

%description
$trunc_desc


%prep
%setup -q


%build
./configure.sh --prefix=/usr
make %{?_smp_m_flags}


%install
rm -rf \$RPM_BUILD_ROOT
make install INSTALL_ROOT=\$RPM_BUILD_ROOT


%files
%{_datadir}/applications/$TARGET.desktop
%{_datadir}/$TARGET
%{_bindir}/$TARGET
%{_mandir}/man1/$TARGET.1.gz

%changelog
* `date +"%a %b %d %Y"` $RPMFULLNAME <$RPMEMAIL> - $VERSION-$revision
- Initial release
_SPECS_

rpmbuild -ba $specs_file

echo "You can find the packages as:"
find $rpmbuild -name "$TARGET*$VERSION*.rpm"

echo "Run rpmlint on them for quality check"
