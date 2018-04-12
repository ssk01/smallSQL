#include "api.h"
#include "token.h"
#include "Tokenizer.h"
using namespace std;
void test1() {
	Attribute a{ "age","int", 4 };
	Attribute b{ "dollor","char", 6};
	Attribute c{ "ages","int", 4 };

	string tableName("ssk");
	string indexName("fcb");
	addTable(tableName, vector<Attribute>{ a, b, c });
	addIndex(tableName, indexName, "age");
	vector<Token> value{ {"int","4555"}, {"char","abc"} ,{ "int","13123" } };
	vector<Token> value1{ { "int","451355" },{ "char","abc" } ,{ "int","1 3" } };
	insertRecord(tableName, value);
	insertRecord(tableName, value1);
	showTableRecord(tableName);
	showIndex(tableName, indexName);
}
void tokenizerTest() {
	string input(R"(create table person(

	height float unique,
		pid int,
		name char(32),
		identity char(128) unique,
		age int unique,
		primary key(pid)
);)");
	string input1(R"(
	insert into person values(188.1, 18, "Person18", "000018", 35);
	insert into person values(189.1, 19, "Person19", "000019", 36);
	insert into person values(190.1, 20, "Person20", "000020", 37);

	create index idx_height on person(height);
	create index idx_identity on person(identity);
	create index idx_age on person(age);

	drop index idx_height;
	drop index idx_identity;
	drop index idx_age;

	drop table person;
)");
	for (auto t : Tokenizer(input1).generate()) {
		show(t);
	}
}
int main() {
	//test1();
	tokenizerTest();
}