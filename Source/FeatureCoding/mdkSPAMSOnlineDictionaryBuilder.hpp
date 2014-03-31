#ifndef __mdkSPAMSOnlineDictionaryBuilder_hpp
#define __mdkSPAMSOnlineDictionaryBuilder_hpp


#include <opencv2/core/core.hpp>

//#include "mdkSPAMSOnlineDictionaryBuilder.h"

namespace mdk
{

template<typename ElementType>
SPAMSOnlineDictionaryBuilder<ElementType>::SPAMSOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
SPAMSOnlineDictionaryBuilder<ElementType>::~SPAMSOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void SPAMSOnlineDictionaryBuilder<ElementType>::Clear()
{
    this->FeatureDictionaryBuilder::Clear();

    m_InitialDictionary = nullptr;

    m_DictionaryLength = 0;
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::SetInitialDictionary(const FeatureDictionary<ElementType>* InitialDictionary)
{
    m_InitialDictionary = InitialDictionary;
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::SetOutputDictionaryLength(int64 DictionaryLength)
{
    if (DictionaryLength <= 0)
    {
        MDK_Error("Invalid Input @ SPAMSOnlineDictionaryBuilder::SetOutputDictionaryLength(DictionaryLength)")
        return false;
    }

    m_DictionaryLength = DictionaryLength;

    return true;
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    bool IsOK = false;

    if (m_InitialDictionary == nullptr) // build a brand new dictionary
    {
        IsOK = this->FirstTimeBuild();
    }
    else // update the Dictionary
    {
        IsOK = this->OnlineUpdate();
    }

    return IsOK;
}




template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::OnlineUpdate()
{
    return true;
}


}// namespace mdk


#endif