#pragma once

namespace mdk
{
//------------------------------------------------- ParallelForLoop ----------------------------------------------------------------------------//
template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, const std::vector<int_max>& LoopIndexList, int_max MaxThreadCount, int_max MinDataCountPerThread)
{
    ParallelBlock([&](std::vector<int_max> SubLoopIndexList, int_max ThreadIndex)
                  {ParallelForLoop_Block_in_a_thread(SingleFunction, SubLoopIndexList, ThreadIndex); },
				  LoopIndexList, MaxThreadCount, MinDataCountPerThread);
}


template<typename FunctionType>
inline
void ParallelForLoop_Block_in_a_thread(FunctionType SingleFunction, std::vector<int_max> SubLoopIndexList, int_max ThreadIndex)
{
    for (int_max i = 0; i < int_max(SubLoopIndexList.size()); ++i)
    {
        SingleFunction(SubLoopIndexList[i]);
    }
}

// Attention : LoopIndex_start <= LoopIndex_end
template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, 
                     int_max MaxThreadCount, int_max MinDataCountPerThread)
{
    ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex)
                  {ParallelForLoop_Block_in_a_thread(SingleFunction, Index_start, Index_end, ThreadIndex); },
                  LoopIndex_start, LoopIndex_end, MaxThreadCount, MinDataCountPerThread);
}


template<typename FunctionType>
inline
void ParallelForLoop_Block_in_a_thread(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, int_max ThreadIndex)
{
	for (int_max i = LoopIndex_start; i <= LoopIndex_end; ++i)
	{
		SingleFunction(i);
	}
}

//------------------------------------------------- ParallelForLoop_WithThreadIndex ----------------------------------------------------------------------------//

template<typename FunctionType>
inline
void ParallelForLoop_WithThreadIndex(FunctionType SingleFunction, const std::vector<int_max>& LoopIndexList, int_max MaxThreadCount, int_max MinDataCountPerThread)
{
	ParallelBlock([&](std::vector<int_max> SubLoopIndexList, int_max ThreadIndex)
	              {ParallelForLoop_WithThreadIndex_Block_in_a_thread(SingleFunction, SubLoopIndexList, ThreadIndex); },
	              LoopIndexList, MaxThreadCount, MinDataCountPerThread);
}


template<typename FunctionType>
inline
void ParallelForLoop_WithThreadIndex_Block_in_a_thread(FunctionType SingleFunction, std::vector<int_max> SubLoopIndexList, int_max ThreadIndex)
{
	for (int_max i = 0; i < int_max(SubLoopIndexList.size()); ++i)
	{
		SingleFunction(SubLoopIndexList[i], ThreadIndex);
	}
}

// Attention : LoopIndex_start <= LoopIndex_end
template<typename FunctionType>
inline
void ParallelForLoop_WithThreadIndex(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end,
int_max MaxThreadCount, int_max MinDataCountPerThread)
{
	ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex)
	              {ParallelForLoop_WithThreadIndex_Block_in_a_thread(SingleFunction, Index_start, Index_end, ThreadIndex); },
	              LoopIndex_start, LoopIndex_end, MaxThreadCount, MinDataCountPerThread);
}


template<typename FunctionType>
inline
void ParallelForLoop_WithThreadIndex_Block_in_a_thread(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, int_max ThreadIndex)
{
	for (int_max i = LoopIndex_start; i <= LoopIndex_end; ++i)
	{
		SingleFunction(i, ThreadIndex);
	}
}

//------------------------------------------------- ParallelBlock ----------------------------------------------------------------------------//

template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, const std::vector<int_max>& DataIndexList, int_max MaxThreadCount, int_max MinDataCountPerThread)
{
    // divide DataIndexList into groups

    auto TotalDataCount = int_max(DataIndexList.size());

    if (TotalDataCount > 1 && MaxThreadCount > 1)
    {
        std::vector<int_max> IndexList_start;
        std::vector<int_max> IndexList_end;

        DivideData_For_ParallelBlock(IndexList_start, IndexList_end, 0, TotalDataCount - 1, MaxThreadCount, MinDataCountPerThread);

        auto ThreadCount = int_max(IndexList_start.size());

        // create and start the threads
		std::vector<std::thread> ThreadList;
		ThreadList.reserve(ThreadCount);

        for (int_max i = 0; i < ThreadCount; ++i)
        {
			std::vector<int_max> SubDataIndexList;
			auto SubDataCount = IndexList_end[i] - IndexList_start[i] + 1;			
   			SubDataIndexList.resize(SubDataCount);
			for (int_max k = 0; k < SubDataCount; ++k)
            {
				SubDataIndexList[k] = DataIndexList[IndexList_start[i] + k];
            }

			try
			{
				ThreadList.emplace_back(BlockFunction(std::move(SubDataIndexList), i));
			}
			catch (...)
			{
				MDK_Error("The thread: " << i << " could not be started")
			}
        }

		if (int_max(ThreadList.size()) < ThreadCount)
		{
			MDK_Error("ThreadList.size() < ThreadCount @ ParallelBlock(...)")
		}

        //wait for all the threads
		for (int_max i = 0; i < int_max(ThreadList.size()); ++i)
        {
            ThreadList[i].join();
        }
    }
    else//single-thread
    {
        BlockFunction(DataIndexList, 0);
    }
}

// Attention : DataIndex_start <= DataIndex_end
template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, int_max DataIndex_start, int_max DataIndex_end, int_max MaxThreadCount, int_max MinDataCountPerThread)
{
	// DataIndex_end must > DataIndex_start
    auto TotalDataCount = DataIndex_end - DataIndex_start +1;
	if (TotalDataCount <= 0)
	{
		MDK_Warning("TotalDataCount <= 0 @ mdkParallelForLoop ParallelBlock(...)")
		return;
	}

    if (TotalDataCount > 1 && MaxThreadCount > 1)
	{ // divide the DataIndex List into groups
        std::vector<int_max> IndexList_start;
        std::vector<int_max> IndexList_end;

        DivideData_For_ParallelBlock(IndexList_start, IndexList_end, 0, TotalDataCount - 1, MaxThreadCount, MinDataCountPerThread);

        int_max ThreadCount = int_max(IndexList_start.size());

        // create and start the threads
        std::vector<std::thread> ThreadList;
        ThreadList.reserve(ThreadCount);

        for (int_max i = 0; i < ThreadCount; ++i)
        {
			int_max ThreadIndex = i;
			int_max Index_start = DataIndex_start + IndexList_start[i];
			int_max Index_end = DataIndex_start + IndexList_end[i];

            try
            {
                // This has problem
                //ThreadList.emplace_back([&]{BlockFunction(Index_start, Index_end, ThreadIndex); });

                //This seems fine
                ThreadList.emplace_back(BlockFunction, Index_start, Index_end, ThreadIndex);
            }
            catch (...)
            {
                MDK_Error("The thread: " << ThreadIndex << " could not be started")
            }

            // this will crash, i can be equal to ThreadCount
            //ThreadList[i] = std::thread([&]{BlockFunction(IndexList_start[i], IndexList_end[i], ThreadIndex); });
        }

        if (int_max(ThreadList.size()) < ThreadCount)
        {
            MDK_Error("ThreadList.size() < ThreadCount @ ParallelBlock(...)")
        }

        //wait for all the threads
        for (int_max i = 0; i < int_max(ThreadList.size()); ++i)
        {
            ThreadList[i].join();
        }
    }
    else//single-thread
    {
        BlockFunction(DataIndex_start, DataIndex_end, 0);
    }
}


inline int_max Compute_Optimal_ThreadCount_For_ParallelBlock(int_max TotalDataCount, int_max MaxThreadCount, int_max MinDataCountPerThread)
{
    if (MaxThreadCount <= 1)
    {       
        return 1;
    }

    int_max ThreadCount = 1;

    int_max DataCountPerThread = 0;

    for (int_max i = MaxThreadCount; i >= 1; --i)
    {
        DataCountPerThread = TotalDataCount / i;

        if (DataCountPerThread >= MinDataCountPerThread)
        {
            ThreadCount = i;
            break;
        }
    }

    return ThreadCount;
}


inline void DivideData_For_ParallelBlock(std::vector<int_max>& DataIndexList_start, std::vector<int_max>& DataIndexList_end,
                                         int_max DataIndex_min, int_max DataIndex_max, 
                                         int_max MaxThreadCount, int_max MinDataCountPerThread)
{
    if (MaxThreadCount <= 1)
    {
        DataIndexList_start.push_back(DataIndex_min);
        DataIndexList_end.push_back(DataIndex_max);
        return;
    }

    auto TotalDataCount = DataIndex_max - DataIndex_min + 1;

	auto ThreadCount = Compute_Optimal_ThreadCount_For_ParallelBlock(TotalDataCount, MaxThreadCount, MinDataCountPerThread);

    if (ThreadCount == 1)
    {//one thread is enough
        DataIndexList_start.push_back(DataIndex_min);
        DataIndexList_end.push_back(DataIndex_max);
        return;
    }

    DataIndexList_start.resize(ThreadCount);
    DataIndexList_end.resize(ThreadCount);

    int_max DataCountPerThread = TotalDataCount / ThreadCount;

    int_max tempIndex = DataIndex_min;

    for (int_max i = 0; i < ThreadCount; ++i)
    {
        DataIndexList_start[i] = tempIndex;
        DataIndexList_end[i]   = tempIndex + DataCountPerThread - 1;

        tempIndex += DataCountPerThread;
    }

    DataIndexList_end[ThreadCount - 1] = DataIndex_max;
}


}//namespace mdk
