#include "util.h"
#include <string>
#include <fstream>
#include "log.h"
using std::string;
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
const std::string indexDir = "db/index/";
const std::string indexInfoDir = "db/indexInfo/";
string recordlistPath(const string& tableName) {
	return recordlistDir + tableName + ".txt";
}
string dataPath(const string& tableName, int blockIndex) {
	return dataDir + tableName + "_" + std::to_string(blockIndex) + ".txt";
}
string tableInfoPath(const string& tableName) {
	return tableInfoDir + tableName + ".txt";
}
string catalogDataPath() {
	return catalogDataDir + "tablename.txt";
}
std::string indexPath(const string& tableName, const string& indexName) {
	return indexDir + tableName + "_" + indexName + ".txt";
}
std::string indexInfoPath(const string& tableName) {
	return indexInfoDir + tableName +  ".txt";
}
bool fileExisted(const string& tableName) {
	std::ifstream in(tableName);
	if (in.is_open()) {
		in.close();
		return true;
	}
	in.close();
	return false;
}
void tryRemove(const string& tableName) {
	if (fileExisted(tableName)) {
		if (std::remove(tableName.c_str())==0) {
			LOG("remove success", tableName);
		}
	}
	LOG("remove faile", tableName);
}