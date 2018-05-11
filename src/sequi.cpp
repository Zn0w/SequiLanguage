#include "lexer.h"

#include <iostream>
#include <string.h>

int main(int argc, char** argv)
{
	const char* filename;
	
	if (argc == 1)
	{
		std::cout << "Specify the command as follows: sequi [command]" << std::endl;
		std::cout << "Command list:" << std::endl;
		std::cout << "version - prints out a version of the sequi interpreter" << std::endl;
		std::cout << "[filename] - interprets the specified source code" << std::endl;
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "version") == 0)
			std::cout << "The itnrepreter's version: 0.0.1d (in development)" << std::endl;
		else
			filename = argv[1];
	}
	else
		std::cout << "Too many arguments." << std::endl;

	return 0;
}