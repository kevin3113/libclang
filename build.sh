#!/bin/bash

set -x

rm ./test
g++ -g test.cpp -I../include ../lib/libclang.so -Wl,-rpath=../lib -o test.bin

LD_LIBRARY_PATH=../lib ./test.bin $@
