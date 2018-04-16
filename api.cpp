#include "api.h"
#include <algorithm>
#include <set>
using std::set;

void selects(const string& tableName, const vector<Condition>& conds) {
	for (auto &c : conds) {
		c.initAttrbuteOrder(tableName);
	}
	//indexes;
	auto indexesConditon = vector<Condition>();
	auto normalConditon = vector<Condition>();
	std::for_each(conds.begin(), conds.end(), [&](const Condition & c) {
		if (CatalogManager::instance().existed(tableName, c.attriName)) {
			indexesConditon.push_back(c);
		}
		else {
			normalConditon.push_back(c);
		}
	});
	vector<char *> ByteValues;
	if (!indexesConditon.empty()) {
		set<pair<int, int>> matched;
		for (auto &c : conds) {
			auto tmp = IndexManager::instance().select(tableName, c);
			matched.insert(tmp.begin(), tmp.end());
		}
		ByteValues = RecordManager::instance().select(tableName, { matched.begin(), matched.end() }, normalConditon);
	}
	//no indexes;
	else {
		cout << "no indexes " << endl;
		ByteValues = RecordManager::instance().select(tableName, conds);
	}
	cout << "select result:" << endl;
	for (auto &value : ByteValues) {
		CatalogManager::instance().showTableRecord(tableName, value);
	}
}

void addTable(const string& name, vector<Attribute>& attr) {
	CatalogManager::instance().addTable(name, attr);
	RecordManager::instance().createTable(name, CatalogManager::instance().getEntrySize(name));
}
void insertRecord(const string &name, const vector<Token>& content) {
	//auto uniq_offsets = CatalogManager::instance().unique


	//todo assert operator;


	for (auto uniqueAttr : CatalogManager::instance().getUniqueAttri(name)) {
		auto i = std::get<0>(uniqueAttr);
		auto hasIndex = std::get<1>(uniqueAttr);
		if (hasIndex) {
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
				cout << content[i] << endl;
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
void showTableRecord(const string &name) {
	//CatalogManager::instance().
	auto values = RecordManager::instance().showReocrd(name);
	cout << "table :" + name << endl;
	for (auto &value : values) {
		CatalogManager::instance().showTableRecord(name, value);
	}
	cout << "\n" << endl;
}
void addIndex(const string& tableName, const string& indexName, const string& attrName) {
	//auto type = CatalogManager::instance().attributeType(name, attrName);
	if (CatalogManager::instance().existed(tableName, indexName)) {
		//return false;
	}
	CatalogManager::instance().addIndex(tableName, indexName, attrName);
}
void showIndex(const string& tableName, const string& indexName) {
	IndexManager::instance().showIndex(tableName, indexName);
}