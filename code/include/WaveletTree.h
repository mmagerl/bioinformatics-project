#ifndef WAVELET_TREE_H
#define WAVELET_TREE_H

#include <map>
#include <set>

#include "../include/RRR.h"

class WaveletTreeNode {
	private:
    	RRR* rrr;
	    map<char, int> sigma; // maps character to position in sorted array of characters

      int getNumCharactersInLeft();

    public:
	    WaveletTreeNode* parent;
	    WaveletTreeNode* left;
	    WaveletTreeNode* right;

	    WaveletTreeNode(WaveletTreeNode* _parent, string arr, map<char, int> _sigma, map<char, WaveletTreeNode*>& charToNode);
      bool hasChar(char c);
	    bool isRight(char c);
	    char getCharacterInSigma(int pos);
	    int rank0(int i);
	    int rank1(int i);
	    int select0(int i);
	    int select1(int i);
	    int access(int i);
};

class WaveletTree {
	private:
		WaveletTreeNode* root; // tree root
    	map<char, WaveletTreeNode*> charToNode; // maps character to tree leaf containing that character

    public:
    	WaveletTree(string arr);
    	int rank(int i, char c);
    	int select(int i, char c);
    	char access(int ind);
};

#endif
