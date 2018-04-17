#pragma once
#include <list>
#include <string>
#include <iostream>
#include "block.h"
#include "errors.h"
#include "to.h"
//extern void fuck1();
using std::cout;
using std::endl;
using std::list;
using std::string;
class Block;

class BufferManager {
public:
	using ListIter = std::list<Block *>::iterator;
	Block& find_or_alloc(const std::string& fileName, int blockIndex);
	static BufferManager& instance()
	{
		static BufferManager instance;
		return instance;
	}
private:
	list<Block *> blocks;
	const static int BlockCount = 128;
		
	Block& alloc_block(const std::string& fileName, int blockIndex);
	ListIter findBlock(const std::string& fileName, int blockIndex);
	Block& lruReplaceBlock(const std::string& fileName, int blockIndex);
};
class Block {
public:
	//friend class Recordlist;
	friend void readFile(Block* newBlock);
	friend class BufferManager;
	const static int BLOCKSIZE = 1024;
	Block(string name, int index) : fileName(name), blockIndex(index), offset(0), _buffer(new char[BLOCKSIZE]), dirty(false) {
		if (_buffer == nullptr) {
			throw InsuffcientSpace("can not new a block any more");
		}
	}
	char *rawPtr() {
		return _buffer;
	}
	~Block() {

	}
private:
	char *_buffer;
	string fileName;
	int blockIndex;
	int offset;
	bool dirty;
};

