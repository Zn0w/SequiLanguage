#include "lexer.h"

static char* concat(char* str1, char* str2)
{
	char* new_str;
	new_str = malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(new_str, str1);
	strcat(new_str, str2);

	return new_str;
}

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
	char* token = "";

	for (int i = 0; i < sb_count(source); i++)
	{
		char* c = source[i];
		token = concat(token, c);
		printf("%s\n", token);
	}
}