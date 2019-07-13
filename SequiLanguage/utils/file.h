#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string>


std::string read_file(const char* filepath)
{
	FILE* file = fopen(filepath, "rt");		// rt stands for 'read text'
	
	if (file == NULL)
		return "";
	
	fseek(file, 0, SEEK_END);				// go to the end of file
	unsigned long length = ftell(file);		// tell the amount of passed bytes
	char* data = (char*)malloc(length + 1);	// +1 for termination character
	memset(data, 0, length + 1);
	fseek(file, 0, SEEK_SET);				// go to the beggining of the file
	fread(data, 1, length, file);
	fclose(file);

	std::string result(data);

	free(data);

	return result;
}