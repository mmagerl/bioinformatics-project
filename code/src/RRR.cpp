#include <vector>
#include <cstdio>

#include "../include/RRR.h"

using namespace std;

// finds index of superblock containing i-th one
int RRR::findSuperblockByOne(int i) {
  int lo = 0;
  int hi = numSuperblocks;
  while(lo < hi) {
    int mid = (lo+hi)/2;
    if (superblockCumSum[mid] < i) {
      lo = mid+1;
    } else {
      hi = mid;
    }
  }
  return lo;
}

// finds index of superblock containing i-th zero
int RRR::findSuperblockByZero(int i){
  int lo = 0;
  int hi = numSuperblocks;
  while(lo < hi){
    int mid = (lo+hi)/2;
    if (b*f*(mid+1) - superblockCumSum[mid] < i) {
      lo = mid+1;
    } else{
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

int RRR::getCumSumToSuperblock(int indSuperblock) {
  return (indSuperblock < 0) ? 0 : superblockCumSum[indSuperblock];
}

int RRR::getZeroCumSumToSuperblock(int indSuperblock){
  return (indSuperblock < 0) ? 0 : b*f - superblockCumSum[indSuperblock];
}

bool RRR::isBlockStart(int ind) {
  return getIndBlock(ind) != getIndBlock(ind-1);
}

// TODO: check this
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

// time-complexity - O(b*2^b) for preprocessing
RRR::RRR(vector<bool> bits, int _b, int _f): b(_b), f(_f) {
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
    cumSumInBlock[classType].resize(nChoosek(b, classType)); // TODO: check what multiple resizing does
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
    blockVal = (blockVal*2) + bits[i]; // NB: currently limits b to 32
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
  int counter = getCumSumToSuperblock(indSuperblock-1);

  // add sum of ones in all blocks of current superblock up to previous block
  for (int i = indSuperblock*f; i < indBlock; ++i) {
    counter += blockClass[i];
  }
  // add number of ones in current block up to i-th position
  counter += cumSumInBlock[blockClass[indBlock]][blockOffset[indBlock]][ind-indBlock*b];
  return counter;
}

// index of the i-th one (0-indexed)
// time-complexity - O(log(n/(b*f))+f+log(b))
// TODO: be consistent with binary search, both implemented in separate methods or both
// in this method
// TODO: implement select0()
int RRR::select(int i) {
  int indSuperblock = findSuperblockByOne(i); // index of superblock containing i-th one

  // cumulative sum of ones up to previous superblock
  int counter = getCumSumToSuperblock(indSuperblock-1);

  int indBlock = indSuperblock*f; // current block index
  // loop through blocks until the sum of i is reached
  while (counter < i) {
    counter += blockClass[indBlock];
    ++indBlock;
  }

  // return to previous block
  --indBlock;
  counter -= blockClass[indBlock];

  // binary search in block
  int lo = 0;
  int hi = b;
  while (lo < hi) {
    int mid = (lo+hi)/2;
    if (cumSumInBlock[blockClass[indBlock]][blockOffset[indBlock]][mid]+counter < i) {
      lo = mid+1;
    } else {
      hi = mid;
    }
  }

  return indBlock*b + lo;
}

int RRR::select0(int i) {
  int indSuperblock = findSuperblockByZero(i); // index of superblock containing i-th one

  // cumulative sum of zeros up to previous superblock
  int counter = getZeroCumSumToSuperblock(indSuperblock-1);

  int indBlock = indSuperblock*f; // current block index
  // loop through blocks until zero count of i is reached
  while (counter < i) {
    counter += b - blockClass[indBlock];
    ++indBlock;
  }

  // return to previous block
  --indBlock;
  counter -= b - blockClass[indBlock];

  // binary search in block
  int lo = 0;
  int hi = b;
  while (lo < hi) {
    int mid = (lo+hi)/2;
    if (mid + 1 - cumSumInBlock[blockClass[indBlock]][blockOffset[indBlock]][mid]+counter < i) {
      lo = mid+1;
    } else {
      hi = mid;
    }
  }

  return indBlock*b + lo;
}

// bit on given index
// time-complexity - same as rank1()
int RRR::access(int ind) {
  return (ind > 0) ? rank1(ind)-rank1(ind-1) : rank1(ind);
}
