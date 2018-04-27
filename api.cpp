#include "api.h"
#include <algorithm>
#include <set>
#include "catalogData.h"
#include <chrono>
#include <iterator>
using std::set;

void dropIndex(const string& tableName, const string& indexName)
{
	CatalogManager::instance().assertExisted(tableName, indexName);
	IndexManager::instance().dropIndex(tableName, indexName);
	CatalogManager::instance().dropIndex(tableName, indexName);
}
void dropTable(const string& tableName) {
	CatalogManager::instance().assertExisted(tableName);
	auto blockIndexs = RecordManager::instance().dropTable(tableName);
	IndexManager::instance().dropIndex(tableName);
	CatalogManager::instance().dropTable(tableName);
	BufferManager::instance().dropTable(tableName, blockIndexs);

}

void addTable(const string& name, vector<Attribute>& attr) {
	CatalogManager::instance().assertNotExisted(name);
	CatalogManager::instance().addTable(name, attr);
	RecordManager::instance().createTable(name, CatalogManager::instance().getEntrySize(name));
}

std::function< pair<char *, Record>()> __selectGen(const string& tableName, const vector<Condition>& conds) {
	CatalogManager::instance().assertExisted(tableName);
	for (const auto &c : conds) {
		c.init(tableName);
	}
	//indexes;
	auto indexesConditon = vector<Condition>();
	auto normalConditon = vector<Condition>();
	std::for_each(conds.begin(), conds.end(), [&](const Condition & c) {
		if (c.indexName != "") {
			indexesConditon.push_back(c);
		}
		else {
			normalConditon.push_back(c);
		}
	});
	if (!indexesConditon.empty()) {
		vector<pair<int, int>> matched;
		for (auto &c : indexesConditon) {
			auto recs = IndexManager::instance().select(tableName, c);
			if (matched.empty()) {
				matched.assign(recs.begin(),recs.end());
			}
			else {
				std::vector<pair<int, int>> v_intersection;
				std::set_intersection(matched.begin(), matched.end(), recs.begin(), recs.end(),std::back_inserter(v_intersection));
				matched.clear();
				matched.assign(v_intersection.begin(),v_intersection.end());
			}
		}
		LOG("index records", matched.size(),"normal cond", normalConditon.size());
		return RecordManager::instance().selectGen(tableName, { matched.begin(), matched.end() }, normalConditon);
	}
	//no indexes;
	else {
		cout << "no indexes " << endl;
		return RecordManager::instance().selectGen(tableName, conds);
	}
}
// add index prev added record;
void addIndex(const string& tableName, const string& indexName, const string& attrName) {
	CatalogManager::instance().assertExisted(tableName);
	CatalogManager::instance().assertNotExisted(tableName, indexName);
	CatalogManager::instance().addIndex(tableName, indexName, attrName);
	//assert it must be unique on attrName;
	auto selected = __selectGen(tableName, {});
	auto s = selected();
	while (s.first != nullptr) {
		auto record = s.second;
		auto v = s.first;
		IndexManager::instance().addIndex(tableName, indexName, attrName, v, record);
		s = selected();
	}
}


void selects(const string& tableName, const vector<Condition>& conds) {
	auto selected = __selectGen(tableName, conds);
	auto start = std::chrono::system_clock::now();
	auto s = selected();
	cout << "select result:" << endl;
	int i = 0;
	while (s.first != nullptr) {
		auto record = s.second;
		auto v = s.first;
		CatalogManager::instance().showTableRecord(tableName, v);
		s = selected();
		i += 1;
	}
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	cout << "selct time: " << elapsed_seconds.count() << "s\n";
	cout << "total results num: " << i << endl;

}
void deleteRecords(const string& tableName, const vector<Condition>& conds) {
	auto selected = __selectGen(tableName, conds);
	vector<Record> entrys;
	auto s = selected();
	while (s.first != nullptr) {
		auto v = s.first;
		entrys.emplace_back(s.second);
		IndexManager::instance().deleteIndexReocrd(tableName, v);
		s = selected();
	}
	for (auto entry : entrys) {
		RecordManager::instance().deleteRecord(tableName, entry);
	}
	cout << "delete num: " << entrys.size() << endl;
}




void insertRecord(const string &name, const vector<Token>& content) {
	CatalogManager::instance().assertExisted(name);
	CatalogManager::instance().assertTypeEqual(name, content);
	for (auto uniqueAttr : CatalogManager::instance().getUniqueAttri(name)) {
		auto i = uniqueAttr.i;
		auto indexName = uniqueAttr.indexName;
		if (indexName != "") {
			if (IndexManager::instance().indexRecordExisted(name, indexName, content[i])) {
				string res("unique> tablename: " + name + " indexName +" + indexName + "  " + content[i].str()+ "already existed");
				throw InsertError(res.c_str());
			};
		}
		else {
			auto offset = uniqueAttr.off;
			if (RecordManager::instance().recordExist(name, content[i], i, offset)) {
				cout << "fuck" << endl;
				string res("unique> tablename: " + name  +" " +content[i].str() + "  already existed");
				throw InsertError(res.c_str());
			}

		}
	}
	//auto value = 
	auto record = RecordManager::instance().insertRecord(name, content);
	//delete value;
	for (auto attr : CatalogManager::instance().getIndexAttr(name)) {
		cout << "indexs record " << endl;
		auto i = attr.i;
		auto indexName = attr.indexName;
		IndexManager::instance().insertIndex(name, indexName, content[i], record);
	}
}
void showTableRecord(const string &name) {
	CatalogManager::instance().assertExisted(name);

	cout << "show  table record table :" + name << endl;
	RecordManager::instance().showReocrd(name);

	cout << "\n" << endl;
}
void showIndex(const string& tableName, const string& indexName) {
	CatalogManager::instance().assertExisted(tableName, indexName);

	IndexManager::instance().showIndex(tableName, indexName);
}