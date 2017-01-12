#ifndef WAVELET_TREE_H
#define WAVELET_TREE_H

#include <map>
#include <set>

#include "../include/RRR.h"

class WaveletTreeNode {
	private:
    	RRR* rrr;

    public:
	    WaveletTreeNode* parent;
	    WaveletTreeNode* left;
	    WaveletTreeNode* right;
	    map<char, int> sigma; // mapa sa znaka na poziciju u sortiranom nizu znakova

	    WaveletTreeNode(WaveletTreeNode* _parent, string arr, map<char, int> _sigma, map<char, WaveletTreeNode*> charToNode);
	    bool isRight(char c);
	    char getCharacterInSigma(int pos);
	    int rank0(int i);
	    int rank1(int i);
	    int select(int i);
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