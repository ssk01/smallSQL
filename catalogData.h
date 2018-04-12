#pragma once
#include "attribute.h"
#include "RecordManager.h"
#include "index.h"
#include <tuple>
#include <set>
#include "token.h"

using std::tuple;
using std::set;
class CatalogManager {
public:
	static CatalogManager& instance() {
		static CatalogManager cm;
		return cm;
	}
	void addTable(const string& name, vector<Attribute>& attr) 
	{
		nameTables[name] = { name, attr };
		RecordManager::instance().createTable(name, nameTables[name].size());
	}
	void addIndex(const string& name, const string& indexName, const string& attriName) {
		if (existed(name, indexName)) {
			//return false;
		}
		auto type = attributeType(name, attriName);
		nameTables[name].addIndex(indexName, attriName);
	}
	string attributeType(const string& name, const string& attriName) {
		return nameTables[name].attributeType(attriName);
	}
	void showTableRecord(const string& name, char *value) {
		cout <<"ttt" << *(int*)value << endl;

		return nameTables[name].showRecord(value);
	}
	void showIndex(const string& tableName, const string& indexName) {
		existed(tableName, indexName);
		nameIndexs[tableName].showIndex(indexName);
	}

	void insertIndex(const string& tableName, const string& indexName, const Token& content,int i, RecordList::Record record) {
		//exist(name, indexName, key);
		if (existed(tableName, indexName)) {
			if (content.type == "int") {
				auto key = nameTables[tableName].intAttri(content.content, i);
				nameIndexs[tableName].insertIndex(indexName, key, record);
			} else if (content.type == "char") {
				auto key = nameTables[tableName].charAttri(content.content, i);
				nameIndexs[tableName].insertIndex(indexName, key, record);

			}
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
	using aa = tuple<int, string, string>;
	vector<aa> getIndexAttri(const string& name) {
		return nameTables[name].getIndexAttri();
	}

private:
	bool existed(const string& name, const string& indexName) {
		return nameTables[name].indexExisted(indexName);
	}


	CatalogManager(){}
	map<string, Table> nameTables;
	map<string, Indexs> nameIndexs;
	//map<string, set<string>> intIndex;
	//map<string, set<string>> stringIndex;
};
