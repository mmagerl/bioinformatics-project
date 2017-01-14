#include <fstream>
#include <string>
#include <iostream>
#include <ctime>
#include <random>
#include <set>

#include "../include/WaveletTree.h"
#include "../other/oWaveletTree.h"
#include "../include/Brute.h"

using namespace std;

const int N_QUERY = 1000000;

set<char> sChars;
vector<char> lChars;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Expects one argument: <input-file-name>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  string input;
  getline(file, input);

  int n = input.size();
  for (auto c : input) {
    sChars.insert(c);
  }
  for (auto c : sChars) {
    lChars.push_back(c);
  }

  // generate random queries
  random_device rd;
  default_random_engine eng {rd()};
  uniform_int_distribution<> dist(0, n);

  vector<int> positions;
  vector<char> chars;
  for (int i = 0; i < N_QUERY; ++i) {
    positions.push_back(dist(eng)%n);
    chars.push_back(lChars[dist(eng)%lChars.size()]);
  }

  clock_t begin, end;

  // test tree construction
  // our solution
  begin = clock();
  WaveletTree vt(input);
  end = clock();
  cout << "WT - constructor (s): " << double(end - begin) / CLOCKS_PER_SEC << endl;

  // other solution
  begin = clock();
  oWaveletTree ovt(input);
  end = clock();
  cout << "oWT - constructor (s): " << double(end - begin) / CLOCKS_PER_SEC << endl;

  // test rank
  int rank;
  // our solution
  begin = clock();
  for (int i = 0; i < N_QUERY; i++){
    rank = vt.rank(positions[i], chars[i]);
  }
  end = clock();
  cout << "WT - rank (us): " << double(end - begin) / CLOCKS_PER_SEC * 1000000 / N_QUERY << endl;

  // other solution
  begin = clock();
  for (int i = 0; i < N_QUERY; i++){
    rank = ovt.rank(positions[i], chars[i]);
  }
  end = clock();
  cout << "oWT - rank (us): " << double(end - begin) / CLOCKS_PER_SEC * 1000000 / N_QUERY << endl;

  // brute force solution
  Brute brute(input);
  begin = clock();
  for (int i = 0; i < 1000; i++){
    rank = brute.rank(positions[i], chars[i]);
  }
  end = clock();
  cout << "BR - rank (us): " << double(end - begin) / CLOCKS_PER_SEC * 1000 << endl;

  return 0;
}
