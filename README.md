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
increased flexibility and security at the cost of some efficiency advantages 
when compared to the ARM64e methodology. See our research write-up at the 
link below for more information and a detailed security analysis.

## References and further reading:
1. J. Yang, O. Webb. ARM64e-Inspired Code Pointer Integrity in LLVM IR. 
        https://docs.google.com/document/d/18yBso_evxJY6Rr9Wv1-cvj49Eq2qZul38kpD4mtxSB0/edit?usp=sharing
2. J. McCall, A. Bougacha. ARM64e, An ABI for Pointer Authentication.
        https://llvm.org/devmtg/2019-10/slides/McCall-Bougacha-arm64e.pdf
3. Qualcomm Technologies. Pointer Authentication on ARMv8.3.
        https://www.qualcomm.com/media/documents/files/whitepaper-pointer-authentication-on-armv8-3.pdf
4. M.R. Khandaker, W. Liu, A. Nasar, Z. Wang, J. Yang. Origin-sensitive 
    Control Flow Integrity.
        https://www.usenix.org/system/files/sec19-khandaker.pdf
5. Apple LLVM Project. Pointer Authentication.
        https://github.com/apple/llvm-project/blob/a63a81bd9911f87a0b5dcd5bdd7ccdda7124af87/clang/docs/PointerAuthentication.rst  
