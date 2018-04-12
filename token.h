#pragma once
#include <string>
using std::string;
class Token {
public:
	Token(string type, string content):type(type), content(content){}
	string type;
	string content;
};