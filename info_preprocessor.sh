#!/bin/bash
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

# This script processes a file and replaces __placeholder__ with the proper value
# Usage: info_preprocessor.sh file
# The result is sent to stdout
# Values:
# name      Program name (lower case)
# package   Program name (lower case)
# title     Program name (capitalized)
# version   Program version
# author    Maintainer name
# email     Maintainer email
# desc      Short description
# long_desc Long description
# download  Direct link to the latest version
# repo      Repo browser URL
# icon      Program icon (for desktop file and such)

src_dir=`dirname ${BASH_SOURCE[0]}`

if [ -x configured_directories.sh ] ; then
    datadir=`./configured_directories.sh datadir`
else
    datadir=$src_dir/data
fi


# get package info
source $src_dir/knotter_info.pri


if  [ ! -f "$1" -a ! -z "$1" ] ; then
    echo 1>&2 "$1 not found"
    exit 1
else
    sed -e "s/__name__/$TARGET/g" \
        -e "s/__package__/$TARGET/g" \
        -e "s/__version__/$VERSION/g" \
        -e "s/__author__/$AUTHOR/g" \
        -e "s/__email__/$EMAIL/g" \
        -e "s/__title__/$TITLE/g" \
        -e "s/__desc__/$DESC/g" \
        -e "s/__long_desc__/$LONG_DESC/g" \
        -e "s,__website__,$WEBSITE,g" \
        -e "s,__download__,$DOWNLOAD,g" \
        -e "s,__repo__,$REPO,g" \
        -e "s,__icon__,$datadir/$ICON_RELPATH,g" \
        $1
fi
