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

	NumValue(double v)
	{
		value = v;
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

	StrValue(std::string v)
	{
		value = v;
	}
};

std::map<std::string, Value*> variables;
std::map<std::string, int> labels;
int current_statement = 0;

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

	AssignStatement()
	{
	
	}

	~AssignStatement()
	{
		delete expression;
	}
};

// OpExpression work only with two operators, no more for now
struct OpExpression : public Expression
{
	enum OpType
	{
		ADD, SUBTRACT, MULTIPLY, DIVIDE, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL, EQUALS, NOT_EQUALS, AND, OR, NOT, NONE // none is used when dealing with the compound op expressions
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
				return new NumValue(l->to_number() + r->to_number());
			}
			else
			{
				return new StrValue(l->to_string() + r->to_string());
			}
		}

		case SUBTRACT:
		{
			return new NumValue(l->to_number() - r->to_number());
		}

		case MULTIPLY:
		{
			return new NumValue(l->to_number() * r->to_number());
		}

		case DIVIDE:
		{
			return new NumValue(l->to_number() / r->to_number());
		}

		case GREATER:
		{
			return new NumValue(l->to_number() > r->to_number());
		}

		case LESS:
		{
			return new NumValue(l->to_number() < r->to_number());
		}

		case GREATER_EQUAL:
		{
			return new NumValue(l->to_number() >= r->to_number());
		}

		case LESS_EQUAL:
		{
			return new NumValue(l->to_number() <= r->to_number());
		}

		case EQUALS:
		{
			if (dynamic_cast<NumValue*>(l) != NULL)
				return new NumValue(l->to_number() == r->to_number());
			else
				return new NumValue(l->to_string() == r->to_string());
		}

		case NOT_EQUALS:
		{
			if (dynamic_cast<NumValue*>(l) != NULL)
				return new NumValue(l->to_number() != r->to_number());
			else
				return new NumValue(l->to_string() != r->to_string());
		}

		case AND:
		{
			return new NumValue(l->to_number() && r->to_number());
		}

		case OR:
		{
			return new NumValue(l->to_number() || r->to_number());
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

	PrintStatement()
	{

	}

	~PrintStatement()
	{
		for (Expression* expr : expression)
			delete expr;
	}
};

struct Block
{
	std::string name;
	std::vector<Statement*> statements;
	std::vector<std::string> variable_names;	// TODO: implement in the future

	Block()
	{
		
	}

	Block(std::string s_name)
	{
		name = s_name;
	}

	void execute()
	{
		for (Statement* statement : statements)
			statement->execute();
	}

	// clear out the local function's variables
	void dispose()
	{
		std::map<std::string, Value*>::iterator it;
		for (std::string var_name : variable_names)
		{
			it = variables.find(var_name);
			variables.erase(it);
		}
	}
};

struct IfStatement : public Statement
{
	Expression* condition;
	Block then;
	Block or_else;

	void execute()
	{
		Value* value = condition->evaluate();

		if (dynamic_cast<NumValue*>(value) != NULL)
		{
			if (value->to_number())
				then.execute();
			else
				or_else.execute();
		}
		else
			;//print error
	}
};

struct GotoStatement : public Statement
{
	std::string label;

	GotoStatement(std::string lbl)
	{
		label = lbl;
	}

	void execute()
	{
		if (labels.count(label))	// if label exists
			current_statement = labels[label];
	}
};

struct VariableExpression : public Expression
{
	std::string id;

	Value* evaluate()
	{
		return variables[id];
	}

	VariableExpression(std::string s_id)
	{
		id = s_id;
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

	LiteralExpression(Value* v)
	{
		val = v;
	}
};


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
		return new LiteralExpression(new NumValue(stod(token.lexeme)));
	}
	case STR:
	{
		return new LiteralExpression(new StrValue(token.lexeme));
	}
	case TRUE:
	{
		 return new LiteralExpression(new NumValue(1));
	}
	case FALSE:
	{
		return new LiteralExpression(new NumValue(0));
	}
	case IDENTIFIER:
	{
		return new VariableExpression(token.lexeme);
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
					OpExpression* op_expr = new OpExpression;
					op_expr->type = getOpType(tokens.at(i + 2).lexeme);
					op_expr->left = get_expr(tokens.at(i + 1));
					op_expr->right = get_expr(tokens.at(i + 3));

					assign_statement->expression = op_expr;
					statements.push_back(assign_statement);

					i += 3;
				}
				else
				{
					assign_statement->expression = get_expr(tokens.at(i + 1));
					statements.push_back(assign_statement);

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
					ps->expression.push_back(new LiteralExpression(new StrValue("\n")));
				}
				else
					ps->expression.push_back(get_expr(tokens.at(j)));
			}

			statements.push_back(ps);
			i = edge;
		}

		// FROM THIS POINT ON I'VE BASICALLY GIVEN UP ON ANY DESIGN PATTERS AND CLEAN CODE THINGIES AS I WAS EAGER TO FINISH THIS HORRIBLY-DESIGNED PARODY OF
		// A LANGUAGE. THE NEXT ONE WILL (HOPEFULLY) BE BETTER :)

		else if (tokens.at(i).type == IF && tokens.at(i + 1).type == LEFT_PAREN)
		{
			IfStatement* is = new IfStatement;

			// get a condition
			if (tokens.at(i + 3).type == OPERATOR && tokens.at(i + 5).type == RIGHT_PAREN)
			{
				OpExpression* op_expr = new OpExpression;
				op_expr->type = getOpType(tokens.at(i + 3).lexeme);
				op_expr->left = get_expr(tokens.at(i + 2));
				op_expr->right = get_expr(tokens.at(i + 4));

				is->condition = op_expr;

				i += 5;
			}
			else if (tokens.at(i + 3).type == RIGHT_PAREN)
			{

				is->condition = get_expr(tokens.at(i + 2));
				i += 2;
			}
			else
			{
				error("");
				continue;
			}

			// get a then expression
			if (tokens.at(i + 1).type == LEFT_BRACE)
			{
				// Extract a segment of token list, which is withing the {} , create struct Function instance and call func.statements = parse(segment)
				// need to find a solution to the local variables problem

				int edge = get_block_edge(&tokens, i + 2, RIGHT_BRACE);

				std::vector<Token> segment;
				segment.reserve(edge - (i + 2));
				for (int j = i + 2; j < edge; j++)
					segment.push_back(tokens.at(j));

				Block block;
				block.statements = parse(segment);
				is->then = block;

				i = edge;
			}
			else
			{
				error("");
				continue;
			}

			// check if the end of the program
			if (i + 2 >= tokens.size())
			{
				statements.push_back(is);
				i++;
				continue;
			}

			// get an else expression
			if (tokens.at(i + 1).type == ELSE && tokens.at(i + 2).type == LEFT_BRACE)	// if contains a single statement (for testing)
			{
				// Extract a segment of token list, which is withing the {} , create struct Function instance and call func.statements = parse(segment)
				// need to find a solution to the local variables problem

				int edge = get_block_edge(&tokens, i + 3, RIGHT_BRACE);

				std::vector<Token> segment;
				segment.reserve(edge - (i + 3));
				for (int j = i + 3; j < edge; j++)
					segment.push_back(tokens.at(j));

				Block block;
				block.statements = parse(segment);
				is->or_else = block;

				i = edge;
			}
			else
			{
				Block block = {};
				is->or_else = block;
			}

			statements.push_back(is);
		}

		else if (tokens.at(i).type == LABEL && tokens.at(i + 1).type == IDENTIFIER)
		{
			labels.insert_or_assign(tokens.at(i + 1).lexeme, statements.size() - 1);	// - 1 because the for loop increments the current_statement
			i += 1;
		}

		else if (tokens.at(i).type == GOTO && tokens.at(i + 1).type == IDENTIFIER)
		{
			statements.push_back(new GotoStatement(tokens.at(i + 1).lexeme));
			i += 1;
		}

		i++;
	}
	
	return statements;
}