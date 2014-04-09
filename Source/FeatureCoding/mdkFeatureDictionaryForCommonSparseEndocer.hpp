#ifndef __mdkFeatureDictionaryForCommonSparseEndocer_hpp
#define __mdkFeatureDictionaryForCommonSparseEndocer_hpp


namespace mdk
{

template<typename ElementType>
FeatureDictionaryForCommonSparseEndocer<ElementType>::FeatureDictionaryForCommonSparseEndocer()
{

}


template<typename ElementType>
FeatureDictionaryForCommonSparseEndocer<ElementType>::FeatureDictionaryForCommonSparseEndocer(const FeatureDictionaryForCommonSparseEndocer& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
FeatureDictionaryForCommonSparseEndocer<ElementType>::FeatureDictionaryForCommonSparseEndocer(FeatureDictionaryForCommonSparseEndocer&& InputDictionary)
{
    m_BasisMatrix = std::move(InputDictionary.m_BasisMatrix);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_StandardDeviation = std::move(InputDictionary.m_StandardDeviation);
}


template<typename ElementType>
FeatureDictionaryForCommonSparseEndocer<ElementType>::~FeatureDictionaryForCommonSparseEndocer()
{

}


template<typename ElementType>
void FeatureDictionaryForCommonSparseEndocer<ElementType>::operator=(const FeatureDictionaryForCommonSparseEndocer& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
void FeatureDictionaryForCommonSparseEndocer<ElementType>::operator=(FeatureDictionaryForCommonSparseEndocer&& InputDictionary)
{
    m_BasisMatrix = std::move(InputDictionary.m_BasisMatrix);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_StandardDeviation = std::move(InputDictionary.m_StandardDeviation);
}


template<typename ElementType>
bool FeatureDictionaryForCommonSparseEndocer<ElementType>::Copy(const FeatureDictionaryForCommonSparseEndocer<ElementType>& InputDictionary)
{
    m_BasisMatrix.Copy(InputDictionary.m_BasisMatrix);

    m_Covariance.Copy(InputDictionary.m_Covariance);

    m_StandardDeviation.Copy(InputDictionary.m_StandardDeviation);

    return true;

}


template<typename ElementType>
bool FeatureDictionaryForCommonSparseEndocer<ElementType>::Copy(const FeatureDictionaryForCommonSparseEndocer<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForCommonSparseEndocer::Copy(FeatureDictionaryForCommonSparseEndocer*)")
        return false;
    }

    return this->Copy(*InputDictionary);
}


template<typename ElementType>
bool FeatureDictionaryForCommonSparseEndocer<ElementType>::Share(FeatureDictionaryForCommonSparseEndocer<ElementType>& InputDictionary)
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
bool FeatureDictionaryForCommonSparseEndocer<ElementType>::Share(FeatureDictionaryForCommonSparseEndocer<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForCommonSparseEndocer::Share(FeatureDictionaryForCommonSparseEndocer*)")
        return false;
    }

    return this->Share(*InputDictionary);
}


template<typename ElementType>
void FeatureDictionaryForCommonSparseEndocer<ElementType>::ForceShare(const FeatureDictionaryForCommonSparseEndocer<ElementType>& InputDictionary)
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
bool FeatureDictionaryForCommonSparseEndocer<ElementType>::ForceShare(const FeatureDictionaryForCommonSparseEndocer<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForCommonSparseEndocer::ForceShare(FeatureDictionaryForCommonSparseEndocer*)")
        return false;
    }

    this->ForceShare(*InputDictionary);

    return true;
}


template<typename ElementType>
void FeatureDictionaryForCommonSparseEndocer<ElementType>::Clear()
{
    m_BasisMatrix.Clear();

    m_Covariance.Clear();

    m_StandardDeviation.Clear();
}


template<typename ElementType>
bool FeatureDictionaryForCommonSparseEndocer<ElementType>::IsEmpty() const
{
    return m_BasisMatrix.IsEmpty();
}


template<typename ElementType>
MatrixSize FeatureDictionaryForCommonSparseEndocer<ElementType>::GetSize() const
{
    return m_BasisMatrix.GetSize();
}

template<typename ElementType>
bool FeatureDictionaryForCommonSparseEndocer<ElementType>::Load(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool FeatureDictionaryForCommonSparseEndocer<ElementType>::Save(const std::string& FilePathAndName) const
{
    return true;
}


template<typename ElementType>
DenseMatrix<ElementType>& FeatureDictionaryForCommonSparseEndocer<ElementType>::BasisMatrix()
{
    return m_BasisMatrix;
}


template<typename ElementType>
const DenseMatrix<ElementType> FeatureDictionaryForCommonSparseEndocer<ElementType>::BasisMatrix() const
{
    return m_BasisMatrix;
}

}// namespace mdk

#endif