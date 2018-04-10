#include "api.h"
using namespace std;

void test1() {
	Attribute a{ "age","int", 4 };
	Attribute b{ "dollor","char", 6};
	Attribute c{ "ages","int", 4 };

	string tableName("ssk");
	addTable(tableName, vector<Attribute>{ a, b, c });
	vector<string> value{ "4555", "abc","13123" };
	insertRecord(tableName, value);
	showTableRecord(tableName);
}
int main() {
	test1();
}