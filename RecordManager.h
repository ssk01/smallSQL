#pragma once
#include <tuple>
#include <list>
#include <vector>
#include <map>
#include <set>
#include "block.h"
#include "util.h"
#include "condition.h"
#include "catalogData.h"
using std::pair;
using std::list;
using std::vector;
using std::map;
using std::set;
using Record = pair<int, int>;
class RecordList {
public:
	using Record = pair<int, int>;

	RecordList(){}
	RecordList(string tableName, int size) : tableName(tableName), nextPos{ 0,0 }, entrySize(size){
	}
	vector<char *> showAllReocrd() {
		vector<char *> res;
		for (auto &entry : records) {
			char *value = new char[entrySize+1];	
			auto block = BufferManager::instance().find_or_alloc(tableName, entry.first);
			memcpy(value, block.rawPtr() + entry.second * entrySize, entrySize);
			res.push_back(value);
		}
		return res;
	}
	vector<char *> select(const vector<Record> &recs, const vector<Condition>& conds) {
		vector<char *> byteValues;
		for (auto &entry : recs) {

			//todo it must be find
			auto block = BufferManager::instance().find_or_alloc(tableName, entry.first);
			char *byteValue = block.rawPtr() + entry.second * entrySize;

			int condTure = 0;
			for (auto &c : conds) {
				auto off = CatalogManager::instance().attributeOffset(tableName, c.i);
				if (c.token.type == "int") {
					auto val = Int(byteValue + off);
					if (Condition::eval(c.op, val, c.token.toInt())) {
						condTure += 1;
					}
					else {
						break;
					}
				}
				else if (c.token.type == "float") {
					auto val = Float(byteValue + off);
					if (Condition::eval(c.op, val, c.token.toFloat())) {
						condTure += 1;
					}
					else {
						break;
					}
				}
			}
			if (condTure == conds.size()) {
				byteValues.push_back(byteValue);
			}
		}
		return byteValues;
	}
	int deleteRecords(const vector<Record> &recs, const vector<Condition>& conds) {
		int deletedNum = 0;
		for (auto &entry : recs) {
			//todo it must be find
			auto block = BufferManager::instance().find_or_alloc(tableName, entry.first);
			char *byteValue = block.rawPtr() + entry.second * entrySize;
			int condTure = 0;
			for (auto &c : conds) {
				auto off = CatalogManager::instance().attributeOffset(tableName, c.i);
				if (c.token.type == "int") {
					auto val = Int(byteValue + off);
					if (Condition::eval(c.op, val, c.token.toInt())) {
						condTure += 1;
					}
					else {
						break;
					}
				}
				else if (c.token.type == "float") {
					auto val = Float(byteValue + off);
					if (Condition::eval(c.op, val, c.token.toFloat())) {
						condTure += 1;
					}
					else {
						break;
					}
				}
			}
			if (condTure == conds.size()) {
				//put entry to freelist;
				records.remove(entry);
				free_records.push_back(entry);
				deletedNum++;
				//byteValues.push_back(byteValue);
			}
		}
		return deletedNum;
	}
	vector<char *> select(const vector<Condition>& conds) {
		return select({records.begin(), records.end()}, conds);
	}
	bool recordExist(const Token& content, int i, int offset) {
		char *byteValue = new char[entrySize];
		for (auto &entry : records) {

			//todo it must be find
			auto block = BufferManager::instance().find_or_alloc(tableName, entry.first);
			memcpy(byteValue, block.rawPtr() + entry.second * entrySize, entrySize);
	
			if (content.type == "int") {
				auto lhs = content.toInt();
				//int Int(char *byteValue, int off) {
					int rhs;
					memcpy(&rhs, byteValue + offset, sizeof(rhs));
				//}
				//auto rhs = Int(byteValue, off);
				if (lhs == rhs) {
					return true;
				}
			}
		}
		return false;
	}
	Record insertRecord(char *content) {
		Record entry;
		if (free_records.size()) {
			cout << "fuck" << endl;
			exit(0);
		} else {
			entry = nextPos;
			auto add = [&]() {
				nextPos.second += 1;
				if (nextPos.second * entrySize > Block::BLOCKSIZE) {
					nextPos.first += 1;
					nextPos.second = 0;
				}
			};
			add();
		}
		records.push_back(entry);
		auto block = BufferManager::instance().find_or_alloc(tableName, entry.first);
		memcpy(block.rawPtr() + entry.second * entrySize, content, entrySize);
		return entry;
	}
private:
	// blockindex and numRecord in block
	
	string tableName;
	list<Record> records;
	vector<Record> free_records;
	int entrySize;
	Record nextPos;
};

class RecordManager {
public:
	static RecordManager& instance() {
		static RecordManager rm;
		return rm;
	}
	vector<char *> select(const string& tableName, const vector<Condition>& conds) {
		return tableInfos[tableName].select(conds);
	}
	vector<char *> select(const string& tableName,const vector<Record> & recs,const vector<Condition>& conds) {
		return tableInfos[tableName].select(recs, conds);
	}
	int deleteRecords(const string& tableName, const vector<Record> & recs, const vector<Condition>& conds) {
		return tableInfos[tableName].deleteRecords(recs, conds);
	}
	
	void createTable(const string &name, int entrySize) {
		if (!tableExisted(name)) {
			tableInfos[name] = RecordList{ name, entrySize };
		}
	}
	RecordList::Record insertRecord(const string &name, char *content) {
		if (tableExisted(name)) {
			return tableInfos[name].insertRecord(content);
		}
	}
	vector<char *>  showReocrd(const string &name) {
		if (tableExisted(name)) {
			return tableInfos[name].showAllReocrd();
		}
	}
	bool recordExist(const string &name, const Token& content, int i, int offset) {
		if (tableExisted(name)) {
			return tableInfos[name].recordExist(content, i, offset);
		}
	}
	bool tableExisted(const string &name) {
		if (tableInfos.find(name) == tableInfos.end()) {
			return false;
		}
		return true;
	}
private:
	RecordManager() {};
	map<string, RecordList> tableInfos;

};