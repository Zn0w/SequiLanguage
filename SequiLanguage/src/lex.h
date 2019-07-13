#pragma once


#include <string>
#include <sstream>
#include <vector>

#include "token.h"


std::vector<Token> lex(std::string source)
{
	// split string by space
	std::stringstream test(source);
	std::string segment;
	std::vector<std::string> seglist;
	while (std::getline(test, segment, ' '))
	{
		seglist.push_back(segment);
	}

	std::vector<Token> tokens;
	tokens.reserve(seglist.size());
	
	std::string token = "";
	for (int i = 0; i < source.size(); i++)
	{
		token += source.at(i);
		
		if (token == "(")
			tokens.push_back(Token(LEFT_PAREN, ""));
		

	}
}