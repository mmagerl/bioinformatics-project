#include <fstream>
#include <string>
#include <iostream>
#include <ctime>

#include "../include/WaveletTree.h"
#include "../include/Brute.h"

using namespace std;

int main(){
  ifstream file("../datasets/output.txt");
  string input;
  getline(file, input);


  clock_t begin = clock();
  WaveletTree vt(input);
  clock_t end = clock();
  cout << "konstruktor: " << double(end - begin) / CLOCKS_PER_SEC << endl;

  int rank;
  begin = clock();
  for (int i = 0; i < 1000000; i++){
    rank = vt.rank(50000, 'F');
  }
  end = clock();
  cout << "WT: 1000000 rank operacija: " << double(end - begin) / CLOCKS_PER_SEC << endl;

  Brute brute(input);
  begin = clock();
  for (int i = 0; i < 1000; i++){
    rank = brute.rank(50000, 'F');
  }
  end = clock();
  cout << "Brute: 1000 rank operacija: " << double(end - begin) / CLOCKS_PER_SEC << endl;

  return 0;
}
