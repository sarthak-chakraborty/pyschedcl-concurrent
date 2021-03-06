#ifndef FEATURE_COLLECTOR_H
#define FEATURE_COLLECTOR_H

#include "thrud/DivergenceAnalysis.h"
#include "thrud/NDRange.h"
#include "thrud/OCLEnv.h"

#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"

#include "llvm/IR/Dominators.h"

#include "llvm/Support/YAMLTraits.h"

#include <map>
#include <tuple>
#include <string>
#include <vector>
#include <sstream>
#define LOG_DEBUG if(0)
#define LOG_ACCESS if(0)
namespace llvm {
class BasicBlock;
class Function;
}

using namespace llvm;

typedef struct _dim3
{
  bool x=false;
  bool y=false;
  bool z=false;

  std::string get_dataset()
  {
    int counter=0;
    if(x)
      counter++;
    if(y)
      counter++;
    if(z)
      counter++;
    if(counter==1)
      return std::string("dataset");
    else if(counter==2)
      return std::string("dataset*dataset");
    else if(counter==3)
      return std::string("dataset*dataset*dataset");
    else
      return std::string("none");
  }
} dim3;

class FeatureCollector {
public:
  FeatureCollector();

public:
  void dump();

public:
  std::map<std::string, int> instTypes;
  std::map<int,std::string> argIndexElementTypeMap;
  std::map<int,std::string> argIndexBufferTypeMap;
  std::map<int,dim3> argIndexElementDimMap;
  std::map<std::string,std::vector<std::string> > arrayAccessExpressions;
  // Overall counters.
  // Map block with estimated ILP.
  //std::map<std::string, float> blockILP;
  std::vector<float> blockILP;
  void computeILP(BasicBlock *blockblock);

  // Map block with estimated MLP: avg and variance.
  //std::vector<std::pair<float, float> > blockMLP;
  std::vector<float> blockMLP;
  void computeMLP(BasicBlock *block);

  // Map block with number of incoming edges.
  std::map<std::string, int> blockIncoming;
  void countIncomingEdges(const BasicBlock &block);

  // Map block with number of outgoing edges.
  std::map<std::string, int> blockOutgoing;
  void countOutgoingEdges(const BasicBlock &block);

  // Number of instructions per block.
  //std::map<std::string, int> blockInsts;
  std::vector<int> blockInsts;
  void countInstsBlock(const BasicBlock &block);

  // Function calls.
  void countBarriers(const BasicBlock &block);
  void countMathFunctions(const BasicBlock &block);

  // Liveness ranges.
  std::vector<int> aliveOutBlocks;
  std::vector<float> avgLiveRange;
  void livenessAnalysis(BasicBlock &block);
//  void coalescingAnalysis(BasicBlock &block, ScalarEvolution *SE, OCLEnv *OCL,
//                          int CoarseningDirection);

  // Phis.
  // Map phi name with arg number.
  std::map<std::string, int> phiArgs;
  // Map block with phi.
  std::map<std::string, std::vector<std::string> > blockPhis;
  void countPhis(const BasicBlock &block);
  // Average of arguments for a phi-node.

  // Constants.
  void countConstants(const BasicBlock &block);
  // Local memory usage.
  void countLocalMemoryUsage(const BasicBlock &block);

  // Function counters.
  //void countDimensions(NDRange *NDR);
  void countEdges(const Function &function);
  void countBranches(const Function &function);
  void getFunctionSignature(const Function &function);
  void getAccessExpressions(Function &function, NDRange *ndr, LoopInfo *loopInfo, PostDominatorTree *pdt);
  void getFunctionTids(Function &function,NDRange *ndr);
  void countDivInsts(Function &function, MultiDimDivAnalysis *mdda,
                     SingleDimDivAnalysis *sdda);
  void countArgs(const Function &function);

  // Loop Function counters.
  void loopCountEdges(const Function &function, LoopInfo *LI);
  void loopCountBranches(const Function &function, LoopInfo *LI);
  void loopCountDivInsts(Function &function, MultiDimDivAnalysis *mdda,
                         SingleDimDivAnalysis *sdda, LoopInfo *LI);
};

#endif
