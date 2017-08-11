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
$CXX -c -o bignumber.o ../src/BigNumber.cpp -I ../ 
$CXX -c -o main.o ../gcc/main.cpp -I../

# Link it all together
$CXX -o main bignumber.o main.o
