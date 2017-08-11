#!/bin/bash

# Exit on failure
set -e

# build var
: ${CXX:=g++}

# build\bin
mkdir -p bin
cd bin

echo $PWD

# Compile
$CXX -c -o bignumber.o ../src/BigNumber.cpp
$CXX -c -o main.o ../gcc/main.cpp -I"../gtest-1.8.0/" -I"../gtest-1.8.0/include/"

# Link it all together
$CXX -o main bignumber.o main.o
