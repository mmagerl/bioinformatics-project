#ifndef RRR_H
#define RRR_H

#include <vector>
using namespace std;

class RRR {

	private:
		int b; // block size
    int f; // number of blocks in superblock
    int numBlocks; // number of blocks
    int numSuperblocks; // number of superblocks

    vector<int> superblockCumSum; // cumulative sum of ones to i-th superblock, inclusive, size ~ (n/(b*f))
    vector<int> blockClass; // block class, ie. number of ones in block, size ~ (n/b)
    vector<int> blockOffset; // block offset in class, size ~ (n/b)
    // cumulative sum of ones to i-th position in block indexed with (class,offset), 0-indexed
    // size ~ (b*2^b)
    vector<vector<vector<int> > > cumSumInBlock;

    int calcForType(int total, int ones, bool type);
    int findSuperblock(int i, bool type);
    int findIndexInBlock(int indBlock, int i, bool type);
    int getIndBlock(int ind);
    int getIndSuperblock(int indBlock);
    int getCumSumToSuperblock(int indSuperblock, bool type);
    int getZeroCumSumToSuperblock(int indSuperblock);
    bool isBlockStart(int ind);
    int nChoosek(int n, int k);
    int select(int i, bool type);

	public:
    RRR(std::vector<bool> bits);
		int rank0(int i);
		int rank1(int ind);
    int select0(int i);
		int select1(int i);
		int access(int ind);

};
#endif

