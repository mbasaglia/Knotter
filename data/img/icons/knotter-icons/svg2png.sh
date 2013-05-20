#!/bin/bash

for size in 16 22 32 48
do
    if [ ! -d custom_$size ]
    then
        mkdir custom_$size
    fi
    
    for file in custom/*.svg
    do
        out=`echo $file | sed -e s~custom/~custom_$size/~ -e s/.svg/.png/ `
        echo $file - $out 
        inkscape $file -h $size -w $size -e $out
    done
done
