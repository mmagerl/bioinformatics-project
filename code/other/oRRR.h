#pragma once
#include <string>
#include <vector>
#include "oRRRTable.h"
#include <cmath>
#include <cstdint>
#include <algorithm>

typedef pair<uint64_t, uint64_t> superBlock;

using namespace std;

// Class representing an oRRR data structure
// Example usage:
//		oRRR rrr(string("001010011010101101000001001010011011000101101101011"));
//		cout << rrr.rank1(10);
class oRRR {
private:
	// Binary oRRR (blocks) stored in 64-bit integers
	vector<uint64_t> content;

	// Superblocks (storing index and current rank)
	vector<superBlock> superBlocks;

	// Number of bits in a block
	uint32_t blockSize;

	// Number of bits in a superblock
	uint32_t superBlockSize;

	// Number of blocks per superblock
	uint32_t blocksPerSuperBlock;

	// Number of bits used to encode a block class in the binary content
	uint32_t bitsForClass;

	// Last index in the last content integer
	// Used to know when to stop searching in rank/select
	uint64_t maxIndexInLastContentElement;

	// Size of the input bitvector
	uint64_t inputVectorLength;

	// Used for shifting bits
	//		leftSide		left side of the shift operator (<< or >>)
	//		rightSide		right side of the shift operator (<< or >>)
	//		isLeft			true if you want to shift to the left (<<), false otherwise (>>)
	uint64_t bitShift(uint64_t leftSide, int32_t rightSide, bool isLeft = true);

	// Compares two superblock by their rank
	// Used for binary search when doing select1
	static bool compareSuperBlock(superBlock a, superBlock b);

	// Returns iterator that points to the last superblock whose rank is smaller or equal to count
	// Used for binary search when doing select0
	vector<superBlock>::iterator getIteratorForSelectZero(uint64_t count);
public:
	// Creates a new oRRR instance from the given bits
	//		bits		string containing only zeros and ones
	oRRR(string &bits);

	// Returns size of the bits vector used when constructing the oRRR
	uint64_t getInputVectorLength() const;

	// Returns the number of times 1 appears in the interval [0, index] in the input bitvector
	// Throws invalid_argument exception if index is too large
	uint64_t rank1(uint64_t index);

	// Returns the number of times 0 appears in the interval [0, index] in the input bitvector
	// Throws invalid_argument exception if index is too large
	uint64_t rank0(uint64_t index);

	// Returns the index of the count-th occurence of a set bit(1) in the input bitvector
	// Throws invalid_argument exception if count value is too large or non-positive
	uint64_t select1(uint64_t count);

	// Returns the index of the count-th occurence of an unset bit(0) in the input bitvector
	// Throws invalid_argument exception if count value is too large or non-positive
	uint64_t select0(uint64_t count);

	// Returns the bit stored in the oRRR at the given index
	// Throws invalid_argument exception if index is too large
	uint8_t access(uint64_t index);
};
