#include "block.h"
#include <algorithm>
#include "log.h"
#include "util.h"
#include "catalogData.h"
using namespace std;

Block* BufferManager::find_or_alloc(const std::string& fileName, int blockIndex)
{	
	//LOG("1333zzzz2","123123");
	auto iter = findBlock(fileName, blockIndex);
	if (iter != blocks.end()) {
		//todo size o(n)?
		if (blocks.size() >= 5 && iter != blocks.begin()) {
			blocks.splice(blocks.begin(), blocks, iter, std::next(iter));
		}
		return *iter;
	}

	return alloc_block(fileName, blockIndex);
}
void BufferManager::save() {
	for (auto b : blocks) {
		delete b;
	}
}
void readFile(Block* block) {
	std::ifstream in{ dataDir + block->fileName +"_"+ std::to_string(block->blockIndex) + ".txt" };
	if (in.is_open()) {
		in.read(block->_buffer, Block::BLOCKSIZE);
		/*CatalogManager::instance().showTableRecord(block->fileName, block->_buffer);*/
	}
	in.close();
}

Block* BufferManager::alloc_block(const std::string& fileName, int blockIndex)
{
	
	if (blocks.size() < BlockCount) {
		LOG("ALLOC");

		auto newBlock = new Block(fileName, blockIndex);
		//readFile(newBlock);
		readFile(newBlock);
		blocks.push_front(newBlock);
		return newBlock;
	} else {
		return BufferManager::lruReplaceBlock(fileName, blockIndex);
		//cout << "fuck" <<  endl;
		//exit(0);
	}

}
Block* BufferManager::lruReplaceBlock(const std::string& fileName, int blockIndex) {
	auto newBlock = new Block(fileName, blockIndex);
	//to get index optimation
	readFile(newBlock);
	LOG("lru: ", fileName, blockIndex);
	auto old = blocks.back();
	LOG("lru replace ", old->fileName, old->blockIndex);
	delete old;
	blocks.pop_back();
	blocks.push_front(newBlock);
	return newBlock;
}

BufferManager::ListIter BufferManager::findBlock(const std::string& fileName, int blockIndex) 
{
	return std::find_if(blocks.begin(), blocks.end(), [&](const auto& elm) {
		return (elm->blockIndex == blockIndex && elm->fileName == fileName);
	});
}
