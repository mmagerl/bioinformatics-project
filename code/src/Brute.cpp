#include "../include/Brute.h"
#include<iostream>

using namespace std;

Brute::Brute(string _arr): arr(_arr) {
  cout << arr << endl;
}
  
int Brute::rank(int i, char c){
  int counter = 0;
  int pos = 0;
  for (auto ch : arr){
    if (ch == c){
      counter++;
    }
    if (pos == i){
      break;
    }
    pos++;
  }
  return counter;
}

int Brute::select(int i, char c){
  int counter = 0;
  int pos = 0;
  for (auto ch : arr){
    if (ch == c){
      counter++;
    }
    if (counter == i){
      return pos;
    }
    pos++;
  }
  return -1;
}

char Brute::access(int ind){
  return arr[ind];
}


