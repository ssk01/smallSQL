#pragma once
#include "catalogData.h"
#include "RecordManager.h"

void addTable(const string& name, vector<Attribute>& attr) {
	CatalogManager::instance().addTable(name, attr);
	RecordManager::instance().createTable(name, CatalogManager::instance().getEntrySize(name));
}
void insertRecord(const string &name, vector<string>& value) {
	auto content = CatalogManager::instance().toEntry(name, value);
	//CatalogManager::instance().showTableRecord(name, content);
	//return;
	cout << "content "<< string(content) << endl;
	RecordManager::instance().insertRecord(name, content);
}
void showTableRecord(const string &name){
	//CatalogManager::instance().
	auto values = RecordManager::instance().showReocrd(name);
	for (auto &value : values) {
		CatalogManager::instance().showTableRecord(name, value);
	}
}
//todo 
//toentry and show;