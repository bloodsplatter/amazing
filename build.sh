#!/bin/bash

gcc -static -Os -o programmeeropdracht main.c -L. -llzma -L/sw/lib/ -lncurses && echo "Build done";
