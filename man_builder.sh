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

# This script creates the man page for Knotter

rm -fr man
mkdir man 

package=`./get_info.sh name`
version=`./get_info.sh version`
title=`./get_info.sh title`
desc=`./get_info.sh desc`
longdesc=`./get_info.sh long-desc`
website=`./get_info.sh website`

maintainer=`./get_info.sh author`
maintainer_email=`./get_info.sh email`

curr_date=`date --rfc-3339=date`

man_section=1

cat >man/$package.$man_section <<_MAN_
.TH $package $man_section $curr_date $version
.SH NAME
$title \- $desc
.SH SYNOPSIS
.B $package
[OPTION]... [INFILE]
.SH DESCRIPTION
$longdesc
.SH OPTIONS
.IP --help
Show help message and exit
.IP --version
Show version info and exit
.SH AUTHOR
$maintainer <$maintainer_email>
_MAN_

gzip -9 man/$package.$man_section
