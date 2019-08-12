#pragma once


#include <vector>
#include <map>
#include <stdio.h>

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

struct PrintStatement : public Statement
{
	std::vector<Expression*> expression;
	
	void execute()
	{
		for (Expression* expr : expression)
			printf("%s", expr->evaluate()->to_string().c_str());
	}
};

struct IfStatement : public Statement
{
	Expression* condition;
	Expression* then;
	Expression* or_else;

	void execute()
	{
		
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

Expression* get_expr(Token token)
{
	switch (token.type)
	{
	case NUMBER:
	{
		LiteralExpression* lit_expr = new LiteralExpression;
		NumValue* val = new NumValue;
		val->value = stod(token.lexeme);
		lit_expr->val = val;

		return lit_expr;
	}
	case STR:
	{
		LiteralExpression* lit_expr = new LiteralExpression;
		StrValue* val = new StrValue;
		val->value = token.lexeme;
		lit_expr->val = val;

		return lit_expr;
	}
	case TRUE:
	{
		LiteralExpression* lit_expr = new LiteralExpression;

		NumValue* val = new NumValue;
		val->value = 1;

		lit_expr->val = val;

		return lit_expr;
	}
	case FALSE:
	{
		LiteralExpression* lit_expr = new LiteralExpression;

		NumValue* val = new NumValue;
		val->value = 0;

		lit_expr->val = val;

		return lit_expr;
	}
	case IDENTIFIER:
	{
		VariableExpression * var_expr = new VariableExpression;
		var_expr->id = token.lexeme;

		return var_expr;
	}
	default:
		error("");
		return NULL;
	}
}

// returns the index of the token that means the end of the compound expression
int get_block_edge(std::vector<Token>* tokens, int start, TokenType terminate_token)
{
	for (int i = start; i < tokens->size(); i++)
		if (tokens->at(i).type == terminate_token)
			return i;

	return 0;
}

Expression* calculate_compound_op_expr(std::vector<Token>* tokens, int start, int edge, int *parse_iterator)
{
	OpExpression* op_expr = new OpExpression;
	op_expr->left = get_expr(tokens->at(start));

	for (int j = start + 1; j < edge; j++)
	{
		if (tokens->at(j).type == OPERATOR)
		{
			op_expr->type = getOpType(tokens->at(j).lexeme);
			op_expr->right = get_expr(tokens->at(j + 1));

			LiteralExpression* lit_expr = new LiteralExpression;
			Value* val = op_expr->evaluate();
			lit_expr->val = val;

			op_expr->left = lit_expr;
		}
	}

	*parse_iterator = edge;

	return op_expr->left;
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

				if (i + 2 < tokens.size() && tokens.at(i + 2).type == OPERATOR)
				{
					assign_statement->expression = calculate_compound_op_expr(&tokens, i + 1, get_block_edge(&tokens, i + 2, SEMICOLON), &i);
					//statements.push_back(assign_statement);
					assign_statement->execute();
				}
				else
				{
					assign_statement->expression = get_expr(tokens.at(i + 1));
					//statements.push_back(assign_statement);
					assign_statement->execute();

					i += 1;
				}
			}
		}

		else if (tokens.at(i).type == PRINT)
		{
			PrintStatement* ps = new PrintStatement;
			
			int edge = get_block_edge(&tokens, i + 1, SEMICOLON);
			
			for (int j = i + 1; j < edge; j++)
			{
				if (tokens.at(j).type == NEWLINE)
				{
					LiteralExpression* lit_expr = new LiteralExpression;

					StrValue* val = new StrValue;
					val->value = "\n";

					lit_expr->val = val;

					ps->expression.push_back(lit_expr);
				}
				else
					ps->expression.push_back(get_expr(tokens.at(j)));
			}

			ps->execute();
			i = edge;
		}

		else if (tokens.at(i).type == IF && tokens.at(i + 1).type == LEFT_PAREN)
		{
			IfStatement* is = new IfStatement;

			// get a condition
			if (tokens.at(i + 3).type == OPERATOR)
			{
				is->condition = calculate_compound_op_expr(&tokens, i + 2, get_block_edge(&tokens, i + 3, SEMICOLON), &i);
			}
			else if (tokens.at(i + 3).type == RIGHT_PAREN)
			{

				is->condition = get_expr(tokens.at(i + 3));
				i += 3;
			}
			else
			{
				error("");
				continue;
			}

			// get a then expression

		}

		i++;
	}
	
	return statements;
}