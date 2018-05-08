@echo off
g++ -c src/lexer.cpp -o obj/lexer.o
g++ -c src/sequi.cpp -o obj/sequi.o
g++ obj/sequi.o obj/lexer.o -o bin/sequi
pause