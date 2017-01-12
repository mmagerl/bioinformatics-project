#include <map>
#include <set>

#include "../include/WaveletTree.h"

using namespace std;

WaveletTreeNode::WaveletTreeNode(WaveletTreeNode* _parent, string arr, map<char, int> _sigma, map<char, WaveletTreeNode*> charToNode): parent(_parent), sigma(_sigma) {
  if (sigma.size() <= 2) {
    left = NULL;
    right = NULL;
    for (auto entry : sigma) {
      charToNode[entry.first] = this;
    }
  } else {
    string lArr = "";
    string rArr = "";
    for (auto x : arr) {
      if (isRight(x)) {
        rArr += x;
      } else {
        lArr += x;
      }
    }
    map<char, int> lSigma;
    map<char, int> rSigma;
    for (auto entry : sigma) {
      if (isRight(entry.first)) {
        rSigma[entry.first] = entry.second-sigma.size()/2;
      } else {
        lSigma[entry.first] = entry.second;
      }
    }

    left = new WaveletTreeNode(this, lArr, lSigma, charToNode);
    right = new WaveletTreeNode(this, rArr, rSigma, charToNode);
  }
}

// returns whether char is in right child
bool WaveletTreeNode::isRight(char c) {
  return (sigma[c] >= sigma.size()/2);
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

int WaveletTreeNode::select(int i) {
  return rrr->select(i);
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
  // start with leaf containing only two characters, one of them is c
  WaveletTreeNode* cur = charToNode[c];

  int pos; // position in the current node
  if (cur->isRight(c)) {
    pos = cur->select(i); // TODO select1
  } else {
    pos = cur->select(i); // TODO select0
  }
  while (cur != root) {
    if (cur->parent->isRight(c)) {
      pos = cur->parent->select(pos+1); // TODO: select1
    } else {
      pos = cur->parent->select(pos+1); // TODO: select0
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
    return cur->getCharacterInSigma(cur->access(pos));
  }
}

