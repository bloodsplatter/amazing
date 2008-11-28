#!/bin/bash

gcc -g -Os -o programmeeropdracht main.c -lpanel -lmenu -lncurses_g -L/sw/lib/ncurses/ && echo "Build done";
