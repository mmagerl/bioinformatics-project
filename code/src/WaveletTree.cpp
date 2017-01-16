#include <map>
#include <set>
#include <vector>
#include <cstdio>

#include "../include/WaveletTree.h"

using namespace std;

WaveletTreeNode::WaveletTreeNode(WaveletTreeNode* _parent, string& arr, map<char, int> _sigma, map<char, WaveletTreeNode*>& charToNode): parent(_parent), sigma(_sigma) {
  string lArr = "";
  string rArr = "";
  vector<bool> bits;
  for (auto x : arr) {
    auto isRightChild = isRight(x);
    bits.push_back(isRightChild);
    if (isRightChild) {
      rArr += x;
    } else {
      lArr += x;
    }
  }
  rrr = new RRR(bits);
  left = right = NULL;

  if (sigma.size() <= 2) {
    for (auto entry : sigma) {
      charToNode[entry.first] = this;
    }
  } else {
    map<char, int> lSigma;
    map<char, int> rSigma;
    for (auto entry : sigma) {
      if (isRight(entry.first)) {
        rSigma[entry.first] = entry.second-getNumCharactersInLeft();
      } else {
        lSigma[entry.first] = entry.second;
      }
    }

    left = new WaveletTreeNode(this, lArr, lSigma, charToNode);
    if (rSigma.size() > 0) {
      right = new WaveletTreeNode(this, rArr, rSigma, charToNode);
    }
  }
}

bool WaveletTreeNode::hasChar(char c) {
  return sigma.count(c);
}

// returns number of characters in left child
int WaveletTreeNode::getNumCharactersInLeft() {
  return (sigma.size()+1)/2;
}

// returns whether char is in right child
bool WaveletTreeNode::isRight(char c) {
  return sigma[c] >= getNumCharactersInLeft(); 
}

char WaveletTreeNode::getCharacterInSigma(int pos) {
    for (auto entry : sigma) {
      if (entry.second == pos) {
        return entry.first;
      }
    }
}

int WaveletTreeNode::rank0(int i)  {
  return rrr->rank0(i);
}

int WaveletTreeNode::rank1(int i)  {
  return rrr->rank1(i);
}

int WaveletTreeNode::select1(int i) {
  return rrr->select1(i);
}

int WaveletTreeNode::select0(int i) {
  return rrr->select0(i);
}

int WaveletTreeNode::access(int i) {
  return rrr->access(i);
}


WaveletTree::WaveletTree(string arr) {
  map<char, int> sigma;
  set<char> chars;
  for (auto x : arr) {
    chars.insert(x);
  }
  int pos = 0;
  for (auto x : chars) {
    sigma[x] = pos;
    ++pos;
  }

  root = new WaveletTreeNode(NULL, arr, sigma, charToNode);
}

// number of occurrences of char c to position i (0-indexed), inclusive
int WaveletTree::rank(int i, char c) {
  WaveletTreeNode* cur = root;

  // if char does not exist return 0
  if (!cur->hasChar(c)) {
    return 0;
  }

  int pos = i; // position in the current node
  while (cur != NULL) {
    if (cur->isRight(c)) {
      pos = cur->rank1(pos)-1;
      cur = cur->right;
    } else {
      pos = cur->rank0(pos)-1;
      cur = cur->left;
    }
    if (pos == -1) {
      break;
    }
  }
  // pos contains position in the last node containing only characters c
  // we need to add 1 to get number of occurrences from position because it is 0-indexed
  return pos+1;
}

// index of the i-th character c
int WaveletTree::select(int i, char c) {
  // if char does not exist, return -1
  if (!charToNode.count(c)) {
    return -1;
  }

  // start with leaf containing only two characters, one of them is c
  WaveletTreeNode* cur = charToNode[c];

  int pos; // position in the current node
  if (cur->isRight(c)) {
    pos = cur->select1(i);
  } else {
    pos = cur->select0(i);
  }
  while (cur != root) {
    if (cur->parent->isRight(c)) {
      pos = cur->parent->select1(pos+1);
    } else {
      pos = cur->parent->select0(pos+1);
    }
    cur = cur->parent;
  }
  return pos;
}

// char on given index
char WaveletTree::access(int ind) {
  WaveletTreeNode* cur = root;
  int pos = ind;
  while (cur->left != NULL) {
    if (cur->access(pos)) { // if 1 go right
      pos = cur->rank1(pos)-1;
      cur = cur->right;
    } else { // else go left
      pos = cur->rank0(pos)-1;
      cur = cur->left;
    }
  }
  return cur->getCharacterInSigma(cur->access(pos));
}

