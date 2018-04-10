#pragma once
#include <string>
#include <iostream>

#include <vector>
#include <sstream>
using std::string;
using std::vector;
using std::istringstream;
using std::ostream;
using std::cout;
using std::endl;
class Attribute {
	friend class Table;
	friend ostream& operator<<(ostream& out, Attribute a) {
		out << a.name << "\t" << a.type << "\t" << a.size << "\t";
		return out;
	}
public:
	Attribute(string name, string type, int size) :name(name), type(type), size(size){}
private:
	string name;
	string type;
	int size;
};

class Table {
public:
	Table(const string& name, vector<Attribute>& attr) : tablename(name), attributes(attr) {
	}
	Table(){}
	void showRecord(char *value) {
		int offset = 0;
		for (auto &attri : attributes) {
			if (attri.type == "int") {
				int i;
				memcpy(&i, value + offset, attri.size);
				cout << attri << i << endl;
			}
			if (attri.type == "char") {
				cout << attri << value + offset << endl;
			}
			/*if (attri.type == "float") {
		 
				cout << attri << i << endl;
			}*/
			offset += attri.size;
		}
	}
	char* toEntry(const vector<string>& content) {
		char *value = new char[size()];
		cout << *(int*)value << endl;
		int offset = 0;
		//for (auto &a : attributes) {
		for (auto i = 0; i < content.size(); i++) {
			auto a = attributes[i]; 
			auto c = content[i];
			if (a.type == "int") {
				int v;
				istringstream in(c);
				in >> v;
				memcpy(value + offset, &v, a.size);
				int a1 = -1;
				memcpy(&a1, value + offset, a.size);
				cout << a1 <<" "<< v <<endl;
			} else if (a.type == "char") {
				memcpy(value + offset, c.c_str(), c.size());
				//memcpy(value+offset)
				memset(value + offset+c.size(),char('\0'), 1);
			}
			offset += a.size;
		}
		cout << *(int*)value << endl;

		return value;
	}
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
