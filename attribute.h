#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <ostream>
#include <fstream>
#include <cstdio>
#include <tuple>
#include "token.h"
#include "util.h"
#include "errors.h"
#include "log.h"
using std::tuple;
using std::string;
using std::vector;
using std::istringstream;
using std::ostream;
using std::cout;
using std::endl;

class Attribute {
	friend class Table;
	friend ostream& operator<<(ostream& out, Attribute a) {
		out << "name: \t"<< a.name << "  type:\t" << a.type << "  size:\t" << a.size << " indexName:\t" 
			<< a.indexName << " unique:\t" << a.unique;
		return out;
	};
	
public:
	string str() {
		std::ostringstream out;
		out << *this;
		return out.str();
	}
	Attribute(){
	}
	Attribute(string name, string type, int i,int s = 0, bool u = false, string indexName="") :name(name), type(type), indexName(indexName), size(s), unique(u), i(i){
		if (type == string("int")) {
			size = 4;
		}
		else if (type == string("float")) {
			size = 4;
		} 
	}
	void serialize(std::ofstream &out) {
		out << name;
		out << " ";
		out << type;
		out << " ";
		out << i;
		out << " ";
		out << size;
		out << " ";
		out << unique;
		out << " ";
		out << indexName;
		out << " :";
		out << "\n";
	}
	Attribute(std::ifstream &in) {
		in >> name;
		in >> type;
		in >> i;
		in >> size;
		in >> unique;
		in >> indexName;
		if (indexName == ":") {
			indexName = "";
		}
		else {
			in >> string("");
		}
	}
	template<class T>
	T val(const string& c, T tmp) {
		T v;
		istringstream in(c);
		in >> v;
		return v;
	}
	void setIndex(const string& Name) {
		unique = true;
		indexName = Name;
	}
	int i;
	string name;
	string type;
	string indexName;
	bool unique;
	int size;
	mutable int off;
private:
};

class Table {
public:
	Table(const string& name, vector<Attribute>& attr) : tablename(name), attributes(attr) {
		init();
	}
	friend ostream& operator<<(ostream& out, Table t) {
		out << "tablename " << t.tablename;
		for (auto &a : t.attributes) {
			out << a << endl;
		}
		return out;
	}
	Table() {}
	Table(const string& name) : tablename(name) { 
		load();
		init();
	}
	void load() {
		std::ifstream in{ tableInfoPath(tablename)};
		if (in.is_open()) {
			string _tableName;
			in >> _tableName;
			assert(_tableName == tablename);
			int attrSize;
			in >> attrSize;
			vector<Attribute> attr;
			for (int i = 0; i < attrSize; i++) {
				attr.emplace_back(in);
			}
			attributes = attr;
			LOG("load table: ", tablename);
		}
		else {
			LOG("load table: empty");
		}
	}
	void drop() {
		std::remove(tableInfoPath(tablename).c_str());
	}
	void save() {
		std::ofstream out{ tableInfoPath(tablename), std::ios::trunc };
		out.seekp(0, std::ios::beg);
		out << tablename << "\n";
		out << attributes.size() << "\n";
		for (auto &a : attributes) {
			a.serialize(out);
		}

	}
	bool typeEqual(const vector<Token>& content, string& res) {
		if (content.size() != attributes.size()) {
			return false;
		}
		for (size_t i = 0; i < content.size(); i++) {
			if (content[i].type != attributes[i].type) {
		
				res = string("content: ") + content[i].str() + "\n attribute: " + attributes[i].str();
				return false;
			}
		}
		return true;
	}


	bool indexExisted(const string& indexName) {
		for (auto &a : attributes) {
			if (a.indexName == indexName) {
				return true;
			}
		}
		return false;
	}
	void dropIndex(const string& indexName) {
		for (auto &a : attributes) {
			if (a.indexName == indexName) {
				a.indexName = "";
				//a.unique = false;
				return;
			}
		}
	}

	void __attributeOffset() {
		int off = 0;
		for (size_t i = 0; i < attributes.size(); i++) {
			auto& a = attributes[i];
			a.off = off;
			off = off + a.size;
		}
	}

	Attribute& attribute (const string& attrName)  {
		for (auto &a : attributes) {
			if (a.name == attrName) {
				return a;
			}
		}
		string res(attrName + " not exist");
		throw AttributeError(res.c_str());
	}

	bool addIndex(const string& indexName, const string& attrName) {
		for (auto &a : attributes) {
			if (a.name == attrName) {
				a.setIndex(indexName);
				return true;
			}
		}
		return false;
	}
	void showRecord(char *value) {
		int offset = 0;
		for (auto attri : attributes) {
			if (attri.type == "int") {
				int i;
				memcpy(&i, value + offset, attri.size);
				cout << attri <<"\t value: "<< i << endl;
			}
			else if (attri.type == "char"){
				cout << attri << "\t value: " << string(value + offset) << endl;
			}
			else if (attri.type == "float") {
				float i;
				memcpy(&i, value + offset, attri.size);
				cout << attri << "\t value: " << i << endl;
			}
			offset += attri.size;
		}
	}
	vector<Attribute> getIndexAttr() {
		vector<Attribute>  res;
		for (size_t i = 0; i < attributes.size(); i++) {
			auto a = attributes[i];
			if (a.indexName != "") {
				res.emplace_back(a);
			}
		}
		return res;
	}
	vector<Attribute> getUniqueAttri() {
		vector<Attribute>  res;
		for (size_t i = 0; i < attributes.size(); i++) {
			auto a = attributes[i];
			if (a.unique == true) {
				res.emplace_back(a);
			}
		}
		return res;
	}

	char* toEntry(const vector<Token>& content, char* value) {
		int offset = 0;
		for (size_t i = 0; i < content.size(); i++) {
			auto a = attributes[i];
			auto c = content[i].content;
			if (a.type != content[i].type) {
				exit(9);
			}
			if (a.type == "int") {
				int v;
				istringstream in(c);
				in >> v;
				memcpy(value + offset, &v, a.size);
			}
			else if (a.type == "char") {
				memcpy(value + offset, c.c_str(), c.size());
				memset(value + offset + c.size(), char('\0'), 1);
			}
			else if (a.type == "float") {
				float v;
				istringstream in(c);
				in >> v;
				memcpy(value + offset, &v, a.size);
			}
			offset += a.size;
		}

		return value;
	}
	void init() {
		__size();
		__attributeOffset();;
	}
	int size() {
		return entrySize;
	}
	void __size() {
		int s = 0;
		for (auto & a : attributes) {
			s += a.size;
		}
		entrySize = s;
	}
	int entrySize;
	string tablename;
	vector<Attribute> attributes;
};


