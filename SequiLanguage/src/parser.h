#pragma once


#include <vector>
#include <map>

#include "lex.h"


enum ValueType
{			// the name STRING is used in lex.h, so here STRING_VALUE is used
	INTEGER, DECIMAL, STRING_VALUE, CHAR, BOOL
};

struct Value
{
	ValueType type;
	std::string literal;
};

std::map<std::string, Value> variables;

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
		variables.insert_or_assign(name, value);
	}
};

struct OperationStatement : public Statement
{
	Value left, right, result;
	
	void execute()
	{
		
	}
};


bool isDecimal(std::string number)
{
	for (int i = 0; i < number.length(); i++)
		if (number.at(i) == '.')
			return true;
	
	return false;
}

// TODO: handle errors
void error(const char*)
{

}

std::vector<Statement*> parse(std::vector<Token> tokens)
{
	std::vector<Statement*> statements;

	int i = 0;
	while (i < tokens.size())
	{
		if (tokens.at(i).type == EQUAL)
		{
			AssignStatement* assign_statement = new AssignStatement;
			// temporary solution
			if (i > 0 & i < tokens.size() - 1 & tokens.at(i - 1).type == IDENTIFIER)
			{
				assign_statement->name = tokens.at(i - 1).lexeme;

				if (tokens.at(i + 1).type == IDENTIFIER)
					assign_statement->value = variables[tokens.at(i - 1).lexeme];
				else if (tokens.at(i + 1).type == NUMBER)
				{
					if (isDecimal(tokens.at(i + 1).lexeme))
						assign_statement->value.type = DECIMAL;
					else
						assign_statement->value.type = INTEGER;

					assign_statement->value.literal = tokens.at(i + 1).lexeme;
				}
				else if (tokens.at(i + 1).type == STR)
				{
					assign_statement->value.type = STRING_VALUE;		// i may eliminate CHAR from ValueType and leave only STRING_VALUE
					assign_statement->value.literal = tokens.at(i + 1).lexeme;
				}
				else if (tokens.at(i + 1).type == TRUE || tokens.at(i + 1).type == FALSE)
				{
					assign_statement->value.type = BOOL;
					assign_statement->value.literal = tokens.at(i + 1).lexeme;
				}
				else
				{
					error("error in the assignment statement");
					continue;
				}

				statements.push_back(assign_statement);
			}
		}

		i++;
	}
	
	return statements;
}