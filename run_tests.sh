#!/bin/sh

rm -rfv build
mkdir -v build
cd build
cmake ..
make
./tests
