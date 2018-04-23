#pragma once
#include "Tokenizer.h"
#include "attribute.h"
#include "condition.h"
#include <sstream>
#include "api.h"
class Interpreter {
public:
	Interpreter(string input):input(input), i(0){
		cout << "kidding me" << endl;
		tokens = Tokenizer(input).generate();
		//for (auto &a : tokens) {
		//	cout << a << endl;
		//}
		cout << "kidding me" << endl;
		__load();
		cout << "after load" << endl;
		//showTableRecord("person");
	}
	void createIndex() {
	//	create index idx_age on person(age);
	//void addIndex(const string& tableName, const string& indexName, const string& attrName)
		auto indexName = get("char");
		assertNext("on");
		auto tableName = get("char");
		assertNext("(");
		auto attrName = get("char");
		assertNext(")");
		addIndex(tableName, indexName, attrName);
		if (!end()) {
			assertNext(";");
		}
	}
	void insert() {
		assertNext("into");
		auto tableName = get("char");
		assertNext("values");
		assertNext("(");
		vector<Token> content;
		while (!peek(")")) {
				content.emplace_back(get());
			if (peek(")")) {
				break;
			}
			assertNext(",");	
		}
		if (!end()) {
			assertNext(";");
		}
		insertRecord(tableName, content);
	}
	void createTable() {
		vector<Attribute> attrs;
		auto tableName = get("char");
		assertNext("(");
		int idx = 0;
		while (!peek(")")) {
			auto name = get("char");
			auto type = get("char");
			int size = 0;
			bool ifunique = false;
			if (peek("(")) {
				istringstream in(get("int"));
				in >> size;
				assertNext(")");
			}
			if (peek().type == "char") {
				assertNext("unique");
				ifunique = true;
			}
			attrs.emplace_back(name, type, idx,size, ifunique);
			idx++;
			//bug
			if (peek(")")) {
				break;
			}
			assertNext(",");
		}
		if (!end()) {
			assertNext(";");
		}
		addTable(tableName, attrs);
		//for (auto &a : attrs) {
		//	cout << a << endl;
		//}
	}
	void deleteRecord() {
		//DELETE FROM Person WHERE LastName = 'Wilson'
		assertNext("from");
		auto tableName = get("char");
		assertNext("where");
		vector<Condition> conds;
		while (!end()) {
			auto attriName = get("char");
			string opType("!=><");
			string op = "";
			while (opType.find(peek().type) != -1) {
				op += get().content;
			}
			auto token = get();
			conds.emplace_back(attriName, op, token);
			if (end()) {
				break;
			}
			else if (peek(";")) {
				break;
			}
			else {
				assertNext("and");
			}
		}
		deleteRecords(tableName, conds);

	}
	void select() {
	//type: char      content : select
	//	type : *content : *
	//	type : char      content : from
	//	type : char      content : person
	//	type : char      content : where
	//	type : char      content : height
	//	type : < content : <
	//	type : = content : =
	//	type : float     content : 176.3
	//	type : ; content:;
		assertNext("*");
		assertNext("from");
		auto tableName = get("char");
		if (!end() && peek(";")) {
			selects(tableName, {});
		}
		else {
			assertNext("where");
			vector<Condition> conds;
			while (!end()) {
				auto attriName = get("char");
				string opType("!=><");
				string op = "";
				while (opType.find(peek().type) != -1) {
					op+=get().content;
				}
				auto token = get();
				conds.emplace_back(attriName, op, token);
				if (end()) {
					break;
				}
				else if (peek(";")) {
					break;
				}
				else {
					assertNext("and");
				}
			}
			/*for (const auto& c : conds){
				cout << c << endl;
			}*/
			selects(tableName, conds);

		}
	}
	void dropIndex() {
		// DROP INDEX index_name ON table_name
		auto indexName = get("char");
		assertNext("on");
		auto tableName = get("char");
		if (!end()) {
			assertNext(";");
		}
		::dropIndex(tableName, indexName);
	}
	void dropTable() {
		auto tableName = get("char");
		if (!end()) {
			assertNext(";");
		}
		::dropTable(tableName);
	}
	void run(const string &input) {
		tokens = Tokenizer(input).generate();
		i = 0;
		run();
	}
	void run() {
		while (!end()) {
			try {
				if (peek("create")) {
					if (peek("table")) {
						createTable();
					}
					else if(peek("index")){
						createIndex();
					} 
				} else if(peek("insert")){
					insert();
				}
				else if (peek("select")) {
					select();
				}
				else if (peek("delete")) {
					deleteRecord();
				}
				else if (peek("drop")) {
					//DROP TABLE ±íÃû³Æ
					if (peek("index")) {
						dropIndex();
					}
					else if (peek("table")) {
						dropTable();
					}
				}
				else {
					cout << "fuck  " << peek() <<endl;
					exit(0);
				}
			}
			catch (std::runtime_error& e) {
				cout << e.what() << endl;
			}
		}
	}
private:
	string input;
	bool end() {
		return i == tokens.size();
	}
	bool peek(string content) {
		if (peek() == content) {
			get();
			return true;
		}
		return false;
	}
	void assertNext(string content) {
		if (peek() == content) {
			get();
			return;
		}
		cout << "assert:  "<<content <<"== " << peek() << endl;
		for (auto j = 0; j < i; j++) {
			cout << tokens[i] << endl;
		}
		exit(11);
	}
	string get(string type) {
		if (tokens[i].type == type) {
			i += 1;
			return tokens[i - 1].content;
		}
		cout << "assert:  " << type <<"== " << peek() << endl;
		exit(11);

	/*	for (auto j = 0; j < tokens.size(); j++) {
			cout << tokens[i] << endl;
		}*/
	}
	Token get() {
		i += 1;
		return tokens[i-1];
	}
	Token peek() {
		return tokens[i];
	}
	vector<Token> tokens;
	int i;

};