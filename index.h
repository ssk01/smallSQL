#pragma once
#include "RecordManager.h"
#include "catalogData.h"
#include <set>
using std::set;
template<class T>
class Index {
public:
	Index() {}
	void insertIndex(T key, RecordList::Record record) {
		//assert();
		indexs[key] = record;
	}
	void showIndex() {
		cout << "show indexs" << endl;
		for (auto r = indexs.begin(); r != indexs.end(); r++) {
			cout <<"key:  " << (*r).first << endl;
			cout << "blockIndex: " << (*r).second.first << " RecordIndex:" << (*r).second.second << endl;
		}

	}
	bool recordExisted(T key) {

		if (indexs.find(key) != indexs.end()) {
			return true;
		}

		return false;
	}
private:
	map<T, RecordList::Record> indexs;
	int i;
};
class Indexs {
	map<string, Index<int>> intIndex;
	map<string, Index<string>> charIndex;
public:

	Indexs() {};
	bool indexRecordExisted(const string& indexName, const Token& content) {
		if (content.type == "int") {
			return intIndex[indexName].recordExisted(content.toInt());
		}
		if (content.type == "char") {
			return charIndex[indexName].recordExisted(content.toString());
		}
	}
	void insertIndex(const string& indexName, int key, RecordList::Record record) {
		//assert();
		intIndex[indexName].insertIndex(key, record);
	}	
	void insertIndex(const string& indexName, string key, RecordList::Record record) {
		//assert();
		charIndex[indexName].insertIndex(key, record);
	}
	void showIndex(const string& indexName) {
		if (intIndex.find(indexName) != intIndex.end()) {
			intIndex[indexName].showIndex();
		}
		if (charIndex.find(indexName) != charIndex.end()) {
			charIndex[indexName].showIndex();
		}
	}
	
};
class IndexManager {
public:
	IndexManager(){}
	static IndexManager& instance() {
		static IndexManager im;
		return im;
	}
	void addIndex(const string& name, const string& indexName, const string& attrName) {
		if (CatalogManager::instance().existed(name, indexName)) {
			//return false;
		}
		auto type = CatalogManager::instance().attributeType(name, attrName);
		CatalogManager::instance().addIndex(name, indexName, attrName);
	}
	void showIndex(const string& tableName, const string& indexName) {
		if (CatalogManager::instance().existed(tableName, indexName)) {
			nameIndexs[tableName].showIndex(indexName);
		}
	}
	bool indexRecordExisted(const string& tableName, const string& indexName, const Token& content) {
		return nameIndexs[tableName].indexRecordExisted(indexName, content);
	}
	void insertIndex(const string& tableName, const string& indexName, const Token& content, RecordList::Record record) {
		//exist(name, indexName, key);
		if (CatalogManager::instance().existed(tableName, indexName)) {
			if (content.type == "int") {
				istringstream in(content.content);
				int key;
				in >> key;
				nameIndexs[tableName].insertIndex(indexName, key, record);
			}
			else if (content.type == "char") {
				auto key = content.content;
				nameIndexs[tableName].insertIndex(indexName, key, record);
			}
		}
	}
	map<string, Indexs> nameIndexs;
}; 