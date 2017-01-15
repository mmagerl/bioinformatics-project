#!/bin/sh

if [ $# -ne 1 ]; then
	echo $0 ": Expected one argument - path to sintetic dataset"
else
	mkdir -p ../build/
	include_src="../src/RRR.cpp ../src/WaveletTree.cpp Brute.cpp"
  other_src="../other/oRRR.cpp ../other/oRRRTable.cpp ../other/oWaveletNode.cpp ../other/oWaveletTree.cpp"
	g++ -std=gnu++11 SpeedTest.cpp $include_src $other_src -o ../build/SpeedTest.out && ../build/SpeedTest.out $1
fi
