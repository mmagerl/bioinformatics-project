#!/usr/bin/env python
import os
import sys
import random

def generate(number_of_characters, ascii_lower, ascii_upper, output_file_name, output_dir="./"):
	if not os.path.exists(output_dir):
		os.makedirs(output_dir)

	with open(os.path.join(output_dir, output_file_name), "w") as output_file:
		characters = []
		for i in range(number_of_characters):
			characters.append(chr(random.randint(ascii_lower, ascii_upper)))

		output_file.write("".join(characters))
		print("Generated file: " + output_file_name)


def main():
	if len(sys.argv) < 5:
		print("%s: Invalid number of arguments" % (sys.argv[0]))
		print("\nArguments:")
		print("\tnumberOfCharacters - the total number of generated characters")
		print("\tasciiLowerBound - the lowest of ASCII codes to choose from (inclusive)")
		print("\tasciiUpperBound - the highest of ASCII codes to choose from (inclusive)")
		print("\toutputFile - the file name where to write the output file")
		print("\toutputDirectory - the directory where to write the output file (optional, default value: ./)")
		print("\n\nExample of usage: python %s 10000 65 70 output.txt" % (sys.argv[0]))
		exit(1)

	number_of_characters = int(sys.argv[1])
	ascii_lower = int(sys.argv[2])
	ascii_upper = int(sys.argv[3])
	output_file = sys.argv[4]
	output_dir = sys.argv[5] if len(sys.argv) > 5 else "./"

	generate(number_of_characters, ascii_lower, ascii_upper, output_file, output_dir)


if __name__ == "__main__":
    main()
