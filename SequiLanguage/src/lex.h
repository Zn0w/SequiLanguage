#pragma once


#include <string>
#include <sstream>
#include <vector>

#include "token.h"


enum LexMode
{
	DEFAULT,
	STRING,
	COMMENT,
	PRINTING
};

std::vector<Token> lex(std::string source)
{
	LexMode mode = DEFAULT;
	
	// split string by space
	std::stringstream test(source);
	std::string segment;
	std::vector<std::string> unprocessed;
	while (std::getline(test, segment, ' '))
	{
		unprocessed.push_back(segment);
	}

	std::vector<Token> tokens;
	tokens.reserve(unprocessed.size());
	
	for (std::string token : unprocessed)
	{
		switch (mode)
		{

		case DEFAULT:
		{
			TokenType type;

			// if single-character token
			if (token.size() == 1)
			{
				switch (token.at(0))
				{
				case '(':
					type = LEFT_PAREN;
					break;
				case ')':
					type = RIGHT_PAREN;
					break;
				case '{':
					type = LEFT_BRACE;
					break;
				case '}':
					type = RIGHT_PAREN;
					break;
				case '[':
					type = LEFT_BRACK;
					break;
				case ']':
					type = RIGHT_PAREN;
					break;
				case ',':
					type = COMMA;
					break;
				case '.':
					type = DOT;
					break;
				case ';':
					type = SEMICOLON;
					break;
				case ':':
					type = COLON;
					break;
				case '+':
				case '-':
				case '*':
				case '/':
				case '=':
				case '>':
				case '<':
					type = OPERATOR;
					break;
				case '"':
					type = QUOTE;
					// go to the string mode
					break;

				default:
					// check if literal, if not print error
					break;
				}
			}

			// if keyword
			else if (token == "var")
				type = VAR;
			else if (token == "func")
				type = FUNC;
			else if (token == "if")
				type = IF;
			else if (token == "else")
				type = ELSE;
			else if (token == "elif")
				type = ELIF;
			else if (token == "and")
				type = AND;
			else if (token == "or")
				type = OR;
			else if (token == "not")
				type = NOT;
			else if (token == "print")
				// go to the printing mode
				type = PRINT;
			else if (token == "true")
				type = TRUE;
			else if (token == "false")
				type = FALSE;
			else if (token == "return")
				type = RETURN;

			// if literal
			else if (std::stoi(token))
				type = NUMBER;
			// else check if valid identifier

			tokens.push_back(Token(type, token));
		} break;

		case STRING :
		{
			
		} break;

		}
	}

	return tokens;
}