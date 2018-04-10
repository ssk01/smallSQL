#pragma once
#include "attribute.h"
#include "RecordManager.h"
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
	void showTableRecord(const string& name, char *value) {
		cout <<"ttt" << *(int*)value << endl;

		return nameTables[name].showRecord(value);
	}
	//void showTableRecord(const string& name) {
	//	return nameTables[name].showRecord();
	//}
	char * toEntry(const string& name, const vector<string>& content) {
		return nameTables[name].toEntry(content);
	}
	int getEntrySize(const string& name) {
		return nameTables[name].size();
	}
private:
	CatalogManager(){}
	map<string, Table> nameTables;
};
