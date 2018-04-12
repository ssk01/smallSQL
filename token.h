#pragma once
#include <string>
#include <iostream>
using std::string;
class Token {
public:
	Token(string type, string content):type(type), content(content){}
	string type;
	string content;
	Token(string content):type(content), content(content) {
	}
};
void show(Token t) {
	std::cout << "type: " << t.type << "\tcontent: " << t.content << std::endl;
}

