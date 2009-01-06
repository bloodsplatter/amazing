#!/bin/bash
#editor.h
#encryption.h
#levelselect.h
#playfield.h
#screenlib.h
#tekst.h
#-mmmx -msse -msse2 -msse3 -Os

gcc -g -o programmeeropdracht main.c -L/sw/lib/ncurses/ -I/sw/include/ -lncurses -L/usr/lib -I/usr/include -lsqlite3 && echo "Build done";