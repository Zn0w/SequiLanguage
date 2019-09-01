#define DEBUG 1

#include <stdio.h>
#include <string>
#include <vector>

#include "../utils/file.h"

#include "lex.h"
#include "parser.h"


int main(int argc, char** argv)
{
	if (argc == 1)
	{
		printf("Usage: sequi <filepath>\n");
		printf("<filepath> is the file with .sequi extension\n");
		return 0;
	}

	std::string file_contents = read_file(argv[1]);

	if (file_contents == "")
	{
		printf("File with the path %s is either empty or doesn't exist.\n", argv[1]);
		return 0;
	}

	std::vector<Token> tokens = lex(file_contents);

	std::vector<Statement*> statements = parse(tokens);

	// execute program
	for (current_statement = 0; current_statement < statements.size(); current_statement++)
		statements.at(current_statement)->execute();

	return 0;
}