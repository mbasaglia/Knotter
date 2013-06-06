#!/bin/bash 

# script to update all code in one command
git pull origin master && git submodule update --init \
    && git submodule foreach git pull origin master 
