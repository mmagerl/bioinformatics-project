#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>

#include "../include/WaveletTree.h"
#include "../other/oWaveletTree.h"
#include "../include/Brute.h"

using namespace std;

const int M = 1000000;
const int N_QUERY = 1000000;
const int N_BRUTE_QUERY = 100;

vector<int> positions;
vector<int> counts;
vector<char> chars;

WaveletTree* vt;
oWaveletTree* ovt;
Brute* brute;

clock_t sTime;
clock_t eTime;

void prepare(string& input) {
  vector<char> lChars;
  map<char, int> charToCount;

  int n = input.size();
  for (auto c : input) {
    ++charToCount[c];
  }
  for (auto& kv : charToCount) {
    lChars.push_back(kv.first);
  }

  // generate random queries
  random_device rd;
  default_random_engine eng {rd()};
  uniform_int_distribution<> dist(0, n);

  for (int i = 0; i < N_QUERY; ++i) {
    positions.push_back(dist(eng)%n);
    auto c = lChars[dist(eng)%lChars.size()];
    chars.push_back(c);
    counts.push_back(1+rand()%charToCount[c]);
  }

  printf("Dataset info\n");
  printf("n = %d\n", n);
  printf("m = %d\n", (int)lChars.size());
  printf("\n");
}

void testConstruction(string& input) {
  printf("Construction (s)\n");
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

void testRank(bool testEquality = false) {
  printf("Rank (us)\n");
  int rank;

  if (testEquality) {
    // test equality
    auto equal = true;
    for (int i = 0; i < N_QUERY; ++i) {
      auto rank1 = vt->rank(positions[i], chars[i]);
      auto rank2 = ovt->rank(chars[i], positions[i]);
      equal &= (rank1 == rank2);

      if (rank1 != rank2) {
        auto rank3 = brute->rank(positions[i], chars[i]);
        printf("NOT EQUAL (i = %d): %d, %lu, %d\n", i, rank1, rank2, rank3);
        return;
      }
    }
    printf("equal = %s\n", equal ? "true" : "false");
  }

  // our solution
  sTime = clock();
  for (int i = 0; i < N_QUERY; i++){
    rank = vt->rank(positions[i], chars[i]);
  }
  eTime = clock();
  cout << "mWT = " << double(eTime - sTime) / CLOCKS_PER_SEC * M / N_QUERY << endl;

  // other solution
  sTime = clock();
  for (int i = 0; i < N_QUERY; i++){
    rank = ovt->rank(chars[i], positions[i]);
  }
  eTime = clock();
  cout << "oWT = " << double(eTime - sTime) / CLOCKS_PER_SEC * M / N_QUERY << endl;

  // brute solution
  sTime = clock();
  for (int i = 0; i < N_BRUTE_QUERY; i++){
    rank = brute->rank(positions[i], chars[i]);
  }
  eTime = clock();
  cout << "BRT = " << double(eTime - sTime) / CLOCKS_PER_SEC * M / N_BRUTE_QUERY << endl;

  cout << endl;
}

void testSelect(bool testEquality = false) {
  printf("Select (us)\n");
  int select;

  // test equality
  if (testEquality) {
    auto equal = true;
    for (int i = 0; i < N_QUERY; ++i) {
      auto select1 = vt->select(counts[i], chars[i]);
      auto select2 = ovt->select(chars[i], counts[i]);
      equal &= (select1 == select2);

      if (select1 != select2) {
        auto select3 = brute->select(counts[i], chars[i]);
        printf("NOT EQUAL (i = %d): %d, %lu, %d\n", i, select1, select2, select3);
        return;
      }
    }
    printf("equal = %s\n", equal ? "true" : "false");
  }

  // our solution
  sTime = clock();
  for (int i = 0; i < N_QUERY; ++i) {
    select = vt->select(counts[i], chars[i]);
  }
  eTime = clock();
  cout << "mWT = " << double(eTime - sTime) / CLOCKS_PER_SEC * M / N_QUERY << endl;

  // other solution
  sTime = clock();
  for (int i = 0; i < N_QUERY; ++i) {
    select = ovt->select(chars[i], counts[i]);
  }
  eTime = clock();
  cout << "oWT = " << double(eTime - sTime) / CLOCKS_PER_SEC * M / N_QUERY << endl;

  // brute solution
  sTime = clock();
  for (int i = 0; i < N_BRUTE_QUERY; i++){
    select = brute->select(counts[i], chars[i]);
  }
  eTime = clock();
  cout << "BRT = " << double(eTime - sTime) / CLOCKS_PER_SEC * M / N_BRUTE_QUERY << endl;

  cout << endl;
}

void testAccess(bool testEquality = false) {
  printf("Access (us)\n");
  char access;

  // test equality
  if (testEquality) {
    auto equal = true;
    for (int i = 0; i < N_QUERY; ++i) {
      auto access1 = vt->access(positions[i]);
      auto access2 = ovt->access(positions[i]);
      equal &= (access1 == access2);

      if (access1 != access2) {
        auto access3 = brute->access(positions[i]);
        printf("NOT EQUAL (i = %d, pos = %d): %c, %c, %c\n", i, positions[i], access1, access2, access3);
        return;
      }
    }
    printf("equal = %s\n", equal ? "true" : "false");
  }

  // our solution
  sTime = clock();
  for (auto pos : positions) {
    access = vt->access(pos);
  }
  eTime = clock();
  cout << "mWT = " << double(eTime - sTime) / CLOCKS_PER_SEC * M / N_QUERY << endl;

  // other solution
  sTime = clock();
  for (auto pos : positions) {
    access = ovt->access(pos);
  }
  eTime = clock();
  cout << "oWT = " << double(eTime - sTime) / CLOCKS_PER_SEC * M / N_QUERY << endl;

  // brute solution
  sTime = clock();
  for (int i = 0; i < N_BRUTE_QUERY; i++){
    access = brute->access(positions[i]);
  }
  eTime = clock();
  cout << "BRT = " << double(eTime - sTime) / CLOCKS_PER_SEC * M / N_BRUTE_QUERY << endl;

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
  testRank(true);
  testSelect(true);
  testAccess(true);

  return 0;
}
