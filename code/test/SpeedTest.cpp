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

vector<int> positions;
vector<char> chars;

WaveletTree* vt;
oWaveletTree* ovt;
Brute* brute;

clock_t sTime;
clock_t eTime;

void prepare(string& input) {
  set<char> sChars;
  vector<char> lChars;

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

  for (int i = 0; i < N_QUERY; ++i) {
    positions.push_back(dist(eng)%n);
    chars.push_back(lChars[dist(eng)%lChars.size()]);
  }
}

void testConstruction(string& input) {
  printf("construction (s)\n");
  // our solution
  sTime = clock();
  vt = new WaveletTree(input);
  eTime = clock();
  cout << "mWT = " << double(eTime - sTime) / CLOCKS_PER_SEC << endl;

  // other solution
  sTime = clock();
  ovt = new oWaveletTree(input);
  eTime = clock();
  cout << "oWT = " << double(eTime - sTime) / CLOCKS_PER_SEC << endl;

  // brute solution
  brute = new Brute(input);

  cout << endl;
}

void testRank() {
  printf("rank (us)\n");
  int rank;
  // our solution
  sTime = clock();
  for (int i = 0; i < N_QUERY; i++){
    rank = vt->rank(positions[i], chars[i]);
  }
  eTime = clock();
  cout << "mWT = " << double(eTime - sTime) / CLOCKS_PER_SEC * 1000000 / N_QUERY << endl;

  // other solution
  sTime = clock();
  for (int i = 0; i < N_QUERY; i++){
    rank = ovt->rank(positions[i], chars[i]);
  }
  eTime = clock();
  cout << "oWT = " << double(eTime - sTime) / CLOCKS_PER_SEC * 1000000 / N_QUERY << endl;

  // brute solution
  sTime = clock();
  for (int i = 0; i < 1000; i++){
    rank = brute->rank(positions[i], chars[i]);
  }
  eTime = clock();
  cout << "BRT = " << double(eTime - sTime) / CLOCKS_PER_SEC * 1000 << endl;

  cout << endl;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Expects one argument: <input-file-name>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  string input;
  getline(file, input);

  prepare(input);
  testConstruction(input);
  testRank();

  return 0;
}
