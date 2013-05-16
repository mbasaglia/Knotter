#!/bin/bash

srcdir=../..
QMAKE=i686-pc-mingw32-qmake $srcdir/configure.sh
package=`echo __package__-__version__ | $srcdir/info_preprocessor.sh`

if [ -d $package ]
then
  rm -rf $package
fi
mkdir $package

if [ -f $package.zip ]
then
  rm -f $package.zip
fi

make || exit

cp release/knotter.exe $package
cp -R $srcdir/data $package
zip -r $package.zip $package