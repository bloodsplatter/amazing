#!/bin/bash
#editor.h
#encryption.h
#levelselect.h
#playfield.h
#screenlib.h
#tekst.h


gcc -Os -static -o programmeeropdracht main.c -L/sw/lib/ -I/sw/include/ -lncurses && echo "Build done";
