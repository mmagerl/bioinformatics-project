#!/bin/sh

include_src="../src/RRR.cpp ../src/WaveletTree.cpp Brute.cpp"
g++ -std=gnu++11 MemTest.cpp $include_src -o MemTest.out && ./MemTest.out
