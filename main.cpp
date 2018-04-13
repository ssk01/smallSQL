#include "api.h"
#include "token.h"
#include "Tokenizer.h"
#include "Interpreter.h"
using namespace std;


// todo  char '\0' insert
//insert index interpreter
//select 
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
void tokenizerIndexsTest() {
	string input(R"(create table person(
	height float unique,
		pid int,
		name char(32),
		identity char(128) unique,
		age int unique,
		primary key(pid)

);)");
}
void tokenizerTest() {
	string input(R"(
create table person(
	height float unique,
		pid int,
		name char(32),
		identity char(128) unique,
		age int unique,

);)");
//	create index idx_age on person(age);
//
//	drop index idx_height;
//	drop table person;
	string input1(R"(
create table person(
	height float unique,
		pid int,
		name char(32),
		identity char(128) unique,
		age int unique,
);
create index idx_age on person(age);
insert into person values(188.1, 18, "Person18", "000018", 35);

)");
	//Interpreter s(input1);
		//.run();
	Interpreter(input1).run();
	//for (auto t : Tokenizer(input).generate()) {
	//	//show(t);
	//	cout << t << endl;
	//}
}
int main() {
	//test1();
	tokenizerTest();
	char *a = "abc\0";
		char *a1 = "abc\0";
	cout <<  strlen(a) << strlen(a1) << endl;
}