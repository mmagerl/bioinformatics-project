#!/usr/bin/env python
from generate_sintetic import generate

def main():
	number_of_characters = 100
	alphabet_sizes = [4, 16, 28]
	ascii_lower = 97
	while number_of_characters <= 100000000:
		for alphabet_size in alphabet_sizes:
			output_file = "sintetic_%d_%d.in" % (number_of_characters, alphabet_size)
			generate(number_of_characters, ascii_lower, ascii_lower + alphabet_size - 1, output_file)
			
		number_of_characters *= 10
	

if __name__ == "__main__":
    main()
