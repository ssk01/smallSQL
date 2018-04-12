#pragma once
#include "catalogData.h"
#include "RecordManager.h"
#include "index.h"
#include "token.h"
#include <tuple>
void addTable(const string& name, vector<Attribute>& attr) {
	CatalogManager::instance().addTable(name, attr);
	RecordManager::instance().createTable(name, CatalogManager::instance().getEntrySize(name));
}
void insertRecord(const string &name, const vector<Token>& content) {
	auto value = CatalogManager::instance().toEntry(name, content);
	//CatalogManager::instance().showTableRecord(name, content);
	//return;
	//cout << "content "<< string(content) << endl;
	auto record = RecordManager::instance().insertRecord(name, value);
	for (auto indexAttri : CatalogManager::instance().getIndexAttri(name)) {
		cout << "indexs record " << endl;
		auto i = std::get<0>(indexAttri);
		auto indexName = std::get<1>(indexAttri);
		auto type = std::get<2>(indexAttri);
		if (type == "int") {
			/*auto key = CatalogManager::instance().intAttri(name, content, i);*/
			CatalogManager::instance().insertIndex(name, indexName, content[i], i, record);
		}
	}
}
void showTableRecord(const string &name){
	//CatalogManager::instance().
	auto values = RecordManager::instance().showReocrd(name);
	for (auto &value : values) {
		CatalogManager::instance().showTableRecord(name, value);
	}
}
void addIndex(const string& tableName, const string& indexName, const string& attriName) {
	CatalogManager::instance().addIndex(tableName, indexName, attriName);
}
void showIndex(const string& tableName, const string& indexName) {
	CatalogManager::instance().showIndex(tableName, indexName);
}
//todo 
//toentry and show;