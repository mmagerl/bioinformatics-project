#include <fstream>
#include <string>
#include <iostream>
#include <ctime>
#include <cstring>
#include <string>

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
  vector<int> mem{2, 3, 4, 5, 6, 7};
  vector<int> sigma{4, 16, 28};

  double begin, end;
  string input;
  int repeat = 100;
  int repeat_index = 5;

  for (auto m : mem){
    cout << "====================" << endl;
    cout << "Size: 10^" << m << endl;
    cout << "====================" << endl;
    for (auto s : sigma){
      cout << "Alphabet size: " << s << endl;
      // cout << genFilename(m, s) << endl;
      ifstream file(generateFilename(m, s));
      getline(file, input);

      if (m <= repeat_index){
        for (int i = 0; i < repeat; i++){
          WaveletTree vt(input);
        }
        end = getMemoryUsage(begin);
        cout << "-------------> " << (end - begin) / repeat << "MB" << endl;
      } else {
        begin = getMemoryUsage(0);
        WaveletTree vt(input);
        end = getMemoryUsage(begin);
        cout << "-------------> " << end - begin << "MB" << endl;
      }
      cout << endl;
    }
  }

  return 0;
}
