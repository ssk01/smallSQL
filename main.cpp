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

	//i.run("create index age on person2(age);");

	timeer(time_select, 500);
	LOG("hit1");
	timeer(time_select, 400);
	LOG("hit2");
	//try {

	//}
	//catch (std::runtime_error& e) {
	//	cout << e.what() << endl;
	//}
	/*string a("bbb");
	string a1("bbc");
	string a12("bbc");
	std::fstream in{ string("tableInfo/") + "3" + ".txt",std::ios::trunc|std::ios::in|std::ios::out };
	in << a;
	in << a1;
	in.seekp(0);
	string b;
	string b1;
	in >> b;
	in >> b1;
	cout << b  << "¡¡¡¡¡¡" << b1;*/


//test unique
	//test(test_unique1);
	//test(test_unique2);
	//test(test_unique3);
	//test(test_unique4);
	return 0;
}