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
	std::vector<std::string> unprocessed;
	while (std::getline(test, segment, ' '))
	{
		unprocessed.push_back(segment);
	}

	std::vector<Token> tokens;
	tokens.reserve(unprocessed.size());
	
	for (std::string token : unprocessed)
	{

	}

	return tokens;
}