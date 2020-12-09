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
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/IRBuilder.h"

// Custom Includes
#include "utils.h"
#include "cpi.h"

using namespace llvm;

// LLVM Pass implementation
namespace {
    struct cpi : public ModulePass {
        static char ID;
        cpi() : ModulePass(ID) {}

        // __builtin_return_address
        Function* getRetAddr;

        // CPI lib functions
        Value* ret_sign;
        Value* ret_auth;
        Value* sign;
        Value* auth;

        bool runOnModule(Module &M) override {
            logStream.open(logFile);
            
            // lookup __builtin_return_address
            getRetAddr = Intrinsic::getDeclaration(&M, Intrinsic::returnaddress);

            // lookup CPI lib ret_sign/ret_auth, sign/auth functions
            ret_sign = M.getOrInsertFunction("_Z8ret_signPh", 
                                             Type::getVoidTy(M.getContext()), 
                                             Type::getInt8PtrTy(M.getContext())
                                            ).getCallee();
            ret_auth = M.getOrInsertFunction("_Z8ret_authPh",
                                             Type::getVoidTy(M.getContext()),
                                             Type::getInt8PtrTy(M.getContext())
                                            ).getCallee();
            sign = M.getOrInsertFunction("_Z4signPhS_",
                                         Type::getVoidTy(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext())
                                        ).getCallee();
            auth = M.getOrInsertFunction("_Z4authPhS_",
                                         Type::getVoidTy(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext()),
                                         Type::getInt8PtrTy(M.getContext())
                                        ).getCallee();
            
            // instrument CPI lib calls on every func
            bool modified = false;
            for (auto &F : M) {
                if (F.getInstructionCount()) { // only if function has body implemented in this module
                    modified = true;
                    runOnFunction(F);
                }
            }

	        return modified;
        }

        bool runOnFunction(Function& F) {
            logStream << std::string(F.getName()) << "\n";

            Instruction& first = *(F.begin())->begin();
            IRBuilder<> builder(&first);
            CallInst* retAddr = builder.CreateCall(getRetAddr, std::vector<Value*>{ builder.getInt32(0) });
            builder.CreateCall(ret_sign, std::vector<Value*>{ retAddr });

            for (Argument& arg : F.args()) {
                if (isPtrToFunc(arg.getType())) {
            //          insert:
            //              if src in map of signed pointers
            //                  auth // need to auth and re-sign to ensure code pointer has not changed since last sign
            //              sign     
                }
            }
            
            for (BasicBlock& BB : F) {
                for (BasicBlock::iterator I = BB.begin(); I != BB.end();) {
                    Instruction& i = *I++;
                    
                    if (StoreInst* store = dyn_cast<StoreInst>(&i)) {
                        Type* type = store->getPointerOperand()->getType()->getContainedType(0);
                        bool ptrToFunc = false;

                        if (isPtrToFunc(type)) {
                            // Value* rhs = store->getValueOperand();
                            ptrToFunc = true;
                            // sign(addressOfFptr, fptrValue)
                        //          insert:
                        //              if src in map of signed pointers
                        //                  auth // need to auth and re-sign to ensure code pointer has not changed since last sign
                        //              sign
                        }

                        logStream << "\tStoreInst: " << store << " to function? " << ptrToFunc << "\n";
                    } else if (CallInst* call = dyn_cast<CallInst>(&i)) {
                        if (call->isIndirectCall()) {
                            // insert indirect call auth
                            // generate address of function pointer
                            // instrument with call to auth, and fptr's as an argument
                            logStream << "\tIndirect CallInst: " << call << "\n";
                        }
                    } else if (ReturnInst* ret = dyn_cast<ReturnInst>(&i)) {
                        // insert return auth
                        IRBuilder<> builder2(&i);
                        retAddr = builder2.CreateCall(getRetAddr, std::vector<Value*>{ builder.getInt32(0) });
                        builder2.CreateCall(ret_auth, std::vector<Value*>{ retAddr });
                        logStream << "\tReturnInst: " << ret << "\n";
                    }
                }
            }
            return false;
        }
    };
}


// Register the pass
char cpi::ID = 0;
static RegisterPass<cpi> X("cpi", "CPI Pass",
    false /* Only looks at CFG */,
    false /* Analysis Pass */);
