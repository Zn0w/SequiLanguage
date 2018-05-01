#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "variable.h"

std::map<int, std::vector<std::string>> get_source(std::ifstream*);

// Source: D:\dev\SequiLanguage\SequiLanguage\test\example1.txt

int main()
{
	std::string sourcename;
	
	std::cout << "File to interpret: " << std::endl;
	std::cout << "                   $ ";

	std::cin >> sourcename;

	std::ifstream file_reader(sourcename);

	if (!file_reader.is_open())
	{
		std::cout << "Failed to read given file." << std::endl;
		system("pause");
		return 1;
	}

	std::map<int, std::vector<std::string>> source = get_source(&file_reader);

	// Label pool

	std::map<std::string, int> labels;

	// Global variables pool

	std::vector<Variable<void*>> globals;

	for (int i = 1; i <= source.size(); i++)
	{
		std::vector<std::string> instruction = source.at(i);

		std::string key = instruction.at(0);
		
		// Instructions handling
		
		if (key == "print_plain")
		{
			std::string text = instruction.at(1);
			if (text == "/#ln")
				std::cout << std::endl;
			else
				std::cout << text;
		}
		else if (key == "goto")
		{
			std::string label_name = instruction.at(1);
			int next_line = labels.at(label_name);
			i = next_line;
		}
		else if (key == "var")
		{
			std::string var_name = instruction.at(1);
			std::string var_type = instruction.at(2);
			std::string var_value = instruction.at(3);

			if (var_type == "integer")
				globals.push_back(Variable<int>(var_name));
		}
		else if (key == "label")
		{
			std::string label_name = instruction.at(1);
			labels.emplace(label_name, i);
		}
	}

	system("pause");
	
	return 0;
}

std::map<int, std::vector<std::string>> get_source(std::ifstream* file_reader)
{
	std::map<int, std::vector<std::string>> source;
	std::string line;
	int line_counter = 0;

	while (getline(*file_reader, line))
	{
		line_counter++;

		std::vector<std::string> elements;
		std::string temp = "";

		for (int i = 0; i < line.size(); i++)
		{
			if (line.at(i) == ' ')
			{
				elements.push_back(temp);
				temp = "";
			}
			else if (line.at(i) == '"')
			{
				line.erase(0, i + 1);
				temp = line;
				break;
			}
			else
				temp += line.at(i);
		}

		if (temp != "")
			elements.push_back(temp);

		if (line == "")
			line_counter--;
		else
			source.emplace(line_counter, elements);
	}

	return source;
}