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
	};
	vector<Token> tokens;
	string input;
	State state;
	string cur;
	//int	i = 0;
	void add_colon() {
		tokens.push_back(Token(cur));
		cur = "";
		state = None;
	}
	void add(const string &type) {
		tokens.push_back(Token(type, cur));
		cur = "";
	}	
	void peek() {
		cur.push_back(input[i]);
		i += 1;
	}
	void skip() {
		i += 1;
		state = None;
	}
	int i = 0;

public:
	Tokenizer(const string& input) :input(input), state(None){

	}
	vector<Token> generate() {
		map<int, string> type{ {Char, "char"}, {Int ,"int"},{Float, "float"}, {None,"None" } };
		auto error = [&]() {
			for (auto t : tokens) {
				show(t);
			}
			cout << cur << "\t" << input[i] <<"\t" << type[state] << endl;
			exit(0);
		};
		auto other = [&](char c = 'a') {
			if (string(",;()\"").find(c) != -1) {
				if (state != None) {
					add(type[state]);
				}
				peek();
				add_colon();
				return true;
			}
			else if (isspace(c)) {
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
				if (other(c)){}
				else if (isalpha(c)) {
					peek();
					state = Char;
				}
				else if (isdigit(c)) {
					peek();
					state = Int;
				}
				else {
					error();
				}
				break;
			case Char:
				if (other(c)){
				}
				else if (isalnum(c) || c == '_') {
					peek();
				}
				else {
					error();
				}
				break;
			case Int:
				if (other(c)){}
				else if (isdigit(c)) {
					peek();
				}
				else if (c == '.') {
					peek();
					state = Float;
				}
				else {
					error();
				}
				break;
			case Float:
				if (other(c)){}
				else if (isdigit(c)) {
					peek();
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