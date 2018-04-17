#pragma once
#include <stdexcept>
class InsuffcientSpace : public std::runtime_error
{
public:
	explicit InsuffcientSpace(const char* msg)
		: std::runtime_error(("not enough space: " + std::string(msg)).c_str())
	{
	}
};

class PraserError : public std::runtime_error
{
public:
	explicit PraserError(const char* msg)
		: std::runtime_error(("praser error: " + std::string(msg)).c_str())
	{
	}
};
class TableError : public std::runtime_error
{
public:
	explicit TableError(const char* msg)
		: std::runtime_error(("table error: " + std::string(msg)).c_str())
	{
	}
};
class IndexError : public std::runtime_error
{
public:
	explicit IndexError(const char* msg)
		: std::runtime_error(("Index error: " + std::string(msg)).c_str())
	{
	}
};
class InsertError : public std::runtime_error
{
public:
	explicit InsertError(const char* msg)
		: std::runtime_error(("Insert error: " + std::string(msg)).c_str())
	{
	}
};

#define fck() do {cout<<"fuck123"<<endl; exit(0);}while(0)