#ifndef BRUTE_H
#define BRUTE_H

#include<string>

class Brute {
  private:
    std::string arr;
  public:
    Brute(std::string arr);
    int rank(int i, char c);
    int select(int i, char c);
    char access(int ind);
};

#endif
