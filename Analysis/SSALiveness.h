//
// Created by 张凯文 on 2022/9/22.
//

#ifndef CLANGSTATICANALYSIS_LIB_SSALIVENESS_H
#define CLANGSTATICANALYSIS_LIB_SSALIVENESS_H

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CFG.h"

#include <map>
#include <set>

using namespace llvm;
using namespace std;

typedef  map<Instruction*, set<Value*>* > MTYPE;

class SSALiveness : public FunctionPass{
private:
    void live(Instruction *user, Instruction* inst, set<Instruction*>& interval);


public:
    static char ID;
    SSALiveness(): FunctionPass(ID){}
    bool runOnFunction(Function &F) override;
};

char SSALiveness::ID = 0;
static RegisterPass<SSALiveness> X("SSALiveness", "1st SSALiveness",
                                   false /* Only looks at CFG*/,
                                   false /*Analysis Pass*/);

#endif //CLANG STATIC ANALYSIS_LIB_SSALIVENESS_H
