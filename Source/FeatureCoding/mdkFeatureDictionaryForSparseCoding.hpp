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

    this->Copy(InputDictionary);
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
    this->Copy(InputDictionary);
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::operator=(FeatureDictionaryForSparseCoding<ElementType>&& InputDictionary)
{
    this->Take(std::forward<FeatureDictionaryForSparseCoding<ElementType>&>(InputDictionary));
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::Copy(const FeatureDictionaryForSparseCoding<ElementType>& InputDictionary)
{
    if (m_DictionaryData == InputDictionary.m_DictionaryData)
    {
        return;
    }

    m_DictionaryData->DictionaryInfo.Name = InputDictionary.m_DictionaryData->DictionaryInfo.Name;
    m_DictionaryData->DictionaryInfo.BasisNonnegative = InputDictionary.m_DictionaryData->DictionaryInfo.BasisNonnegative;
    m_DictionaryData->DictionaryInfo.BasisSumToOne = InputDictionary.m_DictionaryData->DictionaryInfo.BasisSumToOne;

    m_DictionaryData->BasisMatrix = InputDictionary.m_DictionaryData->BasisMatrix;

    m_DictionaryData->StandardDeviationOfL1Distance = InputDictionary.m_DictionaryData->StandardDeviationOfL1Distance;

    m_DictionaryData->StandardDeviationOfL2Distance = InputDictionary.m_DictionaryData->StandardDeviationOfL2Distance;

    m_DictionaryData->StandardDeviationOfKLDivergence = InputDictionary.m_DictionaryData->StandardDeviationOfKLDivergence;

    m_DictionaryData->StandardDeviationOfReconstruction = InputDictionary.m_DictionaryData->StandardDeviationOfReconstruction;

    m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory = InputDictionary.m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory;

    m_DictionaryData->ProbabilityMassFunction = InputDictionary.m_DictionaryData->ProbabilityMassFunction;

    m_DictionaryData->Covariance = InputDictionary.m_DictionaryData->Covariance;

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

    m_DictionaryData->StandardDeviationOfL1Distance = std::move(InputDictionary.m_DictionaryData->StandardDeviationOfL1Distance);

    m_DictionaryData->StandardDeviationOfL2Distance = std::move(InputDictionary.m_DictionaryData->StandardDeviationOfL2Distance);

    m_DictionaryData->StandardDeviationOfKLDivergence = std::move(InputDictionary.m_DictionaryData->StandardDeviationOfKLDivergence);

    m_DictionaryData->StandardDeviationOfReconstruction = std::move(InputDictionary.m_DictionaryData->StandardDeviationOfReconstruction);

    m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory = InputDictionary.m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory;

    m_DictionaryData->ProbabilityMassFunction = std::move(InputDictionary.m_DictionaryData->ProbabilityMassFunction);

    m_DictionaryData->Covariance = std::move(InputDictionary.m_DictionaryData->Covariance);

    InputDictionary.Clear();
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::Clear()
{
    m_DictionaryData->DictionaryInfo.Name.Clear();
    m_DictionaryData->DictionaryInfo.BasisNonnegative = false;
    m_DictionaryData->DictionaryInfo.BasisSumToOne = false;

    m_DictionaryData->BasisMatrix.Clear();

    m_DictionaryData->StandardDeviationOfL1Distance.Clear();
    m_DictionaryData->StandardDeviationOfL2Distance.Clear();
    m_DictionaryData->StandardDeviationOfKLDivergence.Clear();
    m_DictionaryData->StandardDeviationOfReconstruction.Clear();

    m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory = 0;

    m_DictionaryData->ProbabilityMassFunction.Clear();

    m_DictionaryData->Covariance.Clear();

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
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviationOfL1Distance()
{
    return m_DictionaryData->StandardDeviationOfL1Distance;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviationOfL1Distance() const
{
    return m_DictionaryData->StandardDeviationOfL1Distance;
}

template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviationOfL2Distance()
{
    return m_DictionaryData->StandardDeviationOfL2Distance;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviationOfL2Distance() const
{
    return m_DictionaryData->StandardDeviationOfL2Distance;
}

template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviationOfKLDivergence()
{
    return m_DictionaryData->StandardDeviationOfKLDivergence;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviationOfKLDivergence() const
{
    return m_DictionaryData->StandardDeviationOfKLDivergence;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviationOfReconstruction()
{
    return m_DictionaryData->StandardDeviationOfReconstruction;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::StandardDeviationOfReconstruction() const
{
    return m_DictionaryData->StandardDeviationOfReconstruction;
}


template<typename ElementType>
inline
ElementType FeatureDictionaryForSparseCoding<ElementType>::GetWeightedNumberOfTrainingSamplesInHistory() const
{
    return m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetWeightedNumberOfTrainingSamplesInHistory(ElementType Number)
{
    m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory = Number;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::ProbabilityMassFunction()
{
    return m_DictionaryData->ProbabilityMassFunction;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::ProbabilityMassFunction() const
{
    return m_DictionaryData->ProbabilityMassFunction;
}


}// namespace mdk

#endif