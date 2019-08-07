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

// OpExpression work only with two operators, no more for now
struct OpExpression : public Expression
{
	enum OpType
	{
		ADD, SUBTRACT, MULTIPLY, DIVIDE, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL, EQUALS, NOT_EQUALS, AND, OR, NOT
	};
	
	Expression* left;
	Expression* right;
	OpType type;
	
	Value evaluate()
	{
		Value result, l, r;
		
		l = left->evaluate();
		r = right->evaluate();

		switch (type)
		{
		case ADD:
		{
			if (l.type == r.type)
				switch (l.type)
				{
				case INTEGER:
					result
					break;
				case DECIMAL:
					result = add_decimal(l, r);
					break;
				case STRING_VALUE:
					result = add_str(l, r);
					break;
				case BOOL:
					result = add_bool(l, r);
					break;
				}
		}
		}
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

OpExpression::OpType getOpType(std::string token)
{
	if (token == "+")
		return OpExpression::ADD;
	else if (token == "/")
		return OpExpression::SUBTRACT;
	else if (token == "*")
		return OpExpression::MULTIPLY;
	else if (token == "/")
		return OpExpression::DIVIDE;
	else if (token == "==")
		return OpExpression::EQUALS;
	else if (token == ">")
		return OpExpression::GREATER;
	else if (token == "<")
		return OpExpression::LESS;
	else if (token == ">=")
		return OpExpression::GREATER_EQUAL;
	else if (token == "<=")
		return OpExpression::LESS_EQUAL;
	else if (token == "!=")
		return OpExpression::NOT_EQUALS;
	else if (token == "and")
		return OpExpression::AND;
	else if (token == "or")
		return OpExpression::OR;
	else if (token == "not")
		return OpExpression::NOT;
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

				if (tokens.at(i + 2).type == OPERATOR)
				{
					OpExpression* op_expr = new OpExpression;
					op_expr->type = getOpType(tokens.at(i + 2).lexeme);
					
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

					switch (tokens.at(i + 3).type)
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

					assign_statement->expression = op_expr;

					statements.push_back(assign_statement);

					i += 3;
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
						var_expr->id = tokens.at(i + 1).lexeme;
						assign_statement->expression = var_expr;

						statements.push_back(assign_statement);
						break;
					}
					}

					i += 1;
				}
			}
		}

		/*else if (tokens.at(i).type == OPERATOR || tokens.at(i).type == COMPARE)
		{
			OpExpression* op_expr = new OpExpression;
			op_expr->type = getOpType(tokens.at(i).lexeme);

			switch (tokens.at(i - 1).type)
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

			statements.push_back(op_expr);
		}*/

		i++;
	}
	
	return statements;
}