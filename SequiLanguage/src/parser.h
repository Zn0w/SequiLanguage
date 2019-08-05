#pragma once


#include <vector>

#include "lex.h"
#include "statement.h"


struct Node
{
	
};

struct BinaryOp : public Node
{
	Node* left, right;
	Token op;
};

struct Num : public Node
{
	Token number_token;
};


std::vector<Statement> parse(std::vector<Token> tokens)
{
	std::vector<Statement> statements;

	int i = 0;
	while (true)
	{
		if (tokens.at(i).type == LEFT_BRACE)
			;
	}
	
	return statements;
}


// TODO: handle errors
void error(const char*)
{
	
}