#pragma once


#include <string>
#include <sstream>
#include <vector>
#include <cctype>

#include "token.h"


enum LexMode
{
	DEFAULT,
	STRING,
	COMMENT,
	PRINTING
};

bool isNumber(std::string str)
{
	bool withFraction = false;
	
	for (int i = 0; i < str.size(); i++)
	{
		if (str.at(i) == '.' && !withFraction)
			withFraction = true;
		else if (!std::isdigit(str.at(i)))
			return false;
	}

	return true;
}

std::vector<Token> lex(std::string source)
{
	LexMode mode = DEFAULT;
	
	// split string by space and new line
	std::vector<std::string> unprocessed;
	std::string word = "";
	for (int i = 0; i < source.size(); i++)
	{
		if (source.at(i) == ' ' || source.at(i) == '\n' || source.at(i) == EOF)
		{
			if (word != "")
				unprocessed.push_back(word);
			word = "";
		}
		else
			word += source.at(i);
	}

	if (word != "" && word != " " && word != "\n")
		unprocessed.push_back(word);

	std::vector<Token> tokens;
	tokens.reserve(unprocessed.size());
	
	for (std::string token : unprocessed)
	{
		switch (mode)
		{

		case DEFAULT:
		{
			TokenType type = UNDEF;

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
					mode = STRING;
					word = "";
					continue;
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
			else if (isNumber(token))
				type = NUMBER;
			else if (token.at(0) == '"')
			{
				mode = STRING;
				word = token.erase(0, 1);	// delete the first character from the string (token[0])
				continue;
			}
			// else check if valid identifier
			
			// TODO deal with undefined token
			else
				return tokens;

			tokens.push_back(Token(type, token));
		} break;

		case STRING :
		{
			word += ' ';

			if (token.at(token.size() - 1) == '"')
			{
				word += token.erase(token.size() - 1, 1);	// delete the last character from the string
				tokens.push_back(Token(STR, word));
				mode = DEFAULT;
			}
			else
				word += token;
		} break;

		}
	}

	return tokens;
}