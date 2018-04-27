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
	string str()const {
		std::ostringstream out;
		out << *this;
		return out.str();
	}
	bool operator==(string c) {
		return content == c;
	}
	bool operator!=(string c) {
		return content != c;
	}
	int toInt() const {
		return atoi(content.c_str());
	}
	float toFloat() const {
		std::istringstream in(content);
		float key;
		in >> key;
		return key;
	}
	string toString() const {
		return content;
	}
};
void show(Token t);