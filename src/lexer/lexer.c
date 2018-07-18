#include "lexer.h"

char* getSource(const char* filename)
{
	char* source = NULL;
	
	FILE* file;
	file = fopen(filename, "r");
	if (!file)
		source = "file_read_fail";
	
	char c;
	while ((c = fgetc(file)) != EOF)
	{
		printf("%c\n", c);
	}

	return source;
}

void analyse(char* source)
{

}