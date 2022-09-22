//
// Created by 张凯文 on 2022/9/22.
//

#include "SSALiveness.h"

void SSALiveness::live(Instruction *user, Instruction* inst, set<Instruction*>& interval){

    errs()<<"====== USER : "<<*user<<"\n";
    interval.insert(user);
    BasicBlock *bb = user->getParent();

    if( &*(bb->begin()) == user){
        errs()<<"====== IS A BB HEAD "<<*user<<"\n";

        for (pred_iterator PI = pred_begin(bb), E = pred_end(bb); PI != E; ++PI) {
            BasicBlock *PredBB = *PI;
            Instruction *term = PredBB->getTerminator();

            if(term != inst){
                BasicBlock::iterator BI(term);
                live(&*BI, inst, interval);
            }
        }
    }
    else{

        BasicBlock::iterator BI(user);
        BI--;
        if(&*BI != inst){
            live(&*BI, inst, interval);
        }
    }
}


bool SSALiveness::runOnFunction(Function &F) {
    // 不检测main函数
    if(F.getName() == "main")
        return false;

    errs()<<"========== "<<F.getName()<<" =========\n";

    Instruction* addinst;
    for (inst_iterator I = inst_begin(F), E = inst_end(F) ; I != E; ++I){

        if(I->getOpcode() == Instruction::Add){
            errs()<<"THE LIVENESS OF THE INST : "<<*I<<"\n";
            addinst = &*I;
            break;
        }
    }
    set<Instruction *> interval;
    for (User *U : addinst->users()){
        Instruction* user = cast<Instruction>(U);
        errs()<<"\tUSER:  "<<*user<<"\n";

        if(isa<PHINode>(user)){
            continue;
        }

        live(user, addinst, interval);
    }


    errs()<<"======== LIVE INTERVAL =========\n";
    errs()<<interval.size()<<"\n";
    for(auto &inst : interval){
        if(inst != NULL)
            errs()<<"\t"<<*inst<<"\n";
    }
        return false;
}

