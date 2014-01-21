

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Pass.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/DebugInfo.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/Support/CFG.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Support/InstIterator.h"
#include <vector>
#include <string>
#include <map>
#include <list>
#include <sstream>
#include <fstream>
#include <queue>

using namespace llvm;

namespace
{
	class UCPass:public ModulePass
	{
	public:
		UCPass():ModulePass(ID){}
		
		virtual void getAnalysisUsage(AnalysisUsage &AU) const
		{
			AU.addRequired<CallGraph>();
			AU.setPreservesCFG();
			AU.setPreservesAll();
		}
		
		virtual bool runOnModule(Module &_M);
		
		static char ID;
		
		Module *M;
		
		std::vector<Instruction *> InstList;
		
	private:
		std::pair<unsigned, StringRef> getInstInfo(Instruction *I);
        
	};
	
	char UCPass::ID = 0;
	static RegisterPass<UCPass> X("UChecker", "Unit Checker", false, false);
	
    std::pair<unsigned, StringRef> UCPass::getInstInfo(Instruction *I)
    {
        
        if (MDNode *N = I->getMetadata("dbg"))
        {                                           // Here I is an LLVM instruction
            DILocation Loc(N);                      // DILocation is in DebugInfo.h
            unsigned bbline = Loc.getLineNumber();
            StringRef bbfile = Loc.getFilename();
            //errs() << "[getInstInfo]" << bbline << " " << bbfile << "\n";
            return std::make_pair(bbline, bbfile);
        }
        return std::make_pair(0, "");
    }
    
	bool UCPass::runOnModule(Module &_M)
	{
		M = &_M;
		
        M->getGlobalList();
		for(Module::iterator mit=M->begin(); mit!=M->end(); ++mit)
		{
			Function *F = mit;
			for(inst_iterator iit=inst_begin(F); iit!=inst_end(F); ++iit)
			{
                Instruction *inst = &*iit;
				InstList.push_back(inst);
			}
		}
        
		for(std::vector<Instruction *>::iterator vit=InstList.begin(); vit!=InstList.end(); ++vit)
        {
            DEBUG(errs() << "Instruction:" << *vit << "at line:" << getInstInfo(*vit).first << "\n");
        }
		
		return false;	
	}
}
