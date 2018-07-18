#include "lexer.h"

char* getSource(const char* filename)
{
	char* source = NULL;
	
	FILE* file;
	file = fopen(filename, "r");
	if (!file)
		return source;
	
	char c;
	while ((c = fgetc(file)) != EOF)
	{
		sb_push(source, c);
	}

	fclose(file);

	return source;
}

void analyse(char* source)
{
	for (int i = 0; i < sb_count(source); i++)
	{
		printf("%c", source[i]);
	}
}