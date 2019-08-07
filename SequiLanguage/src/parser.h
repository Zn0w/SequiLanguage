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

struct Statement
{
	virtual void execute() = 0;
};

struct Expression
{
	virtual Value evaluate() = 0;
};

struct AssignStatement : public Statement
{
	std::string name;
	Expression* expression;
	
	void execute()
	{
		variables.insert_or_assign(name, expression->evaluate());
	}
};

struct OpExpression : public Expression
{
	enum OpType
	{
		ADD, MULTIPLY, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL, EQUALS, NOT_EQUALS
	};
	
	Expression* left;
	Expression* right;
	OpType type;
	
	Value evaluate()
	{
		
	}
};

struct VariableExpression : public Expression
{
	std::string id;

	Value evaluate()
	{
		return variables[id];
	}
};

// maybe do that in the Expression base class
struct LiteralExpression : public Expression
{
	Value val;

	Value evaluate()
	{
		return val;
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

				if (tokens.at(i + 2).type == OPERATOR | COMPARE)
				{
					OpExpression* op_expr = new OpExpression;
					
					switch (tokens.at(i + 1).type)
					{
					case NUMBER:
					case STR:
						op_expr->right = new LiteralExpression;
						break;
					case IDENTIFIER:
						op_expr->right = new VariableExpression;
						break;
					default:
						error("");
						break;
					}

					switch (tokens.at(i + 1).type)
					{
					case NUMBER:
					case STR:
						op_expr->left = new LiteralExpression;
						break;
					case IDENTIFIER:
						op_expr->left = new VariableExpression;
						break;
					default:
						error("");
						break;
					}

					assign_statement->expression = op_expr;

					statements.push_back(assign_statement);
				}
				else
				{
					switch (tokens.at(i + 1).type)
					{
					case NUMBER:
					case STR:
					{
						LiteralExpression* lit_expr = new LiteralExpression;
						Value val;

						if (tokens.at(i + 1).type == NUMBER)
						{
							if (isDecimal(tokens.at(i + 1).lexeme))
								val.type = DECIMAL;
							else
								val.type = INTEGER;

							val.literal = tokens.at(i + 1).lexeme;
						}
						else if (tokens.at(i + 1).type == STR)
						{
							val.type = STRING_VALUE;		// i may eliminate CHAR from ValueType and leave only STRING_VALUE
							val.literal = tokens.at(i + 1).lexeme;
						}
						else if (tokens.at(i + 1).type == TRUE || tokens.at(i + 1).type == FALSE)
						{
							val.type = BOOL;
							val.literal = tokens.at(i + 1).lexeme;
						}
						else
							error("error in the assignment statement");

						val.literal = tokens.at(i + 1).lexeme;

						lit_expr->val = val;

						assign_statement->expression = lit_expr;

						statements.push_back(assign_statement);
						break;
					}
					case IDENTIFIER:
					{
						VariableExpression* var_expr = new VariableExpression;
						assign_statement->expression = var_expr;

						statements.push_back(assign_statement);
						break;
					}
					}
				}
			}
		}

		//else if (tokens.at(i).type == OPERATOR || tokens.at(i).type == COMPARE)


		i++;
	}
	
	return statements;
}