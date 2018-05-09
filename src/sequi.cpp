#include "lexer.h"

#include <iostream>

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
		std::cout << "argv: " << argv[1] << std::endl;
		if ((const char*)argv[1] == "version")
			std::cout << "The itnrepreter's version: 0.0.1 (development)" << std::endl;
		else
			filename = argv[1];
		std::cout << filename << std::endl;
	}
	else
		std::cout << "Too many arguments." << std::endl;

	return 0;
}