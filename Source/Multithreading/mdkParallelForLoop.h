#pragma once

#include <vector>
#include <thread>

#include "mdkCommonType.h"
#include "mdkDebugConfig.h"

namespace mdk
{
//------------------------------------------------- ParallelForLoop ----------------------------------------------------------------------------//
//  for(int_max k=0; k<L; ++k)
//	{
//     DoSomething(k);
//	}
//
// If DoSomething(i) and DoSomething(j) do not depend on each other for any i & j, then use ParallelForLoop
//
//  //for(int_max k=0; k<L; ++k), put here for reference
//  auto TempFunction = [](int_max k)
//	{
//      DoSomething(k);
//	};
//  ParallelForLoop(TempFunction, 0, L-1, MaxThreadCount);
//
// for performance reason, no input check
//
// Attention : LoopIndex_start <= LoopIndex_end
template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, int_max MaxThreadCount, int_max MinDataCountPerThread = 1);

template<typename FunctionType>
inline
void ParallelForLoop_Block_in_a_thread(FunctionType SingleFunction, int_max SubLoopIndex_start, int_max SubLoopIndex_end, int_max ThreadIndex);

template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, const std::vector<int_max>& LoopIndexList, int_max MaxThreadCount, int_max MinDataCountPerThread = 1);

template<typename FunctionType>
inline
void ParallelForLoop_Block_in_a_thread(FunctionType SingleFunction, const std::vector<int_max>& SubLoopIndexList, int_max ThreadIndex);

//------------------------------------------------- ParallelForLoop_WithThreadIndex ------------------------------------------------------------//
//  for(int_max k=0; k<L; ++k)
//	{
//     DoSomething(k, 0);
//	}
//
// If DoSomething(i) and DoSomething(j) do not depend on each other for any i & j, then use ParallelForLoop
//
//  //for(int_max k=0; k<L; ++k), put here for reference
//  auto TempFunction = [](int_max k, int_max ThreadIndex)
//	{
//      DoSomething(k, ThreadIndex);
//	};
//  ParallelForLoop_WithThreadIndex(TempFunction, 0, L-1);
//
// for performance reason, no input check
//
// Attention : LoopIndex_start <= LoopIndex_end
template<typename FunctionType>
inline
void ParallelForLoop_WithThreadIndex(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, int_max MaxThreadCount, int_max MinDataCountPerThread = 1);

template<typename FunctionType>
inline
void ParallelForLoop_WithThreadIndex_Block_in_a_thread(FunctionType SingleFunction, int_max SubLoopIndex_start, int_max SubLoopIndex_end, int_max ThreadIndex);

template<typename FunctionType>
inline
void ParallelForLoop_WithThreadIndex(FunctionType SingleFunction, const std::vector<int_max>& LoopIndexList, int_max MaxThreadCount, int_max MinDataCountPerThread = 1);

template<typename FunctionType>
inline
void ParallelForLoop_WithThreadIndex_Block_in_a_thread(FunctionType SingleFunction, const std::vector<int_max>& SubLoopIndexList, int_max ThreadIndex);

//-------------------------------------------------- ParallelBlock ----------------------------------------------------------------------//
// for performance reason, no input check

// Attention : DataIndex_start <= DataIndex_end
template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, int_max DataIndex_start, int_max DataIndex_end, int_max MaxThreadCount, int_max MinDataCountPerThread = 1);

template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, const std::vector<int_max>& DataIndexList, int_max MaxThreadCount, int_max MinDataCountPerThread = 1);

inline int_max Compute_Optimal_ThreadCount_For_ParallelBlock(int_max TotalDataCount, int_max MaxThreadCount, int_max MinDataCountPerThread = 1);

inline void DivideData_For_ParallelBlock(std::vector<int_max>& DataIndexList_start, std::vector<int_max>& DataIndexList_end,
                                         int_max DataIndex_min, int_max DataIndex_max, 
                                         int_max MaxThreadCount, int_max MinDataCountPerThread = 1);

}//namespace mdk

#include "mdkParallelForLoop.hpp"
