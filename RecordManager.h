#pragma once
#include <tuple>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include "block.h"
#include "util.h"
#include "condition.h"
#include "catalogData.h"
#include "log.h"

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
	~RecordList() {
		LOG("recordlist deconstruct");
	}
	void load() {
		std::ifstream in{ recordlistPath(tableName)};
		if (in.is_open()) {
			LOG("recordlist load ");
			in >> tableName;
			in >> entrySize;
			in >> nextPos.first;
			in >> nextPos.second;
			size_t size;
			in >> size;
			LOG("load record size", size);
			for (size_t i = 0; i < size; i++) {
				Record r;
				in >> r.first;
				in >> r.second;
				records.emplace_back(r);
			}
			in >> size;
			for (size_t i = 0; i < size; i++) {
				Record r;
				in >> r.first;
				in >> r.second;
				free_records.emplace_back(r);
			}
		}
		else {
			LOG("recordlist error ");
			exit(1);
		}

	}
	void save() {
		std::ofstream out{ recordlistPath(tableName), std::ios::trunc };
		out << tableName << " ";
		out << entrySize << " ";
		out << nextPos.first << " ";
		out << nextPos.second << "\n";
		out << records.size() << "\n";
		LOG("save record size", records.size());
		for (auto r : records) {
			out << r.first << " ";
			out << r.second << "\n";
		}
		out << free_records.size() << "\n";
		for (auto r : free_records) {
			out << r.first << " ";
			out << r.second << "\n";
		}
	}
	set<int> drop() {
		std::remove(recordlistPath(tableName).c_str());
		set<int> blockIndexs;
		for (auto r : records) {
			blockIndexs.emplace(r.first);
		}
		for (auto r : free_records) {
			blockIndexs.emplace(r.first);
		}
		return blockIndexs;
	}
	RecordList(string tableName, int size) : tableName(tableName), nextPos{ 0,0 }, entrySize(size){
	}
	RecordList(const string &tablename) : tableName(tablename) {
		load();
	}
	void showAllReocrd() {
		cout << "show records size: " << records.size() << endl;
		for (auto &entry : records) {
			auto block = BufferManager::instance().find_or_alloc(tableName, entry.first);
			char *value = block->rawPtr() + entry.second * entrySize;
			CatalogManager::instance().showTableRecord(tableName, value);
		}
	}

	size_t i = 0;
	std::function< BlockPtr()> selectGen(const vector<Record> &recs, const vector<Condition>& conds) {
		//cout <<"size :"<< recs.size() << endl;
		auto f = [this, recs,  conds]() ->BlockPtr{
		/*	cout << "size :" << recs.size() <<"i "<< i << "  "<<conds.size()<< endl;*/
			while (i < recs.size()) {
				auto entry = recs[i];
				i++;
				LOG("select block get ", entry.first, entry.second);
				auto block = BufferManager::instance().find_or_alloc(tableName, entry.first);
				LOG("select block end ");

				char *byteValue = block->rawPtr() + entry.second * entrySize;

				int condTure = 0;
				for (auto &c : conds) {
					auto off = CatalogManager::instance().attribute(tableName, c.attriName).off;
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
					else if (c.token.type == "char") {
						auto val = string(byteValue + off);
						if (Condition::eval(c.op, val, c.token.toString())) {
							condTure += 1;
						}
						else {
							break;
						}
					}
				}
				if (condTure == conds.size()) {
					LOG("success ", entry.first, entry.second);
					return{ block, entry, byteValue };
				}

			}
			i = 0;
			return{ nullptr,{ 0,0 },nullptr };
		};
		return f;
	}
	std::function< BlockPtr()> selectGen(const vector<Condition>& conds) {
		cout << records.size() << endl;
		return selectGen({ records.begin(), records.end() }, conds);
	}

	bool recordExist(const Token& content, int offset) {
		char *byteValue;
		for (auto &entry : records) {
			auto block = BufferManager::instance().find_or_alloc(tableName, entry.first);
			byteValue = block->rawPtr() + entry.second * entrySize;
			if (content.type == "int") {
				auto lhs = content.toInt();
				auto rhs = Int(byteValue + offset);
				if (lhs == rhs) {
					return true;
				}
			}
			else if (content.type == "float") {
				auto lhs = content.toFloat();
				auto rhs = Float(byteValue + offset);
				if (lhs == rhs) {
					return true;
				}
			}
			else if (content.type == "char") {
				auto lhs = content.toString();
				auto rhs = string(byteValue + offset);
				if (lhs == rhs) {
					return true;
				}
			}
		}
		return false;
	}
	void deleteRecord(Record entry) {
		records.remove(entry);
		free_records.push_back(entry);
	}
	void update(char * byteValue, vector<Condition> conds) {
		for (auto c : conds) {

		}
	}
	Record insertRecord(const vector<Token>&content) {
		Record entry;
		LOG("nextPos: ", nextPos.first, nextPos.second);

		if (free_records.size()) {
			entry = free_records.back();
			free_records.pop_back();
			LOG("get entry from free_record");
		} else {
			entry = nextPos;
			auto add = [&]() {
				nextPos.second += 1;
				if ((nextPos.second + 1) * entrySize > Block::BLOCKSIZE) {
					nextPos.first += 1;
					nextPos.second = 0;
				}
			};
			add();
		}
		LOG("nextPos: ", nextPos.first, nextPos.second);
		records.push_back(entry);
		auto block = BufferManager::instance().find_or_alloc(tableName, entry.first);
		block->modify();
		CatalogManager::instance().toEntry(tableName, content, block->rawPtr() + entry.second * entrySize);
		return entry;
	}

private:
	// blockindex and numRecord in block	
	string tableName;
	list<Record> records;
	//list<Record> hot_records;
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

	std::function< BlockPtr()>  selectGen(const string& tableName, const vector<Record> & recs, const vector<Condition>& conds) {
		return tableInfos[tableName].selectGen(recs, conds);
	}
	std::function< BlockPtr()> selectGen(const string& tableName, const vector<Condition>& conds) {
		return tableInfos[tableName].selectGen(conds);
	}
	void deleteRecord(const string& tableName, Record entry) {
		return tableInfos[tableName].deleteRecord(entry);
	}

	void createTable(const string &name, int entrySize) {
		tableInfos[name] = RecordList{ name, entrySize };
	}
	set<int> dropTable(const string &name) {
		auto blockIndexs = tableInfos[name].drop();
		tableInfos.erase(name);
		return blockIndexs;
	}

	RecordList::Record insertRecord(const string &name, const vector<Token>&content) {
		return tableInfos[name].insertRecord(content);
	}
	void  showReocrd(const string &name) {
		return tableInfos[name].showAllReocrd();
	}
	bool recordExist(const string &name, const Token& content,  int offset) {
		return tableInfos[name].recordExist(content, offset);
	}
	~RecordManager() {
		save();
	};
private:
	void load() {
		std::ifstream in{ catalogDataPath() };
		if (in.is_open()) {
			size_t size;
			in >> size;
			string tableName;
			while (size--> 0) {
				in >> tableName;
				tableInfos[tableName] = { tableName };
			}
			LOG("load RecordManager success");
		}
		else {
			LOG("load RecordManager: empty");
		}
	}
	void save() {
		for (auto kv : tableInfos) {
			kv.second.save();
		}
	}
	RecordManager() { 
		load(); 
	};
	map<string, RecordList> tableInfos;

};