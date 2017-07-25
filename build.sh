#!/bin/bash

# Exit on failure
set -e

# build var
: ${CXX:=g++}

# build\bin
mkdir -p bin
cd bin

# Compile
$CXX -c -o main.o ../gcc/main.cpp -I../

# Link
$CXX -o main main.o