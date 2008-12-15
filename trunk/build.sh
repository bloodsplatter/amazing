#!/bin/bash
#editor.h
#encryption.h
#levelselect.h
#playfield.h
#screenlib.h
#tekst.h


gcc -g -Os -o programmeeropdracht main.c -L/sw/lib/ -I/sw/include/ -lncurses && echo "Build done";
