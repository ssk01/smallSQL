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
	//auto uniq_offsets = CatalogManager::instance().unique
	
	
	//todo assert operator;
	
	
	for (auto uniqueAttr : CatalogManager::instance().getUniqueAttri(name)) {
		auto i = std::get<0>(uniqueAttr);
		auto unique = std::get<1>(uniqueAttr);
		if (unique) {
			auto indexName = CatalogManager::instance().getIndexName(name, i);
			if (IndexManager::instance().indexRecordExisted(name, indexName, content[i])) {
					//"fuck"
				assert(0);

			};
		}
		else {
			auto offset = CatalogManager::instance().attributeOffset(name, i);
			if (RecordManager::instance().recordExist(name, content[i], i, offset)) {
				//cout << "fuck"<<a
				assert(0);
			}
		}
	}
	auto value = CatalogManager::instance().toEntry(name, content);

	auto record = RecordManager::instance().insertRecord(name, value);
	for (auto indexAttri : CatalogManager::instance().getIndexAttri(name)) {
		cout << "indexs record " << endl;
		auto i = std::get<0>(indexAttri);
		auto indexName = std::get<1>(indexAttri);
		auto type = std::get<2>(indexAttri);
			/*auto key = CatalogManager::instance().intAttri(name, content, i);*/
		IndexManager::instance().insertIndex(name, indexName, content[i], record);
	}
}
void showTableRecord(const string &name){
	//CatalogManager::instance().
	auto values = RecordManager::instance().showReocrd(name);
	for (auto &value : values) {
		CatalogManager::instance().showTableRecord(name, value);
	}
}
void addIndex(const string& tableName, const string& indexName, const string& attrName) {
	IndexManager::instance().addIndex(tableName, indexName, attrName);
}
void showIndex(const string& tableName, const string& indexName) {
	IndexManager::instance().showIndex(tableName, indexName);
}
//todo 
//toentry and show;