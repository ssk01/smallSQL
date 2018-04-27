#pragma once
#include <stdexcept>
#include <string>
#include <assert.h>
using std::string;
int Int(char *value);
float Float(char *value);

string recordlistPath(const string& tableName);
string tableInfoPath(const string& tableName);
string dataPath(const string& tableName, int blockIndex);
std::string indexPath(const string& tableName, const string& indexName);
std::string indexInfoPath(const string& tableName);
string catalogDataPath();
void tryRemove(const string& tableName);
bool fileExisted(const string& tableName);