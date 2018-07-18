#include <stdio.h>
#include <string.h>

#include "lexer/lexer.h"

int main(int argc, char** argv)
{
	const char* filename;
	
	if (argc == 1)
	{
		printf("Specify the command as follows: sequi [command]\n");
		printf("Command list: (words in square brackets mean it is user-secified)\n");
		printf("version - prints out a version of the sequi interpreter\n");
		printf("[filename] - interprets the specified source code\n");
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "version") == 0)
		{
			printf("The current sequi's version: 0.0.1d (in development)\n");
			return 0;
		}
		else
			filename = argv[1];
	}
	else
	{
		printf("Too many arguments.\n");
		return -1;
	}

	char* source = getSource(filename);
	if (source == NULL)
	{
		printf("Failed to read file with path '%s'.\n", filename);
		return -1;
	}

	analyse(source);
	
	return 0;
}