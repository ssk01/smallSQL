#include "util.h"
#include <string>
int Int(char *value) {
	int a;
	memcpy(&a, value, sizeof(a));
	return a;
}
float Float(char *value) {
	float a;
	memcpy(&a, value, sizeof(a));
	return a;
}
const std::string recordlistDir = "db/recordlist/";
const std::string catalogDataDir = "db/catalogData/";
const std::string tableInfoDir = "db/tableInfo/";
const std::string dataDir = "db/data/";
