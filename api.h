#pragma once
#include "catalogData.h"
#include "index.h"
#include <tuple>
#include "condition.h"
void addTable(const string& name, vector<Attribute>& attr);
void insertRecord(const string &name, const vector<Token>& content);
void showTableRecord(const string &name);
void addIndex(const string& tableName, const string& indexName, const string& attrName);
void showIndex(const string& tableName, const string& indexName);
void selects(const string& tableName, const vector<Condition>& conds);
//todo 
//toentry and show;