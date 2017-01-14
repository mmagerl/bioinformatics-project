#pragma once
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

// Type definitions used in the lookup table
typedef vector<vector<pair<uint32_t, vector<uint32_t> > > > table_t;
typedef pair<uint32_t, vector<uint32_t> > element_t;
typedef vector<pair<uint32_t, vector<uint32_t> > > class_t;

// Abstraction of the lookup table used in RRR structure
// This class is a singleton, and is common for all the RRRs used in the Wavelet tree
// Because of that it should be declared only once
// Example usage:
//			oRRRTable::getInstance();
//			if (oRRRTable::initialBlockLength == 0) {
//				oRRRTable::getInstance().createTable(5);
//			}
class oRRRTable {
public:
	// Returns lookup table instance
	static oRRRTable& getInstance();

	// Fills the table with possible 0-1 permutations for each possible rank
	// Also calculates rank for each position in the block
	//		b		maximum block length
	void createTable(uint32_t blockLengthInBits);

	// Returns offset index in the lookup table given the blocks binary content
	//		classIndex		block class(rank) index in the table
	//		block			binary block content
	//		blockLength		block length (used to store data for blocks of different sizes in the table)
	uint32_t getOffset(uint32_t classIndex, uint32_t block, uint32_t blockLength);
	
	// Returns number of possible block permutations with the given class (popcount)
	uint32_t getClassSize(uint32_t class_);

	// Returns lookup table
	table_t& getTable();

	// Maximum block length in the lookup table
	// Used to know when the table is first constructed and initialized
	// Also used to index ranks for smaller blocks (for other RRRs in the Wavelet tree) that are
	// implicitly stored in the lookup table
	static uint32_t initialBlockLength;

	// Returns number of bits needed to store the offset for the given block class(rank)
	//		class_		block class(rank)
	uint32_t getBitsForOffset(uint64_t class_);

	// Returns the rank in the block defined with class_ and offset_ up to the given position_
	//		class_		block class(total rank)
	//		offset_		offset for the appropriate block representation in the table
	//		position_	index in the block representation
	uint64_t getRankForBlockAtPosition(uint64_t class_, uint64_t offset_, uint32_t position_);

	// Returns index of the first set bit for which the current block rank equals rank_
	// in the given block defined with class_ and offset_
	//		class_		block class(total rank)
	//		offset_		offset for the appropriate block representation in the table
	//		rank_		searched rank
	uint64_t getIndexForRank(uint64_t class_, uint64_t offset_, uint32_t rank_);

	// Returns index of the first zero bit for which the current block rank equals rank_
	// in the given block defined with class_ and offset_
	//		class_		block class(total rank)
	//		offset_		offset for the appropriate block representation in the table
	//		rank_		searched rank
	//		blockSize_	block size of the current RRR
	uint64_t getIndexForRankZero(uint64_t class_, uint64_t offset_, uint32_t rank_, uint32_t blockSize_);

private:
	// Lookup table
	// 2D representation	-> first index is the block class
	//						-> second index is the offset
	// Elements are pairs  containing original binary representation and ranks for each block element
	table_t table_;

	// How many bits are needed to store the offset in the 
	// binary RRR representation for each possible block class(rank)
	// Elements are the needed offsets while the block class is used as index
	vector<uint32_t> bitsForOffset;

	// Private constructor because this is a singleton
	oRRRTable(){};

	// Private copy constructor because this is a singleton
	oRRRTable(oRRRTable const&);

	// Private assignment operator because this is a singleton
	void operator=(oRRRTable const&);

	// Generates the next permutation given the previous lexicographical value v
	uint32_t nextPermutation(uint32_t v);

	// Generates the first permutation with c set bits, which is used to generate all oher permutations
	uint32_t firstElement(uint32_t c);
};
