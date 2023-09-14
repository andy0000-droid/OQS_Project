#!/bin/bash

echo "Build Docker image"

# remove exist build folder & git clone
rm -rf liboqs
git clone --depth 1 --branch oqs https://github.com/andy0000-droid/OQS_Project.git liboqs

#download requires
sudo apt install astyle cmake gcc ninja-build libssl-dev python3-pytest python3-pytest-xdist unzip xsltproc doxygen graphviz python3-yaml valgrind

#build liboqs
cd liboqs && mkdir build && cd build && cmake -GNinja .. && ninja
echo "test message" > plaintext.txt #file exist in build folder

