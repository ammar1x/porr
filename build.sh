#!/usr/bin/env bash

echo mkdir -p build
mkdir -p build

echo mkdir -p output
mkdir -p output

cd build
rm -rf *

echo cmake $@ ../
cmake $@ ../

echo make
make

