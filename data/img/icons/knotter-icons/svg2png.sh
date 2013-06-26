#!/bin/bash

for size in 16 22 32 48
do
    if [ ! -d custom/${size}x${size} ]
    then
        mkdir custom/${size}x${size}
    fi
    
    for file in custom/scalable/*.svg
    do
        out=`echo $file | sed -e s~scalable/~${size}x${size}/~ -e s/.svg/.png/ `
        echo $file - $out 
        inkscape $file -h $size -w $size -e $out
    done
done
