#ifndef __mdkParallelForLoop_hpp
#define __mdkParallelForLoop_hpp

namespace mdk
{

template<typename FunctionType>
inline
void ParallelForLoop(FunctionType SingleFunction, const std::vector<int_max>& LoopIndexList, int_max MaxNumberOfThreads)
{
    ParallelBlock([&](std::vector<int_max> SubLoopIndexList){ParallelForLoop_SubBlock_in_a_thread(SingleFunction, SubLoopIndexList); },
                  DataIndexList, MaxNumberOfThreads);
}


template<typename FunctionType>
inline
void ParallelForLoop_SubBlock_in_a_thread(FunctionType SingleFunction, std::vector<int_max> SubLoopIndexList)
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
    ParallelBlock([&](int_max Index_start, int_max Index_end){ParallelForLoop_SubBlock_in_a_thread(SingleFunction, Index_start, Index_end); },
                  LoopIndex_start, LoopIndex_end, MaxNumberOfThreads);    
}


template<typename FunctionType>
inline
void ParallelForLoop_SubBlock_in_a_thread(FunctionType SingleFunction, int_max LoopIndex_start, int_max LoopIndex_end)
{
    for (int_max i = LoopIndex_start; i <= LoopIndex_end; ++i)
    {
        SingleFunction(i);
    }
}


template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, const std::vector<int_max>& DataIndexList, int_max MaxNumberOfThreads)
{
    // divide the LoopIndexList into groups

    auto TotalDataNumber = int_max(DataIndexList.size());

    if (TotalDataNumber > 1 && MaxNumberOfThreads > 1)
    {
        std::vector<int_max> IndexList_start;
        std::vector<int_max> IndexList_end;

        DivideData_For_ParallelComputation(IndexList_start, IndexList_end, 0, TotalDataNumber - 1, MaxNumberOfThreads);

        auto ThreadNumber = int_max(IndexList_start.size());

        // create and start the threads
        std::vector<std::thread> ThreadList(ThreadNumber);

        for (int_max i = 0; i < ThreadNumber; ++i)
        {
            auto subNumber = IndexList_end[i] - IndexList_start[i] + 1;

            std::vector<int_max> SubDataIndexList(subNumber);

            for (int_max k = 0; k < subNumber; ++k)
            {
                SubDataIndexList[k] = DataIndexList[IndexList_start[i] + k];
            }

            ThreadList[i] = std::thread([&]{BlockFunction(std::move(SubDataIndexList)); });

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
        BlockFunction(DataIndexList);
    }
}


template<typename FunctionType>
inline
void ParallelBlock(FunctionType BlockFunction, int_max DataIndex_start, int_max DataIndex_end, int_max MaxNumberOfThreads)
{
    // divide the LoopIndexList into groups

    auto TotalDataNumber = DataIndex_end - DataIndex_start +1;

    if (TotalDataNumber > 1 && MaxNumberOfThreads > 1)
    {
        std::vector<int_max> IndexList_start;
        std::vector<int_max> IndexList_end;

        DivideData_For_ParallelComputation(IndexList_start, IndexList_end, 0, TotalDataNumber - 1, MaxNumberOfThreads);

        auto ThreadNumber = int_max(IndexList_start.size());

        // create and start the threads
        std::vector<std::thread> ThreadList(ThreadNumber);

        for (int_max i = 0; i < ThreadNumber; ++i)
        {
            auto SubDataIndex_start = IndexList_start[i];
            auto SubDataIndex_end = IndexList_end[i];

            ThreadList[i] = std::thread([&]{BlockFunction(SubDataIndex_start, SubDataIndex_end); });

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
        BlockFunction(DataIndex_start, DataIndex_end);
    }
}


inline void DivideData_For_ParallelComputation(std::vector<int_max>& IndexList_start, std::vector<int_max>& IndexList_end,
                                               int_max DataIndex_min, int_max DataIndex_max, int_max MaxNumberOfThreads)
{
    if (MaxNumberOfThreads == 1)
    {
        IndexList_start.push_back(DataIndex_min);
        IndexList_end.push_back(DataIndex_max);
        return;
    }

    auto TotalDataNumber = DataIndex_max - DataIndex_min + 1;

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

        IndexList_start.push_back(DataIndex_min);
        IndexList_end.push_back(DataIndex_max);
        return;
    }

    IndexList_start.resize(ThreadNumber);
    IndexList_end.resize(ThreadNumber);

    int_max tempIndex = DataIndex_min;

    for (int_max i = 0; i < ThreadNumber; ++i)
    {
        IndexList_start[i] = tempIndex;
        IndexList_end[i] = tempIndex + DataNumberPerThread - 1;

        tempIndex += DataNumberPerThread;
    }

    IndexList_end[ThreadNumber - 1] = DataIndex_max;
}


}//namespace mdk

#endif