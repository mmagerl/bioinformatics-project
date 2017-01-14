#include "oRRRTable.h"

// Maximum block length in the lookup table
// Used to know when the table is first constructed and initialized
// Also used to index ranks for smaller blocks (for other RRRs in the Wavelet tree) that are
// implicitly stored in the lookup table
uint32_t oRRRTable::initialBlockLength = 0;

// Returns lookup table instance
oRRRTable& oRRRTable::getInstance() {
	static oRRRTable instance;
	return instance;
}

// Fills the table with possible 0-1 permutations for each possible rank
// Also calculates rank for each position in the block
//		b		maximum block length
void oRRRTable::createTable(uint32_t b) {
	// Set maximum table block length if this is the first call to createTable
	if (oRRRTable::initialBlockLength == 0) {
		oRRRTable::initialBlockLength = b;
		this->table_.clear();
	}

	// Senerate all possible 0-1 permutations for all popcounts (ranks) and store them in the table
	// Edge case - when popcount equals 0
	vector<uint32_t> cumulativeZeroRank(b, 0);
	element_t elZero(0, cumulativeZeroRank);
	class_t subtableZero;
	subtableZero.push_back(elZero);
	this->table_.push_back(subtableZero);
	this->bitsForOffset = vector<uint32_t>(b + 1, 1);

	// For all other popcount values generate appropriate table elements
	for (uint32_t p = 1; p <= b; p++) {
		class_t subtable;
		uint32_t  v, initial;
		// Generate first permutation with p set bits
		v = initial = firstElement(p);
		// Mask used for getting permutations of appropriate length
		uint32_t block_mask = firstElement(b);

		while (v >= initial) {
			vector<uint32_t> cumulativeRank;
			// Mask used for reading bits in the permutation in order to compute the cumulative rank
			uint32_t mask = 1 << (b - 1);
			uint32_t rankCounter = 0;

			while (mask) {
				if (v & mask) {
					rankCounter++;
				}
				cumulativeRank.push_back(rankCounter);
				mask /= 2;
			}

			// Store the permutation in the lookup table
			element_t el(v, cumulativeRank);
			subtable.push_back(el);

			// Generate next permutation (lexicographical)
			v = nextPermutation(v) & block_mask;
		}

		// Store all the permutations with popcount p in the lokup table
		this->table_.push_back(subtable);
		uint32_t numberOfBits = (uint32_t)ceil(log2(subtable.size()));
		this->bitsForOffset[p] = numberOfBits == 0 ? 1 : numberOfBits;
	}
}

// Generates the next permutation given the previous lexicographical value v
uint32_t oRRRTable::nextPermutation(uint32_t v) {
	uint32_t t = (v | (v - 1)) + 1;
	uint32_t w = t | ((((t & (~t+1)) / (v & (~v + 1))) >> 1) - 1);
	return w;
}

// Generates the first permutation with c set bits, which is used to generate all oher permutations
uint32_t oRRRTable::firstElement(uint32_t c) {
	return (1 << c) - 1;
}

// Returns offset index in the lookup table given the blocks binary content
//		classIndex		block class(rank) index in the table
//		block			binary block content
//		blockLength		block length (used to store data for blocks of different sizes in the table)
uint32_t oRRRTable::getOffset(uint32_t classIndex, uint32_t block, uint32_t blockLength) {
	uint32_t blockOffset = initialBlockLength - blockLength;
	class_t cls = table_[classIndex];
	uint32_t upper = (uint32_t)cls.size();
	uint32_t lower = 0;
	uint32_t middle = (upper + lower) / 2;

	block = (block << blockOffset);

	// Binary search
	while (true) {
		uint32_t value = cls[middle].first;
		if (value < block) {
			lower = middle;
		}
		else if (value > block) {
			upper = middle;
		}
		else {
			return middle;
		}
		middle = (lower + upper) / 2;
	}
}

// Returns number of possible block permutations with the given class (popcount)
uint32_t oRRRTable::getClassSize(uint32_t class_) {
	return (uint32_t)table_[class_].size();
}

// Returns number of bits needed to store the offset for the given block class(rank)
//		class_		block class(rank)
uint32_t oRRRTable::getBitsForOffset(uint64_t class_) {
	return this->bitsForOffset[(uint32_t)class_];
}

// Returns lookup table
table_t& oRRRTable::getTable() { 
	return table_; 
}

// Returns the rank in the block defined with class_ and offset_ up to the given position_
//		class_		block class(total rank)
//		offset_		offset for the appropriate block representation in the table
//		position_	index in the block representation
uint64_t oRRRTable::getRankForBlockAtPosition(uint64_t class_, uint64_t offset_, uint32_t position_) {
	return this->table_[(uint32_t)class_][(uint32_t)offset_].second[position_];
}

// Returns index of the first set bit for which the current block rank equals rank_
// in the given block defined with class_ and offset_
//		class_		block class(total rank)
//		offset_		offset for the appropriate block representation in the table
//		rank_		searched rank
uint64_t oRRRTable::getIndexForRank(uint64_t class_, uint64_t offset_, uint32_t rank_) {
	auto ranks = this->table_[(uint32_t)class_][(uint32_t)offset_].second;
	// Binary search to find the appropriate index
	return lower_bound(ranks.begin(), ranks.end(), rank_) - ranks.begin();
}

// Returns index of the first zero bit for which the current block rank equals rank_
// in the given block defined with class_ and offset_
//		class_		block class(total rank)
//		offset_		offset for the appropriate block representation in the table
//		rank_		searched rank
//		blockSize_	block size of the current RRR
uint64_t oRRRTable::getIndexForRankZero(uint64_t class_, uint64_t offset_, uint32_t rank_, uint32_t blockSize_) {
	auto ranks = this->table_[(uint32_t)class_][(uint32_t)offset_].second;
	vector<uint32_t>::iterator first = ranks.begin();
	vector<uint32_t>::iterator last = ranks.end();
	vector<uint32_t>::iterator it = ranks.begin();
	iterator_traits<vector<uint32_t>::iterator>::difference_type count, step;

	// Binary search to find the appropriate index
	// We use our implementation because the needed data structure is not stored explicitly in this object
	count = distance(first, last);
	while (count>0) {
		it = first; step = count / 2; advance(it, step);

		if ((it - ranks.begin()) - *it + 1 < rank_) {
			first = ++it;
			count -= step + 1;
		}
		else count = step;
	}
	return first - ranks.begin();
}
