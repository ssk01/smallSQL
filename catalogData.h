#pragma once
#include <tuple>
#include <set>
#include <map>
#include "attribute.h"
#include "errors.h"
#include "token.h"
using std::map;
using std::tuple;
class CatalogManager {
public:
	static CatalogManager& instance() {
		static CatalogManager cm;
		return cm;
	}
	vector<string> name() {
		vector<string> names;
		for (auto &kv : nameTables) {
			names.emplace_back(kv.first);
		}
		return names;
	}
	int attributeOffset(const string& name, int i) {
		return nameTables[name].attributeOffset(i);
	}

	int attributeOrder(const string& name, const string& attrName) {
		return nameTables[name].attributeOrder(attrName);
	}
	const Attribute& attribute(const string& name, const string& attrName) {
		return nameTables[name].attribute(attrName);
		
	}

	void addTable(const string& name, vector<Attribute>& attr) 
	{
		nameTables[name] = { name, attr };
	}
	void dropTable(const string& name)
	{
		nameTables[name].drop();
		nameTables.erase(name);
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
		if (!nameTables[name].addIndex(indexName, attrName)) {
			string res = "tablename: " + name + "no  addIndexattrName: " + attrName + "for indexName: " + indexName;
			throw IndexError(res.c_str());
		}
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
	//pair<string, int> getIndexName(const string& name) {

	//}
	string getIndexName(const string& name, int i) {
		return nameTables[name].getIndexName(i);
	}
	//bool isIndex(const string& name, int i) {
	//	return nameTables[name].isIndex(i);
	//}
	vector<tuple<int, bool>> getUniqueAttri(const string& name) {
		return nameTables[name].getUniqueAttri();
	}
	void dropIndex(const string& name, const string& indexName) {
		nameTables[name].dropIndex(indexName);
	}
	using aa = tuple<int, string, string>;
	vector<aa> getIndexAttri(const string& name) {
		return nameTables[name].getIndexAttri();
	}
	void assertTypeEqual(const string& name, const vector<Token>& content) {
		string res;
		if (!nameTables[name].typeEqual(content, res)) {
			string msg = "table: " + name + "type not match :\n" + res;
			throw TableError(msg.c_str());
		}
	}
	void assertExisted(const string& name) {
		if (!existed(name)) {
			string msg = "table: " + name + "not existed ";
			throw TableError(msg.c_str());
		}
	}
	void assertNotExisted(const string& name) {
		if (existed(name)) {
			string msg = "table: " + name + " existed ";
			throw TableError(msg.c_str());
		}
	}
	void assertExisted(const string& name, const string& indexName) {
		if (!existed(name, indexName)) {
			string msg = "table: " + name + "'s +index: " + name + "  not existed ";
			throw TableError(msg.c_str());
		}
	}
	void assertNotExisted(const string& name, const string& indexName) {
		if (existed(name, indexName)) {
			string msg = "table: "+ name+"'s +index: " + name + " already existed ";
			throw TableError(msg.c_str());
		}
	}
	bool existed(const string& name) {
		if (nameTables.find(name) == nameTables.end()) {
			return false;
		}
		return true;
	}
	bool existed(const string& name, const string& indexName) {
		return (existed(name) && nameTables[name].indexExisted(indexName));
	}
private:

	~CatalogManager() {
		cout << "deconstruct catalog" << endl;
		save();
	}
	CatalogManager() {
		load();
	}
	void save() {
		std::ofstream out{ catalogDataDir + "tablename.txt", std::ios::trunc };
		out << nameTables.size() << endl;
		for (auto value : nameTables) {
			out << value.first << "\n";
			cout << value.first << endl;
			value.second.save();
		}
	}
	void load() {
		std::ifstream in{ catalogDataDir + "tablename.txt" };
		if (in.is_open()) {
			size_t size;
			in >> size;
			string tableName;
			while (size--> 0) {
				in >> tableName;
				auto table = Table::load(tableName);
				nameTables[tableName] = table;
				LOG("load catalogData: table attributes size", table.attributes.size());
				//cout << table.attributes.size();
			}
			/*cout << "after load " << endl;
			for (auto value : nameTables) {
				cout << value.first << "\n";
				cout << value.second << endl;
			}*/
		}
		else {
			//cout << "not open catalogData" << endl;
			LOG("load catalogData: empty");
		}

	}
	map<string, Table> nameTables;
	//map<string, set<string>> intIndex;
	//map<string, set<string>> stringIndex;
};
