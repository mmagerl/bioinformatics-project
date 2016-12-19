class WaveletTree {
  private:
    WaveletTreeNode* root; // korijen stabla
    map<char, WaveletTreeNode*> charToNode; // mapa sa znaka na cvor s tim znakom

  public:
    WaveletTree(string arr): root(NULL) {}

    // koliko puta se znak c pojavljuje do mjesta i
    int rank(int i, char c) {
      WaveletTreeNode* cur = root;
      int pos = i; // pozicija u trenutnom cvoru
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
      // u pos je pozicija u zadnjem cvoru gdje su samo znakovi c, pa je moramo uvecati za 1
      return pos+1;
    }

    // pozicija i-tog pojavljivanja znaka c
    int select(int i, char c) {
      WaveletTreeNode* cur = charToNode[c]; // list u kojem su najvise 2 znaka, jedan od njih c

      int pos; // pozicija u trenutnom cvoru
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

    // znak na pociziji i
    char access(int i) {
      WaveletTreeNode* cur = root;
      int pos = i;
      while (cur->left != NULL) {
        if (cur->access(pos)) { // ako je 1 idemo desno
          pos = cur->rank1(pos)-1;
          cur = cur->right;
        } else { // inace lijevo
          pos = cur->rank0(pos)-1;
          cur = cur->left;
        }
        return cur->sigma[cur->access(pos)];
      }
    }
}

class WaveletTreeNode {
  private:
    RRR* rrr;
  public:
    WaveletTreeNode* parent;
    WaveletTreeNode* left;
    WaveletTreeNode* right;

    map<char, int> sigma; // mapa sa znaka na poziciju u sortiranom nizu znakova

    // vraca je li znak c u lijevom ili desnom djetetu
    bool isRight(char c) {
      return (sigma[c] >= sigma.size()/2);
    }

    int rank(int i)  {
      return rrr->rank(i);
    }

    int select(int i) {
      return rrr->select(i);
    }

    int access(int i) {
      return rrr->access(i);
    }
}

int main(void) {
  return 0;
}
