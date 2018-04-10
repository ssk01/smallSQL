#include "block.h"
#include <algorithm>
using namespace std;

Block& BufferManager::find_or_alloc(const std::string& fileName, int blockIndex)
{
	auto iter = findBlock(fileName, blockIndex);
	if (iter != blocks.end()) {
		if (blocks.size() >= 5 && iter != blocks.begin()) {
			blocks.splice(blocks.begin(), blocks, iter, std::next(iter));
		}
		return **iter;
	}
	return alloc_block(fileName, blockIndex);
}
Block& BufferManager::alloc_block(const std::string& fileName, int blockIndex)
{
	auto newBlock = new Block(fileName, blockIndex);
	if (blocks.size() < BlockCount) {
		blocks.push_front(newBlock);
		return *newBlock;
	} else {
		cout << "fuck" <<  endl;
		exit(0);
	}

}
BufferManager::ListIter BufferManager::findBlock(const std::string& fileName, int blockIndex) 
{
	return std::find_if(blocks.begin(), blocks.end(), [&](const auto& elm) {
		return (elm->blockIndex == blockIndex && elm->fileName == fileName);
	});
}
