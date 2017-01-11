#!/bin/sh

g++ -isystem ../../include -pthread ../../test/RRRTest.cpp libgtest.a -o test && ./test
