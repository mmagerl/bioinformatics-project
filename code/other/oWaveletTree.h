#pragma once

#include "oRRR.h"
#include "oRRRTable.h"
#include <string>
#include <vector>
#include <cstdint>
#include "oWaveletNode.h"
#include "Common.h"

using namespace std;

// Class representing a wavelet tree that uses oRRR
// Example usage:
//		oWaveletTree tree("Example text", NULL);
//		cout << tree.rank(e, 10);
class oWaveletTree {
private:
	// Vector encoding the symbol => index mapping
	// Vector indices correspond to ascii codes
	// Vector elements are indices used in the wavelet tree (or -1 if the character isn't used)
	alphabet alphabetIndices;

	// Vector encoding the index => symbol mapping
	// Vector indices symbol indices used in the wavelet tree
	// Vector elements are ascii codes for the given symbol indices
	inverseAlphabet alphabetCharacters;

	// Tree root node
	oWaveletNode* root;

	// Number of symbols that appear in given input
	// Used for tree construction
	uint8_t alphabetSize;

public:
	// Returns root node of the wavelet tree
	oWaveletNode* getRoot() const;

	// Creates a new wavelet tree
	//		input			input string for which the wavelet tree is constructed
	//		visualOutput	file handler used to generate graphviz data
	oWaveletTree(string input, FILE* visualOutput = NULL);

	// Destructor
	virtual ~oWaveletTree();

	// Returns rank for the given character up to the given index (including)
	//		character		character for which rank is calculated
	//		index			index (starting from 0) up to which (included) rank is calculated
	// Throws invalid_argument exceptions if the stored string doesn't contain the requested 
	// character or if the index is larger then stored string length
	uint64_t rank(uint8_t character, uint64_t index);

	// Returns the index of the count-th appearance of the given character
	//		character		character for which select is calculated
	//		count			number of searched for appearances
	// Throws invalid_argument exceptions if the stored string doesn't contain the requested 
	// character or if the count value is not positive or is larger then stored string length
	uint64_t select(uint8_t character, uint64_t count);

	// Returns character stored at the given index
	// Throws invalid_argument exception if index is too large
	uint8_t access(uint64_t index);

};
