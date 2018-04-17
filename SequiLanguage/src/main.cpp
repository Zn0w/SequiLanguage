#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

int main()
{
	std::string sourcename;
	std::string line;
	
	std::cout << "File to interpret: " << std::endl;
	std::cout << "                   $ ";

	std::cin >> sourcename;

	std::ifstream file_reader(sourcename);

	if (!file_reader.is_open())
		std::cout << "Failed to read given file." << std::endl;

	std::map<int, std::vector<std::string>> source;

	int line_counter = 0;
	while (getline(file_reader, line))
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
			else
				temp += line.at(i);
		}

		elements.push_back(temp);

		source.emplace(line_counter, elements);
	}

	for (int i = 1; i <= source.size(); i++)
	{
		std::vector<std::string> instruction = source.at(i);

		for (int j = 0; j < instruction.size(); j++)
		{
			std::string element = instruction.at(j);
			
			// Instruction handling
			if (element == "print")
			{
				std::string next_element = instruction.at(j + 1);
				if (next_element.at(0) == '"' && next_element.at(next_element.size() - 1) == '"')
					std::cout << next_element;
			}
		}
	}

	system("pause");
	
	return 0;
}