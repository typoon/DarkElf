#!/bin/sh

gcc -Wall -c darkelf.c
gcc -Wall -c darkelfRead.c
gcc -Wall -o main darkelfRead.o main.c


