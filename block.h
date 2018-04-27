#pragma once
#include <list>
#include <string>
#include <iostream>
#include <cstdio>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <set>
#include "block.h"
#include "errors.h"
#include "log.h"
#include "util.h"
using std::cout;
using std::endl;
using std::list;
using std::string;
using std::set;
class BufferManager;
class Block {
public:
	//friend class Recordlist;
	friend class BufferManager;
	const static int BLOCKSIZE = 4096;
	void save() {
		if (dirty == 1) {
			////cout << "save block" << endl;
			LOG("Save block:", fileName, blockIndex);
			std::ofstream out{ dataDir + fileName + "_" + std::to_string(blockIndex) + ".txt", std::ios::trunc };
			out.write(_buffer, BLOCKSIZE);
		}
		else if (dirty == 2) {
			LOG("save block: remove");
		}
		else {
			LOG("save block: clear");
		}
	}
	void remove() {
		std::ifstream in{ dataDir + fileName + "_" + std::to_string(blockIndex) + ".txt" };
		if (in.is_open()) {
			in.close();
			std::remove((dataDir + fileName + "_" + std::to_string(blockIndex) + ".txt").c_str());
		}
		else {
			LOG("heeeee");
		}
		dirty = 2;
	}
	void modify() {
		dirty = 2;
	}
	Block(string name, int index) : fileName(name), blockIndex(index), offset(0), _buffer(new char[BLOCKSIZE]), dirty(0) {
		if (_buffer == nullptr) {
			throw InsuffcientSpace("can not new a block any more");
		}
		readFile();
		LOG("block constructor", name, index);
	}
	char *rawPtr() {
		return _buffer;
	}
	~Block() {
		save();
		LOG("block delete");
		delete[] _buffer;
	}
private:
	void readFile() {
		std::ifstream in{ dataDir + fileName + "_" + std::to_string(blockIndex) + ".txt" };
		if (in.is_open()) {
			in.read(_buffer, Block::BLOCKSIZE);
		}
		in.close();
	}
	char *_buffer;
	string fileName;
	int blockIndex;
	int offset;
	int dirty;
};


class BufferManager {
public:
	using ListIter = std::list<Block *>::iterator;
	static BufferManager& instance()
	{
		static BufferManager instance;
		return instance;
	}
	void dropTable(const std::string& fileName, const set<int> &blockIndexs) {
		cout << "b__" << endl;
		for (auto b : blocks) {
			cout << b->fileName << endl;
		}
		cout << "b__" << blocks.size();
		auto removed = std::remove_if(blocks.begin(), blocks.end(), [&](const auto &b) {
			if (b->fileName == fileName) {
				LOG("drop block, ", fileName);
				b->remove();
				return true;
			}
			return false;
		});
		auto b = removed;
		int i = 0;
		while (b != blocks.end()) {
			delete *b;
			b++;
			i++;
		}
		LOG("removed num", i);
		for (auto i : blockIndexs) {
			std::ifstream in{ dataDir + fileName + "_" + std::to_string(i) + ".txt"};
			if (in.is_open()) {
				in.close();
				LOG("drop block out ", fileName, i);
				string file = (dataDir + fileName + "_" + std::to_string(i) + ".txt");
			cout <<"remove "<<file<<"\t"<<std::remove(file.c_str());
			}
		}
		blocks.erase(removed, blocks.end());
		LOG("amazed");
	}
	Block* find_or_alloc(const std::string& fileName, int blockIndex);
	BufferManager() = default;
	~BufferManager() {
		LOG("buffermanager deconstructor");
		save();
	}
private:
	void save();
	list<Block *> blocks;
	//const static int BlockCount = 1;
	const  int BlockCount = 128;
	//void read_file(const Block* block);
	Block* alloc_block(const std::string& fileName, int blockIndex);
	ListIter findBlock(const std::string& fileName, int blockIndex);
	Block* lruReplaceBlock(const std::string& fileName, int blockIndex);
};
