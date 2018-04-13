#pragma once
#include <string>
#include <iostream>
#include "util.h"
#include <vector>
#include <sstream>
#include <tuple>
#include "token.h"

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
	Attribute(string name, string type, int s = 0, bool u = false) :name(name), type(type), indexName(""), size(s), unique(u){
		if (type == string("int")) {
			size = 4;
		}
		else if (type == string("float")) {
			size = 4;
		} 
		cout << type << (type == string("int")) << endl;
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
private:
	string name;
	string type;
	string indexName;
	bool unique;
	int size;
};

class Table {
public:
	Table(const string& name, vector<Attribute>& attr) : tablename(name), attributes(attr) {
	}
	Table(){}
	//todo can be do init
	int attributeOffset(int j) {
		int off = 0;
		for (auto i = 0; i < j; i++) {
			auto a = attributes[i];
			off += a.size;
		}
		return off;
	}
	bool indexExisted(const string& indexName) {
		for (auto &a : attributes) {
			if (a.indexName == indexName) {
				return true;
			}
		}
		return false;
	}
	string getIndexName(int i) {
		assert(attributes[i].indexName != "");
		return attributes[i].indexName;
	}
	void addIndex(const string& indexName, const string& attrName) {
		for (auto &a : attributes) {
			if (a.name == attrName) {
				a.setIndex(indexName);
				return;
			}
		}
	}
	string attributeType(const string& attrName) {
		for (auto &a : attributes) {
			if (a.name == attrName) {
				return a.type;
			}
		}
	}
	void showRecord(char *value) {
		int offset = 0;
		for (auto &attri : attributes) {
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
	using aa = tuple<int, string, string>;
	vector<aa> getIndexAttri() {
		vector<tuple<int, string, string>>  res;
		for (auto i = 0; i < attributes.size(); i++) {
			auto a = attributes[i];
			if (a.indexName != "") {
				res.emplace_back(i, a.indexName, a.type);
			}
		}
		return res;
	}
	vector<tuple<int, bool>> getUniqueAttri() {
		vector<tuple<int, bool>>  res;
		for (auto i = 0; i < attributes.size(); i++) {
			auto a = attributes[i];
			if (a.unique == true) {
				res.emplace_back(i, a.indexName != "");
			}
		}
		return res;
	}
	//int attriValue(const vector<string>& content, int i) {
	//	auto a = attributes[i];
	//	auto c = content[i];
	//	if (a.type == "int" )
	//	a.val(c,)
	//}
	char* toEntry(const vector<Token>& content) {
		char *value = new char[size()];
		cout << *(int*)value << endl;
		int offset = 0;
		//for (auto &a : attributes) {
		for (auto i = 0; i < content.size(); i++) {
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
	/*			int a1 = -1;
				memcpy(&a1, value + offset, a.size);
				cout << a1 << " " << v << endl;*/
			}
			else if (a.type == "char") {
				memcpy(value + offset, c.c_str(), c.size());
				//memcpy(value+offset)
				memset(value + offset + c.size(), char('\0'), 1);
			}
			else if (a.type == "float") {
				float v;
				istringstream in(c);
				in >> v;
				memcpy(value + offset, &v, a.size);
	/*			cout << "float" <<v << endl;
				sprintf(value + offset, "%f", v);
				float i;
				memcpy(&i, value + offset, 4);
				cout << "\t value: " << i << endl;*/
			}
			offset += a.size;
		}

		return value;
	}
	//int intAttri(const string& content) {
	//	auto a = attributes[i];
	//	return a.val(content, int(0));
	//}
	//string charAttri(const string& content, int i) {
	//	auto a = attributes[i];
	//	return a.val(content, string());
	//}
	int size() {
		int s = 0;
		for (auto & a : attributes) {
			s += a.size;
		}
		return s;
	}
	string tablename;
	vector<Attribute> attributes;
};
class value {

};

//char* insert(const string& name, const vector<string>& content)
//{
//	auto Table = getTable(name);
//	assert(content.size() == Table.attributeNum());
//	for (int i = 0; i < Table.attributeNum(); i++) {
//		auto attri = Table.getAttri[i];
//		if (attri[i].type == "int") {
//			int ivalue;
//			istringstream (content[i]) >> ivalue;
//			memcpy(res + offset, &ivalue, sizeof(ivalue));
//		}
//	}
//	auto result = "";
//
//
//}
