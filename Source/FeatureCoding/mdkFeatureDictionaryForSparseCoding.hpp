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

    m_DictionaryData->Name = InputDictionary.m_DictionaryData->Name;

    m_DictionaryData->BasisPositive = InputDictionary.m_DictionaryData->BasisPositive;
    m_DictionaryData->BasisNormalizedWithL1Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL1Norm;
    m_DictionaryData->BasisNormalizedWithL2Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL2Norm;

    m_DictionaryData->BasisMatrix = InputDictionary.m_DictionaryData->BasisMatrix;

    m_DictionaryData->StandardDeviationOfL1Distance = InputDictionary.m_DictionaryData->StandardDeviationOfL1Distance;
    m_DictionaryData->StandardDeviationOfL2Distance = InputDictionary.m_DictionaryData->StandardDeviationOfL2Distance;
    m_DictionaryData->StandardDeviationOfKLDivergence = InputDictionary.m_DictionaryData->StandardDeviationOfKLDivergence;
    m_DictionaryData->StandardDeviationOfReconstruction = InputDictionary.m_DictionaryData->StandardDeviationOfReconstruction;

    m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory = InputDictionary.m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory;

    m_DictionaryData->ProbabilityMassFunction = InputDictionary.m_DictionaryData->ProbabilityMassFunction;

    m_DictionaryData->BasisCovariance = InputDictionary.m_DictionaryData->BasisCovariance;

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
    m_DictionaryData->Name = std::move(InputDictionary.m_DictionaryData->Name);

    m_DictionaryData->BasisPositive = InputDictionary.m_DictionaryData->BasisPositive;
    m_DictionaryData->BasisNormalizedWithL1Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL1Norm;
    m_DictionaryData->BasisNormalizedWithL2Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL2Norm;

    m_DictionaryData->BasisMatrix = std::move(InputDictionary.m_DictionaryData->BasisMatrix);

    m_DictionaryData->StandardDeviationOfL1Distance = std::move(InputDictionary.m_DictionaryData->StandardDeviationOfL1Distance);
    m_DictionaryData->StandardDeviationOfL2Distance = std::move(InputDictionary.m_DictionaryData->StandardDeviationOfL2Distance);
    m_DictionaryData->StandardDeviationOfKLDivergence = std::move(InputDictionary.m_DictionaryData->StandardDeviationOfKLDivergence);
    m_DictionaryData->StandardDeviationOfReconstruction = std::move(InputDictionary.m_DictionaryData->StandardDeviationOfReconstruction);

    m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory = InputDictionary.m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory;

    m_DictionaryData->ProbabilityMassFunction = std::move(InputDictionary.m_DictionaryData->ProbabilityMassFunction);

    m_DictionaryData->BasisCovariance = std::move(InputDictionary.m_DictionaryData->BasisCovariance);

    InputDictionary.Clear();
}


template<typename ElementType>
void FeatureDictionaryForSparseCoding<ElementType>::Clear()
{
    m_DictionaryData->Name.Clear();

    m_DictionaryData->BasisMatrix.Clear();

    m_DictionaryData->BasisPositive = false;
    m_DictionaryData->BasisNormalizedWithL1Norm = false;
    m_DictionaryData->BasisNormalizedWithL2Norm = false;

    m_DictionaryData->SimilarityTypeToComputeBasisRedundancy.Clear();
    m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy = 0;
    m_DictionaryData->BasisRedundancy.Clear();

    m_DictionaryData->StandardDeviationOfL1Distance.Clear();
    m_DictionaryData->StandardDeviationOfL2Distance.Clear();
    m_DictionaryData->StandardDeviationOfKLDivergence.Clear();
    m_DictionaryData->StandardDeviationOfReconstruction.Clear();

    m_DictionaryData->WeightedNumberOfTrainingSamplesInHistory = 0;

    m_DictionaryData->ProbabilityMassFunction.Clear();

    m_DictionaryData->BasisCovariance.Clear();

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
const CharString& FeatureDictionaryForSparseCoding<ElementType>::GetName() const
{
    return m_DictionaryData->Name;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetName(const CharString& Name)
{
    m_DictionaryData->Name = Name;
}


template<typename ElementType>
inline 
void FeatureDictionaryForSparseCoding<ElementType>::SetInfo_BasisPositive(bool YesNO)
{
    m_DictionaryData->BasisPositive = YesNO;
}


template<typename ElementType>
inline 
void FeatureDictionaryForSparseCoding<ElementType>::SetInfo_BasisNormalizedWithL1Norm(bool YesNO)
{
    m_DictionaryData->SetInfo_BasisNormalizedWithL1Norm = YesNO;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetInfo_BasisNormalizedWithL2Norm(bool YesNO)
{
    m_DictionaryData->SetInfo_BasisNormalizedWithL2Norm = YesNO;
}


template<typename ElementType>
inline
bool FeatureDictionaryForSparseCoding<ElementType>::GetInfo_BasisPositive() const
{
    return m_DictionaryData->BasisPositive;
}


template<typename ElementType>
inline
bool FeatureDictionaryForSparseCoding<ElementType>::GetInfo_BasisNormalizedWithL1Norm() const
{
    return m_DictionaryData->BasisNormalizedWithL1Norm;
}


template<typename ElementType>
inline
bool FeatureDictionaryForSparseCoding<ElementType>::GetInfo_BasisNormalizedWithL2Norm() const
{
    return m_DictionaryData->BasisNormalizedWithL2Norm;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetInfo_SimilarityTypeToComputeBasisRedundancy(const CharString& SimilarityType)
{
    m_DictionaryData->SimilarityTypeToComputeBasisRedundancy = SimilarityType;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetInfo_SimilarityThresholdToComputeBasisRedundancy(ElementType SimilarityThreshold)
{
    m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy = SimilarityThreshold;
}


template<typename ElementType>
inline
const CharString& FeatureDictionaryForSparseCoding<ElementType>::GetInfo_SimilarityTypeToComputeBasisRedundancy() const
{
    return m_DictionaryData->SimilarityTypeToComputeBasisRedundancy;
}


template<typename ElementType>
inline
ElementType FeatureDictionaryForSparseCoding<ElementType>::SetInfo_SimilarityThresholdToComputeBasisRedundancy() const
{
    return m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy;
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
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisRedundancy()
{
    return m_DictionaryData->BasisRedundancy;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisRedundancy() const
{
    return m_DictionaryData->BasisRedundancy;
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