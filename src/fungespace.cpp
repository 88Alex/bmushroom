#include "fungespace.hpp"

FungeSpaceBlock::FungeSpaceBlock()
{
	for(uint8_t i = 0; i < FUNGESPACE_BLOCK_SIZE; i++)
	{
		for(uint8_t j = 0; j < FUNGESPACE_BLOCK_SIZE; j++)
		{
			data[i][j] = 0;
		}
	}
}

void FungeSpaceBlock::set(uint8_t x, uint8_t y, int32_t val)
{
	data[x][y] = val;
}

int32_t FungeSpaceBlock::get(uint8_t x, uint8_t y)
{
	return data[x][y];
}

FungeSpace::FungeSpace()
{
	blockKeyMap = std::map<int32_t, std::map<int32_t, int32_t> >();
	blockMap = std::map<int32_t, FungeSpaceBlock*>();
	numBlocks = 0;
}

void FungeSpace::set(int32_t x, int32_t y, int32_t val)
{
	int32_t blockX = x / FUNGESPACE_BLOCK_SIZE;
	int32_t blockY = y / FUNGESPACE_BLOCK_SIZE;
	int32_t index = -1;
	if(blockKeyMap.find(blockX) == blockKeyMap.end())
	{
		blockKeyMap[blockX] = std::map<int32_t, int32_t>();
	}
	if(blockKeyMap[blockX].find(blockY) == blockKeyMap[blockX].end())
	{
		// allocate a new block
		index = numBlocks;
		blockMap[index] = new FungeSpaceBlock();
		blockKeyMap[blockX][blockY] = index;
		numBlocks++;
	}
	else
	{
		index = blockKeyMap[blockX][blockY];
	}
	uint8_t subX = x % FUNGESPACE_BLOCK_SIZE;
	uint8_t subY = y % FUNGESPACE_BLOCK_SIZE;
	blockMap[index]->set(subX, subY, val);
}

int32_t FungeSpace::get(int32_t x, int32_t y)
{
	int32_t blockX = x / FUNGESPACE_BLOCK_SIZE;
	int32_t blockY = y / FUNGESPACE_BLOCK_SIZE;
	if(blockKeyMap.find(blockX) == blockKeyMap.end() || blockKeyMap[blockX].find(blockY) == blockKeyMap[blockX].end())
	{
		return 0;
	}
	int32_t index = blockKeyMap[blockX][blockY];
	uint8_t subX = x % FUNGESPACE_BLOCK_SIZE;
	uint8_t subY = y % FUNGESPACE_BLOCK_SIZE;
	return blockMap[index]->get(subX, subY);
}