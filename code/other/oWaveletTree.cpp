#include <stdio.h>
#include "oWaveletTree.h"

// Returns root node of the wavelet tree
oWaveletNode* oWaveletTree::getRoot() const {
	return root;
}

// Creates a new wavelet tree
//		input			input string for which the wavelet tree is constructed
//		visualOutput	file handler used to generate graphviz data
oWaveletTree::oWaveletTree(string content, FILE* visualOutput) {
	this->alphabetIndices = alphabet(256, -1);
	this->alphabetCharacters = inverseAlphabet(256, 0);

	// Create symbol required mappings
	for (unsigned int i = 0; i < content.length(); i++) {
		if (this->alphabetIndices[content[i]] == -1) {
			this->alphabetIndices[content[i]] = 1;
		}
	}
	
	int cumSum = 0;
	for (unsigned int i = 0; i < this->alphabetIndices.size(); i++) {
		if (this->alphabetIndices[i] == 1) {
			this->alphabetIndices[i] = cumSum;
			this->alphabetCharacters[cumSum] = i;
			cumSum++;
		}
	}
	this->alphabetSize = cumSum;

	if (visualOutput != NULL) {
		fprintf(visualOutput, "digraph bioinf { \n");
		fprintf(visualOutput, "\tnode [shape = box, fontname=\"courier\"]; \n");
	}

	// Build the binary tree
	this->root = new oWaveletNode(content, NULL, 0, this->alphabetSize - 1, this->alphabetIndices, true, visualOutput);

	if (visualOutput != NULL) {
		fprintf(visualOutput, "} \n");
		fclose(visualOutput);
	}
}

// Destructor
oWaveletTree::~oWaveletTree() {
	delete this->root;
}

// Returns rank for the given character up to the given index (including)
//		character		character for which rank is calculated
//		index			index (starting from 0) up to which (included) rank is calculated
// character or if the index is larger then stored string length
uint64_t oWaveletTree::rank(uint8_t character, uint64_t index) {
	oWaveletNode* v = root;
	uint64_t r = index;
	int16_t characterIndex = this->alphabetIndices[character];

	if (characterIndex == -1) {
	}

	bool isRoot = true;
	while (v != NULL) {
		uint8_t threshold = v->getThreshold();

		// Edge case -> index up to which is being search for in the oRRR is decreased for child nodes
		if (!isRoot) {
			r--;
		}
		else {
			isRoot = false;
		}

		// Depending on the current node alphabet coding look into the left or right child
		if (characterIndex <= threshold) {
			r = v->getContent()->rank0(r);
			v = v->getLeftChild();
		}
		else {
			r = v->getContent()->rank1(r);
			v = v->getRightChild();
		}

		// Edge case - symbol not found in internal node
		if (r == 0) return 0;
	}
	return r;
}

// Returns the index of the count-th appearance of the given character
//		character		character for which select is calculated
//		count			number of searched for appearances
// character or if the count value is not positive or is larger then stored string length
uint64_t oWaveletTree::select(uint8_t character, uint64_t count) {
	int16_t characterIndex = this->alphabetIndices[character];

	if (characterIndex == -1) {
	}

	oWaveletNode* v = NULL;
	oWaveletNode* temp = root;
	uint64_t r = count;
	// Go down the tree to the child containing the requested symbol
	while (temp != NULL) {
		uint8_t threshold = temp->getThreshold();
		v = temp;

		if (characterIndex <= threshold) {
			temp = temp->getLeftChild();
		}
		else {
			temp = temp->getRightChild();
		}
	}
	// v is leaf
	// Handle leaf and the edge case when the tree has only one node
	uint8_t threshold = v->getThreshold();
	if (characterIndex <= threshold) {
		r = v->getContent()->select0(r);
	}
	else {
		r = v->getContent()->select1(r);
	}

	// Go up the tree and compute select index
	while (v != root) {
		r++;
		oWaveletNode* p = v->getParent();
		if (v->getIsLeftChild()) {
			r = p->getContent()->select0(r);
		}
		else {
			r = p->getContent()->select1(r);
		}

		v = p;
	}

	return r;
}

// Returns character stored at the given index
uint8_t oWaveletTree::access(uint64_t index) {
	oWaveletNode* v = root;
	uint64_t r = index;

	bool isRoot = true;

	while (v != NULL) {
		if (!isRoot) {
			r--;
		}
		else {
			isRoot = false;
		}

		if (v->getContent()->access(r) == 0) {
			r = v->getContent()->rank0(r);
			if (v->getLeftChild() == NULL) {
				return this->alphabetCharacters[v->getStart()];
			}
			v = v->getLeftChild();
		}
		else {
			r = v->getContent()->rank1(r);
			if (v->getRightChild() == NULL) {
				return this->alphabetCharacters[v->getEnd()];
			}
			v = v->getRightChild();
		}
	}
	return 0;
}
