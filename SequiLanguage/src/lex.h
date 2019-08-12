#pragma once


#include <string>
#include <sstream>
#include <vector>
#include <cctype>


/*enum TokenType
{
KEYWORD, IDENTIFIER, NUMBER, STRING, CHAR, OPERATOR, PUNCTUATOR
};*/

enum TokenType
{
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACK, RIGHT_BRACK,
	COMMA, DOT, SEMICOLON, COLON, QUOTE,

	OPERATOR, EQUAL,

	IDENTIFIER, STR, NUMBER,

	NEWLINE, FUNC, IF, ELSE, ELIF, AND, OR, NOT, PRINT, TRUE, FALSE, RETURN,

	END_OF_FILE,
	UNDEF // ??
};

struct Token
{
	TokenType type;
	std::string lexeme;

	Token(TokenType t, std::string l)
	{
		type = t;
		lexeme = l;
	}
};

enum LexMode
{
	DEFAULT,
	STRING,
	COMMENT
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

bool isValidId(std::string t)
{
	if (std::isdigit(t.at(0)) && t.at(0) == '-' && t.at(0) == '+' && t.at(0) == '*' && t.at(0) == '/' && t.at(0) == '%' && t.at(0) == '!' && t.at(0) == '=')
		return false;

	// at least one symbol must be a letter
	for (int i = 0; i < t.size(); i++)
	{
		if (((int)t.at(i) >= 65 && (int)t.at(i) <= 90) || ((int)t.at(i) >= 97 && (int)t.at(i) <= 122))
			return true;
	}

	return false;
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
					type = RIGHT_BRACE;
					break;
				case '[':
					type = LEFT_BRACK;
					break;
				case ']':
					type = RIGHT_BRACK;
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
				case '>':
				case '<':
					type = OPERATOR;
					break;
				case '=':
					type = EQUAL;
					break;
				case '"':
					type = QUOTE;
					mode = STRING;
					word = "";
					continue;
					break;

				default:
				{
					// check if literal or identifier, if not print error

					if (isNumber(token))
						type = NUMBER;
					else if (isValidId(token)) // is a letter
						type = IDENTIFIER;

					break;
				}
				}
			}

			// if multiple-symbol operator
			else if (token == "==" || token == ">=" || token == "<=" || token == "!=" || token == "and" || token == "or" || token == "not")
				type = OPERATOR;

			// if keyword
			else if (token == "newline")
				type = NEWLINE;
			else if (token == "func")
				type = FUNC;
			else if (token == "if")
				type = IF;
			else if (token == "else")
				type = ELSE;
			else if (token == "elif")
				type = ELIF;
			else if (token == "print")
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
			else if (isValidId(token))
				type = IDENTIFIER;

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