#include "oRRR.h"

// Used for shifting bits
//		leftSide		left side of the shift operator (<< or >>)
//		rightSide		right side of the shift operator (<< or >>)
//		isLeft			true if you want to shift to the left (<<), false otherwise (>>)
uint64_t oRRR::bitShift(uint64_t leftSide, int32_t rightSide, bool isLeft) {
	if (rightSide < 0) {
		isLeft = !isLeft;
		rightSide *= -1;
	}

	if (isLeft) {
		return leftSide << rightSide;
	}
	
	return leftSide >> rightSide;
}

// Creates a new oRRR instance from the given bits
//		bits		string containing only zeros and ones
oRRR::oRRR(string &bits) {
	// Number of bits in input string
	uint64_t n = bits.size();
	this->inputVectorLength = n;
	if (n > 3) {
		// Bit number in each block
		this->blockSize = (uint32_t)(floor(log2(bits.size())) / 2);
	}
	else {
		this->blockSize = 1;
	}
	this->bitsForClass = (uint32_t)ceil(log2(this->blockSize + 1));
	this->bitsForClass = this->bitsForClass == 0 ? 1 : this->bitsForClass;

	// Bit number in super blocks
	this->superBlockSize = (uint32_t)(pow(floor(log2(n)), 2.));
	if (this->superBlockSize == 0) {
		this->superBlockSize = 1;
	}
	this->blocksPerSuperBlock = this->superBlockSize / this->blockSize;

	// Initialize table
	oRRRTable::getInstance();
	if (oRRRTable::initialBlockLength == 0) {
		oRRRTable::getInstance().createTable(this->blockSize);
	}

	// Auxiliary data structures
	uint64_t blockContent = 0;
	uint64_t currentBitInContent = ((uint64_t)1) << 63;
	uint64_t currentContentElement = 0;
	uint64_t alignValue = 0;

	uint64_t blockRank = 0;
	uint64_t superBlockRank = 0;

	superBlock superBlock(0, 0);
	uint64_t blockIndex = 0;
	uint64_t superBlockOffset = 0;

	this->maxIndexInLastContentElement = 0;

	for (uint64_t i = 0; i < n; i++) {
		// Read original block content (binary)
		blockContent = (blockContent << 1);

		if (bits[(unsigned int)i] == '1') {
			blockContent = (blockContent | 1);
			blockRank++;
		}

		if ((i != 0 && ((i + 1) % this->blockSize) == 0) || (i + 1 == this->blockSize) || (i + 1 == n)) {
			blockIndex++;
			if (i + 1 == n && ((i + 1) % this->blockSize)) {
				blockContent = this->bitShift(blockContent, ((int32_t)this->blockSize - ((i + 1) % this->blockSize)));
			}
			// blockSize used for bits alignment for table search
			// Calculate block rank and offset in the lookup table and store them in the binary oRRR content
			uint32_t blockOffset = oRRRTable::getInstance().getOffset((uint32_t)blockRank, (uint32_t)blockContent, this->blockSize);
			uint32_t bitsForOffset = oRRRTable::getInstance().getBitsForOffset(blockRank);

			superBlockOffset += (bitsForOffset + this->bitsForClass);

			uint64_t blockRankCopy = this->bitShift(blockRank, (64 - this->bitsForClass - alignValue));
			for (uint32_t k = 0; k < this->bitsForClass; k++) {
				currentContentElement = currentContentElement | (currentBitInContent & blockRankCopy);
				currentBitInContent = currentBitInContent >> 1;
				this->maxIndexInLastContentElement++;
				alignValue++;
				if (currentBitInContent == 0) {
					// I need to add another uint64_t into the content vector
					content.push_back(currentContentElement);
					currentContentElement = 0;
					currentBitInContent = ((uint64_t)1) << 63;
					alignValue = 0;
					int overFlowBits = this->bitsForClass - k - 1;
					if (overFlowBits > 0) {
						// We have overflow
						blockRankCopy = this->bitShift(blockRank, (64 - overFlowBits));
						currentContentElement = currentContentElement | (currentBitInContent & blockRankCopy);
						currentBitInContent = currentBitInContent >> 1;
						this->maxIndexInLastContentElement++;
						alignValue++;
						k++;
					}
				}
			}

			uint64_t blockOffsetCopy = this->bitShift(((uint64_t)blockOffset), 64 - (int)bitsForOffset - (int)alignValue);
			for (uint32_t k = 0; k < bitsForOffset; k++) {
				currentContentElement = currentContentElement | (currentBitInContent & blockOffsetCopy);
				currentBitInContent = currentBitInContent >> 1;
				this->maxIndexInLastContentElement++;
				alignValue++;
				if (currentBitInContent == 0 || ((i + 1) == n && k == bitsForOffset - 1)) {
					// I need to add another uint64_t into the content vector
					content.push_back(currentContentElement);
					currentContentElement = 0;
					currentBitInContent = ((uint64_t)1) << 63;
					alignValue = 0;
					int overflowExtraBits = bitsForOffset - k - 1;
					if (overflowExtraBits > 0) {
						// We have overflow
						blockOffsetCopy = this->bitShift(blockOffset, 64 - overflowExtraBits);
						currentContentElement = currentContentElement | (currentBitInContent & blockOffsetCopy);
						currentBitInContent = currentBitInContent >> 1;
						this->maxIndexInLastContentElement++;
						alignValue++;
						k++;
						// If we had overlow, and we are at the end of block, wee need to add that block to content
						if (((i + 1) == n && k == bitsForOffset - 1)) {
							content.push_back(currentContentElement);
						}
					}
				}
			}

			// Add the superblock to the superblock vector
			// First element is superblock rank
			// Second one is superblock offset
			superBlockRank += blockRank;
			if ((blockIndex % this->blocksPerSuperBlock == 0) || (i + 1 == n)) {
				superBlocks.push_back(superBlock);

				superBlock.second = superBlockOffset;
				superBlock.first = superBlockRank;
			}

			blockRank = 0;
			blockContent = 0;
		}
	}

	this->maxIndexInLastContentElement = (this->maxIndexInLastContentElement - 1) % 64 + 1;
}

// Returns size of the bits vector used when constructing the oRRR
uint64_t oRRR::getInputVectorLength() const {
	return this->inputVectorLength;
}

// Returns the number of times 1 appears in the interval [0, index] in the input bitvector
// Throws invalid_argument exception if index is too large
uint64_t oRRR::rank1(uint64_t index) {
	// Index checks
	if (index >= this->inputVectorLength) {
	}

	// Calculate block index
	uint64_t ib = index / this->blockSize;

	// Calculate which superblock the ib-th block resides in
	uint32_t is = (uint32_t) (ib / this->blocksPerSuperBlock);

	// Set rankSum to the rank contained in the current superblock
	uint64_t rankSum = this->superBlocks[is].first;

	// Used to know in which block we current are in order to add the current block rank to the rankSum
	uint64_t currentBlockIndex = this->blocksPerSuperBlock * is;
	uint64_t currentIndex = this->superBlocks[is].second;
	uint64_t contentIndex = currentIndex / 64;
	uint64_t currentIndexInContentElement = currentIndex % 64;
	uint64_t currentContentElement = this->content[(uint32_t)contentIndex];
	uint64_t mask = (((uint64_t)1) << this->bitsForClass) - 1;

	// Sum up block ranks up to the ib-th block
	while (currentBlockIndex <= ib) {
		uint64_t tempRank = this->bitShift(currentContentElement, (64 - this->bitsForClass - currentIndexInContentElement), false);
		uint64_t leftOverBits = currentIndexInContentElement + this->bitsForClass;
		if (leftOverBits >= 64) {
			// We have overflow
			contentIndex++;
			currentContentElement = this->content[(uint32_t)contentIndex];
			currentIndexInContentElement = leftOverBits % 64;
			if (currentIndexInContentElement > 0) {
				uint64_t tempMask = UINT64_MAX ^ (((uint64_t)1 << currentIndexInContentElement) - 1);
				tempRank = (tempRank & tempMask) | (currentContentElement >> (64 - currentIndexInContentElement));
			}
		}
		else {
			currentIndexInContentElement += this->bitsForClass;
		}
		tempRank = tempRank & mask;
		if (currentBlockIndex != ib) {
			rankSum += tempRank;
		}

		// Skip offset part of the block if we aren't in the ib-th one
		// In that case read the rank of the appropriate set bit in the 
		// lookup table and add it to the rankSum
		uint32_t bitsForOffset = oRRRTable::getInstance().getBitsForOffset(tempRank);
		if (currentBlockIndex != ib) {
			currentIndexInContentElement += bitsForOffset;
			if (currentIndexInContentElement >= 64) {
				// We have overflow
				contentIndex++;
				currentContentElement = this->content[(uint32_t)contentIndex];
				currentIndexInContentElement = currentIndexInContentElement % 64;
			}
		}
		else {
			uint64_t maskForOffset = (((uint64_t)1) << bitsForOffset) - 1;
			uint64_t tempOffset = this->bitShift(currentContentElement, (64 - bitsForOffset - currentIndexInContentElement), false);
			leftOverBits = currentIndexInContentElement + bitsForOffset;
			if (leftOverBits >= 64) {
				// We have overflow
				contentIndex++;
				currentContentElement = this->content[(uint32_t)contentIndex];
				currentIndexInContentElement = leftOverBits % 64;
				if (currentIndexInContentElement > 0) {
					uint64_t tempMask = UINT64_MAX ^ (((uint64_t)1 << currentIndexInContentElement) - 1);
					tempOffset = (tempOffset & tempMask) | (currentContentElement >> (64 - currentIndexInContentElement));
				}
			}

			tempOffset = tempOffset & maskForOffset;
			// We are in the ib-th block; read the appropriate rank from oRRRTable
			uint64_t j = index % ((uint64_t)this->blockSize);
			rankSum += oRRRTable::getInstance().getRankForBlockAtPosition(tempRank, tempOffset, (uint32_t)j);
		}
		currentBlockIndex++;
	}
	return rankSum;
}

// Returns the number of times 0 appears in the interval [0, index] in the input bitvector
// Throws invalid_argument exception if index is too large
uint64_t oRRR::rank0(uint64_t index) {
	return index + 1 - this->rank1(index);
}

// Returns the index of the count-th occurence of a set bit(1) in the input bitvector
// Throws invalid_argument exception if count value is too large or non-positive
uint64_t oRRR::select1(uint64_t count) {
	if (count > this->inputVectorLength) {
	}
	if(count == 0) {
	}

	// Find the appropriate superblock (the last one whose 1-rank is smaller or equal to count)
	superBlock countBlock(count, 36);
	vector<superBlock>::iterator supBlockIterator = 
		lower_bound(this->superBlocks.begin(), this->superBlocks.end(), countBlock, oRRR::compareSuperBlock);
	superBlock supBlock = *(--supBlockIterator);
	uint64_t superBlockIndex = supBlockIterator - this->superBlocks.begin();

	// Set rankSum to the rank contained in the current superblock
	uint64_t rankSum = supBlock.first;

	// Used to know in which block we currently are in order to add the current block rank to the rankSum
	uint64_t currentBlockIndex = 0;
	uint64_t currentIndex = supBlock.second;
	uint64_t contentIndex = currentIndex / 64;
	uint64_t currentIndexInContentElement = currentIndex % 64;
	uint64_t currentContentElement = this->content[(uint32_t)contentIndex];
	uint64_t mask = (((uint64_t)1) << this->bitsForClass) - 1;

	// Add the index of the first element in found superblock
	uint64_t indexOfith1 = this->blocksPerSuperBlock * this->blockSize * superBlockIndex;

	// Sum up block ranks up to the ib-th block
	// While doing that skip the offset part of each block
	// Use only class part of each block
	uint64_t lastContentIndex = this->content.size() - 1;
	while (rankSum < count && 
		(contentIndex < lastContentIndex || 
			(contentIndex == lastContentIndex && 
				currentIndexInContentElement < this->maxIndexInLastContentElement))) {
		uint64_t tempRank = this->bitShift(currentContentElement, (64 - this->bitsForClass - currentIndexInContentElement), false);
		uint64_t leftOverBits = currentIndexInContentElement + this->bitsForClass;
		if (leftOverBits >= 64) {
			// We have overflow
			contentIndex++;
			currentContentElement = this->content[(uint32_t)contentIndex];
			currentIndexInContentElement = leftOverBits % 64;
			if (currentIndexInContentElement > 0) {
				uint64_t tempMask = UINT64_MAX ^ (((uint64_t)1 << currentIndexInContentElement) - 1);
				tempRank = (tempRank & tempMask) | (currentContentElement >> (64 - currentIndexInContentElement));
			}
		}
		else {
			currentIndexInContentElement += this->bitsForClass;
		}
		tempRank = tempRank & mask;
		uint64_t tempRankSum = rankSum + tempRank;

		uint32_t bitsForOffset = oRRRTable::getInstance().getBitsForOffset(tempRank);

		if (tempRankSum < count) {
			rankSum = tempRankSum;
		}
		else {
			// The searched for block is found
			// Add the partial index of the appropriate set bit to the cumulative index sum and return it
			uint64_t maskForOffset = (((uint64_t)1) << bitsForOffset) - 1;
			uint64_t tempOffset = this->bitShift(currentContentElement, (64 - bitsForOffset - currentIndexInContentElement), false);
			leftOverBits = currentIndexInContentElement + bitsForOffset;
			if (leftOverBits >= 64) {
				// We have overflow
				contentIndex++;
				currentContentElement = this->content[(uint32_t)contentIndex];
				currentIndexInContentElement = leftOverBits % 64;
				if (currentIndexInContentElement > 0) {
					uint64_t tempMask = UINT64_MAX ^ (((uint64_t)1 << currentIndexInContentElement) - 1);
					tempOffset = (tempOffset & tempMask) | (currentContentElement >> (64 - currentIndexInContentElement));
				}
			}

			tempOffset = tempOffset & maskForOffset;

			indexOfith1 += oRRRTable::getInstance().getIndexForRank(tempRank, tempOffset, (uint32_t)(count-rankSum));
			break;
		}
		// Skip offset part of the block
		currentIndexInContentElement += bitsForOffset;
		if (currentIndexInContentElement >= 64) {
			// We have overflow
			contentIndex++;
			currentContentElement = this->content[(uint32_t)contentIndex];
			currentIndexInContentElement = currentIndexInContentElement % 64;
		}
		currentBlockIndex++;
		indexOfith1 += this->blockSize;
	}

	if (indexOfith1 >= this->inputVectorLength) {
	}
	return indexOfith1;
}

// Returns the index of the count-th occurence of an unset bit(0) in the input bitvector
uint64_t oRRR::select0(uint64_t count) {
	if (count > this->inputVectorLength) {
	}
	if (count == 0) {
	}

	// Find the appropriate superblock
	superBlock countBlock(count, 36);
	vector<superBlock>::iterator supBlockIterator = this->getIteratorForSelectZero(count);
	superBlock supBlock = *(--supBlockIterator);
	uint64_t superBlockIndex = supBlockIterator - this->superBlocks.begin();

	// Auxiliary data
	uint64_t currentBlockIndex = 0;
	uint64_t currentIndex = supBlock.second;
	uint64_t contentIndex = currentIndex / 64;
	uint64_t currentIndexInContentElement = currentIndex % 64;
	uint64_t currentContentElement = this->content[(uint32_t)contentIndex];
	uint64_t mask = (((uint64_t)1) << this->bitsForClass) - 1;

	// Add the index of the first element in found superblock
	uint64_t indexOfith0 = this->blocksPerSuperBlock * this->blockSize * superBlockIndex;
	// Set rankSum to the 0-rank contained in the current superblock
	uint64_t rankSum = indexOfith0 - supBlock.first;

	// Sum up block ranks in the current superblock until you find the last one whose rank is smaller or equal to count
	// While doing this skip the offset block parts and read only class part of each block
	uint64_t lastContentIndex = this->content.size() - 1;
	while (rankSum < count && 
		(contentIndex < lastContentIndex || 
			(contentIndex == lastContentIndex && 
				currentIndexInContentElement < this->maxIndexInLastContentElement))) {
		uint64_t tempRank = this->bitShift(currentContentElement, (64 - this->bitsForClass - currentIndexInContentElement), false);
		uint64_t leftOverBits = currentIndexInContentElement + this->bitsForClass;
		if (leftOverBits >= 64) {
			// We have overflow
			contentIndex++;
			currentContentElement = this->content[(uint32_t)contentIndex];
			currentIndexInContentElement = leftOverBits % 64;
			if (currentIndexInContentElement > 0) {
				uint64_t tempMask = UINT64_MAX ^ (((uint64_t)1 << currentIndexInContentElement) - 1);
				tempRank = (tempRank & tempMask) | (currentContentElement >> (64 - currentIndexInContentElement));
			}
		}
		else {
			currentIndexInContentElement += this->bitsForClass;
		}
		tempRank = tempRank & mask;
		uint64_t tempRankSum = rankSum + this->blockSize - tempRank;

		uint32_t bitsForOffset = oRRRTable::getInstance().getBitsForOffset(tempRank);

		if (tempRankSum < count) {
			rankSum = tempRankSum;
		}
		else {
			// We have found the block in which the searched bit is located
			uint64_t maskForOffset = (((uint64_t)1) << bitsForOffset) - 1;
			uint64_t tempOffset = this->bitShift(currentContentElement, (64 - bitsForOffset - currentIndexInContentElement), false);
			leftOverBits = currentIndexInContentElement + bitsForOffset;
			if (leftOverBits >= 64) {
				// We have overflow
				contentIndex++;
				currentContentElement = this->content[(uint32_t)contentIndex];
				currentIndexInContentElement = leftOverBits % 64;
				if (currentIndexInContentElement > 0) {
					uint64_t tempMask = UINT64_MAX ^ (((uint64_t)1 << currentIndexInContentElement) - 1);
					tempOffset = (tempOffset & tempMask) | (currentContentElement >> (64 - currentIndexInContentElement));
				}
			}

			tempOffset = tempOffset & maskForOffset;

			// Just add the partial index to the accumulated index and return it
			indexOfith0 += oRRRTable::getInstance().getIndexForRankZero(tempRank, tempOffset, (uint32_t)(count - rankSum), this->blockSize);
			break;
		}

		// Skip the offset part of the block
		currentIndexInContentElement += bitsForOffset;
		if (currentIndexInContentElement >= 64) {
			// We have overflow
			contentIndex++;
			currentContentElement = this->content[(uint32_t)contentIndex];
			currentIndexInContentElement = currentIndexInContentElement % 64;
		}
		currentBlockIndex++;
		indexOfith0 += this->blockSize;
	}

	if (indexOfith0 >= this->inputVectorLength) {
	}
	return indexOfith0;
}

// Returns the bit stored in the oRRR at the given index
// Throws invalid_argument exception if index is too large
uint8_t oRRR::access(uint64_t index) {
	if (index >= this->inputVectorLength) {
	}

	if (index == 0) {
		return (uint8_t)this->rank1(index);
	}

	// Get 1 or 0 in constant time by using rank
	return (uint8_t)(this->rank1(index) - this->rank1(index - 1));
}

// Compares two superblock by their rank
// Used for binary search when doing select1
bool oRRR::compareSuperBlock(superBlock a, superBlock b) {
	return a.first < b.first;
}

// Returns iterator that points to the last superblock whose 0-rank is smaller or equal to count
// Used for binary search when doing select0
vector<superBlock>::iterator oRRR::getIteratorForSelectZero(uint64_t count_) {
	uint64_t superBlockLength = this->blocksPerSuperBlock * this->blockSize;
	vector<superBlock>::iterator first = this->superBlocks.begin();
	++first;
	vector<superBlock>::iterator last = this->superBlocks.end();
	vector<superBlock>::iterator it = this->superBlocks.begin();
	iterator_traits<vector<uint32_t>::iterator>::difference_type count, step;

	// Binary search
	count = distance(first, last);
	while (count > 0) {
		it = first; step = count / 2; advance(it, step);

		if (((it - this->superBlocks.begin()) * superBlockLength - (*it).first) + 1 < count_) {
			first = ++it;
			count -= step + 1;
		}
		else count = step;
	}
	return first;
}
