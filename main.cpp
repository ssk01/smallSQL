#include <chrono>
#include <functional>
#include "test.h"
using namespace std;


int main() {
	//test_deleteRecord();
	auto test = [](std::function<void()> fun) {
		try {
			fun();
		}
		catch (std::runtime_error& e) {
			cout << e.what() << endl;
		}
	};
	//test(test_dropTable);
	//test(index1);
	//test(test_deleteRecord);
	//test(deleteRecord);
	//deleteRecord();
	auto timeer = [](function<void(int)> f, int age) {
		//fstream fout("log.txt", ios::app | ios::out);
		auto start = std::chrono::system_clock::now();
		f(age);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	};
	//timeer(time_create, "");
	Interpreter i;
	//timeer(time_select, 1110);


	//i.run("");
	//i.run("drop index age on person2;");
	//i.run("drop table person2");
	//showTableRecord("person2");

	i.run("select * from person2  where age = 24;");
	i.run("select * from person2  where age = 25;");
	i.run("select * from person2  where age = 26;");
	//i.run("delete from person where age = 32;");
	//i.run("select * from person2  where age = 32;");

	//i.runFile("scripts/no_index.sql");
	
	cout << "before" << endl;
	LOG("hit1");
	//i.run("create index age on person2(age);");
	//i.run("delete from person2 where age = 500");

	//i.run(input1);
	//timeer(time_select, 499);

	LOG("hit2");


//test unique
	//test(test_unique1);
	//test(test_unique2);
	//test(test_unique3);
	//test(test_unique4);
	return 0;
}