# ARM64e-Inspired Code Pointer Integrity at the LLVM IR Level
## EECS 583 - Advanced Compiler Construction 
## Final Project

### Authors: Johnathan Yang and Owen Webb
### Date: 12/16/2020

## Description
The following software implements an LLVM IR pass that ensures 
Code Pointer Integrity (CPI) using a methodology similar to the hardware 
Pointer Authentication Codes (PACs) implemented in Apple's ARM64e architecture.
By enforcing CPI at the IR level, we believe our implementation offers 
increased flexibility and security at the cost of some performance
when compared to the ARM64e methodology.

For demonstrations, see the README file under demos/
