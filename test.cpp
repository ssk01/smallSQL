#include "test.h"
using namespace std;

void test_unique1() {
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
	//delete from person where age = 331;
	Interpreter s(input);
	s.run();
	cout << "\n\n\n" << endl;
	showTableRecord("person");

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