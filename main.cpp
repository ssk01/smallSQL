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
auto showToken = [](string intput) {
	for (auto t : Tokenizer(intput).generate()) {
		//show(t);
		cout << t << endl;
	}
};
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
	height float,
		pid int,
		name char(32),
		identity char(128),
		age int unique,
);
insert into person values(188.1, 18, "Person18", "000018", 35);
insert into person values(188.1, 18, "Person18", "000018", 35);

)");
	string search(R"(
create table person(
	height float,
		pid int,
		name char(32),
		identity char(128),
		age int unique,
);
create index name1 on person(name);
insert into person values(188.1, 18, "Person11", "000018", 35);
insert into person values(168.1, 18, "Person12", "000018", 32);
insert into person values(168.1, 18, "Person13", "000018", 33);
insert into person values(168.1, 18, "Person14", "000018", 332);
insert into person values(168.1, 18, "Person15", "000018", 331);
	select * from person where name != "Person15" and height < 170.1;


)");
	//select * from person where height <= 176.3 and age != 33; 
	Interpreter s(search);
	s.run();
		//.run();
	//Interpreter(search)

}
class A {
public:
	A() { cout << "construct" << endl; }
	~A() { cout << "delete" << endl; }
};
int main() {
	//test1();

	//py  À©ÈÝ
	//tokenizerTest();
	map<int, A>b;
	b[1] = A();
	cout << " wtf " << endl;
	b.erase(1);
	cout << "end" << endl;
	vector<int> v{ 0 ,1,2,3};
	cout << v.capacity() << endl;
}