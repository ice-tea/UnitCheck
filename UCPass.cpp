
#include "llvm/ADT/ArrayRef.h"
#include "llvm/BasicBlock.h"
#include "llvm/Constants.h"
#include "llvm/Function.h"
#include "llvm/GlobalVariable.h"
#include "llvm/Pass.h"
#include "llvm/Type.h"
#include "llvm/Instruction.h"
#include "llvm/Instructions.h"
#include "llvm/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Analysis/DebugInfo.h"
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
		
		virtual bool runOnModual(Module &_M);
		
		static char ID;
		
		Module *M;
		
	private:
		
	};
	
	char CEPass::ID = 0;
	static RegisterPass<UCPass> X("UChecker", "Unit Checker", false, false);
	
	bool UCPass::runOnModule(Module &_M)
	{
		M = _M;
			
		return false;	
	}
}
