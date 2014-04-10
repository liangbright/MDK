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
bool FeatureDictionaryBasedEncoder<ElementType>::CheckInput()
{
    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::Preprocess()
{
    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::Postprocess()
{
    return true;
}


template<typename ElementType>
bool FeatureDictionaryBasedEncoder<ElementType>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    if (this->Preprocess() == false)
    {
        MDK_Error("Preprocess() return false @ FeatureDictionaryBasedEncoder::Update()")
        return false;
    }

    int_max FeatureVectorNumber = this->GetTotalNumberOfInputFeatureVectors();

    // multi-thread -----------------------------------------------------------------

    ParallelBlock([&](int_max Index_start, int_max Index_end){this->GenerateCode_in_a_Thread(Index_start, Index_end); }, 
                  0, FeatureVectorNumber - 1, this->GetMaxNumberOfThreads());
    //------------------------------------------------------------

    if (this->Postprocess() == false)
    {
        MDK_Error("Postprocess() return false @ FeatureDictionaryBasedEncoder::Update()")
        return false;
    }

    this->UpdatePipelineOutput();

    return true;
}

}

#endif