/* Utility Functions Implementations File
 *
 * EECS 583 - Advanced Compiler Construction 
 * Final Project
 * 
 * Authors: Johnathan Yang and Owen Webb
 * Date: 12/16/2020
 */
#include "utils.h"

void printBufAsHex(uint8_t* data, unsigned len, llvm::raw_ostream &s) {
    std::stringstream ss;
    for (int i = 0; i < len; ++i)
        ss << std::hex << (int)data[i];
    s << ss.str() << "\n";
}
