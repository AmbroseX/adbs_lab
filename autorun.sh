#!/bin/sh
rm -rf build/;
mkdir build;
cd build;
xmake -P .;
cd ./linux/x86_64/release/;
./adbs_lab -l -c -f /home/rongkang/WorkSpace/CPP/adbs-lab/data/data-5w-50w-zipf.txt;

