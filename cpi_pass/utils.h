/* Utility Functions Header File
 *
 * EECS 583 - Advanced Compiler Construction 
 * Final Project
 * 
 * Authors: Johnathan Yang and Owen Webb
 * Date: 12/16/2020
 */
// Standard Includes
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

// LLVM Includes
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"

void printBufAsHex(uint8_t* data, unsigned len, llvm::raw_ostream &s);

bool isPtrToFunc(llvm::Type* t);
