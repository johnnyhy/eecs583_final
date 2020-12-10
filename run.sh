#!/bin/bash

# print command as exec'd
set -x

# get name of prog to run
fname=$1
arg=$2

# compile CPI lib to LLVM bitcode, compile prog to LLVM bitcode
clang++ -emit-llvm cpi_pass/cpi.cpp -c -o cpi.bc &&
clang++ -emit-llvm demos/$fname.cpp -c -o $fname.bc &&

# run CPI pass on prog (inserts calls to CPI lib)
opt -load build/cpi_pass/CPI.so -cpi < $fname.bc > $fname.opt.bc &&

# link CPI lib to instrumented prog
llvm-link cpi.bc $fname.opt.bc -o prog.bc &&

# compile and run prog
clang++ prog.bc -o $fname &&
chmod +x $fname &&
./$fname $arg

# cleanup bitcode files
rm cpi.bc &&
rm $fname
rm $fname.bc &&
rm $fname.opt.bc &&
rm prog.bc