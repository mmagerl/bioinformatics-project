#! /usr/bin/python3
import argparse
import os

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file', type=str, help='path to input file')
    parser.add_argument('output_file', type=str, help='path to output file')
    args = parser.parse_args()

    content = ""
    with open(args.input_file) as f:
        for x in f.readlines()[1:]:
            content += x.strip()
        
    with open(args.output_file, 'w') as f:
        f.write(content)
