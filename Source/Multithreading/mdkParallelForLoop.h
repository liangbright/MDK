#ifndef mdk_ParallelForLoop_h
#define mdk_ParallelForLoop_h

#include <vector>
#include <thread>

#include "mdkCommonType.h"
#include "mdkDebugConfig.h"

namespace mdk
{
//  for(int_max k=0; k<L; ++k)
//	{
//     DoSomething(k);
//	}
//
// If DoSomething(i) and DoSomething(j) do not depend on each other for any i & j, then use ParallelForLoop
//
//  auto TempFunction = [](int_max k)
//	{
//      DoSomething(k);
//	};
//  ParallelForLoop(TempFunction, 0, L-1);
//
// for performance reason, no input check in ParallelForLoop or ParallelBlock

template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, const std::vector<int_max>& LoopIndexList, int_max MaxNumberOfThread, int_max MinNumberOfDataPerThread = 1);

template<typename FunctionType>
inline
void ParallelForLoop_Block_in_a_thread(FunctionType SingleFunction, const std::vector<int_max>& SubLoopIndexList, int_max ThreadIndex);

// Attention : LoopIndex_start <= LoopIndex_end
template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, 
                     int_max MaxNumberOfThread, int_max MinNumberOfDataPerThread = 1);

template<typename FunctionType>
inline
void ParallelForLoop_Block_in_a_thread(FunctionType SingleFunction, int_max SubLoopIndex_start, int_max SubLoopIndex_end, int_max ThreadIndex);

//------------------------------------------------------------------------------------------------------------------------------------//

template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, const std::vector<int_max>& DataIndexList, int_max MaxNumberOfThread, int_max MinNumberOfDataPerThread = 1);

// Attention : DataIndex_start <= DataIndex_end
template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, int_max DataIndex_start, int_max DataIndex_end, int_max MaxNumberOfThread, int_max MinNumberOfDataPerThread = 1);

inline int_max Compute_NumberOfThreadTobeCreated_For_ParallelBlock(int_max TotalDataNumber, int_max MaxNumberOfThread, int_max MinNumberOfDataPerThread = 1);

inline void DivideData_For_ParallelBlock(std::vector<int_max>& DataIndexList_start, std::vector<int_max>& DataIndexList_end,
                                         int_max DataIndex_min, int_max DataIndex_max, 
                                         int_max MaxNumberOfThread, int_max MinNumberOfDataPerThread = 1);

}//namespace mdk

#include "mdkParallelForLoop.hpp"

#endif