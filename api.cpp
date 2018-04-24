#include "api.h"
#include <algorithm>
#include <set>
#include "catalogData.h"
using std::set;
void __load() {
	//for (auto &name : CatalogManager::instance().name()) {
	//	RecordManager::instance().createTable(name, CatalogManager::instance().getEntrySize(name));
	//}
}
void dropIndex(const string& tableName, const string& indexName)
{
	CatalogManager::instance().assertExisted(tableName, indexName);
	IndexManager::instance().dropIndex(tableName, indexName);
	CatalogManager::instance().dropIndex(tableName, indexName);
	//CatalogManager::instance().dropIndex(tableName, indexName);
}
void dropTable(const string& tableName) {
	CatalogManager::instance().assertExisted(tableName);
	auto blockIndexs = RecordManager::instance().dropTable(tableName);
	//IndexManager::instance().dropIndex(tableName);
	CatalogManager::instance().dropTable(tableName);
	BufferManager::instance().dropTable(tableName, blockIndexs);
}

void addTable(const string& name, vector<Attribute>& attr) {
	CatalogManager::instance().assertNotExisted(name);
	CatalogManager::instance().addTable(name, attr);
	RecordManager::instance().createTable(name, CatalogManager::instance().getEntrySize(name));
}
//static pair<vector<char *>, vector<Record>> __selects(const string& tableName, const vector<Condition>& conds) {
//	CatalogManager::instance().assertExisted(tableName);
//	for (const auto &c : conds) {
//		c.init(tableName);
//	}
//	//indexes;
//	auto indexesConditon = vector<Condition>();
//	auto normalConditon = vector<Condition>();
//	std::for_each(conds.begin(), conds.end(), [&](const Condition & c) {
//		if (c.indexName != "") {
//			indexesConditon.push_back(c);
//		}
//		else {
//			normalConditon.push_back(c);
//		}
//	});
//	//vector<char *> ByteValues;
//	if (!indexesConditon.empty()) {
//		set<pair<int, int>> matched;
//		for (auto &c : indexesConditon) {
//			auto recs = IndexManager::instance().select(tableName, c);
//			matched.insert(recs.begin(), recs.end());
//		}
//		return RecordManager::instance().select(tableName, { matched.begin(), matched.end() }, normalConditon);
//	}
//	//no indexes;
//	else {
//		cout << "no indexes " << endl;
//		return RecordManager::instance().select(tableName, conds);
//	}
//}
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
	//vector<char *> ByteValues;
	if (!indexesConditon.empty()) {
		set<pair<int, int>> matched;
		for (auto &c : indexesConditon) {
			auto recs = IndexManager::instance().select(tableName, c);
			matched.insert(recs.begin(), recs.end());
		}
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

//void selects(const string& tableName, const vector<Condition>& conds) {
//	auto selected = __selects(tableName, conds);
//	auto byteValues = selected.first;
//	cout << "select result:" << endl;
//
//	for (auto v : byteValues) {
//		CatalogManager::instance().showTableRecord(tableName, v);
//	}
//}
void selects(const string& tableName, const vector<Condition>& conds) {
	auto selected = __selectGen(tableName, conds);
	//auto byteValues = selected.first;
	auto s = selected();
	while (s.first != nullptr) {
		auto record = s.second;
		auto v = s.first;
		CatalogManager::instance().showTableRecord(tableName, v);
		s = selected();
	}
	cout << "select result:" << endl;
}
void deleteRecords(const string& tableName, const vector<Condition>& conds) {
	auto selected = __selectGen(tableName, conds);
	//auto byteValues = selected.first;
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
	//cout << "select result:" << endl;
	//auto selected = __selects(tableName, conds);
	//auto byteValues = selected.first;
	//auto entrys = selected.second;
	//for (auto entry : entrys) {
	//	RecordManager::instance().deleteRecord(tableName, entry);
	//}
	//for (auto v : byteValues) {
	//	IndexManager::instance().deleteIndexReocrd(tableName, v);
	//}
	//cout << "delete result:" << endl;

}




void insertRecord(const string &name, const vector<Token>& content) {
	CatalogManager::instance().assertExisted(name);
	CatalogManager::instance().assertTypeEqual(name, content);
	for (auto uniqueAttr : CatalogManager::instance().getUniqueAttri(name)) {
		auto i = std::get<0>(uniqueAttr);
		auto hasIndex = std::get<1>(uniqueAttr);
		if (hasIndex) {
			auto indexName = CatalogManager::instance().getIndexName(name, i);
			if (IndexManager::instance().indexRecordExisted(name, indexName, content[i])) {
				//"fuck"
				string res("unique> tablename: " + name + " indexName +" + indexName + "  " + content[i].str()+ "already existed");
				throw InsertError(res.c_str());
			};
		}
		else {
			auto offset = CatalogManager::instance().attributeOffset(name, i);
			if (RecordManager::instance().recordExist(name, content[i], i, offset)) {
				cout << "fuck" << endl;
				string res("unique> tablename: " + name  +" " +content[i].str() + "  already existed");
				throw InsertError(res.c_str());
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
	CatalogManager::instance().assertExisted(name);

	auto values = RecordManager::instance().showReocrd(name);
	cout << "show  table record table :" + name << endl;
	//for (auto &value : values) {
	//	CatalogManager::instance().showTableRecord(name, value);
	//}
	cout << "\n" << endl;
}
void showIndex(const string& tableName, const string& indexName) {
	CatalogManager::instance().assertExisted(tableName, indexName);

	IndexManager::instance().showIndex(tableName, indexName);
}