#ifndef __mdkParallelForLoop_h
#define __mdkParallelForLoop_h

#include <vector>
#include <thread>


#include "mdkType.h"


namespace mdk
{

template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, std::vector<int_max> LoopIndexList, int_max MaxNumberOfThreads);

template<typename FunctionType>
inline
void ParallelForLoop_InnerLoop(FunctionType SingleFunction, std::vector<int_max> SubLoopIndexList);

template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, int_max MaxNumberOfThreads);

template<typename FunctionType>
inline
void ParallelForLoop_InnerLoop(FunctionType SingleFunction, int_max SubLoopIndex_start, int_max SubLoopIndex_end);

inline void ParallelForLoop_DivideData(std::vector<int_max>& IndexList_start, std::vector<int_max>& IndexList_end,
                                       int_max Index_min, int_max Index_max, int_max MaxNumberOfThreads);

}//namespace mdk

#include "mdkParallelForLoop.hpp"

#endif