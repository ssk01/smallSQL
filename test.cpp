#include "test.h"
using namespace std;
void serializer() {
//	string input(R"(
//create table person(
//	height float,
//		pid int,
//		name char(32),
//		identity char(128),
//		age int unique,
//);
//insert into person values(188.1, 18, "Person11", "000018", 32);
//insert into person values(188.1, 18, "Person11", "000018", 1);
//)");
	string input1(R"(
	select * from person;
)");
	Interpreter s(input1);
	s.run();
	showTableRecord("person");
//	s.run(R"(insert into person values(188.1, 18, "Person11", "000018", 35);
//)");
	//select * from person;
}
void index2() {
	string input(R"(
create table person(
	height float,
		pid int,
		name char(32),
		identity char(128),
		age int unique,
);

insert into person values(188.1, 18, "Person11", "000018", 35);
insert into person values(168.1, 18, "Person12", "000018", 32);
insert into person values(168.1, 19, "Person15", "000018", 32);
insert into person values(168.1, 18, "Person12", "000018", 33);

)");
	Interpreter s(input);
	s.run();
	string input1(R"(
create index age1 on person(age);

select * from person;
)");
	cout << "mmmm" << endl;
	s.run(input1);
	cout << "\n\n\n" << endl;
	//showTableRecord("person");
	showIndex("person", "age1");
}
void index1() {
	string input(R"(
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
insert into person values(168.1, 19, "Person15", "000018", 31);
insert into person values(168.1, 18, "Person14", "000018", 33);
)");
	Interpreter s(input);
	s.run();
//	string input1(R"(
//drop index name1 on person;
//insert into person values(16.0, 18, "Person12", "000018", 33);
//select * from person;
//)");
//	cout << "mmmm" << endl;
//	s.run(input1);
	cout << "\n\n\n" << endl;
	showTableRecord("person");
	//showIndex("person", "name1");
}


void test_unique1() {
	//delete from person where age = 32;
//select * from person;
	string input(R"(
create table person(
	height float,
		pid int,
		name char(32),
		identity char(128),
		age int unique,
);
insert into person values(188.1, 18, "Person11", "000018", 35);
insert into person values(168.1, 18, "Person12", "000018", 32);
insert into person values(168.1, 18, "Person12", "000018", 33);
insert into person values(168.1, 19, "Person15", "000018", 32);
)");
//drop table person;
	Interpreter s(input);
	s.run();
	cout << "\n\n\n" << endl;
	showTableRecord("person");

}
void droptable() {
	string input(R"(
	drop table person;
	)");
	Interpreter s(input);
	s.run();
	cout << "\n\n\n" << endl;
}
void test_unique2() {
	string input(R"(
drop table person;

create table person(
	height float unique,
		pid int,
		name char(32),
		identity char(128),
		age int ,
);
insert into person values(168.11, 19, "Person12", "000018", 33);
insert into person values(168.11, 18, "Person12", "000018", 32);
insert into person values(188.21, 18, "Person11", "000018", 35);
insert into person values(168.331, 19, "Person15", "000018", 32);

)");
	//delete from person where age = 331;
	Interpreter s(input);
	s.run();
	cout << "\n\n\n" << endl;
	showTableRecord("person");
}
void test_unique3() {
	string input(R"(
drop table person;

create table person(
	height float ,
		pid int,
		name char(32) unique,
		identity char(128),
		age int ,
);
insert into person values(188.21, 18, "Person11", "000018", 35);
insert into person values(168.11, 18, "Person12", "000018", 32);
insert into person values(168.331, 19, "Person15", "000018", 32);
insert into person values(168.11, 19, "Person12", "000018", 33);
)");
	//delete from person where age = 331;
	Interpreter s(input);
	s.run();
	cout << "\n\n\n" << endl;
	showTableRecord("person");
}
void test_unique4() {
	string input(R"(
drop table person;
create table person(
	height float ,
		pid int,
		name char(32) unique,
		identity char(128),
		age int unique,
);
insert into person values(188.21, 18, "Person11", "000018", 35);
insert into person values(168.11, 18, "Person12", "000018", 32);
insert into person values(168.331, 19, "Person15", "000018", 32);
insert into person values(168.11, 19, "Person12", "000018", 33);
)");
	//delete from person where age = 331;
	Interpreter s(input);
	s.run();
	cout << "\n\n\n" << endl;
	showTableRecord("person");
}

void test1() {
	Attribute a{ "age","int", 4 };
	Attribute b{ "dollor","char", 6 };
	Attribute c{ "ages","int", 4 };

	string tableName("ssk");
	string indexName("fcb");
	addTable(tableName, vector<Attribute>{ a, b, c });
	addIndex(tableName, indexName, "age");
	vector<Token> value{ { "int","4555" },{ "char","abc" } ,{ "int","13123" } };
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

void test_deleteRecord() {
	string input(R"(

	delete from person where age = 32;


)");

	//delete from person where age = 331;

	Interpreter s(input);
	s.run();
	cout << "\n\n\n" << endl;
	showTableRecord("person");
	showIndex("person", "name1");
}

//create table person6(
//	height float,
//		pid int,
//		name char(32),
//		identity char(128),
//		age int,
//);
//insert into person6 values(188.1, 18, "Person11", "000018", 2);
//insert into person6 values(188.1, 18, "Person11", "000018", 3);
//insert into person6 values(188.1, 18, "Person11", "000018", 4);
void test_insert_memory() {
	string input(R"(
insert into person6 values(188.1, 18, "Person11", "000018", 2);
insert into person6 values(188.1, 18, "Person11", "000018", 3);
insert into person6 values(188.1, 18, "Person11", "000018", 4);
)");
//insert into person1 values(188.1, 18, "Person11", "000018", 1);

//insert into person1 values(188.1, 18, "Person11", "000018", 3);
//select * from person ;
//insert into person values(188.1, 18, "Person11", "000018", 35);
	//drop table person;
	//showTableRecord("person");
	Interpreter s(input);
	s.run();
	showTableRecord("person6");

	cout << "\n\n\n" << endl;
}

void deleteRecord() {
	string input(R"(
delete from person6 where age = 2;
)");
	Interpreter s(input);
	s.run();
	//showTableRecord("person6");
}
void test_dropTable() {
	string input(R"(
drop table person;
)");
	//drop table person;
	//drop table person1;
	//drop table person2;
	//drop table person3;
	//drop table person4;
	//drop table person5;
	Interpreter s(input);
	s.run();
}
void time_create(string a) {
	Interpreter i;
	i.runFile("scripts/no_index.sql");
}
void time_select(int age) {
	try {
		Interpreter i;
		i.runFile("scripts/no_index.sql");
		//i.run("create index age on person2(age);");

	/*	i.run("select * from person2 where age = " + to_string(age) + " ;");*/
		/*i.run("select * from person2 where age > " + to_string(age) + " and " + " age < " + to_string(age + 2) + " ;");
*/
		//dropTable("person");
		//cout <<"remove "<<(dataDir + "person"+ "_" + std::to_string(0) + ".txt").c_str();
		//showTableRecord("person");
	}
	catch (std::runtime_error& e) {
		cout << e.what() << endl;
	}
}
//	string input1(R"(
//create table person(
//	height float,
//		pid int,
//		name char(32),
//		identity char(128),
//		age int unique,
//);
//insert into person values(188.1, 18, "Person11", "000018", 35);
//insert into person values(168.1, 18, "Person12", "000018", 32);
//insert into person values(168.1, 18, "Person15", "000018", 32);
//)");
//	Interpreter s1(input1);
//	s1.run();
//	cout << "\n\n\n" << endl;
//	showTableRecord("person");
//	showIndex("person", "name1");
//
//showTableRecord("person2");
/*	i.run("select * from person2  where age > 3 and ;");*/
//i.run("select * from person2  where age
//i.run("");
//i.run("drop index age on person2;");
//i.run("drop table person2");
//showTableRecord("person2");
//ifstream in("db/data/person2_5.txt");
//if (in.is_open()) {
//	char buf[1024];
//	in.read(buf,1024);
//	cout << *(int *)(buf + 172 + 168) << endl;
//	//cout << *(int *)(buf  + 168) << endl;
//}
//std::cout.write(reinterpret_cast<const char*>(&x), sizeof(x));
//i.run("select * from person2  where age = 24;");
//i.run("select * from person2  where age = 25;");
//i.run("select * from person2  where age = 3;");
//i.run(R"(insert into person2 values(188.1, 18, "Person11", "000018", 100);)");
//i.run("delete from person2 where age = 3;");
//i.run("select * from person2  where age = 3;");
//i.run("select * from person2  where age = 2;");
//i.run("select * from person2  where age = 100;");
//i.runFile("scripts/no_index.sql");
//
//cout << "before" << endl;
//LOG("hit1");
//i.run("create index age on person2(age);");
//i.run("delete from person2 where age = 500");

//i.run(input1);
//timeer(time_select, 499);
//showTableRecord("person2


//test unique
//test(test_unique1);
//test(test_unique2);
//test(test_unique3);
//test(test_unique4);