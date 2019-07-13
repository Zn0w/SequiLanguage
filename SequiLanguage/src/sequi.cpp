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
		printf("File with the path %s is either empty or doesn't exist.\n");
		return 0;
	}

#if DEBUG == 1
	printf("%s\n", file_contents.c_str());
	getchar();
#endif

	std::vector<Token> tokens = lex(file_contents);
	std::vector<Statement> statements = parse(tokens);

	return 0;
}