#!/bin/bash

archbit=$1

if [ -z "$archbit" ]
then
    echo 1>&2 "Please pass 32 or 64 to select the architecture"
    exit 1
fi

srcdir=../..
QMAKE=qmake-qt5-windows-$archbit $srcdir/configure.sh
package=`echo __package__-__version__ | $srcdir/info_preprocessor.sh`

if [ -d $package ]
then
  rm -rf $package
fi
mkdir $package

if [ -f $package-windows.zip ]
then
  rm -f $package-windows.zip
fi

make || exit

cp release/knotter.exe $package
cp -R $srcdir/data $package
zip -r $package-windows-$archbit.zip $package