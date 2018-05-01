#pragma once

#include <string>

template <typename T>
struct Variable
{
	std::string binder;
	T value;

	Variable(std::string name)
	{
		
	}
};