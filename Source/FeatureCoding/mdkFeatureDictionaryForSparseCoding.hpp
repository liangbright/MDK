#ifndef __mdkFeatureDictionaryForSparseCoding_hpp
#define __mdkFeatureDictionaryForSparseCoding_hpp


namespace mdk
{

template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>::FeatureDictionaryForSparseCoding()
{
    m_DictionaryData = std::make_shared<DictionaryData_Of_FeatureDictionaryForSparseCoding<ScalarType>>();

    this->Clear();
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>::FeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding<ScalarType>& InputDictionary)
{
    m_DictionaryData = std::make_shared<DictionaryData_Of_FeatureDictionaryForSparseCoding<ScalarType>>();

    this->Copy(InputDictionary);
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>::FeatureDictionaryForSparseCoding(FeatureDictionaryForSparseCoding<ScalarType>&& InputDictionary)
{
    m_DictionaryData = std::move(InputDictionary.m_DictionaryData);
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>::~FeatureDictionaryForSparseCoding()
{
}


template<typename ScalarType>
void FeatureDictionaryForSparseCoding<ScalarType>::operator=(const FeatureDictionaryForSparseCoding<ScalarType>& InputDictionary)
{
    this->Copy(InputDictionary);
}


template<typename ScalarType>
void FeatureDictionaryForSparseCoding<ScalarType>::operator=(FeatureDictionaryForSparseCoding<ScalarType>&& InputDictionary)
{
    if (!m_DictionaryData)
    {
        m_DictionaryData = std::make_shared<DictionaryData_Of_FeatureDictionaryForSparseCoding<ScalarType>>();

        this->Clear();
    }

    this->Take(std::forward<FeatureDictionaryForSparseCoding<ScalarType>&>(InputDictionary));
}


template<typename ScalarType>
void FeatureDictionaryForSparseCoding<ScalarType>::Copy(const FeatureDictionaryForSparseCoding<ScalarType>& InputDictionary)
{
    if (m_DictionaryData == InputDictionary.m_DictionaryData)
    {
        return;
    }

	if (InputDictionary.IsEmpty() == true)
	{
		this->Clear();
		return;
	}

    m_DictionaryData->Name = InputDictionary.m_DictionaryData->Name;

    m_DictionaryData->BasisMatrix = InputDictionary.m_DictionaryData->BasisMatrix;

	m_DictionaryData->WeightMatrix = InputDictionary.m_DictionaryData->WeightMatrix;

    m_DictionaryData->SeedForNewBasisIDGeneration = InputDictionary.m_DictionaryData->SeedForNewBasisIDGeneration.load();

    m_DictionaryData->BasisID = InputDictionary.m_DictionaryData->BasisID;

    m_DictionaryData->BasisPositive = InputDictionary.m_DictionaryData->BasisPositive;
    m_DictionaryData->BasisNormalizedWithL1Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL1Norm;
    m_DictionaryData->BasisNormalizedWithL2Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL2Norm;

    m_DictionaryData->CurrentDictionaryTime = InputDictionary.m_DictionaryData->CurrentDictionaryTime;

    m_DictionaryData->BasisAge = InputDictionary.m_DictionaryData->BasisAge;

    m_DictionaryData->BasisExperience = InputDictionary.m_DictionaryData->BasisExperience;

    m_DictionaryData->SimilarityType = InputDictionary.m_DictionaryData->SimilarityType;
    m_DictionaryData->SimilarityMatrix = InputDictionary.m_DictionaryData->SimilarityMatrix;

    m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy = InputDictionary.m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy;
    m_DictionaryData->BasisRedundancy = InputDictionary.m_DictionaryData->BasisRedundancy;

    m_DictionaryData->VarianceOfL1Distance = InputDictionary.m_DictionaryData->VarianceOfL1Distance;
    m_DictionaryData->VarianceOfL2Distance = InputDictionary.m_DictionaryData->VarianceOfL2Distance;
    m_DictionaryData->VarianceOfKLDivergence = InputDictionary.m_DictionaryData->VarianceOfKLDivergence;
    m_DictionaryData->VarianceOfReconstruction = InputDictionary.m_DictionaryData->VarianceOfReconstruction;
    
    //m_DictionaryData->BasisCovariance = InputDictionary.m_DictionaryData->BasisCovariance;
}


template<typename ScalarType>
bool FeatureDictionaryForSparseCoding<ScalarType>::Copy(const FeatureDictionaryForSparseCoding<ScalarType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForSparseCoding::Copy(FeatureDictionaryForSparseCoding*)")
        return false;
    }

    this->Copy(*InputDictionary);

    return true;
}


template<typename ScalarType>
void FeatureDictionaryForSparseCoding<ScalarType>::Share(FeatureDictionaryForSparseCoding<ScalarType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return;
    }

    m_DictionaryData = InputDictionary.m_DictionaryData;
}


template<typename ScalarType>
bool FeatureDictionaryForSparseCoding<ScalarType>::Share(FeatureDictionaryForSparseCoding<ScalarType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForSparseCoding::Share(FeatureDictionaryForSparseCoding*)")
        return false;
    }

    this->Share(*InputDictionary);

    return true;
}


template<typename ScalarType>
void FeatureDictionaryForSparseCoding<ScalarType>::ForceShare(const FeatureDictionaryForSparseCoding<ScalarType>& InputDictionary)
{
    if (this == &InputDictionary)
    {
        return;
    }

    m_DictionaryData = InputDictionary.m_DictionaryData;
}


template<typename ScalarType>
bool FeatureDictionaryForSparseCoding<ScalarType>::ForceShare(const FeatureDictionaryForSparseCoding<ScalarType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("Input is nullptr @ FeatureDictionaryForSparseCoding::ForceShare(FeatureDictionaryForSparseCoding*)")
        return false;
    }

    this->ForceShare(*InputDictionary);

    return true;
}


template<typename ScalarType>
void FeatureDictionaryForSparseCoding<ScalarType>::Take(FeatureDictionaryForSparseCoding<ScalarType>& InputDictionary)
{
    m_DictionaryData->Name = std::move(InputDictionary.m_DictionaryData->Name);

    m_DictionaryData->BasisMatrix = std::move(InputDictionary.m_DictionaryData->BasisMatrix);
	
	m_DictionaryData->WeightMatrix = std::move(InputDictionary.m_DictionaryData->WeightMatrix);

    m_DictionaryData->SeedForNewBasisIDGeneration = InputDictionary.m_DictionaryData->SeedForNewBasisIDGeneration.load();

    m_DictionaryData->BasisID = std::move(InputDictionary.m_DictionaryData->BasisID);

    m_DictionaryData->BasisPositive = InputDictionary.m_DictionaryData->BasisPositive;
    m_DictionaryData->BasisNormalizedWithL1Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL1Norm;
    m_DictionaryData->BasisNormalizedWithL2Norm = InputDictionary.m_DictionaryData->BasisNormalizedWithL2Norm;

    m_DictionaryData->CurrentDictionaryTime = InputDictionary.m_DictionaryData->CurrentDictionaryTime;

    m_DictionaryData->BasisAge = std::move(InputDictionary.m_DictionaryData->BasisAge);

    m_DictionaryData->BasisExperience = std::move(InputDictionary.m_DictionaryData->BasisExperience);

    m_DictionaryData->SimilarityType = InputDictionary.m_DictionaryData->SimilarityType;
    m_DictionaryData->SimilarityMatrix = std::move(InputDictionary.m_DictionaryData->SimilarityMatrix);

    m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy = InputDictionary.m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy;
    m_DictionaryData->BasisRedundancy = std::move(InputDictionary.m_DictionaryData->BasisRedundancy);

    m_DictionaryData->VarianceOfL1Distance = std::move(InputDictionary.m_DictionaryData->VarianceOfL1Distance);
    m_DictionaryData->VarianceOfL2Distance = std::move(InputDictionary.m_DictionaryData->VarianceOfL2Distance);
    m_DictionaryData->VarianceOfKLDivergence = std::move(InputDictionary.m_DictionaryData->VarianceOfKLDivergence);
    m_DictionaryData->VarianceOfReconstruction = std::move(InputDictionary.m_DictionaryData->VarianceOfReconstruction);

    //m_DictionaryData->BasisCovariance = std::move(InputDictionary.m_DictionaryData->BasisCovariance);

    InputDictionary.Clear();
}


template<typename ScalarType>
void FeatureDictionaryForSparseCoding<ScalarType>::Take(FeatureDictionaryForSparseCoding<ScalarType>* InputDictionary)
{
    if (InputDictionary == nullptr)
    {
        MDK_Error("InputDictionary is nullptr @ FeatureDictionaryForSparseCoding::Take(...)")
    }

    this->Take(*InputDictionary);
}


template<typename ScalarType>
void FeatureDictionaryForSparseCoding<ScalarType>::Clear()
{
	if (!m_DictionaryData)
	{
		return;
	}

	m_DictionaryData->Name = "";

    m_DictionaryData->BasisMatrix.Clear();

	m_DictionaryData->WeightMatrix.Clear();

    m_DictionaryData->SeedForNewBasisIDGeneration = 0;

    m_DictionaryData->BasisID.Clear();

    m_DictionaryData->BasisPositive = false;
    m_DictionaryData->BasisNormalizedWithL1Norm = false;
    m_DictionaryData->BasisNormalizedWithL2Norm = false;

    m_DictionaryData->CurrentDictionaryTime = 0;

    m_DictionaryData->BasisAge.Clear();

    m_DictionaryData->BasisExperience.Clear();

    m_DictionaryData->SimilarityType = VectorSimilarityTypeEnum::Unknown;
    m_DictionaryData->SimilarityMatrix.Clear();

    m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy = 0;
    m_DictionaryData->BasisRedundancy.Clear();

    m_DictionaryData->VarianceOfL1Distance.Clear();
    m_DictionaryData->VarianceOfL2Distance.Clear();
    m_DictionaryData->VarianceOfKLDivergence.Clear();
    m_DictionaryData->VarianceOfReconstruction.Clear();

    //m_DictionaryData->BasisCovariance.Clear();
}


template<typename ScalarType>
bool FeatureDictionaryForSparseCoding<ScalarType>::IsEmpty() const
{
    return m_DictionaryData->BasisMatrix.IsEmpty();
}


template<typename ScalarType>
MatrixSize FeatureDictionaryForSparseCoding<ScalarType>::GetSize() const
{
    return m_DictionaryData->BasisMatrix.GetSize();
}


template<typename ScalarType>
bool FeatureDictionaryForSparseCoding<ScalarType>::Load(const std::string& FilePathAndName)
{
    auto temp = LoadFeatureDictionaryForSparseCodingFromJsonDataFile<ScalarType>(FilePathAndName);
    if (temp.IsEmpty() == false)
    {
        this->Take(temp);
        return true;
    }

    return false;
}


template<typename ScalarType>
bool FeatureDictionaryForSparseCoding<ScalarType>::Save(const std::string& FilePathAndName) const
{
    return SaveFeatureDictionaryForSparseCodingAsJsonDataFile(*this, FilePathAndName);
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>::GetSubDictionary(const DenseMatrix<int_max>& BasisIndexList_to_keep) const
{
    FeatureDictionaryForSparseCoding<ScalarType> SubDictionary;

    if (BasisIndexList_to_keep.IsVector() == false)
    {
        MDK_Error("Input is invalid @ FeatureDictionaryForSparseCoding::GetSubDictionary(...)")
        return SubDictionary;
    }

    int_max BasisNumber = m_DictionaryData->BasisMatrix.GetColNumber();

    // check index range
    for (int_max k = 0; k < BasisIndexList_to_keep.GetElementNumber(); ++k)
    {
        auto tempIndex = BasisIndexList_to_keep[k];

        if (tempIndex < 0 || tempIndex >= BasisNumber)
        {
            MDK_Error("Input Index is invalid @ FeatureDictionaryForSparseCoding::GetSubDictionary(...)")
            return SubDictionary;
        }
    }

    // get data

    SubDictionary.m_DictionaryData->BasisMatrix = m_DictionaryData->BasisMatrix.GetSubMatrix(ALL, BasisIndexList_to_keep);

    SubDictionary.m_DictionaryData->BasisID = m_DictionaryData->BasisID.GetSubMatrix(BasisIndexList_to_keep);

    SubDictionary.m_DictionaryData->SeedForNewBasisIDGeneration = m_DictionaryData->SeedForNewBasisIDGeneration.load();

    SubDictionary.m_DictionaryData->BasisPositive = m_DictionaryData->BasisPositive;
    SubDictionary.m_DictionaryData->BasisNormalizedWithL1Norm = m_DictionaryData->BasisNormalizedWithL1Norm;
    SubDictionary.m_DictionaryData->BasisNormalizedWithL2Norm = m_DictionaryData->BasisNormalizedWithL2Norm;

    SubDictionary.m_DictionaryData->CurrentDictionaryTime = m_DictionaryData->CurrentDictionaryTime;

    SubDictionary.m_DictionaryData->BasisAge = m_DictionaryData->BasisAge.GetSubMatrix(BasisIndexList_to_keep);

    SubDictionary.m_DictionaryData->BasisExperience = m_DictionaryData->BasisExperience.GetSubMatrix(BasisIndexList_to_keep);

    SubDictionary.m_DictionaryData->SimilarityType = m_DictionaryData->SimilarityType;
    SubDictionary.m_DictionaryData->SimilarityMatrix = m_DictionaryData->SimilarityMatrix.GetSubMatrix(BasisIndexList_to_keep, BasisIndexList_to_keep);

    SubDictionary.m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy = m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy;
    SubDictionary.m_DictionaryData->BasisRedundancy = m_DictionaryData->BasisRedundancy.GetSubMatrix(BasisIndexList_to_keep);

    SubDictionary.m_DictionaryData->VarianceOfL1Distance = m_DictionaryData->VarianceOfL1Distance.GetSubMatrix(BasisIndexList_to_keep);

    SubDictionary.m_DictionaryData->VarianceOfL2Distance = m_DictionaryData->VarianceOfL2Distance.GetSubMatrix(BasisIndexList_to_keep);

    SubDictionary.m_DictionaryData->VarianceOfKLDivergence = m_DictionaryData->VarianceOfKLDivergence.GetSubMatrix(BasisIndexList_to_keep);

    SubDictionary.m_DictionaryData->VarianceOfReconstruction = m_DictionaryData->VarianceOfReconstruction.GetSubMatrix(BasisIndexList_to_keep);

    //SubDictionary.m_DictionaryData->BasisCovariance.Clear();

    //------------------------------------------------------

    return SubDictionary;
}


template<typename ScalarType>
void FeatureDictionaryForSparseCoding<ScalarType>::CombineDictionary(const FeatureDictionaryForSparseCoding<ScalarType>& InputDictionary)
{
    if (InputDictionary.IsEmpty() == true)
    {
        MDK_Warning("InputDictionary is empty @ FeatureDictionaryForSparseCoding::CombineDictionary(...)")
        return;
    }

    if (this->IsEmpty() == true)
    {
        this->Copy(InputDictionary);
        return;
    }

    // check size

    auto InputSize = InputDictionary.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber != SelfSize.RowNumber)
    {
        MDK_Error("RowNumber is no the same @ FeatureDictionaryForSparseCoding::CombineDictionary(...)")
        return;
    }

    // check basis property

    if (m_DictionaryData->BasisPositive != InputDictionary.m_DictionaryData->BasisPositive)
    {
        MDK_Error("BasisPositive is not the same @ FeatureDictionaryForSparseCoding::CombineDictionary(...)")
        return;
    }

    if (m_DictionaryData->BasisNormalizedWithL1Norm != InputDictionary.m_DictionaryData->BasisNormalizedWithL1Norm)
    {
        MDK_Error("BasisNormalizedWithL1Norm is not the same @ FeatureDictionaryForSparseCoding::CombineDictionary(...)")
        return;
    }

    if (m_DictionaryData->BasisNormalizedWithL2Norm != InputDictionary.m_DictionaryData->BasisNormalizedWithL2Norm)
    {
        MDK_Error("BasisNormalizedWithL2Norm is not the same @ FeatureDictionaryForSparseCoding::CombineDictionary(...)")
        return;
    }

    // check SimilarityType
    if (m_DictionaryData->SimilarityType != InputDictionary.m_DictionaryData->SimilarityType)
    {
        MDK_Error("SimilarityType is not the same @ FeatureDictionaryForSparseCoding::CombineDictionary(...)")
        return;
    }

    // check SimilarityThreshold_For_ComputeBasisRedundancy
    if (m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy != InputDictionary.m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy)
    {
        MDK_Error("SimilarityThreshold_For_ComputeBasisRedundancy is not the same @ FeatureDictionaryForSparseCoding::CombineDictionary(...)")
        return;
    }

    //----------------------- start to combine ---------------------------------------------------------------

    int_max BasisNumber_input = InputDictionary.m_DictionaryData->BasisMatrix.GetColNumber();

    int_max BasisNumber_self = m_DictionaryData->BasisMatrix.GetColNumber();

    int_max BasisNumber_combined = BasisNumber_self + BasisNumber_input;

    // update SeedForNewBasisIDGeneration
    m_DictionaryData->SeedForNewBasisIDGeneration = std::max(m_DictionaryData->SeedForNewBasisIDGeneration.load(), 
                                                             InputDictionary.m_DictionaryData->SeedForNewBasisIDGeneration.load());

    // update Dictionary Time
    m_DictionaryData->CurrentDictionaryTime = std::max(m_DictionaryData->CurrentDictionaryTime, 
                                                       InputDictionary.m_DictionaryData->CurrentDictionaryTime);

    // update other

    m_DictionaryData->BasisMatrix = { &m_DictionaryData->BasisMatrix, &InputDictionary.m_DictionaryData->BasisMatrix };

    m_DictionaryData->BasisID = { &m_DictionaryData->BasisID, &InputDictionary.m_DictionaryData->BasisID };

    m_DictionaryData->BasisID = { &m_DictionaryData->BasisID, &InputDictionary.m_DictionaryData->BasisID };

    m_DictionaryData->BasisAge = { &m_DictionaryData->BasisAge, &InputDictionary.m_DictionaryData->BasisAge };

    m_DictionaryData->BasisExperience = { &m_DictionaryData->BasisExperience, &InputDictionary.m_DictionaryData->BasisExperience };

    // update SimilarityMatrix : fill 0 to unknown Similarity

    DenseMatrix<ScalarType> SimilarityMatrix_self = m_DictionaryData->SimilarityMatrix;

    m_DictionaryData->SimilarityMatrix.FastResize(BasisNumber_combined, BasisNumber_combined);
    m_DictionaryData->SimilarityMatrix.Fill(0);

    auto IndexRange_self = span(0, BasisNumber_self - 1);
    m_DictionaryData->SimilarityMatrix(IndexRange_self, IndexRange_self) = SimilarityMatrix_self;

    auto IndexRange_input = span(BasisNumber_self, BasisNumber_combined - 1);
    m_DictionaryData->SimilarityMatrix(IndexRange_input, IndexRange_input) = InputDictionary.m_DictionaryData->SimilarityMatrix;

    // update BasisRedundancy
    m_DictionaryData->BasisRedundancy = { &m_DictionaryData->BasisRedundancy, &InputDictionary.m_DictionaryData->BasisRedundancy };

    // update Variance

    m_DictionaryData->VarianceOfL1Distance = { &m_DictionaryData->VarianceOfL1Distance, &InputDictionary.m_DictionaryData->VarianceOfL1Distance };

    m_DictionaryData->VarianceOfL2Distance = { &m_DictionaryData->VarianceOfL2Distance, &InputDictionary.m_DictionaryData->VarianceOfL2Distance };

    m_DictionaryData->VarianceOfKLDivergence = { &m_DictionaryData->VarianceOfKLDivergence, &InputDictionary.m_DictionaryData->VarianceOfKLDivergence };

    m_DictionaryData->VarianceOfReconstruction = { &m_DictionaryData->VarianceOfReconstruction, &InputDictionary.m_DictionaryData->VarianceOfReconstruction };

    // update BasisCovariance : not used yet
    //m_DictionaryData->BasisCovariance.Clear();
}


template<typename ScalarType>
inline
const std::string& FeatureDictionaryForSparseCoding<ScalarType>::GetName() const
{
    return m_DictionaryData->Name;
}


template<typename ScalarType>
inline
void FeatureDictionaryForSparseCoding<ScalarType>::SetName(const std::string& Name)
{
    m_DictionaryData->Name = Name;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::BasisMatrix()
{
    return m_DictionaryData->BasisMatrix;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::BasisMatrix() const
{
    return m_DictionaryData->BasisMatrix;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::WeightMatrix()
{
	return m_DictionaryData->WeightMatrix;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::WeightMatrix() const
{
	return m_DictionaryData->WeightMatrix;
}


template<typename ScalarType>
inline
int_max FeatureDictionaryForSparseCoding<ScalarType>::GenerateNewBasisID()
{
    // overflow detection ?

    m_DictionaryData->SeedForNewBasisIDGeneration += 1;

    return m_DictionaryData->SeedForNewBasisIDGeneration;
}


template<typename ScalarType>
inline 
int_max FeatureDictionaryForSparseCoding<ScalarType>::GetCurrentSeedForNewBasisIDGeneration() const
{
    return m_DictionaryData->SeedForNewBasisIDGeneration.load();
}


template<typename ScalarType>
inline void 
FeatureDictionaryForSparseCoding<ScalarType>::SetCurrentSeedForNewBasisIDGeneration(int_max Seed)
{
    m_DictionaryData->SeedForNewBasisIDGeneration = Seed;
}


template<typename ScalarType>
inline
DenseMatrix<int_max>& FeatureDictionaryForSparseCoding<ScalarType>::BasisID()
{
    return m_DictionaryData->BasisID;
}


template<typename ScalarType>
inline
const DenseMatrix<int_max>& FeatureDictionaryForSparseCoding<ScalarType>::BasisID() const
{
    return m_DictionaryData->BasisID;
}


template<typename ScalarType>
inline 
void FeatureDictionaryForSparseCoding<ScalarType>::SetProperty_BasisPositive(bool YesNO)
{
    m_DictionaryData->BasisPositive = YesNO;
}


template<typename ScalarType>
inline 
void FeatureDictionaryForSparseCoding<ScalarType>::SetProperty_BasisNormalizedWithL1Norm(bool YesNO)
{
    m_DictionaryData->SetProperty_BasisNormalizedWithL1Norm = YesNO;
}


template<typename ScalarType>
inline
void FeatureDictionaryForSparseCoding<ScalarType>::SetProperty_BasisNormalizedWithL2Norm(bool YesNO)
{
    m_DictionaryData->SetProperty_BasisNormalizedWithL2Norm = YesNO;
}


template<typename ScalarType>
inline
bool FeatureDictionaryForSparseCoding<ScalarType>::GetProperty_BasisPositive() const
{
    return m_DictionaryData->BasisPositive;
}


template<typename ScalarType>
inline
bool FeatureDictionaryForSparseCoding<ScalarType>::GetProperty_BasisNormalizedWithL1Norm() const
{
    return m_DictionaryData->BasisNormalizedWithL1Norm;
}


template<typename ScalarType>
inline
bool FeatureDictionaryForSparseCoding<ScalarType>::GetProperty_BasisNormalizedWithL2Norm() const
{
    return m_DictionaryData->BasisNormalizedWithL2Norm;
}


template<typename ScalarType>
inline
void FeatureDictionaryForSparseCoding<ScalarType>::SetCurrentDictionaryTime(int_max DictionaryTime)
{
    m_DictionaryData->CurrentDictionaryTime = DictionaryTime;
}


template<typename ScalarType>
inline
int_max FeatureDictionaryForSparseCoding<ScalarType>::GetCurrentDictionaryTime() const
{
    return m_DictionaryData->CurrentDictionaryTime;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::BasisAge()
{
    return m_DictionaryData->BasisAge;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::BasisAge() const
{
    return m_DictionaryData->BasisAge;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::BasisExperience()
{
    return m_DictionaryData->BasisExperience;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::BasisExperience() const
{
    return m_DictionaryData->BasisExperience;
}


template<typename ScalarType>
inline
void FeatureDictionaryForSparseCoding<ScalarType>::SetProperty_SimilarityType(VectorSimilarityTypeEnum SimilarityType)
{
    m_DictionaryData->SimilarityType = SimilarityType;
}


template<typename ScalarType>
inline
VectorSimilarityTypeEnum FeatureDictionaryForSparseCoding<ScalarType>::GetProperty_SimilarityType() const
{
    return m_DictionaryData->SimilarityType;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::SimilarityMatrix()
{
    return m_DictionaryData->SimilarityMatrix;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::SimilarityMatrix() const
{
    return m_DictionaryData->SimilarityMatrix;
}


template<typename ScalarType>
inline
void FeatureDictionaryForSparseCoding<ScalarType>::SetProperty_SimilarityThresholdForComputeBasisRedundancy(ScalarType SimilarityThreshold)
{
    m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy = SimilarityThreshold;
}


template<typename ScalarType>
inline
ScalarType FeatureDictionaryForSparseCoding<ScalarType>::GetProperty_SimilarityThresholdForComputeBasisRedundancy() const
{
    return m_DictionaryData->SimilarityThreshold_For_ComputeBasisRedundancy;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::BasisRedundancy()
{
    return m_DictionaryData->BasisRedundancy;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::BasisRedundancy() const
{
    return m_DictionaryData->BasisRedundancy;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::VarianceOfL1Distance()
{
    return m_DictionaryData->VarianceOfL1Distance;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::VarianceOfL1Distance() const
{
    return m_DictionaryData->VarianceOfL1Distance;
}

template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::VarianceOfL2Distance()
{
    return m_DictionaryData->VarianceOfL2Distance;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::VarianceOfL2Distance() const
{
    return m_DictionaryData->VarianceOfL2Distance;
}

template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::VarianceOfKLDivergence()
{
    return m_DictionaryData->VarianceOfKLDivergence;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::VarianceOfKLDivergence() const
{
    return m_DictionaryData->VarianceOfKLDivergence;
}


template<typename ScalarType>
inline
DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::VarianceOfReconstruction()
{
    return m_DictionaryData->VarianceOfReconstruction;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& FeatureDictionaryForSparseCoding<ScalarType>::VarianceOfReconstruction() const
{
    return m_DictionaryData->VarianceOfReconstruction;
}


}// namespace mdk

#endif