@echo off
gcc -c src/sequi.c -o obj/sequi.o
gcc obj/sequi.o -o bin/sequi
pause