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

# This script processes a file and replaces __placeholder__ with the value from get_info.sh
# Usage: info_preprocessor.sh file
# The result is sent to stdout


if [ ! -x get_info.sh ] ; then
    ./configure.sh
fi

get_info() {
    ./get_info.sh $1 | sed -e 's/\//\\\//g' -e ':a;N;$!ba;s/\n/\\n/g'
}

name=`get_info name`
version=`get_info version`
author=`get_info author`
email=`get_info email`
title=`get_info title`
desc=`get_info desc`
long_desc=`get_info long-desc`
website=`get_info website`
icon=`get_info icon`


if  [ ! -f "$1" ] ; then
    echo 1>&2 "$1 not found"
    exit 1
else
    sed -e "s/__name__/$name/g" \
        -e "s/__package__/$name/g" \
        -e "s/__version__/$version/g" \
        -e "s/__author__/$author/g" \
        -e "s/__email__/$email/g" \
        -e "s/__title__/$title/g" \
        -e "s/__desc__/$desc/g" \
        -e "s/__long_desc__/$long_desc/g" \
        -e "s/__website__/$website/g" \
        -e "s/__icon__/$icon/g" \
        $1
fi
