#pragma once
#include "token.h"
#include <iostream>
class Condition {
	friend ostream& operator<<(ostream& out, const Condition &c) {
		out << "codition: " << "attriName: " << c.attriName << "\top: " << c.op << "\ttoken: " << c.token;
		return out;
	};
public:
	void initAttrbuteOrder(const string& tableName) const {
		i = CatalogManager::instance().attributeOrder(tableName, attriName);
	}
	template<class T>
	static bool eval(string op, T lhs, T rhs) {
		if (op == "!=") return lhs != rhs;
		if (op == "=") return lhs == rhs;
		if (op == ">=") return lhs >= rhs;
		if (op == ">") return lhs > rhs;
		if (op == "<=") return lhs <= rhs;
		if (op == "<") return lhs < rhs;
	}
	Condition(){}
	Condition(string attriName, string op, Token token): attriName(attriName), op(op), token(token),i(-1){

	}
	string attriName;
	string op;
	Token token;
	mutable int i;
};