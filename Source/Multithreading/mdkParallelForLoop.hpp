#ifndef __mdkParallelForLoop_hpp
#define __mdkParallelForLoop_hpp

namespace mdk
{

template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, std::vector<int_max> LoopIndexList, int_max MaxNumberOfThreads)
{
    // divide the LoopIndexList into groups

    auto TotalLoopNumber = int_max(LoopIndexList.size());

    if (TotalLoopNumber > 1 && MaxNumberOfThreads > 1)
    {
        std::vector<int_max> IndexList_start;
        std::vector<int_max> IndexList_end;

        ParallelForLoop_DivideData(IndexList_start, IndexList_end, 0, TotalLoopNumber - 1, MaxNumberOfThreads);

        auto ThreadNumber = int_max(IndexList_start.size());

        // create and start the threads
        std::vector<std::thread> ThreadList(ThreadNumber);

        for (int_max i = 0; i < ThreadNumber; ++i)
        {
            auto subNumber = IndexList_end[i] - IndexList_start[i] + 1;
            std::vector<int_max> SubLoopIndexList(subNumber);
            for (int_max k = 0; k < subNumber; ++k)
            {
                SubLoopIndexList[k] = LoopIndexList[IndexList_start[i] + k];
            }

            ThreadList[i] = std::thread([&]{ParallelForLoop_InnerLoop(SingleFunction, std::move(SubLoopIndexList));});

            // this will crash, i can be ThreadNumber
            //ThreadList[i] = std::thread([&]{this->GenerateCode_in_a_Thread(IndexList_start[i], IndexList_end[i]); });
        }

        //wait for all the threads
        for (int_max i = 0; i < ThreadNumber; ++i)
        {
            ThreadList[i].join();
        }
    }
    else//single-thread
    {
        ParallelForLoop_InnerLoop(SingleFunction, LoopIndexList);
    }
}


template<typename FunctionType>
inline
void ParallelForLoop_InnerLoop(FunctionType SingleFunction, std::vector<int_max> SubLoopIndexList)
{
    for (int_max i = 0; i < int_max(SubLoopIndexList.size()); ++i)
    {
        SingleFunction(SubLoopIndexList[i]);
    }
}


template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end, int_max MaxNumberOfThreads)
{
    // divide the LoopIndexList into groups

    auto TotalLoopNumber = LoopIndex_end - LoopIndex_start + 1;

    if (TotalLoopNumber > 1 && MaxNumberOfThreads > 1)
    {
        std::vector<int_max> IndexList_start;
        std::vector<int_max> IndexList_end;

        ParallelForLoop_DivideData(IndexList_start, IndexList_end, 0, TotalLoopNumber - 1, MaxNumberOfThreads);

        auto ThreadNumber = int_max(IndexList_start.size());

        // create and start the threads
        std::vector<std::thread> ThreadList(ThreadNumber);

        for (int_max i = 0; i < ThreadNumber; ++i)
        {
            auto SubLoopIndex_start = IndexList_start[i];
            auto SubLoopIndex_end = IndexList_end[i];

            ThreadList[i] = std::thread([&]{ParallelForLoop_InnerLoop(SingleFunction, SubLoopIndex_start, SubLoopIndex_end);});

            // this will crash, i can be ThreadNumber
            //ThreadList[i] = std::thread([&]{this->GenerateCode_in_a_Thread(IndexList_start[i], IndexList_end[i]); });
        }

        //wait for all the threads
        for (int_max i = 0; i < ThreadNumber; ++i)
        {
            ThreadList[i].join();
        }
    }
    else//single-thread
    {
        ParallelForLoop_InnerLoop(SingleFunction, LoopIndex_start, LoopIndex_end);
    }
}


template<typename FunctionType>
inline
void ParallelForLoop_InnerLoop(FunctionType SingleFunction, int_max SubLoopIndex_start, int_max SubLoopIndex_end)
{
    for (int_max i = SubLoopIndex_start; i <= SubLoopIndex_end; ++i)
    {
        SingleFunction(i);
    }
}


inline void ParallelForLoop_DivideData(std::vector<int_max>& IndexList_start, std::vector<int_max>& IndexList_end,
                                       int_max Index_min, int_max Index_max, int_max MaxNumberOfThreads)
{
    if (MaxNumberOfThreads == 1)
    {
        IndexList_start.push_back(Index_min);
        IndexList_end.push_back(Index_max);
        return;
    }

    auto TotalDataNumber = Index_max - Index_min + 1;

    int_max ThreadNumber = 1;

    int_max DataNumberPerThread = 0;

    for (int_max i = MaxNumberOfThreads; i >= 1; --i)
    {
        DataNumberPerThread = TotalDataNumber / i;

        if (DataNumberPerThread >= 1)
        {
            ThreadNumber = i;
            break;
        }
    }

    if (ThreadNumber == 1)
    {//one thread is enough

        IndexList_start.push_back(Index_min);
        IndexList_end.push_back(Index_max);
        return;
    }

    IndexList_start.resize(ThreadNumber);
    IndexList_end.resize(ThreadNumber);

    int_max tempIndex = Index_min;

    for (int_max i = 0; i < ThreadNumber; ++i)
    {
        IndexList_start[i] = tempIndex;
        IndexList_end[i] = tempIndex + DataNumberPerThread - 1;

        tempIndex += DataNumberPerThread;
    }

    IndexList_end[ThreadNumber - 1] = Index_max;
}


}//namespace mdk

#endif