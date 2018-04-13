#pragma once
#include "token.h"
#include <vector>
using std::vector;

class Tokenizer {
	enum  State : int {
		None,
		Char,
		Int,
		Float,
		SingeQuote,
		DoubleQute,
	};
	vector<Token> tokens;
	string input;
	State state;
	string cur;
	int line;
	int col;
	//int	i = 0;
	void add_colon() {
		tokens.push_back(Token(cur, line, col));
		cur = "";
		state = None;
	}
	void add(const string &type) {
		tokens.push_back(Token(type, cur, line, col));
		cur = "";
		state = None;
	}	
	void get() {
		cur.push_back(input[i]);
		col++;
		i += 1;
	}
	void skip() {
		i += 1;
		col++;
	}
	int i = 0;

public:
	Tokenizer(const string& input) :input(input), state(None), line(1), col(1){

	}
	vector<Token> generate() {
		map<int, string> type{ {Char, "char"}, {Int ,"int"},{Float, "float"}, {None,"None" },{ SingeQuote ,"char" },{DoubleQute ,"char" } };
		auto error = [&]() {
			for (auto t : tokens) {
				show(t);
			}
			cout <<"cur: " <<cur << "\t" << "c: "<< input[i] <<"\tstate: " << type[state] << "line: " << line <<"col: " << col << endl;
			exit(0);
		};
		auto other = [&](char c = 'a') {
			if (string(",;()").find(c) != -1) {
				if (state != None) {
					add(type[state]);
				}
				get();
				add_colon();
				return true;
			}
			else if (isspace(c)) {
				if (c == '\n') {
					line++;
				}
				if (state != None) {
					add(type[state]);
				}
				skip();
				return true;
			} 
			return false;
		};
		while (i < input.size()) {
			char c = input[i];
			switch (state) {
			case None:
				if (other(c)){
					assert(cur.empty());
				}
				else if (isalpha(c)) {
					get();
					state = Char;
				}
				else if (isdigit(c)) {
					get();
					state = Int;
				}
				else if (c == '\''){
					//get();
					skip();
					state = SingeQuote;
				}
				else if (c == '"') {
					//get();
					skip();
					state = DoubleQute;
				}
				else {
					error();
				}
				break;
			case SingeQuote:
				if (other(c)) {
					assert(cur.empty());
				} else if (c == '\'') {
					skip();
					add("char");
				}
				else {
					get();
				}
				break;
			case DoubleQute:
				if (other(c)) {
					assert(cur.empty());
				}
				else if (c == '"') {
					skip();
					add("char");
				}
				else {
					get();
				}
				break;
			case Char:
				if (other(c)){
					assert(cur.empty());
				}
				else if (isalnum(c) || c == '_') {
					get();
				}
				else {
					error();
				}
				break;
			case Int:
				if (other(c)){
					assert(cur.empty());
				}
				else if (isdigit(c)) {
					get();
				}
				else if (c == '.') {
					get();
					state = Float;
				}
				else {
					error();
				}
				break;
			case Float:
				if (other(c)){
					assert(cur.empty());
				}
				else if (isdigit(c)) {
					get();
				}
				else {
					error();
				}
				break;
			}
		}
		if (cur.size() > 0) {
			//add("state");
			other();
		}
		return tokens;
	}
};