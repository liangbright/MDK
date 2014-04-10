#ifndef __mdkFeatureDictionaryForSparseCoding_hpp
#define __mdkFeatureDictionaryForSparseCoding_hpp


namespace mdk
{

template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::FeatureDictionaryForSparseCoding()
{

}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::FeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::FeatureDictionaryForSparseCoding(FeatureDictionaryForSparseCoding&& InputDictionary)
{
    m_BasisMatrix = std::move(InputDictionary.m_BasisMatrix);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_StandardDeviation = std::move(InputDictionary.m_StandardDeviation);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::~FeatureDictionaryForSparseCoding()
{

}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::operator=(const FeatureDictionaryForSparseCoding& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::operator=(FeatureDictionaryForSparseCoding&& InputDictionary)
{
    m_BasisMatrix = std::move(InputDictionary.m_BasisMatrix);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_StandardDeviation = std::move(InputDictionary.m_StandardDeviation);
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Copy(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    m_BasisMatrix.Copy(InputDictionary.m_BasisMatrix);

    m_Covariance.Copy(InputDictionary.m_Covariance);

    m_StandardDeviation.Copy(InputDictionary.m_StandardDeviation);

    return true;

}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Copy(const FeatureDictionaryForSparseCoding<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForSparseCoding::Copy(FeatureDictionaryForSparseCoding*)")
        return false;
    }

    return this->Copy(*InputDictionary);
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Share(FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return true;
    }

    auto IsOK_1 = m_BasisMatrix.Share(InputDictionary.m_BasisMatrix);

    auto IsOK_2 = m_Covariance.Share(InputDictionary.m_Covariance);

    auto IsOK_3 = m_StandardDeviation.Share(InputDictionary.m_StandardDeviation);

    if (IsOK_1 == false || IsOK_2 == false || IsOK_3 == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Share(FeatureDictionaryForSparseCoding<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForSparseCoding::Share(FeatureDictionaryForSparseCoding*)")
        return false;
    }

    return this->Share(*InputDictionary);
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::ForceShare(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return;
    }

    m_BasisMatrix.ForceShare(InputDictionary.m_BasisMatrix);

    m_Covariance.ForceShare(InputDictionary.m_Covariance);

    m_StandardDeviation.ForceShare(InputDictionary.m_StandardDeviation);
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::ForceShare(const FeatureDictionaryForSparseCoding<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForSparseCoding::ForceShare(FeatureDictionaryForSparseCoding*)")
        return false;
    }

    this->ForceShare(*InputDictionary);

    return true;
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::Clear()
{
    m_BasisMatrix.Clear();

    m_Covariance.Clear();

    m_StandardDeviation.Clear();
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::IsEmpty() const
{
    return m_BasisMatrix.IsEmpty();
}


template<typename ElementType>
MatrixSize FeatureDictionaryForSparseCoding<ElementType>::GetSize() const
{
    return m_BasisMatrix.GetSize();
}

template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Load(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Save(const std::string& FilePathAndName) const
{
    return true;
}


template<typename ElementType>
const std::string& FeatureDictionaryForSparseCoding<ElementType>::GetName() const
{
    return m_Name;
}


template<typename ElementType>
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisMatrix()
{
    return m_BasisMatrix;
}


template<typename ElementType>
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisMatrix() const
{
    return m_BasisMatrix;
}

}// namespace mdk

#endif