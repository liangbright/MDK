#ifndef __mdkFeatureDictionaryForDenseCoding_hpp
#define __mdkFeatureDictionaryForDenseCoding_hpp

namespace mdk
{

template<typename ScalarType>
FeatureDictionaryForDenseCoding<ScalarType>::FeatureDictionaryForDenseCoding()
{
}


template<typename ScalarType>
FeatureDictionaryForDenseCoding<ScalarType>::FeatureDictionaryForDenseCoding(const FeatureDictionaryForDenseCoding& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ScalarType>
FeatureDictionaryForDenseCoding<ScalarType>::FeatureDictionaryForDenseCoding(FeatureDictionaryForDenseCoding&& InputDictionary)
{
    m_Name = std::move(InputDictionary.m_Name);
    m_BasisMatrix = std::move(InputDictionary.m_BasisMatrix);
    m_Covariance = std::move(InputDictionary.m_Covariance);
    m_StandardDeviation = std::move(InputDictionary.m_StandardDeviation);
}


template<typename ScalarType>
FeatureDictionaryForDenseCoding<ScalarType>::~FeatureDictionaryForDenseCoding()
{

}


template<typename ScalarType>
void FeatureDictionaryForDenseCoding<ScalarType>::operator=(const FeatureDictionaryForDenseCoding& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ScalarType>
void FeatureDictionaryForDenseCoding<ScalarType>::operator=(FeatureDictionaryForDenseCoding&& InputDictionary)
{
    m_Name = std::move(InputDictionary.m_Name);

    m_BasisMatrix = std::move(InputDictionary.m_BasisMatrix);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_StandardDeviation = std::move(InputDictionary.m_StandardDeviation);
}


template<typename ScalarType>
bool FeatureDictionaryForDenseCoding<ScalarType>::Copy(const FeatureDictionaryForDenseCoding<ScalarType>& InputDictionary)
{
    m_Name.Copy(InputDictionary.m_Name);

    m_BasisMatrix.Copy(InputDictionary.m_BasisMatrix);

    m_Covariance.Copy(InputDictionary.m_Covariance);

    m_StandardDeviation.Copy(InputDictionary.m_StandardDeviation);

    return true;

}


template<typename ScalarType>
bool FeatureDictionaryForDenseCoding<ScalarType>::Copy(const FeatureDictionaryForDenseCoding<ScalarType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForDenseCoding::Copy(FeatureDictionaryForDenseCoding*)")
        return false;
    }

    return this->Copy(*InputDictionary);
}


template<typename ScalarType>
bool FeatureDictionaryForDenseCoding<ScalarType>::Share(FeatureDictionaryForDenseCoding<ScalarType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return true;
    }

    auto IsOK_0 = m_Name.Share(InputDictionary.m_Name);

    auto IsOK_1 = m_BasisMatrix.Share(InputDictionary.m_BasisMatrix);

    auto IsOK_2 = m_Covariance.Share(InputDictionary.m_Covariance);

    auto IsOK_3 = m_StandardDeviation.Share(InputDictionary.m_StandardDeviation);

    if (IsOK_0 == false || IsOK_1 == false || IsOK_2 == false || IsOK_3 == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}


template<typename ScalarType>
bool FeatureDictionaryForDenseCoding<ScalarType>::Share(FeatureDictionaryForDenseCoding<ScalarType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForDenseCoding::Share(FeatureDictionaryForDenseCoding*)")
        return false;
    }

    return this->Share(*InputDictionary);
}


template<typename ScalarType>
void FeatureDictionaryForDenseCoding<ScalarType>::ForceShare(const FeatureDictionaryForDenseCoding<ScalarType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return;
    }

    m_Name.ForceShare(InputDictionary.m_Name);

    m_BasisMatrix.ForceShare(InputDictionary.m_BasisMatrix);

    m_Covariance.ForceShare(InputDictionary.m_Covariance);

    m_StandardDeviation.ForceShare(InputDictionary.m_StandardDeviation);
}


template<typename ScalarType>
bool FeatureDictionaryForDenseCoding<ScalarType>::ForceShare(const FeatureDictionaryForDenseCoding<ScalarType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForDenseCoding::ForceShare(FeatureDictionaryForDenseCoding*)")
        return false;
    }

    this->ForceShare(*InputDictionary);

    return true;
}


template<typename ScalarType>
void FeatureDictionaryForDenseCoding<ScalarType>::Clear()
{
    m_Name.Clear();

    m_BasisMatrix.Clear();

    m_Covariance.Clear();

    m_StandardDeviation.Clear();
}


template<typename ScalarType>
bool FeatureDictionaryForDenseCoding<ScalarType>::IsEmpty() const
{
    return m_BasisMatrix.IsEmpty();
}


template<typename ScalarType>
MatrixSize FeatureDictionaryForDenseCoding<ScalarType>::GetSize() const
{
    return m_BasisMatrix.GetSize();
}


template<typename ScalarType>
bool FeatureDictionaryForDenseCoding<ScalarType>::Load(const CharString& FilePathAndName)
{
    return true;
}


template<typename ScalarType>
bool FeatureDictionaryForDenseCoding<ScalarType>::Save(const CharString& FilePathAndName) const
{
    return true;
}


template<typename ScalarType>
const CharString& FeatureDictionaryForDenseCoding<ScalarType>::Name() const
{
    return m_Name;
}


template<typename ScalarType>
CharString& FeatureDictionaryForDenseCoding<ScalarType>::Name()
{
    return m_Name;
}


template<typename ScalarType>
DenseMatrix<ScalarType>& FeatureDictionaryForDenseCoding<ScalarType>::BasisMatrix()
{
    return m_BasisMatrix;
}


template<typename ScalarType>
const DenseMatrix<ScalarType>& FeatureDictionaryForDenseCoding<ScalarType>::BasisMatrix() const
{
    return m_BasisMatrix;
}

}// namespace mdk

#endif