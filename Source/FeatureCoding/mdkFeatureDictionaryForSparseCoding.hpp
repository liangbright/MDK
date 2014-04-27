#ifndef __mdkFeatureDictionaryForSparseCoding_hpp
#define __mdkFeatureDictionaryForSparseCoding_hpp


namespace mdk
{

template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::FeatureDictionaryForSparseCoding()
{
    m_DictionaryData = std::make_shared<DictionaryData_Of_FeatureDictionaryForSparseCoding<ElementType>>();
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::FeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    m_DictionaryData = std::make_shared<DictionaryData_Of_FeatureDictionaryForSparseCoding<ElementType>>();

    this->Copy(InputDictionary)
}


template<typename ElementType>
FeatureDictionaryForSparseCoding<ElementType>::FeatureDictionaryForSparseCoding(FeatureDictionaryForSparseCoding<ElementType>&& InputDictionary)
{
    m_DictionaryData = std::move(InputDictionary.m_DictionaryData);
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
void FeatureDictionaryForSparseCoding<ElementType>::Copy(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return;
    }

    m_DictionaryData->DictionaryInfo.Name.Copy(InputDictionary.m_DictionaryData->DictionaryInfo.Name);
    m_DictionaryData->DictionaryInfo.BasisNonnegative = InputDictionary.m_DictionaryData->DictionaryInfo.BasisNonnegative;
    m_DictionaryData->DictionaryInfo.BasisSumToOne = InputDictionary.m_DictionaryData->DictionaryInfo.BasisSumToOne;

    m_DictionaryData->BasisMatrix.Copy(InputDictionary.m_DictionaryData->BasisMatrix);

    m_DictionaryData->Covariance.Copy(InputDictionary.m_DictionaryData->Covariance);

    m_DictionaryData->StandardDeviation.Copy(InputDictionary.m_DictionaryData->StandardDeviation);
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Copy(const FeatureDictionaryForSparseCoding<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForSparseCoding::Copy(FeatureDictionaryForSparseCoding*)")
        return false;
    }

    this->Copy(*InputDictionary);

    return true;
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::Share(FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return;
    }

    m_DictionaryData = InputDictionary.m_DictionaryData;
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::Share(FeatureDictionaryForSparseCoding<ElementType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForSparseCoding::Share(FeatureDictionaryForSparseCoding*)")
        return false;
    }

    this->Share(*InputDictionary);

    return true;
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::ForceShare(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return;
    }

    m_DictionaryData = InputDictionary.m_DictionaryData;
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
    m_DictionaryData->DictionaryInfo.Name = std::move(InputDictionary.m_DictionaryData->DictionaryInfo.Name);
    m_DictionaryData->DictionaryInfo.BasisNonnegative = InputDictionary.m_DictionaryData->DictionaryInfo.BasisNonnegative;
    m_DictionaryData->DictionaryInfo.BasisSumToOne = InputDictionary.m_DictionaryData->DictionaryInfo.BasisSumToOne;

    m_DictionaryData->BasisMatrix = std::move(InputDictionary.m_DictionaryData->BasisMatrix);

    m_DictionaryData->Covariance = std::move(InputDictionary.m_DictionaryData->Covariance);

    m_DictionaryData->StandardDeviation = std::move(InputDictionary.m_DictionaryData->StandardDeviation);
}



template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::Clear()
{
    m_DictionaryData->DictionaryInfo.Name.Clear();
    m_DictionaryData->DictionaryInfo.BasisNonnegative = false;
    m_DictionaryData->DictionaryInfo.BasisSumToOne = false;

    m_DictionaryData->BasisMatrix.Clear();

    m_DictionaryData->Covariance.Clear();

    m_DictionaryData->StandardDeviation.Clear();
}


template<typename ElementType>
bool FeatureDictionaryForSparseCoding<ElementType>::IsEmpty() const
{
    return m_DictionaryData->BasisMatrix.IsEmpty();
}


template<typename ElementType>
MatrixSize FeatureDictionaryForSparseCoding<ElementType>::GetSize() const
{
    return m_DictionaryData->BasisMatrix.GetSize();
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
inline
const Infomation_Of_FeatureDictionaryForSparseCoding& FeatureDictionaryForSparseCoding<ElementType>::GetDictionaryInformation() const
{
    return m_DictionaryData->DictionaryInfo;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetDictionaryInformation(const Infomation_Of_FeatureDictionaryForSparseCoding& Info)
{
    m_DictionaryData->DictionaryInfo = Info;
}


template<typename ElementType>
inline
const CharString& FeatureDictionaryForSparseCoding<ElementType>::GetName() const
{
    return m_DictionaryData->DictionaryInfo.Name;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetName(const CharString& Name)
{
    m_DictionaryData->DictionaryInfo.Name = Name;
}


template<typename ElementType>
inline 
void FeatureDictionaryForSparseCoding<ElementType>::SetDictionaryInfo_BasisNonnegative(bool Nonnegative = true)
{
    m_DictionaryData->DictionaryInfo.BasisNonnegative = Nonnegative;
}


template<typename ElementType>
inline 
void FeatureDictionaryForSparseCoding<ElementType>::SetDictionaryInfo_BasisSumToOne(bool SumToOne)
{
    m_DictionaryData->DictionaryInfo.BasisSumToOne = SumToOne;
}


template<typename ElementType>
inline
bool FeatureDictionaryForSparseCoding<ElementType>::IsBasisNonnegative() const
{
    return m_DictionaryData->DictionaryInfo.BasisNonnegative;
}


template<typename ElementType>
inline
bool FeatureDictionaryForSparseCoding<ElementType>::IsBasisSumToOne() const
{
    return m_DictionaryData->DictionaryInfo.BasisSumToOne;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisMatrix()
{
    return m_DictionaryData->BasisMatrix;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisMatrix() const
{
    return m_DictionaryData->BasisMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviation()
{
    return m_DictionaryData->StandardDeviation;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviation() const
{
    return m_DictionaryData->StandardDeviation;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisProbability()
{
    return m_DictionaryData->BasisProbability;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisProbability() const
{
    return m_DictionaryData->BasisProbability;
}


}// namespace mdk

#endif