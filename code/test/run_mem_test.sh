#!/bin/sh

include_src="../src/RRR.cpp ../src/WaveletTree.cpp Brute.cpp"
other_src="../other/oRRR.cpp ../other/oRRRTable.cpp ../other/oWaveletNode.cpp ../other/oWaveletTree.cpp"

g++ -std=gnu++11 MemTest.cpp $include_src $other_src -o MemTest.out & ./cgmemtime ./MemTest.out x 10 20 30
