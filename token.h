#pragma once
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
using std::string;
using std::cout;
using std::endl;
using std::ostream;
class Token {
	friend ostream& operator<<(ostream& out, Token t);
public:
	string type;
	string content;
	int line;
	int col;
	Token() = default;
	Token(string type, string content, int line = 0, int col = 0) :type(type), content(content), line(line), col(col) {}
	Token(string content, int line = 0, int col = 0):type(content), content(content), line(line), col(col) {
	}
	bool operator==(string c) {
		return content == c;
	}
	bool operator!=(string c) {
		return content != c;
	}
	int toInt() const {
		std::istringstream in(content);
		int key;
		in >> key;
		return key;
	}
	string toString() const {
		return content;
	}
};
void show(Token t) {
	std::cout << "type: " << t.type << "\tcontent: " << t.content << std::endl;
}
ostream& operator<<(ostream& out, Token t)
{
	out << "type: " << t.type << "\tcontent: " << t.content;
	return out;
};