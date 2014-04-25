#ifndef __mdkFeatureDictionaryForSparseCoding_hpp
#define __mdkFeatureDictionaryForSparseCoding_hpp


namespace mdk
{

template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::FeatureDictionaryForSparseCoding()
{

}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::FeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::FeatureDictionaryForSparseCoding(FeatureDictionaryForSparseCoding<ElementType>&& InputDictionary)
{
    m_Name = std::move(InputDictionary.m_Name);

    m_BasisMatrix = std::move(InputDictionary.m_BasisMatrix);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_ReconstructionStd = std::move(InputDictionary.m_ReconstructionStd);
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::~FeatureDictionaryForSparseCoding()
{

}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::operator=(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    this->Copy(InputDictionary)
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::operator=(FeatureDictionaryForSparseCoding<ElementType>&& InputDictionary)
{
    this->Take(std::forward<FeatureDictionaryForSparseCoding<ElementType>&>(InputDictionary));
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Copy(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    m_Name.Copy(InputDictionary.m_Name);

    m_BasisMatrix.Copy(InputDictionary.m_BasisMatrix);

    m_Covariance.Copy(InputDictionary.m_Covariance);

    m_ReconstructionStd.Copy(InputDictionary.m_ReconstructionStd);

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

    auto IsOK_0 = m_Name.Share(InputDictionary.m_Name);

    auto IsOK_1 = m_BasisMatrix.Share(InputDictionary.m_BasisMatrix);

    auto IsOK_2 = m_Covariance.Share(InputDictionary.m_Covariance);

    auto IsOK_3 = m_ReconstructionStd.Share(InputDictionary.m_ReconstructionStd);

    if (IsOK_0 == false || IsOK_1 == false || IsOK_2 == false || IsOK_3 == false)
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

    m_Name.ForceShare(InputDictionary.m_Name);

    m_BasisMatrix.ForceShare(InputDictionary.m_BasisMatrix);

    m_ReconstructionStd.ForceShare(InputDictionary.m_ReconstructionStd);

    m_Covariance.ForceShare(InputDictionary.m_Covariance);
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
void FeatureDictionaryForSparseCoding<ElementType>::Take(FeatureDictionaryForSparseCoding& InputDictionary)
{
    m_Name = std::move(InputDictionary.m_Name);

    m_BasisMatrix = std::move(InputDictionary.m_BasisMatrix);

    m_Covariance = std::move(InputDictionary.m_Covariance);

    m_ReconstructionStd = std::move(InputDictionary.m_ReconstructionStd);
}



template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::Clear()
{
    m_Name.Clear();

    m_BasisMatrix.Clear();

    m_Covariance.Clear();

    m_ReconstructionStd.Clear();
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
bool FeatureDictionaryForSparseCoding<ElementType>::Load(const CharString& FilePathAndName)
{
    auto temp = LoadFeatureDictionaryForSparseCoding<ElementType>(FilePathAndName);
    if (temp.IsEmpty() == false)
    {
        this->Take(temp);
        return true;
    }

    return false;
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Save(const CharString& FilePathAndName) const
{
    return SaveFeatureDictionaryForSparseCoding(*this, FilePathAndName);
}


template<typename ElementType>
CharString& FeatureDictionaryForSparseCoding<ElementType>::Name()
{
    return m_Name;
}


template<typename ElementType>
const CharString& FeatureDictionaryForSparseCoding<ElementType>::Name() const
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


template<typename ElementType>
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::ReconstructionStd()
{
    return m_ReconstructionStd;
}


template<typename ElementType>
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::ReconstructionStd() const
{
    return m_ReconstructionStd;
}


}// namespace mdk

#endif