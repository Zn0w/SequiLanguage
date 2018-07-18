@echo off
gcc -c src/lexer/lexer.c -o obj/lexer.o
gcc -c src/sequi.c -o obj/sequi.o
gcc obj/sequi.o obj/lexer.o -o bin/sequi
pause