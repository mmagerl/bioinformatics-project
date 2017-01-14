#include <fstream>
#include <string>
#include <iostream>
#include <ctime>

#include "../include/WaveletTree.h"
#include "../include/Brute.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Expects one argument: <input-file-name>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  string input;
  getline(file, input);

  clock_t begin = clock();
  WaveletTree vt(input);
  clock_t end = clock();
  cout << "WT - constructor (s): " << double(end - begin) / CLOCKS_PER_SEC << endl;

  int rank;
  begin = clock();
  for (int i = 0; i < 1000000; i++){
    rank = vt.rank(0, 'A');
  }
  end = clock();
  cout << "WT - rank (us): " << double(end - begin) / CLOCKS_PER_SEC << endl;

  Brute brute(input);
  begin = clock();
  for (int i = 0; i < 1000; i++){
    rank = brute.rank(0, 'A');
  }
  end = clock();
  cout << "BR - rank (us): " << double(end - begin) / CLOCKS_PER_SEC * 1000 << endl;

  return 0;
}
