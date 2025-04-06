# KORG-Tools
[![Build Linux x86-64](https://github.com/aczwink/KORG-Tools/actions/workflows/build.yml/badge.svg)](https://github.com/aczwink/KORG-Tools/actions/workflows/build.yml)

libkorg is the first open source library that can read various file formats of SETs that are used by KORG.
It is able to decompress OC31 data - an LZSS variant developed by KORG.
This project is the first to publicly document the algorithm for decompressing OC31 compressed data.

The project is still in an early development status and isn't useable yet.

## Building
First of all build and install [Std++](https://github.com/aczwink/StdPlusPlus).
Then do something like the following:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```
