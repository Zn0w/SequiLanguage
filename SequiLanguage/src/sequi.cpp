#define DEBUG 1

#include <stdio.h>
#include <string>
#include <vector>

#include "../utils/file.h"

#include "lex.h"
#include "parser.h"

/*
int main(int argc, char** argv)
{
	if (argc == 1)
	{
		printf("Usage: sequi <filepath>\n");
		printf("<filepath> is the file with .sequi extension\n");
		return 0;
	}

	std::string file_contents = read_file(argv[1]);

	if (file_contents == "")
	{
		printf("File with the path %s is either empty or doesn't exist.\n", argv[1]);
		return 0;
	}

#if DEBUG == 1
	printf("Source File:\n");
	printf("##############################DEBUG#############################\n");
	printf("%s\n", file_contents.c_str());
	printf("##############################DEBUG#############################\n");
#endif

	std::vector<Token> tokens = lex(file_contents);

#if DEBUG == 1
	printf("\n\nTokens:\n");
	printf("##############################DEBUG#############################\n");
	for (Token token : tokens)
	{
		printf("%s\n", token.lexeme.c_str());
	}
	printf("##############################DEBUG#############################\n");
#endif

	std::vector<Statement*> statements = parse(tokens);

	return 0;
}*/

int main()
{
	std::vector<Token> tokens;
	tokens.reserve(20);
	
	tokens.push_back(Token(IDENTIFIER, "a"));
	tokens.push_back(Token(EQUAL, "="));
	tokens.push_back(Token(NUMBER, "15"));

	tokens.push_back(Token(IDENTIFIER, "length"));
	tokens.push_back(Token(EQUAL, "="));
	tokens.push_back(Token(NUMBER, "30.4"));

	tokens.push_back(Token(IDENTIFIER, "a"));
	tokens.push_back(Token(EQUAL, "="));
	tokens.push_back(Token(NUMBER, "5"));
	tokens.push_back(Token(OPERATOR, "+"));
	tokens.push_back(Token(NUMBER, "7"));

	tokens.push_back(Token(IDENTIFIER, "length"));
	tokens.push_back(Token(EQUAL, "="));
	tokens.push_back(Token(IDENTIFIER, "a"));
	tokens.push_back(Token(OPERATOR, "+"));
	tokens.push_back(Token(NUMBER, "6"));

	tokens.push_back(Token(PRINT, "print"));
	tokens.push_back(Token(STR, "Hello World! "));
	tokens.push_back(Token(IDENTIFIER, "a"));
	tokens.push_back(Token(SEMICOLON, ";"));

	std::vector<Statement*> statements = parse(tokens);

	// execute program
	for (Statement* statement : statements)
		statement->execute();

	return 0;
}