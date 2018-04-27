#pragma once
#include "token.h"
#include <iostream>
#include <ostream>

class Condition {
	friend ostream& operator<<(ostream& out, const Condition &c) {
		out << "codition: " << "attriName: " << c.attriName << "\top: " << c.op << "\ttoken: " << c.token;
		return out;
	};
public:
	void init(const string& tableName) const {
		auto attr = CatalogManager::instance().attribute(tableName, attriName);
		if (attr.type != token.type) {
			string res("not existed: " + str());
			throw ConditionError(res.c_str());
		}
		i = attr.i;
		indexName = attr.indexName;
	}
	string str() const {
		std::ostringstream out;
		out << *this;
		return out.str();
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
	mutable string indexName;
	string op;
	Token token;
	mutable int i;
};