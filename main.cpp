#include "test.h"

//todo add index pre records;
// input
//index  unique feature;
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
	test(index2);



//test unique
	//test(test_unique1);
	//test(test_unique2);
	//test(test_unique3);
	//test(test_unique4);
}