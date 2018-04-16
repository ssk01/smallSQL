#pragma once
#include "attribute.h"
#include <tuple>
#include <set>
#include "token.h"
#include <map>
using std::map;
using std::tuple;
class CatalogManager {
public:
	static CatalogManager& instance() {
		static CatalogManager cm;
		return cm;
	}
	int attributeOffset(const string& name, int i) {
		return nameTables[name].attributeOffset(i);
	}
	int attributeOrder(const string& name, const string& attrName) {
		return nameTables[name].attributeOrder(attrName);
	}
	void addTable(const string& name, vector<Attribute>& attr) 
	{
		nameTables[name] = { name, attr };
	}

	string attributeType(const string& name, const string& attrName) {
		return nameTables[name].attributeType(attrName);
	}
	void showTableRecord(const string& name, char *value) {
		//cout <<"ttt" << *(int*)value << endl;
		cout << "Table: " + name << endl;
		return nameTables[name].showRecord(value);
	}

	void addIndex(const string& name, const string& indexName, const string& attrName) {
		nameTables[name].addIndex(indexName, attrName);
	}
	//void showTableRecord(const string& name) {
	//	return nameTables[name].showRecord();
	//}
	char * toEntry(const string& name, const vector<Token>& content) {
		return nameTables[name].toEntry(content);
	}
	int getEntrySize(const string& name) {
		return nameTables[name].size();
	}
	string getIndexName(const string& name, int i) {
		return nameTables[name].getIndexName(i);
	}
	//bool isIndex(const string& name, int i) {
	//	return nameTables[name].isIndex(i);
	//}
	vector<tuple<int, bool>> getUniqueAttri(const string& name) {
		return nameTables[name].getUniqueAttri();
	}
	using aa = tuple<int, string, string>;
	vector<aa> getIndexAttri(const string& name) {
		return nameTables[name].getIndexAttri();
	}
	bool existed(const string& name) {
		if (nameTables.find(name) == nameTables.end()) {
			return false;
		}
		return true;
	}
	bool existed(const string& name, const string& indexName) {
		return nameTables[name].indexExisted(indexName);
	}
private:


	CatalogManager() {
	}
	map<string, Table> nameTables;
	//map<string, set<string>> intIndex;
	//map<string, set<string>> stringIndex;
};
