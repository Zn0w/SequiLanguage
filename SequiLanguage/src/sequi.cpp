#include <stdio.h>
#include <string>

#include "../utils/file.h"


int main(int argc, char** argv)
{
	if (argc == 1)
	{
		printf("Usage: sequi <filepath>\n");
		printf("<filepath> is the file with .sequi extension\n");
		return 0;
	}

	std::string file_contents = read_file(argv[1]);

	printf("%s\n", file_contents.c_str());
	getchar();

	return 0;
}