#include "test.h"
#include "Interpreter.h"
void Interpreter::runFile(const string& fileName) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw FileError(fileName.c_str());
	}
	string input;
	//auto c = in.get()
	char c;
	while ((c = in.get()) != EOF) {
		input.push_back(c);
		if (c == ';') {
			init(input);
			run();
			input = "";
		}
	}
	if (!input.empty()) {
		init(input);
		run();
	}
}