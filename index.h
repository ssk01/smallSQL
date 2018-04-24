#pragma once
#include "RecordManager.h"
#include "catalogData.h"
#include "condition.h"
#include <set>
#include <assert.h>
using std::set;
using Record = pair<int, int>;
using Records = vector<Record>;

template<class T>
class Index {
public:
	/*Index(const string& tableName):tableName(tableName) {}*/
	Index() {}
	Index(const string& tableName, const string& indexName)  {
		//load(tableName, indexName);
	}
	void load(const string& tableName, const string& indexName) {
		std::ifstream in{ string("indexes/") + tableName + "_" + indexName ".txt" };
		if (!in.is_open()) {
			fck();
		}
		size_t size = 0;
		in >> size;
		while (size--) {
			T key;
			Record rec;
			in >> key;
			in >> res.first;
			in >> res.second;
			indexs[key] = rec;
		}
	}
	//tableName
	void save(const string& tableName, const string& indexName) {
		std::ofstream out{ string("indexes/") + tableName +"_"+indexName + ".txt" };
		out << indexs.size() << "\n";
		for (auto &kv : indexs) {
			out << kv.first << "\n";
			out << kv.second.first << " " << kv.second.second << "\n";
		}
	}
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
	void deleteIndexReocrd(T key) {
		if (indexs.find(key) != indexs.end()) {
			indexs.erase(key);
			return;
		}
		//exit(0);
		fck();
	}
	set<Record> deleteRecords(const Condition& c, T condiValue) {
		set<Record> res;
		for (auto r = indexs.begin(); r != indexs.end(); r++) {
			auto key = (*r).first;
			if (Condition::eval(c.op, key, condiValue)) {
				//continue;
				res.insert(indexs[key]);
			}
		}
		return res;
	}
	set<Record> select(const Condition& c, T condiValue) {
		set<Record> res;
		// map upper_bound
		for (auto r = indexs.begin(); r != indexs.end(); r++) {
			auto key = (*r).first;
			if (Condition::eval(c.op, key, condiValue)) {
				//todo;
				res.insert(indexs[key]);
			}
		}
		return res;
	}
private:
	map<T, RecordList::Record> indexs;
	string tableName;
	string indexName;
	//int i;
};
class Indexs {
	string tableName;
	map<string, Index<int>> intIndex;
	map<string, Index<string>> charIndex;
	map<string, Index<float>> floatIndex;
public:
	void dropIndex(const string& indexName) {
		if (intIndex.find(indexName) != intIndex.end()) {
			intIndex.erase(indexName);
		}
		if (charIndex.find(indexName) != charIndex.end()) {
			charIndex.erase(indexName);
		}
		if (floatIndex.find(indexName) != floatIndex.end()) {
			floatIndex.erase(indexName);
		}
	}
	Indexs() {};
	//Indexs(const string& tablName): {};
	void save() {
		std::ofstream out{ string("indexes/") + tableName + ".txt" };
		//in <<
		out << tableName << "\n";
		out << intIndex.size() << "\n";
		out << charIndex.size() << "\n";
		out << floatIndex.size() << "\n";
		for (auto &kv : intIndex) {
			out << kv.first << "\n";
			kv.second.save(tableName, kv.first);
		}
		for (auto &kv : charIndex) {
			out << kv.first << "\n";
			kv.second.save(tableName, kv.first);
		}		
		for (auto &kv : floatIndex) {
			out << kv.first << "\n";
			kv.second.save(tableName, kv.first);
		}
	}
	void load() {
		std::ifstream in{ string("indexesInfo/") + tableName + ".txt" };
		if (in.is_open()) {
			string name;
			in >> name;
			assert(name == tableName);
			int intIndexSize;
			int charIndexSize;
			int floatIndexSize;
			in >> intIndexSize;
			in >> charIndexSize;
			in >> floatIndexSize;
			while (intIndexSize--) {
				string indexName;
				in >> indexName;
				intIndex[indexName] = { tableName, indexName };
			}
			while (charIndexSize--) {
				string indexName;
				in >> indexName;
				charIndex[indexName] = { tableName, indexName };
			}
			while (floatIndexSize--) {
				string indexName;
				in >> indexName;
				floatIndex[indexName] = { tableName, indexName };
			}
		}
		else {
			cout << "first time, not load table: " + tableName << endl;
		}
	}
	Indexs(const string& tableName):tableName(tableName) {
		//load();
	};

	~Indexs() {
		cout << "index deconstruct" << endl;
	};
	bool indexRecordExisted(const string& indexName, const Token& content) {
		if (content.type == "int") {
			return intIndex[indexName].recordExisted(content.toInt());
		}
		if (content.type == "char") {
			return charIndex[indexName].recordExisted(content.toString());
		}		
		if (content.type == "float") {
			return floatIndex[indexName].recordExisted(content.toFloat());
		}
	}
	void insertIndex(const string& indexName, float key, RecordList::Record record) {
		//assert();
		floatIndex[indexName].insertIndex(key, record);
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
	//template<class T>
	set<Record> select(const string& indexName, const Condition& c) {
		if (intIndex.find(indexName) != intIndex.end()) {
			return intIndex[indexName].select(c, c.token.toInt());
		}
		if (charIndex.find(indexName) != charIndex.end()) {
			return charIndex[indexName].select(c, c.token.toString());
		}		
		if (floatIndex.find(indexName) != floatIndex.end()){
			return floatIndex[indexName].select(c, c.token.toFloat());
		}
	}
	void deleteIndexReocrd(const string& indexName, int value) {
		intIndex[indexName].deleteIndexReocrd(value);
	}
	void deleteIndexReocrd(const string& indexName, float value) {
		floatIndex[indexName].deleteIndexReocrd(value);
	}
	void deleteIndexReocrd(const string& indexName, string value) {
		charIndex[indexName].deleteIndexReocrd(value);
	}
};
class IndexManager {
public:
	IndexManager(){}
	static IndexManager& instance() {
		static IndexManager im;
		return im;
	}
	void save() {
		for (auto &kv : nameIndexs) {
			kv.second.save();
		}
	}
	void load() {
		std::ifstream in{ string("catalogData/") + "tablename.txt" };
		if (in.is_open()) {
			size_t size;
			in >> size;
			string tableName;
			while (size--> 0) {
				in >> tableName;
				nameIndexs[tableName] = { tableName };
			}
			cout << "after load " << endl;
		}
		else {
			cout << "not open catalogData" << endl;
		}
	}
	// add index prev added record;
	void addIndex(const string& name, const string& indexName, const string& attrName, char* value, const Record & record) {
		auto attribute = CatalogManager::instance().attribute(name, attrName);
		auto offset = CatalogManager::instance().attributeOffset(name, attribute.i);
		auto type = attribute.type;
		//auto bytevalues = res.first;
		//auto records = res.second;
		//assert(records.size() == bytevalues.size());
		//nameIndexs[name] = { name };
		//for (size_t i = 0; i < records.size(); i++) {
		//	auto value = bytevalues[i];
		//	auto record = records[i];
			if (type == "int") {
				int val = Int(value + offset);
				nameIndexs[name].insertIndex(indexName, val ,record);
			}
			else if (type == "char") {
				string val = string(value + offset);
				nameIndexs[name].insertIndex(indexName, val, record);
			}
			else if (type == "float") {
				float val = Float(value + offset);
				nameIndexs[name].insertIndex(indexName, val, record);
			}
		//}
	}
	void dropIndex(const string& name, const string& indexName) {
		nameIndexs[name].dropIndex(indexName);
	}
	void dropIndex(const string& name) {
		nameIndexs.erase(name);
	}
	set<Record> select(const string& tableName, const Condition& c) {
		if (c.indexName == "") {
			string res = "tableName :" + tableName + c.str() + "is not index , in select index mode";
			throw IndexError(res.c_str());
		}
		return nameIndexs[tableName].select(c.indexName, c);
	}

	void showIndex(const string& tableName, const string& indexName) {
		if (CatalogManager::instance().existed(tableName, indexName)) {
			nameIndexs[tableName].showIndex(indexName);
		}
	}
	void deleteIndexReocrd(const string& tableName,  char *value) {
		for (auto _index : CatalogManager::instance().getIndexAttri(tableName)) {
			auto i = std::get<0>(_index);
			auto indexName = std::get<1>(_index);
			auto type = std::get<2>(_index);
			auto offset = CatalogManager::instance().attributeOffset(tableName, i);
			if (type == "int") {
				int val = Int(value + offset);
				nameIndexs[tableName].deleteIndexReocrd(indexName, val);
			} 			
			else if (type == "char") {
				string val = string(value + offset);
				nameIndexs[tableName].deleteIndexReocrd(indexName, val);
			}
			else if (type == "float") {
				float val = Float(value + offset);
				nameIndexs[tableName].deleteIndexReocrd(indexName, val);
			}
		}
	}
	bool indexRecordExisted(const string& tableName, const string& indexName, const Token& content) {
		return nameIndexs[tableName].indexRecordExisted(indexName, content);
	}
	void insertIndex(const string& tableName, const string& indexName, const Token& content, RecordList::Record record) {
		//exist(name, indexName, key);
		if (CatalogManager::instance().existed(tableName, indexName)) {
			if (content.type == "int") {
			
				nameIndexs[tableName].insertIndex(indexName, content.toInt(), record);
			}
			else if (content.type == "char") {
				nameIndexs[tableName].insertIndex(indexName, content.toString(), record);
			}
			else if (content.type == "float") {
				nameIndexs[tableName].insertIndex(indexName, content.toFloat(), record);
			}
		}
	}
	map<string, Indexs> nameIndexs;
}; 