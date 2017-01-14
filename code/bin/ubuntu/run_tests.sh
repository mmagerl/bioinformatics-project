#!/bin/sh

run_test() {
	test_file=$1
	file_name=$(basename $test_file)
	test_name="${file_name%.*}"
	include_src="../../src/RRR.cpp ../../src/WaveletTree.cpp ../../test/Brute.cpp"
	printf "\nRunning test file: $file_name\n"
	g++ -std=gnu++11 -isystem ../../include -pthread $test_file $include_src libgtest.a -o ../../build/$test_name.out && ../../build/$test_name.out
}

mkdir -p ../../build/
if [ $# -eq 0 ]; then
	for test_file in $(find ../../unittest/ -name "*.cpp"); do run_test $test_file; done
else
	for test_file in $*;
	do
		file_path="../../unittest/$test_file.cpp"
		if [ ! -f $file_path ]; then
			printf "\nrun_tests error: $file_path: No such file or directory\n"
		else
			run_test $file_path; 
		fi
	done
fi
