#pragma once
#include "RecordManager.h"
#include "catalogData.h"
#include "condition.h"
#include <set>
#include <assert.h>
#include <string>
using std::set;
using Record = pair<int, int>;
using Records = vector<Record>;

template<class T>
class Index {
public:
	Index() {}
	~Index() {
		cout << "~Index" << endl;
	}
	Index(const string& tableName, const string& indexName)  {
		load(tableName, indexName);
	}
	void update(T old, T update) {
		if (recordExisted(old) && !recordExisted(update)) {
			//indexs.erase(key);
			auto entry = indexs[old];
			indexs.erase(old);
			indexs[update] = entry;
			return;
		}
		string res("during update key error");
		LOG("during update key error ", old, update);

		throw IndexError(res.c_str());
	}
	void load(const string& tableName, const string& indexName) {
		std::ifstream in{ indexPath(tableName, indexName )};
		if (!in.is_open()) {
			LOG("index load error", tableName, indexName);
		}
		size_t size = 0;
		in >> size;
		while (size--) {
			T key;
			Record rec;
			in >> key;
			in >> rec.first;
			in >> rec.second;
			indexs[key] = rec;
		}
	}
	void save(const string& tableName, const string& indexName) {
		std::ofstream out{ indexPath(tableName, indexName), std::ios::trunc };
		out << indexs.size() << "\n";
		for (auto &kv : indexs) {
			out << kv.first << "\n";
			out << kv.second.first << " " << kv.second.second << "\n";
		}
	}
	void insertIndex(T key, RecordList::Record record) {
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
		if (recordExisted(key)) {
			indexs.erase(key);
			return;
		}
		string res("no record existed ");
		LOG("no record existed ", key);

		throw IndexError(res.c_str());
	}

	set<Record> select(Condition c, T condiValue) {
		set<Record> res;
		if (c.op == ">=" || c.op == "<=" || c.op == "=") {
			if (indexs.find(condiValue) != indexs.end()) {
				res.insert(indexs[condiValue]);
			}
			if (c.op.size() == 2) {
				c.op = c.op.substr(0, 1);
			}
		}
		if (c.op == "<") {
			auto end = indexs.lower_bound(condiValue);
			for (auto r = indexs.begin(); r != end; r++) {
				auto key = (*r).first;
				res.insert(indexs[key]);
			}
		}
		else if (c.op == ">") {
			auto beg = indexs.upper_bound(condiValue);
			for (auto r = beg; r != indexs.end(); r++) {
				auto key = (*r).first;
				res.insert(indexs[key]);
			}
		}
		else if (c.op == "!=") {
			auto iter = indexs.find(condiValue);
			if (iter != indexs.end()) {
				for (auto r = indexs.begin(); r != iter; r++) {
					auto key = (*r).first;
					res.insert(indexs[key]);
				}
				iter++;
				for (auto r = iter; r != indexs.end(); r++) {
					auto key = (*r).first;
					res.insert(indexs[key]);
				}
			}
			else {
				for (auto r = indexs.begin(); r != indexs.end(); r++) {
					auto key = (*r).first;
					res.insert(indexs[key]);
				}
			}
		}
		return res;
	}
private:
	map<T, RecordList::Record> indexs;
	string tableName;
	string indexName;
};
class Indexs {
	map<string, Index<int>> intIndex;
	map<string, Index<string>> charIndex;
	map<string, Index<float>> floatIndex;
public:
	void dropIndex(const string&tableName) {
		for (auto i : intIndex) {
			tryRemove(indexPath(tableName, i.first).c_str());
		}
		for (auto i : charIndex) {
			tryRemove(indexPath(tableName, i.first).c_str());
		}
		for (auto i : floatIndex) {
			tryRemove(indexPath(tableName, i.first).c_str());
		}
	}
	void dropIndex(const string&tableName, const string& indexName) {
		tryRemove(indexPath(tableName, indexName).c_str());
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
	void save(const string& tableName) {
		std::ofstream out{ indexInfoPath(tableName), std::ios::trunc };
		out << tableName << "\n";
		out << intIndex.size() << "\n";
		for (auto &kv : intIndex) {
			out << kv.first << "\n";
			kv.second.save(tableName, kv.first);
		}
		out << charIndex.size() << "\n";
		for (auto &kv : charIndex) {
			out << kv.first << "\n";
			kv.second.save(tableName, kv.first);
		}		
		out << floatIndex.size() << "\n";
		for (auto &kv : floatIndex) {
			out << kv.first << "\n";
			kv.second.save(tableName, kv.first);
		}
	}
	void load(const string& tableName) {
		std::ifstream in{ indexInfoPath( tableName ) };
		if (in.is_open()) {
			string name;
			in >> name;
			assert(name == tableName);
			int intIndexSize;
			int charIndexSize;
			int floatIndexSize;
			in >> intIndexSize;
			while (intIndexSize--) {
				string indexName;
				in >> indexName;
				intIndex[indexName] = { tableName, indexName };
			}
			in >> charIndexSize;
			while (charIndexSize--) {
				string indexName;
				in >> indexName;
				charIndex[indexName] = { tableName, indexName };
			}
			in >> floatIndexSize;
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
		floatIndex[indexName].insertIndex(key, record);
	}
	void insertIndex(const string& indexName, int key, RecordList::Record record) {
		intIndex[indexName].insertIndex(key, record);
	}	
	void insertIndex(const string& indexName, string key, RecordList::Record record) {
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
	void update(const string& indexName, int old, int update) {
		intIndex[indexName].update(old, update);
	}
	void update(const string& indexName, float old, float update) {
		floatIndex[indexName].update(old, update);
	}
	void update(const string& indexName, string old, string update) {
		charIndex[indexName].update(old, update);
	}
};
class IndexManager {
public:
	IndexManager(){
		load();
	}
	~IndexManager() {
		save();
		cout << "hehe" << endl;
	}
	static IndexManager& instance() {
		static IndexManager im;
		return im;
	}
	void save() {
		for (auto &kv : nameIndexs) {
			kv.second.save(kv.first);
		}
	}
	void load() {
		std::ifstream in{ catalogDataPath() };
		if (in.is_open()) {
			size_t size;
			in >> size;
			string tableName;
			while (size--> 0) {
				in >> tableName;
				nameIndexs[tableName] = {  };
				nameIndexs[tableName].load(tableName);
			}
			cout << "after load " << endl;
		}
		else {
			cout << "not open catalogData" << endl;
		}
	}
	void update(const string& name, const string& indexName, const string& attrName, char* value, const Token& content) {
		auto attribute = CatalogManager::instance().attribute(name, attrName);
		auto offset = attribute.off;
		auto type = attribute.type;
		if (type == "int") {
			int val = Int(value + offset);
			nameIndexs[name].update(indexName, val, content.toInt());
		}
		else if (type == "char") {
			string val = string(value + offset);
			nameIndexs[name].update(indexName, val, content.toString());
		}
		else if (type == "float") {
			float val = Float(value + offset);
			nameIndexs[name].update(indexName, val, content.toFloat());
		}
	}
	void addIndex(const string& name, const string& indexName, const string& attrName, char* value, const Record & record) {
		auto attribute = CatalogManager::instance().attribute(name, attrName);
		auto offset = attribute.off;
		auto type = attribute.type;
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
	}
	void dropIndex(const string& name, const string& indexName) {
		nameIndexs[name].dropIndex(name,indexName);
	}
	void dropIndex(const string& name) {
		tryRemove(indexInfoPath(name).c_str());
		nameIndexs[name].dropIndex(name);
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
		for (auto attr : CatalogManager::instance().getIndexAttr(tableName)) {
			auto i = attr.i;
			auto indexName = attr.indexName;
			auto type = attr.type;
			auto offset = attr.off;
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