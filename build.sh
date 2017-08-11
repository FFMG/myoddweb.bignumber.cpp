#!/bin/bash

# Exit on failure
set -e

# build var
: ${CXX:=g++}

# build\bin
mkdir -p bin
cd bin

echo $PWD

# compile the big number source itself
$CXX -c -o bignumber.o ../src/BigNumber.cpp

# Compile all the tests
$CXX -c ../tests/*.cpp -I"../gtest-1.8.0/include/"

# Compile the main file that will pull it all together
$CXX -c -o main.o ../gcc/main.cpp -I"../gtest-1.8.0/" -I"../gtest-1.8.0/include/" -I"../tests/"

# Link it all together
$CXX -o main *.o -pthread
