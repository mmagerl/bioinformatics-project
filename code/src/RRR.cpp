#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>

#include "../include/RRR.h"

using namespace std;

// calculates number of ones or zeros depending on type
int RRR::calcForType(int total, int ones, bool type) {
  return type ? ones : total-ones;
}

// finds index of superblock containing i-th one/zero (type)
int RRR::findSuperblock(int i, bool type) {
  int lo = 0;
  int hi = numSuperblocks;
  while(lo < hi) {
    int mid = (lo+hi)/2;
    int cumsum = calcForType(b*f*(mid+1), superblockCumSum[mid], type);
    if (cumsum < i) {
      lo = mid+1;
    } else {
      hi = mid;
    }
  }
  return lo;
}

// finds index in indBlock containing i-th one/zero (x)
int RRR::findIndexInBlock(int indBlock, int i, bool type) {
  int lo = 0;
  int hi = b;
  while (lo < hi) {
    int mid = (lo+hi)/2;
    int cumsum = \
      calcForType(mid+1, cumSumInBlock[blockClass[indBlock]][blockOffset[indBlock]][mid], type);
    if (cumsum < i) {
      lo = mid+1;
    } else {
      hi = mid;
    }
  }
  return lo;
}

int RRR::getIndBlock(int ind) {
  return ind/b;
}

int RRR::getIndSuperblock(int indBlock) {
  return indBlock/(b*f);
}

int RRR::getCumSumToSuperblock(int indSuperblock, bool type) {
  return (indSuperblock < 0) ? 0 : calcForType(b*f, superblockCumSum[indSuperblock], type);
}

bool RRR::isBlockStart(int ind) {
  return getIndBlock(ind) != getIndBlock(ind-1);
}

int RRR::nChoosek(int n, int k) {
  if (k > n) return 0;
  if (k*2 > n) k = n-k;
  if (k == 0) return 1;

  int res = n;
  for (int i = 2; i <= k; ++i) {
    res *= (n-i+1);
    res /= i;
  }
  return res;
}

int RRR::select(int i, bool type) {
  // index of superblock containing i-th one/zero
  int indSuperblock = findSuperblock(i, type);

  // cumulative sum of ones/zeros up to previous superblock
  int counter = getCumSumToSuperblock(indSuperblock-1, type);

  int indBlock = indSuperblock*f; // current block index
  // loop through blocks until the sum of i is reached
  while (counter < i) {
    counter += calcForType(b, blockClass[indBlock], type);
    ++indBlock;
  }

  // return to previous block
  --indBlock;
  counter -= calcForType(b, blockClass[indBlock], type);

  // index in indBlock containing (i-counter)-th one
  int indInBlock = findIndexInBlock(indBlock, i-counter, type);

  return indBlock*b + indInBlock;
}

// time-complexity - O(b*2^b) for preprocessing
RRR::RRR(vector<bool> bits) {
  b = max((int)(log2(bits.size()) / 2), 1);
  f = max((int)pow(log2(bits.size()), 2), 1);

  // preprocessing - O(b*2^b) time and memory
  vector<int> blockValToOffset;
  blockValToOffset.resize(1<<b);
  vector<int> indInClass; // index in class
  indInClass.resize(b+1, 0); // b+1 different classes in total (0 ones, .., b ones)
  cumSumInBlock.resize(b+1); // b+1 different classes in total
  for (int i = 0; i < (1<<b); ++i) {
    int classType = __builtin_popcount(i);
    blockValToOffset[i] = indInClass[classType];
    ++indInClass[classType];

    // calculate cumSumInBlock
    cumSumInBlock[classType].resize(nChoosek(b, classType));
    cumSumInBlock[classType][blockValToOffset[i]].resize(b);
    int cumsum = 0;
    for (int j = 0; j < b; ++j) {
      cumsum += !!(i&(1<<(b-j-1)));
      cumSumInBlock[classType][blockValToOffset[i]][j] = cumsum;
    }
  }
  // preprocessing finished

  int n = bits.size();
  numBlocks = (n-1)/b+1;
  numSuperblocks = (n-1)/(b*f)+1;
  blockClass.resize(numBlocks, 0);
  blockOffset.resize(numBlocks);
  superblockCumSum.resize(numSuperblocks);

  int cumsum = 0;
  int blockVal = 0;
  for (int i = 0; i < numBlocks*b; ++i) {
    if (isBlockStart(i)) {
      blockVal = 0; // when new block starts reset block value to zero
    }
    // update block class and offset
    int indBlock = getIndBlock(i);
    blockVal = (blockVal*2) + bits[i]; // TODO: don't limit b to 32
    blockOffset[indBlock] = blockValToOffset[blockVal];
    blockClass[indBlock] += bits[i];

    // update superblockCumSum
    cumsum += bits[i];
    superblockCumSum[getIndSuperblock(i)] = cumsum;
  }
}

// number of zeros to position i, inclusive
// time-complexity - same as rank1()
int RRR::rank0(int i) {
  // calculate using number of ones
  return i+1-rank1(i);
}

// number of ones to position i, inclusive
// time-complexity - O(f)
// TODO: convert to O(1), hint: remove looping through each block in superblock and
// use cumulative sum of blocks in superblock instead
int RRR::rank1(int ind) {
  int indBlock = getIndBlock(ind); // block index
  int indSuperblock = getIndSuperblock(ind); // superblock index

  // cumulative sum of ones up to previous superblock
  int counter = getCumSumToSuperblock(indSuperblock-1, true);

  // add sum of ones in all blocks of current superblock up to previous block
  for (int i = indSuperblock*f; i < indBlock; ++i) {
    counter += blockClass[i];
  }
  // add number of ones in current block up to i-th position
  counter += cumSumInBlock[blockClass[indBlock]][blockOffset[indBlock]][ind-indBlock*b];
  return counter;
}

// index of the i-th zero (0-indexed)
// time-complexity - same as select1()
int RRR::select0(int i) {
  return select(i, false);
}

// index of the i-th one (0-indexed)
// time-complexity - O(log(n/(b*f))+f+log(b))
int RRR::select1(int i) {
  return select(i, true);
}

// bit on given index
// time-complexity - same as rank1()
int RRR::access(int ind) {
  return (ind > 0) ? rank1(ind)-rank1(ind-1) : rank1(ind);
}
