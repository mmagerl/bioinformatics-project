#!/bin/sh

if [ $# -eq 0 ]; then
	echo $0 ": Expected one argument - path to sintetic dataset"
else
	mkdir -p ../build/
	include_src="../src/RRR.cpp ../src/WaveletTree.cpp Brute.cpp"
	g++ -std=gnu++11 SpeedTest.cpp $include_src -o ../build/SpeedTest.out && ../build/SpeedTest.out $1
fi
