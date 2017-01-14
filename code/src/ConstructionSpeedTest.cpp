#include <fstream>
#include <string>
#include <iostream>
#include <ctime>

#include "../include/WaveletTree.h"
#include "../include/Brute.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << argv[0] << ": Expected one argument - path to directory with sintetic datasets";
		return 1;
	}

	const string sinteticDir = argv[1];
	const string characterSizes[] = {"100", "1000", "10000", "100000", "1000000", "10000000", "100000000"};
	const string alphabetSizes[] = {"4", "16", "28"};
	ofstream output_file("construction_times.txt");
	output_file << "characterSize / alphabetSize\t4\t16\t28" << endl;


	for (string characterSize : characterSizes) {
		output_file << characterSize;
		for (string alphabetSize : alphabetSizes) {
			ifstream input_file(sinteticDir + "sintetic_" + characterSize + "_" + alphabetSize + ".in");
			string input;
			getline(input_file, input);
			input_file.close();
			
			clock_t begin = clock();
			WaveletTree vt(input);
			clock_t end = clock();

			double time_ms = (double(end - begin) * 1000.0) / CLOCKS_PER_SEC;

			cout << "File: " << "sintetic_" << characterSize << "_" << alphabetSize << ".in" << "\tConstruction time: " << time_ms << " ms" << endl;
			output_file << "\t" << time_ms;
		}
		output_file << endl;
	}

	output_file.close();
	cout << "Output written to file: construction_times.txt";

  return 0;
}
