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

# This script creates the man page for Knotter from the docbook file
# WARNING this script may not be very portable

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
man_section_desc="User Commands"

docbook_file=man/$package.$man_section.xml
cat >$docbook_file <<_DOCBOOK_
<?xml version="1.0"?>
<!DOCTYPE refentry PUBLIC "-//OASIS//DTD DocBook XML V4.4//EN"
                   "http://www.oasis-open.org/docbook/xml/4.4/docbookx.dtd">
<refentry xmlns:xlink="http://www.w3.org/1999/xlink" xml:id="man_page" >
<?dbhtml filename="man.htm" ?>

    <refmeta>
        <refentrytitle>$package</refentrytitle>
        <manvolnum>$man_section</manvolnum>
        <refmiscinfo class='source'>$version</refmiscinfo>
        <refmiscinfo class='manual'>$man_section_desc</refmiscinfo>
    </refmeta>

    <refnamediv>
        <refname>$package</refname>
        <refpurpose>$desc</refpurpose>
    </refnamediv>

    <refsynopsisdiv>
        <cmdsynopsis>
            <command>$package</command>
            <arg choice='opt' rep='repeat'><replaceable>OPTION</replaceable></arg>
            <arg choice='opt'><replaceable>INFILE</replaceable></arg>
        </cmdsynopsis>
    </refsynopsisdiv>

    <refsect1>
        <title>DESCRIPTION</title>
        <para>$longdesc</para>
    </refsect1>

    <refsect1>
        <title>OPTIONS</title>
        <variablelist>
            <varlistentry>
                <term>--help</term>
                <term>-h</term>
                <listitem>
                    <para>Show help message and exit</para>
                </listitem>
            </varlistentry>
            <varlistentry>
                <term>--version</term>
                <term>-v</term>
                <listitem>
                    <para>Show version info and exit</para>
                </listitem>
            </varlistentry>
        </variablelist>
    </refsect1>

    <refsect1>
        <title>AUTHOR</title>
        <para>$maintainer &lt;$maintainer_email&gt;</para>
    </refsect1>

    <refsect1>
        <title>SEE ALSO</title>
        <para>$title website <link xlink:href="$website">$website</link></para>
    </refsect1>

</refentry>
_DOCBOOK_

if  [ ! -x `which xsltproc` ] ; then
    echo 1>&2 "Cannot convert DocBook file"
    exit 1
fi

#style_url() {
#    if  [ -z "$1" ] ; then
#        return;
#    fi
#    base=/usr/share/xml/docbook/stylesheet/docbook-xsl
#    echo $base/$1/docbook.xsl
#}
#xsltproc -o man/ `style_url manpages` $docbook_file
#xsltproc --stringparam html.stylesheet style.css \
#         --stringparam refentry.generate.title 1 \
#         --stringparam refentry.separator 1 \
#         `style_url html` $docbook_file >user_guide/man.htm


xmlto --skip-validation man $docbook_file -o man

xmlto --skip-validation \
        --stringparam html.stylesheet=style.css \
        --stringparam base.dir="user_guide/" \
        --stringparam htmlhelp.button.home=1 \
        html $docbook_file
text=`echo "<!DOCTYPE html>" | cat - user_guide/man.htm`
echo "$text" >user_guide/man.htm

sed -ri user_guide/man.htm \
    -e 's/<\/body>/<a href="index.htm">Back<\/a>\0/'



# old Groff:
#cat >man/$package.$man_section <<_MAN_
#.TH $package $man_section $curr_date $version
#.SH NAME
#$title \- $desc
#.SH SYNOPSIS
#.SY $package
#.RI [ OPTION "...]"
#.RI [ INFILE ]
#.YS
#.SH DESCRIPTION
#$longdesc
#.SH OPTIONS
#.IP --help
#Show help message and exit
#.IP --version
#Show version info and exit
#.SH AUTHOR
#$maintainer <$maintainer_email>
#_MAN_

gzip -9 man/$package.$man_section
