#!/bin/bash

cd "/Users/jonasvanderaa/school/Gestructureerd programmeren/programmeeropdracht/" &&
gcc -Os -o programmeeropdracht main.c -lpanel -lmenu -lncurses_g -L/sw/lib/ncurses/;