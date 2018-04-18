#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

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

	for (int i = 1; i <= source.size(); i++)
	{
		std::vector<std::string> instruction = source.at(i);

		for (int j = 0; j < instruction.size(); j++)
		{
			std::string element = instruction.at(j);
			
			// Instructions handling

			if (element == "print_plain")
			{
				std::string next_element = instruction.at(j + 1);
				if (next_element == "/#ln")
					std::cout << std::endl;
				else
					std::cout << next_element;
			}
			else if (element == "goto")
			{
				std::string next_element = instruction.at(j + 1);
				i = std::stoi(next_element) - 1; // -1 because i will be incremented at the beginning of the next iteration
				break;
			}
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