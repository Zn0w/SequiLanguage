#pragma once


#include <vector>

#include "lex.h"
//#include "statement.h"


enum ValueType
{
	INTEGER, DECIMAL, STRING, CHAR
};

struct Value
{
	ValueType type;
	std::string literal;
};

// Here the terminology is messed up, so in some context Statement is an expression,
// in other a statement
struct Statement
{
	virtual void execute() = 0;
};

struct AssignStatement : public Statement
{
	std::string name;
	Value value;

	void execute()
	{
		
	}
};

struct OperationStatement : public Statement
{
	Value left, right, result;
	
	void execute()
	{
		
	}
};

struct ComparisonStatement : public Statement
{
	Value left, right;
	bool result;

	void execute()
	{
		
	}
};


std::vector<Statement> parse(std::vector<Token> tokens)
{
	std::vector<Statement> statements;

	int i = 0;
	while (true)
	{
		if (tokens.at(i).type == EQUAL)
	}
	
	return statements;
}


// TODO: handle errors
void error(const char*)
{
	
}