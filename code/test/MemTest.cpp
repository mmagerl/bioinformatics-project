#include <fstream>
#include <string>
#include <iostream>
#include <ctime>
#include <cstring>
#include <string>

#include "../other/oWaveletTree.h"
#include "../include/WaveletTree.h"
#include "../include/Brute.h"

using namespace std;

// Used for parsing proces memory files in bio linux
int parseLine(char* line) {
  int i = strlen(line);
  while (*line < '0' || *line > '9') line++;
  line[i-3] = '\0';
  i = atoi(line);
  return i;
}

// Print memory usage
double getMemoryUsage(double startMemory) {
  double currentMemory = 0;
  #ifdef __linux__ 
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1; 
    char line[128];
  

    while (fgets(line, 128, file) != NULL){
      if (strncmp(line, "VmRSS:", 6) == 0){ 
        result = parseLine(line);
        break;
      }   
    }   
    fclose(file);
    currentMemory = result / 1024.;
  #else
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
    currentMemory = physMemUsedByMe / (1024. * 1024);
  #endif
  return currentMemory;
}

string generateFilename(int m, int s){
  string dir = "../datasets/";
  return dir + "in-" + to_string(m) + "-" + to_string(s) + ".txt";
}

int main(int argc, char *argv[]) {
  double begin, end;
  string input;

  ifstream file(argv[1]);
  getline(file, input);

  if (strcmp(argv[2], "m") == 0) {
    begin = getMemoryUsage(0);
    WaveletTree vt(input);
    end = getMemoryUsage(begin);
    cout << "mWT = " << end - begin << "MB" << endl;
  } else {
    begin = getMemoryUsage(0);
    oWaveletTree ovt(input);
    end = getMemoryUsage(begin);
    cout << "oWT = " << end - begin << "MB" << endl;
  }

  return 0;
}
