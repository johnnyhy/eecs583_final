#!/bin/bash

# print command as exec'd
set -x

# get name of prog to run
shopt -s nullglob
fnames=(demos/*.cpp)

echo $fnames

for fname in demos/*.cpp; do
    fname=${fname%.*}

    # compile CPI lib and user prog to LLVM bitcode
    clang++ -emit-llvm src/cpi.cpp -c -o cpi.bc &&
    clang++ -fno-stack-protector -emit-llvm $fname.cpp -c -o $fname.bc &&

    # run CPI pass on prog (instruments calls to CPI lib)
    opt -load build/cpi_pass/CPI.so -cpi < $fname.bc > $fname.opt.bc &&

    # link CPI lib to instrumented prog
    llvm-link cpi.bc $fname.opt.bc -o $fname.cpilib.bc &&

    # compile and run prog
    clang++ -fno-stack-protector -lssl -lcrypto $fname.cpilib.bc -o $fname.cpilib &&
    clang++ -fno-stack-protector $fname.cpp -o $fname.insecure

    # cleanup bitcode files
    rm cpi.bc &&
    rm $fname.bc &&
    rm $fname.opt.bc &&
    rm $fname.cpilib.bc
done

for fname in demos/*.cpilib; do
    (time ./$fname) &> $fname.time
done