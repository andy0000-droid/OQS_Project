#!/bin/bash

echo "Build Docker image"

# remove exist build folder & git clone
rm -rf liboqs
git clone --depth 1 --branch oqs https://github.com/andy0000-droid/OQS_Project.git liboqs

#build liboqs
cd liboqs && mkdir build && cd build && cmake -GNinja .. && ninja
echo "test message" > plaintext.txt

