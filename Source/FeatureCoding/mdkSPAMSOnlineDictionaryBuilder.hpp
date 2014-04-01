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

    m_Parameter.Clear();

    m_State.Clear();
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::SetInitialState(State_Of_SPAMSOnlineDictionaryBuilder<ElementType> InitialState)
{
    //check InitialState

    m_State.Take(InitialState);

    return true;
}


template<typename ElementType>
State_Of_SPAMSOnlineDictionaryBuilder<ElementType>*
SPAMSOnlineDictionaryBuilder<ElementType>::GetCurrentState()
{
    return &m_State;
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::SaveStateAndParameter(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::LoadStateAndParameter(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{

}



}// namespace mdk


#endif