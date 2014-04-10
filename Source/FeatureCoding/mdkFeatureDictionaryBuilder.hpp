#ifndef __mdkFeatureDictionaryBuilder_hpp
#define __mdkFeatureDictionaryBuilder_hpp

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBuilder<ElementType>::FeatureDictionaryBuilder()
{
}


template<typename ElementType>
FeatureDictionaryBuilder<ElementType>::~FeatureDictionaryBuilder()
{
}


template<typename ElementType>
bool FeatureDictionaryBuilder<ElementType>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    //--------------------------------------------------

    this->GenerateDictionary();

    //--------------------------------------------------

    this->UpdatePipelineOutput();

    //--------------------------------------------------

    return true;
}


}// namespace mdk


#endif