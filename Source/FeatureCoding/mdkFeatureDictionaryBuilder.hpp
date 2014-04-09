#ifndef __mdkFeatureDictionaryBuilder_hpp
#define __mdkFeatureDictionaryBuilder_hpp

namespace mdk
{

template<typename ElementType>
FeatureDictionaryBuilder<ElementType>::FeatureDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
FeatureDictionaryBuilder<ElementType>::~FeatureDictionaryBuilder()
{
}


template<typename ElementType>
void FeatureDictionaryBuilder<ElementType>::Clear()
{
    m_FeatureData = nullptr;

    if (m_Dictionary_SharedCopy)
    {
        m_Dictionary_SharedCopy->Clear();
    }
}

//---------------------------------------------------//

template<typename ElementType>
bool FeatureDictionaryBuilder<ElementType>::SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData)
{
    if (InputFeatureData == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBuilder::SetInputFeatureData(InputFeatureData)")
        return false;
    }

    m_FeatureData = InputFeatureData;

    return true;
}

//---------------------------------------------------//

template<typename ElementType>
bool FeatureDictionaryBuilder<ElementType>::SetOutputDictionary(FeatureDictionary<ElementType>* OutputDictionary)
{
    if (OutputDictionary == nullptr)
    {
        MDK_Error("Invalid input @ FeatureDictionaryBuilder::SetOutputDictionary(OutputDictionary)")
        return false;
    }

    m_Dictionary = OutputDictionary;

    m_Dictionary_SharedCopy->Share(OutputDictionary);

    return true;
}

//---------------------------------------------------------------------------------------------------------------//

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