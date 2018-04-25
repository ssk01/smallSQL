#pragma once
#include <stdexcept>
#include <string>
#include <assert.h>
using std::string;
int Int(char *value);
float Float(char *value);
//class InsuffcientSpace : public std::runtime_error
//{
//public:
//	explicit InsuffcientSpace(const char* msg)
//		: std::runtime_error(("not enough space: " + std::string(msg)).c_str())
//	{
//	}
//};

extern const  std::string recordlistDir;
extern const  std::string catalogDataDir;
extern const  std::string tableInfoDir;
extern const  std::string dataDir;
extern const std::string  indexDir;
std::string indexPath(const string& tableName, const string& indexName);
std::string indexInfoPath(const string& tableName);
string catalogDataPath();
void tryRemove(const string& tableName);