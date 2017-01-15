#!/bin/sh

if [ $# -ne 2 ]; then
	echo $0 ": Expected two arguments: path to sintetic dataset and implementation type 'm' or 'o'"
else
	mkdir -p ../build/
	include_src="../src/RRR.cpp ../src/WaveletTree.cpp Brute.cpp"
  other_src="../other/oRRR.cpp ../other/oRRRTable.cpp ../other/oWaveletNode.cpp ../other/oWaveletTree.cpp"
  g++ -std=gnu++11 MemTest.cpp $include_src $other_src -o ../build/MemTest.out && ./cgmemtime ../build/MemTest.out $1 $2 x 10 20 30
fi

