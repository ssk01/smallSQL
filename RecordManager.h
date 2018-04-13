#pragma once
#include <tuple>
#include <list>
#include <vector>
#include <map>
#include "block.h"
#include "util.h"

using std::pair;
using std::list;
using std::vector;
using std::map;
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