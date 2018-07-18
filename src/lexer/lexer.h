#ifndef LEXER_H
#define LEXER_H


#include <stdio.h>
#include <string.h>

#include "../vendor/stb_stretchy_buffer.h"

char* getSource(const char* filename);

void analyse(char* source);

static char* concat(char*, char*);


#endif // LEXER_H