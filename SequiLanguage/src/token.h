#pragma once


/*enum TokenType
{
	KEYWORD, IDENTIFIER, NUMBER, STRING, CHAR, OPERATOR, PUNCTUATOR
};*/

enum TokenType
{
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACK, RIGHT_BRACK,
	COMMA, DOT, SEMICOLON, COLON, QUOTE,
	
	OPERATOR,
	//ADD_OPERATOR,		// + - OR
	//MULTIPLY_OPERATOR	// * / AND DIV MOD
	//RELATION_OPERATOR	// > < 

	//EQUAL, EQUAL_EQUAL,
	//GREATER, GREATER_EQUAL,
	//LESS, LESS_EQUAL,

	IDENTIFIER, STR, NUMBER,

	VAR, FUNC, IF, ELSE, ELIF, AND, OR, NOT, PRINT, TRUE, FALSE, RETURN,

	END_OF_FILE
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