#!/bin/bash

# print command as exec'd
set -x

# get name of prog to run
fname=$1

# compile CPI lib and user prog to LLVM bitcode
clang++ -emit-llvm src/cpi.cpp -c -o cpi.bc &&
clang++ -g3 -fno-sanitize=address -fno-sanitize=bounds -fno-stack-protector -emit-llvm demos/$fname.cpp -c -o $fname.bc &&

# run CPI pass on prog (instruments calls to CPI lib)
opt -load build/cpi_pass/CPI.so -cpi < $fname.bc > $fname.opt.bc &&

# link CPI lib to instrumented prog
llvm-link cpi.bc $fname.opt.bc -o $fname.cpilib.bc &&

# compile and run prog
clang++ -fno-stack-protector -g3 -lssl -lcrypto $fname.cpilib.bc -o $fname.cpilib &&
clang++ -fno-sanitize=address -fno-sanitize=bounds -fno-stack-protector -g3 demos/$fname.cpp -o $fname.insecure

# cleanup bitcode files
rm cpi.bc &&
rm $fname.bc &&
rm $fname.opt.bc &&
rm $fname.cpilib.bc