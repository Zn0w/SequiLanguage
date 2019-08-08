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
	virtual double to_number() = 0;
	virtual std::string to_string() = 0;
};

struct NumValue : public Value
{
	double value;

	double to_number()
	{
		return value;
	}

	std::string to_string()
	{
		return std::to_string(value);
	}
};

struct StrValue : public Value
{
	std::string value;
	
	double to_number()
	{
		return stod(value);
	}

	std::string to_string()
	{
		return value;
	}
};

std::map<std::string, Value*> variables;

struct Statement
{
	virtual void execute() = 0;
};

struct Expression
{
	virtual Value* evaluate() = 0;
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
	
	Value* evaluate()
	{
		Value* l;
		Value* r;
		
		l = left->evaluate();
		r = right->evaluate();

		switch (type)
		{
		case ADD:
		{
			if (dynamic_cast<NumValue*>(l) != NULL)
			{
				NumValue* result = new NumValue;
				result->value = l->to_number() + r->to_number();
				return result;
			}
			else
			{
				StrValue* result = new StrValue;
				result->value = l->to_string() + r->to_string();
				return result;
			}
		}

		case SUBTRACT:
		{
			NumValue* result = new NumValue;
			result->value = l->to_number() - r->to_number();
			return result;
		}

		case MULTIPLY:
		{
			NumValue* result = new NumValue;
			result->value = l->to_number() * r->to_number();
			return result;
		}

		case DIVIDE:
		{
			NumValue* result = new NumValue;
			result->value = l->to_number() / r->to_number();
			return result;
		}

		case GREATER:
		{
			NumValue* result = new NumValue;
			result->value = l->to_number() > r->to_number();
			return result;
		}

		case LESS:
		{
			NumValue* result = new NumValue;
			result->value = l->to_number() < r->to_number();
			return result;
		}

		case GREATER_EQUAL:
		{
			NumValue* result = new NumValue;
			result->value = l->to_number() >= r->to_number();
			return result;
		}

		case LESS_EQUAL:
		{
			NumValue* result = new NumValue;
			result->value = l->to_number() <= r->to_number();
			return result;
		}

		case EQUALS:
		{
			NumValue* result = new NumValue;
			
			if (dynamic_cast<NumValue*>(l) != NULL)
				result->value = l->to_number() == r->to_number();
			else
				result->value = l->to_string() == r->to_string();

			return result;
		}

		case NOT_EQUALS:
		{
			NumValue* result = new NumValue;

			if (dynamic_cast<NumValue*>(l) != NULL)
				result->value = l->to_number() != r->to_number();
			else
				result->value = l->to_string() != r->to_string();

			return result;
		}

		case AND:
		{
			NumValue* result = new NumValue;
			result->value = l->to_number() && r->to_number();
			return result;
		}

		case OR:
		{
			NumValue* result = new NumValue;
			result->value = l->to_number() || r->to_number();
			return result;
		}

		default:
			// error
			return NULL;
		}
	}
};

struct VariableExpression : public Expression
{
	std::string id;

	Value* evaluate()
	{
		return variables[id];
	}
};

// maybe do that in the Expression base class
struct LiteralExpression : public Expression
{
	Value* val;

	Value* evaluate()
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
					{
						LiteralExpression* lit_expr = new LiteralExpression;
						
						NumValue* val = new NumValue;
						val->value = stod(tokens.at(i + 1).lexeme);

						lit_expr->val = val;

						assign_statement->expression = lit_expr;
						statements.push_back(assign_statement);

						break;
					}
					case TRUE:
					{
						LiteralExpression* lit_expr = new LiteralExpression;
						
						NumValue* val = new NumValue;
						val->value = 1;

						lit_expr->val = val;

						assign_statement->expression = lit_expr;
						statements.push_back(assign_statement);

						break;
					}
					case FALSE:
					{
						LiteralExpression* lit_expr = new LiteralExpression;
						
						NumValue* val = new NumValue;
						val->value = 0;

						lit_expr->val = val;

						assign_statement->expression = lit_expr;
						statements.push_back(assign_statement);

						break;
					}
					case STR:
					{
						LiteralExpression* lit_expr = new LiteralExpression;
						
						StrValue* val = new StrValue;
						val->value = tokens.at(i + 1).lexeme;

						lit_expr->val = val;

						assign_statement->expression = lit_expr;
						statements.push_back(assign_statement);

						break;
					}
					default:
							error("error in the assignment statement");
							break;
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