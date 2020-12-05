
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/CallSite.h"

using namespace llvm;

namespace {
    struct FINAL : public FunctionPass {
        static char ID;
        FINAL() : FunctionPass(ID) {}

        bool runOnFunction(Function& F) override {
            for (BasicBlock& BB : F) {
                for (BasicBlock::iterator I = BB.begin(); I != BB.end();) {
                    Instruction& i = *I++;
                    // auto load = dyn_cast<LoadInst>(&i);
                    // if (load) {
                    //     Type* loadOpType = load->getPointerOperand()->getType();
                    //     // // if (loadOpType->isFunctionTy()) {
                    //     // //     errs() << "FunctionType identified\n";
                    //     // // }
                    //     // errs() << *loadOpType << "\n";
                    // }
                    // for every load pointing to a function
                    // if load's operand points to a function
                    //      insert auth
                    // virtual calls are already identified correctly here
                    auto call = dyn_cast<CallInst>(&i);
                    if (call && call->isIndirectCall()) {
                        // this is where we need to insert auths
                        // sign after all users that write to callInst's ops
                        errs() << "Indirect call!!! " << i << "\n";
                    }
                    // errs() << i << "\n";

                    auto ret = dyn_cast<ReturnInst>(&i);
                    if (ret) {
                        // this is where we need to insert auths
                        // sign return address at start of function
                        errs() << "Indirect call!!! " << i << "\n";
                    }
                }
            }
            // errs() << "Hello: ";
            return false;
        }
    }; // end of struct Hello
}  // end of anonymous namespace

char FINAL::ID = 0;
static RegisterPass<FINAL> X("FINAL", "Hello World Pass",
    false /* Only looks at CFG */,
    false /* Analysis Pass */);