#!/bin/bash
#editor.h
#encryption.h
#levelselect.h
#playfield.h
#screenlib.h
#tekst.h


gcc -g -Os -o programmeeropdracht main.c -L/sw/lib/ncurses/ -I/sw/include/ -lncurses_g -L/usr/lib -I/usr/include -lsqlite3 && echo "Build done";