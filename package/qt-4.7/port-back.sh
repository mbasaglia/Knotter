#!/bin/bash

set -ex

srcdir=../..
$srcdir/configure.sh
package=`echo __package__-__version__ | $srcdir/info_preprocessor.sh` 
make dist
if [ -d $package ]
then
    rm -rf $package
fi
tar -xaf $package.tar.gz
cd $package
./configure.sh

make compiler_uic_make_all
mkdir src/ui
mv src/generated/ui_*.h src/ui
rmdir src/generated obj

find -name '*.ui' -path './src/*' -delete

cd src/ui

cat >ui.pri <<_PRI_
INCLUDEPATH += \$\$PWD
HEADERS += \\
_PRI_

for uih in ui_*
do
    echo "\$\$PWD/$uih \\" >>ui.pri
    sed -i -e s/QStringLiteral/QString/g -e s~QtWidgets/~~g $uih
done
echo  >>ui.pri

cd ..
cat >>src.pri  <<_PRI_

include(\$\$PWD/ui/ui.pri)

_PRI_


find -name '*.pri' -exec sed -i s/.*\\.ui.*//g {} \;

cd ../..
tar -caf $package-qt4.7.tar.gz $package
