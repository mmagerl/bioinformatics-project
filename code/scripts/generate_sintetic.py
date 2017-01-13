#!/usr/bin/env python
import sys
import random

def main():
	if len(sys.argv) != 5:
		print("%s: Invalid number of arguments" % (sys.argv[0]))
		print("\nArguments:")
		print("\tnumberOfCharacters - the total number of generated characters")
		print("\tasciiLowerBound - the lowest of ASCII codes to choose from (inclusive)")
		print("\tasciiUpperBound - the highest of ASCII codes to choose from (inclusive)")
		print("\toutputFile - the path where to write the output file")
		print("\n\nExample of usage: python %s 10000 65 70 output.txt" % (sys.argv[0]))
		exit(1)

	number_of_characters = int(sys.argv[1])
	ascii_lower = int(sys.argv[2])
	ascii_upper = int(sys.argv[3])
	output_file = sys.argv[4]

	with open(output_file, "w") as output_file:
		characters = []
		for i in range(number_of_characters):
			characters.append(chr(random.randint(ascii_lower, ascii_upper)))

		output_file.write("".join(characters))

if __name__ == "__main__":
    main()
