#!/bin/bash
for sizedir in * ; do
    if [ -d $sizedir ]; then
        echo "Entering $sizedir";
        cd $sizedir
        for catdir in * ; do
            if [ -d $catdir ]; then
                echo "Entering $catdir";
                cd $catdir
                for file in * ; do
                    if [ -h "$file" ]; then
                        rm -f $file
                    fi
                done
                cd ..
            fi
        done
        cd ..
    fi
done