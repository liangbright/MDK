#ifndef __mdkFeatureDictionaryBasedEncoder_hpp
#define __mdkFeatureDictionaryBasedEncoder_hpp

//#include "mdkFeatureDictionaryBasedEncoder.h"

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBasedEncoder<ElementType>::FeatureDictionaryBasedEncoder()
{

}


template<typename ElementType>
FeatureDictionaryBasedEncoder<ElementType>::~FeatureDictionaryBasedEncoder()
{

}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::IsDenseEncoder()
{
    return m_IsDenseEncoder;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::IsSparseEncoder()
{
    return !m_IsDenseEncoder;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::Update()
{
    // multi-thread -----------------------------------------------------------------

    // divide the input feature column-vectors into groups

    int64 FeatureVectorNumber = this->GetTotalNumberOfInputFeatureVectors();

    std::vector<int64> IndexList_start;
    std::vector<int64> IndexList_end;

    this->DivideData(0, FeatureVectorNumber, IndexList_start, IndexList_end);

    int64 ThreadNumber = IndexList_start.size();

    if (ThreadNumber > 1)
    {
        // create and start the threads
        std::vector<std::thread> ThreadList(ThreadNumber);

        for (int64 i = 0; i < ThreadNumber; ++i)
        {
            ThreadList[i] = std::thread([&]{this->GenerateCode_in_a_Thread(IndexList_start[i], IndexList_end[i]); });
        }

        //wait for all the threads
        for (int64 i = 0; i < ThreadNumber; ++i)
        {
            ThreadList[i].join();
        }
    }
    else//single-thread
    {
        this->GenerateCode_in_a_Thread(0, FeatureVectorNumber-1);
    }


    return true;
}


template<typename ElementType>
void FeatureDictionaryBasedEncoder<ElementType>::
DivideData(int64 Index_min, int64 Index_max, std::vector<int64>& IndexList_start, std::vector<int64>& IndexList_end)
{
    int64 MaximunNumberOfThreads = this->GetMaxNumberOfThreads();

    if (MaximunNumberOfThreads == 1)
    {
        IndexList_start.push_back(Index_min);
        IndexList_end.push_back(Index_max);
        return;
    }

    auto TotalDataNumber = Index_max - Index_min + 1;

    int64 ThreadNumber = 1;

    int64 DataNumberPerThread = 0;

    for (int64 i = MaximunNumberOfThreads; i >= 1; --i)
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

    int64 tempIndex = Index_min;

    for (int64 i = 0; i < ThreadNumber; ++i)
    {
        IndexList_start.push_back(tempIndex);
        IndexList_end.push_back(tempIndex + DataNumberPerThread - 1);

        tempIndex += DataNumberPerThread;
    }

    IndexList_end[ThreadNumber - 1] = Index_max;
}


template<typename ElementType>
void FeatureDictionaryBasedEncoder<ElementType>::GenerateCode_in_a_Thread(int64 IndexOfFeatureVector_start, int64 IndexOfFeatureVector_end)
{
    for (int64 i = IndexOfFeatureVector_start; i <= IndexOfFeatureVector_end; ++i)
    {
        this->EncodingFunction(i);
    }
}


}

#endif