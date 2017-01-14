#!/bin/sh

if [ $# -eq 0 ]; then
	echo $0 ": Expected one argument - path to directory with sintetic datasets"
else
	mkdir -p ../build/
	include_src="../src/RRR.cpp ../src/WaveletTree.cpp Brute.cpp"
	g++ -std=gnu++11 ConstructionSpeedTest.cpp $include_src -o ../build/ConstructionSpeedTest.out && ../build/ConstructionSpeedTest.out $1
fi
