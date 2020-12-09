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

        Value* sign;
        Value* auth;

        Function* getAddrOfRetAddr;
        Function* getRetAddr;

        bool runOnModule(Module &M) override {
            getAddrOfRetAddr = Intrinsic::getDeclaration(&M, Intrinsic::frameaddress);//, std::vector<Type*>{ Type::getVoidTy(M.getContext()) });
            getRetAddr = Intrinsic::getDeclaration(&M, Intrinsic::returnaddress);//, std::vector<Type*>{ Type::getInt32Ty(M.getContext()) });

            sign = M.getOrInsertFunction("sign", Type::getVoidTy(M.getContext()), Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())).getCallee();
            auth = M.getOrInsertFunction("auth", Type::getVoidTy(M.getContext()), Type::getInt8PtrTy(M.getContext()), Type::getInt8PtrTy(M.getContext())).getCallee();
            
            for (auto &F : M) {
                runOnFunction(F);
            }

	        return false;
        }

        bool runOnFunction(Function& F) {
            errs() << F.getName() << "\n";
            if (F.getInstructionCount() == 0) {
                errs() << "\tnot defined within module\n";
                return false;
            }

            if (F.getName() == "_Z7wrapperiPPc") {
            Instruction& first = *(F.begin())->begin();
            IRBuilder<> builder(&first);
            errs() << "here\n";
            CallInst* addrOfRetAddr = builder.CreateCall(getAddrOfRetAddr);
            errs() << "here\n";
            CallInst* retAddr = builder.CreateCall(getRetAddr, std::vector<Value*>{ builder.getInt32(0) });
            errs() << "here\n";
            sign->print(errs());
            errs() << "\n";
            getAddrOfRetAddr->print(errs());
            errs() << "\n\n";
            retAddr->print(errs());
            errs() << "\n";
            builder.CreateCall(sign, std::vector<Value*>{addrOfRetAddr, retAddr});
            errs() << "here\n";
            }

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

                        errs() << "\tStoreInst: " << store << " to function? " << ptrToFunc << "\n";
                    } else if (CallInst* call = dyn_cast<CallInst>(&i)) {
                        if (call->isIndirectCall()) {
                            // insert indirect call auth
                            // generate address of function pointer
                            // instrument with call to auth, and fptr's as an argument
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
char cpi::ID = 0;
static RegisterPass<cpi> X("cpi", "CPI Pass",
    false /* Only looks at CFG */,
    false /* Analysis Pass */);
