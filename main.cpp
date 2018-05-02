#include <chrono>
#include <functional>
#include "test.h"
#include <bitset>
using namespace std;
 
int main() {
	auto test = [](std::function<void()> fun) {
		try {
			fun();
		}
		catch (std::runtime_error& e) {
			cout << e.what() << endl;
		}
	};

	auto timeer = [](function<void(int)> f, int age) {
		auto start = std::chrono::system_clock::now();
		f(age);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	};
	try {
		cout << "fucc" << endl;
		dropTable("person");
		cout << "hehe" << endl;
		timeer(time_select, 1110);
	}
	catch (std::runtime_error& e) {
		cout << "error" << endl;
		cout << e.what() << endl;
	}
	return 0;
}