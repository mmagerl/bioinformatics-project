#!/bin/sh

g++ -isystem ../include -pthread ../test/ExampleTest.cpp libgtest.a -o test && ./test