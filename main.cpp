#include "test.h"

//todo add index pre records ;
//what if no add.
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
	//test(test_unique1);
	try {
	dropTable("person");
		//cout <<"remove "<<(dataDir + "person"+ "_" + std::to_string(0) + ".txt").c_str();
	//showTableRecord("person");
	}
	catch (std::runtime_error& e) {
		cout << e.what() << endl;
	}
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
}