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


# Script to generate binary rpm package
# env vars RPMEMAIL and RPMEMAIL are used, if empty set to upstream maintainer info
# WARNING This will edit files under ~/rpmbuild

set -ex

if  [ ! -x get_info.sh ] ; then
    ./configure.sh --prefix=/usr
fi

package=`./get_info.sh name`
version=`./get_info.sh version`
revision=1 #version of the rpm package
title=`./get_info.sh title`
desc=`./get_info.sh desc`
longdesc=`./get_info.sh long-desc`
website=`./get_info.sh website`
download=`./get_info.sh download`
maintainer=`./get_info.sh author`
maintainer_email=`./get_info.sh email`

tarball=$package-$version.tar.gz

if  [ -z "$RPMFULLNAME" ] ; then
    RPMEMAIL=$maintainer_email
    RPMFULLNAME=$maintainer
elif [ -z "$RPMEMAIL"  ] ; then
    echo 1>&2 "Please provide RPMEMAIL if you provide RPMFULLNAME"
    exit 1;
fi


rpmbuild=~/rpmbuild

rpmdev-setuptree
make dist
cp $tarball $rpmbuild/SOURCES

specs_file=$rpmbuild/SPECS/$package.spec
cat >$specs_file <<_SPECS_
Name:           $package
Version:        $version
Release:        $revision
Summary:        $desc

License:        GPLv3+
URL:            $website
Source0:        $download/$tarball

BuildRequires:  qt-devel, qtwebkit-devel
Requires:       qt, qtwebkit

%description
$longdesc


%prep
%setup -q


%build
./configure.sh --prefix=/usr
make %{?_smp_m_flags}


%install
rm -rf \$RPM_BUILD_ROOT
make install INSTALL_ROOT=\$RPM_BUILD_ROOT


%files
%{_mandir}/man1/$package.1.gz
%{_datadir}/$package
%{_bindir}/$package
%{_defaultdocdir}/$package

%changelog
* `date +"%a %b %d %Y"` $RPMFULLNAME <$RPMDATE> - $version-$revision
- Initial release
_SPECS_

rpmbuild -ba $specs_file

echo "You can find the packages as:"
find $rpmbuild/RPMS -name "$package*$version*.rpm"

echo "Run rpmlint on them for quality check"
