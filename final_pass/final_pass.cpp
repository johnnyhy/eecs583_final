/* ARM64e-Inspired Code Pointer Integrity at the LLVM IR Level
 *
 * EECS 583 - Advanced Compiler Construction
 * Final Project
 *
 * Authors: Johnathan Yang and Owen Webb
 * Date: 12/16/2020
 *
 * Description: The following software implements an LLVM IR pass that ensures
 * Code Pointer Integrity (CPI) using a methodology similar to the hardware
 * Pointer Authentication Codes (PACs) implemented in Apple's ARM64e architecture.
 * By enforcing CPI at the IR level, we believe our implementation offers
 * increased flexibility and security at the cost of some efficiency advantages
 * when compared to the ARM64e methodology. See our research write-up at the
 * link below for more information and a detailed security analysis.
 *
 * References and further reading:
 *      (1) J. Yang, O. Webb. ARM64e-Inspired Code Pointer Integrity in LLVM IR.
 *              https://docs.google.com/document/d/18yBso_evxJY6Rr9Wv1-cvj49Eq2qZul38kpD4mtxSB0/edit?usp=sharing
 *      (2) J. McCall, A. Bougacha. ARM64e, An ABI for Pointer Authentication.
 *              https://llvm.org/devmtg/2019-10/slides/McCall-Bougacha-arm64e.pdf
 *      (3) Qualcomm Technologies. Pointer Authentication on ARMv8.3.
 *              https://www.qualcomm.com/media/documents/files/whitepaper-pointer-authentication-on-armv8-3.pdf
 *      (4) M.R. Khandaker, W. Liu, A. Nasar, Z. Wang, J. Yang. Origin-sensitive
 *          Control Flow Integrity.
 *              https://www.usenix.org/system/files/sec19-khandaker.pdf
 *      (5) Apple LLVM Project. Pointer Authentication.
 *              https://github.com/apple/llvm-project/blob/a63a81bd9911f87a0b5dcd5bdd7ccdda7124af87/clang/docs/PointerAuthentication.rst
 *      (6) Securing C++ Virtual Calls from Memory Corruption Attacks
 *              https://cseweb.ucsd.edu/~lerner/papers/ndss14.pdf
 */
 // Standard Includes
#include <string>

// LLVM Includes
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/CallSite.h"

// OpenSSL Includes
#include "openssl/sha.h"
#include "openssl/hmac.h"
#include "openssl/evp.h"

// Custom Includes
#include "utils.h"

// Options and Config
const size_t PTR_LEN = 8;                   // in bytes for all
const size_t KEY_LEN = 32;
const size_t SIG_LEN = 32;
const std::string KEY_PATH = "auth.key";
const uint8_t KEY[KEY_LEN] = { };

using namespace llvm;

// SIGN a raw pointer, producing a signed pointer
std::string sign(Instruction& i) {
    (void)i;
    std::string digest;

    uint8_t k[KEY_LEN] = {};
    uint8_t ptrval[PTR_LEN] = { 0xff, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef };
    uint8_t digest1[SIG_LEN] = {};

    // plain 256 hash
    SHA256(ptrval, PTR_LEN, digest1);
    printBufAsHex(digest1, SIG_LEN, errs());

    // HMAC 
    uint8_t* digest2 = HMAC(EVP_sha256(), k, KEY_LEN, ptrval, PTR_LEN, nullptr, nullptr);
    printBufAsHex(digest2, SIG_LEN, errs());

    // use i.insertAfter() to place sign instructions after i
    return digest;
}

// AUTHENTICATE a signed pointer, producing a raw pointer
std::string auth(Instruction& i) {
    (void)i;
    std::string raw_ptr;
    return raw_ptr;
}

// LLVM Pass implementation
namespace {
    struct FINAL : public FunctionPass {
        static char ID;
        FINAL() : FunctionPass(ID) {}

        bool runOnFunction(Function& F) override {
            // insert sign (for return addresses)

            // loop through parameters
            //      if paramter is pointer to a function
            //          insert:
            //              if src in map of signed pointers
            //                  auth // need to auth and re-sign to ensure code pointer has not changed since last sign
            //              sign      
            // for (auto arg = F.arg_begin(); arg != F.arg_end(); ++arg) {
            //     if (auto* pointer = dyn_cast<Pointer>(arg)) {

            //     }
            // }

            errs() << F.getName() << "\n";
            for (BasicBlock& BB : F) {
                for (BasicBlock::iterator I = BB.begin(); I != BB.end();) {
                    Instruction& i = *I++;
                    
                    if (StoreInst* store = dyn_cast<StoreInst>(&i)) {
                        Type* type = store->getPointerOperand()->getType()->getContainedType(0);
                        bool ptrToFunc = false;

                        if (type->isPointerTy() && type->getContainedType(0)->isFunctionTy()) {
                            Value* rhs = store->getValueOperand();
                            ptrToFunc = true;
                        //          insert:
                        //              if src in map of signed pointers
                        //                  auth // need to auth and re-sign to ensure code pointer has not changed since last sign
                        //              sign
                        }

                        errs() << "\tStoreInst: " << store << " to function? " << ptrToFunc << "\n";
                    } else if (CallInst* call = dyn_cast<CallInst>(&i)) {
                        if (call && call->isIndirectCall()) {
                            // insert indirect call auth
                            errs() << "\tIndirect CallInst: " << call << "\n";
                        }
                    } else if (ReturnInst* ret = dyn_cast<ReturnInst>(&i)) {
                        // insert return auth
                        errs() << "\tReturnInst: " << ret << "\n";
                    }
                }
            }
            return false;
        }
    };
}


// Register the pass
char FINAL::ID = 0;
static RegisterPass<FINAL> X("FINAL", "CPI Pass",
    false /* Only looks at CFG */,
    false /* Analysis Pass */);
