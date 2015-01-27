#ifndef FUNGESPACE_HPP_INCLUDED
#define FUNGESPACE_HPP_INCLUDED

#define FUNGESPACE_BLOCK_SIZE 64

#include <map>
#include <cstdint>

class FungeSpaceBlock
{
	public:
		FungeSpaceBlock();
		void set(uint8_t x, uint8_t y, int32_t val);
		int32_t get(uint8_t x, uint8_t y);
	private:
		uint8_t data[FUNGESPACE_BLOCK_SIZE][FUNGESPACE_BLOCK_SIZE];
};

class FungeSpace
{
	public:
		FungeSpace();
		void set(int32_t x, int32_t y, int32_t val);
		int32_t get(int32_t x, int32_t y);
	private:
		// the first int is x, the second is y, the third is the index into blockMap
		std::map<int32_t, std::map<int32_t, int32_t> > blockKeyMap;
		// use the index (contained in blockKeyMap) to access the proper block map.
		std::map<int32_t, FungeSpaceBlock*> blockMap;
		int32_t numBlocks;
};

#endif // #ifndef FUNGESPACE_HPP_INCLUDED