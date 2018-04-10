#pragma once
#include <stdexcept>
#include <string>

class InsuffcientSpace : public std::runtime_error
{
public:
	explicit InsuffcientSpace(const char* msg)
		: std::runtime_error(("not enough space: " + std::string(msg)).c_str())
	{
	}
};