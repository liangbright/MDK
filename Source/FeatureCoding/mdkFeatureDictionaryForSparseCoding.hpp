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

    m_DictionaryData->BasisMatrix = InputDictionary.m_DictionaryData->BasisMatrix;

    m_DictionaryData->BasisPositive = InputDictionary.m_DictionaryData->BasisPositive;
    m_DictionaryData->BasisNormalizedWithL1Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL1Norm;
    m_DictionaryData->BasisNormalizedWithL2Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL2Norm;

    m_DictionaryData->CurrentDictionaryTime = InputDictionary.m_DictionaryData->CurrentDictionaryTime;

    m_DictionaryData->BasisAge = InputDictionary.m_DictionaryData->BasisAge;

    m_DictionaryData->NewBasisIndexList = InputDictionary.m_DictionaryData->NewBasisIndexList;

    m_DictionaryData->BasisExperience = InputDictionary.m_DictionaryData->BasisExperience;

    m_DictionaryData->SimilarityType = InputDictionary.m_DictionaryData->SimilarityType;
    m_DictionaryData->SimilarityMatrix = InputDictionary.m_DictionaryData->SimilarityMatrix;

    m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy = InputDictionary.m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy;
    m_DictionaryData->BasisRedundancy = InputDictionary.m_DictionaryData->BasisRedundancy;

    m_DictionaryData->VarianceOfL1Distance = InputDictionary.m_DictionaryData->VarianceOfL1Distance;
    m_DictionaryData->VarianceOfL2Distance = InputDictionary.m_DictionaryData->VarianceOfL2Distance;
    m_DictionaryData->VarianceOfKLDivergence = InputDictionary.m_DictionaryData->VarianceOfKLDivergence;
    m_DictionaryData->VarianceOfReconstruction = InputDictionary.m_DictionaryData->VarianceOfReconstruction;
    
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

    m_DictionaryData->BasisMatrix = std::move(InputDictionary.m_DictionaryData->BasisMatrix);

    m_DictionaryData->BasisPositive = InputDictionary.m_DictionaryData->BasisPositive;
    m_DictionaryData->BasisNormalizedWithL1Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL1Norm;
    m_DictionaryData->BasisNormalizedWithL2Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL2Norm;

    m_DictionaryData->CurrentDictionaryTime = InputDictionary.m_DictionaryData->CurrentDictionaryTime;

    m_DictionaryData->BasisAge = std::move(InputDictionary.m_DictionaryData->BasisAge);

    m_DictionaryData->NewBasisIndexList = std::move(InputDictionary.m_DictionaryData->NewBasisIndexList);

    m_DictionaryData->BasisExperience = std::move(InputDictionary.m_DictionaryData->BasisExperience);

    m_DictionaryData->SimilarityType = InputDictionary.m_DictionaryData->SimilarityType;
    m_DictionaryData->SimilarityMatrix = std::move(InputDictionary.m_DictionaryData->SimilarityMatrix);

    m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy = InputDictionary.m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy;
    m_DictionaryData->BasisRedundancy = std::move(InputDictionary.m_DictionaryData->BasisRedundancy);

    m_DictionaryData->VarianceOfL1Distance = std::move(InputDictionary.m_DictionaryData->VarianceOfL1Distance);
    m_DictionaryData->VarianceOfL2Distance = std::move(InputDictionary.m_DictionaryData->VarianceOfL2Distance);
    m_DictionaryData->VarianceOfKLDivergence = std::move(InputDictionary.m_DictionaryData->VarianceOfKLDivergence);
    m_DictionaryData->VarianceOfReconstruction = std::move(InputDictionary.m_DictionaryData->VarianceOfReconstruction);

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

    m_DictionaryData->CurrentDictionaryTime = 0;

    m_DictionaryData->BasisAge.Clear();

    m_DictionaryData->NewBasisIndexList.Clear();

    m_DictionaryData->BasisExperience.Clear();

    m_DictionaryData->SimilarityType = MDK_SimilarityType_Enum_For_FeatureCoding::Unknown;
    m_DictionaryData->SimilarityMatrix.Clear();

    m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy = 0;
    m_DictionaryData->BasisRedundancy.Clear();

    m_DictionaryData->VarianceOfL1Distance.Clear();
    m_DictionaryData->VarianceOfL2Distance.Clear();
    m_DictionaryData->VarianceOfKLDivergence.Clear();
    m_DictionaryData->VarianceOfReconstruction.Clear();

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
void FeatureDictionaryForSparseCoding<ElementType>::SetProperty_BasisPositive(bool YesNO)
{
    m_DictionaryData->BasisPositive = YesNO;
}


template<typename ElementType>
inline 
void FeatureDictionaryForSparseCoding<ElementType>::SetProperty_BasisNormalizedWithL1Norm(bool YesNO)
{
    m_DictionaryData->SetProperty_BasisNormalizedWithL1Norm = YesNO;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetProperty_BasisNormalizedWithL2Norm(bool YesNO)
{
    m_DictionaryData->SetProperty_BasisNormalizedWithL2Norm = YesNO;
}


template<typename ElementType>
inline
bool FeatureDictionaryForSparseCoding<ElementType>::GetProperty_BasisPositive() const
{
    return m_DictionaryData->BasisPositive;
}


template<typename ElementType>
inline
bool FeatureDictionaryForSparseCoding<ElementType>::GetProperty_BasisNormalizedWithL1Norm() const
{
    return m_DictionaryData->BasisNormalizedWithL1Norm;
}


template<typename ElementType>
inline
bool FeatureDictionaryForSparseCoding<ElementType>::GetProperty_BasisNormalizedWithL2Norm() const
{
    return m_DictionaryData->BasisNormalizedWithL2Norm;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetCurrentDictionaryTime(int_max DictionaryTime)
{
    m_DictionaryData->CurrentDictionaryTime = DictionaryTime;
}


template<typename ElementType>
inline
int_max FeatureDictionaryForSparseCoding<ElementType>::GetCurrentDictionaryTime() const
{
    return m_DictionaryData->CurrentDictionaryTime;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisAge()
{
    return m_DictionaryData->BasisAge;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisAge() const
{
    return m_DictionaryData->BasisAge;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::NewBasisIndexList()
{
    return m_DictionaryData->NewBasisIndexList;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::NewBasisIndexList() const
{
    return m_DictionaryData->NewBasisIndexList;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisExperience()
{
    return m_DictionaryData->BasisExperience;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::BasisExperience() const
{
    return m_DictionaryData->BasisExperience;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetProperty_SimilarityType(MDK_SimilarityType_Enum_For_FeatureCoding SimilarityType)
{
    m_DictionaryData->SimilarityType = SimilarityType;
}


template<typename ElementType>
inline
MDK_SimilarityType_Enum_For_FeatureCoding FeatureDictionaryForSparseCoding<ElementType>::GetProperty_SimilarityType() const
{
    return m_DictionaryData->SimilarityType;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::SimilarityMatrix()
{
    return m_DictionaryData->SimilarityMatrix;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::SimilarityMatrix() const
{
    return m_DictionaryData->SimilarityMatrix;
}


template<typename ElementType>
inline
void FeatureDictionaryForSparseCoding<ElementType>::SetProperty_SimilarityThresholdToComputeBasisRedundancy(ElementType SimilarityThreshold)
{
    m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy = SimilarityThreshold;
}


template<typename ElementType>
inline
ElementType FeatureDictionaryForSparseCoding<ElementType>::GetProperty_SimilarityThresholdToComputeBasisRedundancy() const
{
    return m_DictionaryData->SimilarityThresholdToComputeBasisRedundancy;
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
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::VarianceOfL1Distance()
{
    return m_DictionaryData->VarianceOfL1Distance;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::VarianceOfL1Distance() const
{
    return m_DictionaryData->VarianceOfL1Distance;
}

template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::VarianceOfL2Distance()
{
    return m_DictionaryData->VarianceOfL2Distance;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::VarianceOfL2Distance() const
{
    return m_DictionaryData->VarianceOfL2Distance;
}

template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::VarianceOfKLDivergence()
{
    return m_DictionaryData->VarianceOfKLDivergence;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::VarianceOfKLDivergence() const
{
    return m_DictionaryData->VarianceOfKLDivergence;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::VarianceOfReconstruction()
{
    return m_DictionaryData->VarianceOfReconstruction;
}


template<typename ElementType>
inline
const DenseMatrix<ElementType>& FeatureDictionaryForSparseCoding<ElementType>::VarianceOfReconstruction() const
{
    return m_DictionaryData->VarianceOfReconstruction;
}


}// namespace mdk

#endif