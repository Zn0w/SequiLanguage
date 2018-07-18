#ifndef LEXER_H
#define LEXER_H


#include "../vendor/stb_stretchy_buffer.h"

char* getSource(const char* filename);

void analyse(char* source);


#endif // LEXER_H