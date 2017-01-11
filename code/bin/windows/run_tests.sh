#!/bin/sh

for test_file in $(find ../../test/ -name "*.cpp")
do
	file_name=$(basename $test_file);
	test_name="${file_name%.*}"
	echo "Running test file:" $file_name;
	g++ -isystem ../../include -pthread $test_file ../../src/RRR.cpp libgtest.a -o $test_name.out && ./$test_name.out
done
