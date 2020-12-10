/* Utility Functions Implementations File
 *
 * EECS 583 - Advanced Compiler Construction 
 * Final Project
 * 
 * Authors: Johnathan Yang and Owen Webb
 * Date: 12/16/2020
 */
#include "utils.h"

bool isPtrToFunc(llvm::Type* t) {
    return t->isPointerTy() && t->getContainedType(0)->isFunctionTy();
}
