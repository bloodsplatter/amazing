#!/bin/bash
#editor.h
#encryption.h
#levelselect.h
#playfield.h
#screenlib.h
#tekst.h


gcc -g -Os -o programmeeropdracht main.c -L/usr/lib/ -I/usr/include/ -lncurses -lsqlite3 && echo "Build done";