#!/bin/bash

for file in custom/*.svg
do
	out=`echo $file | sed -e s~custom/~custom_22/~ -e s/.svg/.png/ `
	echo $file - $out 
	inkscape $file -h 22 -w 22 -e $out
done

