#ifndef __mdkParallelForLoop_hpp
#define __mdkParallelForLoop_hpp

namespace mdk
{

template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, const std::vector<int_max>& LoopIndexList, int_max MaxNumberOfThreads, int_max MinNumberOfDataPerThread)
{
    ParallelBlock([&](std::vector<int_max> SubLoopIndexList, int_max ThreadIndex)
                  {ParallelForLoop_Block_in_a_thread(SingleFunction, SubLoopIndexList, ThreadIndex); },
                  DataIndexList, MaxNumberOfThreads, MinNumberOfDataPerThread);
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

// Attention : 
// if LoopIndex_end >= LoopIndex_start, the range is  [LoopIndex_start:1:LoopIndex_end] : for(int_max k = LoopIndex_start; k <= LoopIndex_end; ++k)
// if LoopIndex_end < LoopIndex_start, the range is  [LoopIndex_start:-1:LoopIndex_end] : for(int_max k = LoopIndex_start; k >= LoopIndex_end; --k)

template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, 
                     int_max MaxNumberOfThreads, int_max MinNumberOfDataPerThread)
{
    ParallelBlock([&](int_max Index_start, int_max Index_end, int_max ThreadIndex)
                  {ParallelForLoop_Block_in_a_thread(SingleFunction, Index_start, Index_end, ThreadIndex); },
                  LoopIndex_start, LoopIndex_end, MaxNumberOfThreads, MinNumberOfDataPerThread);
}


template<typename FunctionType>
inline
void ParallelForLoop_Block_in_a_thread(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, int_max ThreadIndex)
{
	if (LoopIndex_start <= LoopIndex_end)
	{
		for (int_max i = LoopIndex_start; i <= LoopIndex_end; ++i)
		{
			SingleFunction(i);
		}
	}
	else
	{
		for (int_max i = LoopIndex_start; i >= LoopIndex_end; --i)
		{
			SingleFunction(i);
		}
	}
}


template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, const std::vector<int_max>& DataIndexList, int_max MaxNumberOfThreads, int_max MinNumberOfDataPerThread)
{
    // divide the LoopIndexList into groups

    auto TotalDataNumber = int_max(DataIndexList.size());

    if (TotalDataNumber > 1 && MaxNumberOfThreads > 1)
    {
        std::vector<int_max> IndexList_start;
        std::vector<int_max> IndexList_end;

        DivideData_For_ParallelBlock(IndexList_start, IndexList_end, 0, TotalDataNumber - 1, MaxNumberOfThreads, MinNumberOfDataPerThread);

        auto ThreadNumber = int_max(IndexList_start.size());

        // create and start the threads
		std::vector<std::thread> ThreadList;
		ThreadList.reserve(ThreadNumber);

        for (int_max i = 0; i < ThreadNumber; ++i)
        {
            auto subNumber = IndexList_end[i] - IndexList_start[i] + 1;

            std::vector<int_max> SubDataIndexList(subNumber);

            for (int_max k = 0; k < subNumber; ++k)
            {
                SubDataIndexList[k] = DataIndexList[IndexList_start[i] + k];
            }

			try
			{
				ThreadList.emplace_back(BlockFunction(std::move(SubDataIndexList), Index_start, Index_end, i));
			}
			catch (...)
			{
				MDK_Error("The thread: " << i << " could not be started")
			}
        }

		if (int_max(ThreadList.size()) < ThreadNumber)
		{
			MDK_Error("ThreadList.size() < ThreadNumber @ ParallelBlock(...)")
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


template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, int_max DataIndex_start, int_max DataIndex_end, int_max MaxNumberOfThreads, int_max MinNumberOfDataPerThread)
{
    // divide the LoopIndexList into groups

    auto TotalDataNumber = DataIndex_end - DataIndex_start +1;

    if (TotalDataNumber > 1 && MaxNumberOfThreads > 1)
    {
        std::vector<int_max> IndexList_start;
        std::vector<int_max> IndexList_end;

        DivideData_For_ParallelBlock(IndexList_start, IndexList_end, 0, TotalDataNumber - 1, MaxNumberOfThreads, MinNumberOfDataPerThread);

        int_max ThreadNumber = int_max(IndexList_start.size());

        // create and start the threads
        std::vector<std::thread> ThreadList;
        ThreadList.reserve(ThreadNumber);

        for (int_max i = 0; i < ThreadNumber; ++i)
        {
			int_max ThreadIndex = i;
            int_max Index_start = IndexList_start[i];
            int_max Index_end   = IndexList_end[i];
			if (DataIndex_start <= DataIndex_end)
			{
				Index_start += DataIndex_start;
				Index_end += DataIndex_start;
			}
			else
			{
				Index_start = DataIndex_start - Index_start;
				Index_end = DataIndex_start - Index_end;
			}

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

            // this will crash, i can be equal to ThreadNumber
            //ThreadList[i] = std::thread([&]{BlockFunction(IndexList_start[i], IndexList_end[i], ThreadIndex); });
        }

        if (int_max(ThreadList.size()) < ThreadNumber)
        {
            MDK_Error("ThreadList.size() < ThreadNumber @ ParallelBlock(...)")
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


inline int_max Compute_NumberOfThreadsTobeCreated_For_ParallelBlock(int_max TotalDataNumber, int_max MaxNumberOfThreads, int_max MinNumberOfDataPerThread)
{
    if (MaxNumberOfThreads <= 1)
    {       
        return 1;
    }

    int_max ThreadNumber = 1;

    int_max DataNumberPerThread = 0;

    for (int_max i = MaxNumberOfThreads; i >= 1; --i)
    {
        DataNumberPerThread = TotalDataNumber / i;

        if (DataNumberPerThread >= MinNumberOfDataPerThread)
        {
            ThreadNumber = i;
            break;
        }
    }

    return ThreadNumber;
}


inline void DivideData_For_ParallelBlock(std::vector<int_max>& DataIndexList_start, std::vector<int_max>& DataIndexList_end,
                                         int_max DataIndex_min, int_max DataIndex_max, 
                                         int_max MaxNumberOfThreads, int_max MinNumberOfDataPerThread)
{
    if (MaxNumberOfThreads <= 1)
    {
        DataIndexList_start.push_back(DataIndex_min);
        DataIndexList_end.push_back(DataIndex_max);
        return;
    }

    auto TotalDataNumber = DataIndex_max - DataIndex_min + 1;

    auto ThreadNumber = Compute_NumberOfThreadsTobeCreated_For_ParallelBlock(TotalDataNumber, MaxNumberOfThreads, MinNumberOfDataPerThread);

    if (ThreadNumber == 1)
    {//one thread is enough

        DataIndexList_start.push_back(DataIndex_min);
        DataIndexList_end.push_back(DataIndex_max);
        return;
    }

    DataIndexList_start.resize(ThreadNumber);
    DataIndexList_end.resize(ThreadNumber);

    int_max DataNumberPerThread = TotalDataNumber / ThreadNumber;

    int_max tempIndex = DataIndex_min;

    for (int_max i = 0; i < ThreadNumber; ++i)
    {
        DataIndexList_start[i] = tempIndex;
        DataIndexList_end[i]   = tempIndex + DataNumberPerThread - 1;

        tempIndex += DataNumberPerThread;
    }

    DataIndexList_end[ThreadNumber - 1] = DataIndex_max;
}


}//namespace mdk

#endif