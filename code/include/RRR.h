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

    vector<uint32_t> superblockCumSum; // cumulative sum of ones to i-th superblock, inclusive, size ~ (n/(b*f))
    vector<uint8_t> blockClass; // block class, ie. number of ones in block, size ~ (n/b)
    vector<uint16_t> blockOffset; // block offset in class, size ~ (n/b)
    // cumulative sum of ones to i-th position in block indexed with (class,offset), 0-indexed
    // size ~ (b*2^b)
    vector<vector<vector<uint8_t> > > cumSumInBlock;

    void construct(const vector<bool>& bits, int _b, int _f);
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
    RRR(const vector<bool>& bits);
    RRR(const vector<bool>& bits, int _b, int _f);
		int rank0(int i);
		int rank1(int ind);
    int select0(int i);
		int select1(int i);
		int access(int ind);

};
#endif

