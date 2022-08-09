#!/bin/sh

rm -rfv build
mkdir -v build
ls -la
cd build
cmake ..
make
./json_packer ../sample.txt output.bin
# hexdump -C output.bin
